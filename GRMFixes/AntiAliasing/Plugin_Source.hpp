// Source file
// Multi-platform file can not have `#pragma once`!!!
// For compile plugin in Multiplatform mode - select 'Debug' or 'Release' configuration.

#include "Plugin_Header.h"

namespace NAMESPACE
{
	/*
	// Idee 1: Renderstates setzen
	// 0x0070E530 private: int __thiscall zCRnd_D3D::XD3D_InitPerDX(long,int,int,int,int)
	int __fastcall zCRnd_D3D_XD3D_InitPerDX(zCRnd_D3D* _this, void* vtable, long flags, int x, int y, int bpp, int id);
	CInvoke<int(__thiscall*)(zCRnd_D3D * _this, long flags, int x, int y, int bpp, int id)> Ivk_zCRnd_D3D_XD3D_InitPerDX(GothicMemoryLocations::zCRnd_D3D::XD3D_InitPerDX, &zCRnd_D3D_XD3D_InitPerDX);

	int __fastcall zCRnd_D3D_XD3D_InitPerDX(zCRnd_D3D* _this, void* vtable, long flags, int x, int y, int bpp, int id)
	{
		int result = Ivk_zCRnd_D3D_XD3D_InitPerDX(_this, flags, x, y, bpp, id);

		D3DDEVICEDESC7 caps;
		ZeroMemory(&caps, sizeof(caps));
		_this->xd3d_pd3dDevice7->GetCaps(&caps);

		if (caps.dpcTriCaps.dwRasterCaps && D3DPRASTERCAPS_ANTIALIASSORTDEPENDENT)
		{
			_this->xd3d_pd3dDevice7->SetRenderState(D3DRENDERSTATE_ANTIALIAS, D3DANTIALIAS_SORTDEPENDENT);
		}
		else if (caps.dpcTriCaps.dwRasterCaps && D3DPRASTERCAPS_ANTIALIASSORTINDEPENDENT)
		{
			_this->xd3d_pd3dDevice7->SetRenderState(D3DRENDERSTATE_ANTIALIAS, D3DANTIALIAS_SORTINDEPENDENT);
		}

		if (caps.dpcTriCaps.dwRasterCaps && D3DPRASTERCAPS_ANTIALIASEDGES)
		{
			_this->xd3d_pd3dDevice7->SetRenderState(D3DRENDERSTATE_EDGEANTIALIAS, TRUE);
		}

		return result;
	}
	*/

	// Idee 2: FXAA auf der CPU
	IDirectDrawSurface7* g_fxaa = 0;
	int g_threadNum = fmax(1, std::thread::hardware_concurrency() - 1);
	std::mutex g_d3dMutex = std::mutex();
	float** g_intensityCache = 0;
	D3DCOLOR** g_threadBuffers = 0;
	int g_width = 0;
	int g_height = 0;
	const float EDGE_THRESHOLD_MIN = 0.0312F * 4.0F;
	const float EDGE_THRESHOLD_MAX = 0.125F;
	const int ITERATIONS = 12;
	const float QUALITY[] = { 1.0F, 1.0F, 1.0F, 1.0F, 1.0F, 1.5F, 2.0F, 2.0F, 2.0F, 2.0F, 4.0F, 8.0F };
	const float SUBPIXEL_QUALITY = 0.75F;

	inline D3DCOLOR ColorLerp(D3DCOLOR col1, D3DCOLOR col2, float t)
	{
		BYTE r1 = RGB_GETRED(col1);
		BYTE g1 = RGB_GETGREEN(col1);
		BYTE b1 = RGB_GETBLUE(col1);
		BYTE r2 = RGB_GETRED(col2);
		BYTE g2 = RGB_GETGREEN(col2);
		BYTE b2 = RGB_GETBLUE(col2);
		BYTE r = r1 + t * (r2 - r1);
		BYTE g = g1 + t * (g2 - g1);
		BYTE b = b1 + t * (b2 - b1);
		D3DCOLOR rgb = RGBA_MAKE(r, g, b, 255);
		return rgb;
	}

	D3DCOLOR* GetPixel(LPDDSURFACEDESC2 ddsd, int y, int x)
	{
		x = std::clamp(x, 0, g_width - 1);
		y = std::clamp(y, 0, g_height - 1);
		DWORD offset = y * ddsd->lPitch + x * 4;
		D3DCOLOR* result = (D3DCOLOR*)((DWORD)ddsd->lpSurface + offset);
		return result;
	}

	D3DCOLOR GetPixel(LPDDSURFACEDESC2 ddsd, zVEC2 vec)
	{
		D3DCOLOR color = *GetPixel(ddsd, (int)vec[1], (int)vec[0]);
		D3DCOLOR result = 0;

		float offXMantissa = vec[0] - (int)vec[0];
		float offYMantissa = vec[1] - (int)vec[1];

		if (offXMantissa != 0.0F)
		{
			int offX = offXMantissa > 0 ? ceilf(offXMantissa) : floorf(offXMantissa);
			D3DCOLOR colorX = *GetPixel(ddsd, vec[1], (int)vec[0] + offX);
			result = ColorLerp(color, colorX, fabs(offXMantissa));
		}
		else if (offYMantissa != 0.0F)
		{
			int offY = offYMantissa > 0 ? ceilf(offYMantissa) : floorf(offYMantissa);
			D3DCOLOR colorY = *GetPixel(ddsd, (int)vec[1] + offY, (int)vec[0]);
			result = ColorLerp(color, colorY, fabs(offYMantissa));
		}
		else
		{
			result = color;
		}

		return result;
	}

	inline float GetIntensity(BYTE red, BYTE green, BYTE blue)
	{
		return (0.299F * (red / 256.0f) + 0.587F * (green / 256.0f) + 0.114F * (blue / 256.0f));
	}

	inline void SetCachedIntensity(int x, int y, D3DCOLOR color)
	{
		g_intensityCache[y][x] = GetIntensity(RGB_GETRED(color), RGB_GETGREEN(color), RGB_GETBLUE(color));
	}

	inline float GetCachedIntensity(int y, int x)
	{
		x = std::clamp(x, 0, g_width - 1);
		y = std::clamp(y, 0, g_height - 1);
		return g_intensityCache[y][x];
	}

	float GetCachedIntensity(zVEC2 vec)
	{
		float intensity = GetCachedIntensity((int)vec[1], (int)vec[0]);
		float intensityX = intensity;
		float intensityY = intensity;

		float offXMantissa = vec[0] - (int)vec[0];
		float offYMantissa = vec[1] - (int)vec[1];

		if (offXMantissa != 0.0F)
		{
			int offX = offXMantissa > 0 ? ceilf(offXMantissa) : floorf(offXMantissa);
			intensityX = GetCachedIntensity((int)vec[1], (int)vec[0] + offX);
		}
		
		if (offYMantissa != 0.0F)
		{
			int offY = offYMantissa > 0 ? ceilf(offYMantissa) : floorf(offYMantissa);
			intensityY = GetCachedIntensity((int)vec[1] + offY, (int)vec[0]);
		}
		
		float result = intensity + fabs(offXMantissa) * (intensityX - intensity) + fabs(offYMantissa) * (intensityY - intensity);
		return result;
	}

	void CalculateIntensityCache(LPDDSURFACEDESC2 ddsd, int threadId, int threadCount)
	{
		for (int y = threadId; y < g_height; y += threadCount)
		{
			D3DCOLOR* firstPixel = GetPixel(ddsd, y, 0);
			memcpy(g_threadBuffers[threadId], firstPixel, g_width * sizeof(D3DCOLOR));

			for (int x = 0; x < g_width; x++)
			{
				SetCachedIntensity(x, y, g_threadBuffers[threadId][x]);
			}
		}
	}

	// http://blog.simonrodriguez.fr/articles/2016/07/implementing_fxaa.html
	void ApplyFxaa(LPDDSURFACEDESC2 ddsd, int threadId, int threadCount)
	{
		for (int y = threadId + 1; y < g_height - 1; y += threadCount)
		{
			for (int x = 1; x < g_width - 1; x++)
			{
				// Luma at the current fragment
				float lumaCenter = GetCachedIntensity(y, x);

				// Luma at the four direct neighbours of the current fragment
				float lumaDown = GetCachedIntensity(y - 1, x);
				float lumaUp = GetCachedIntensity(y + 1, x);
				float lumaLeft = GetCachedIntensity(y, x - 1);
				float lumaRight = GetCachedIntensity(y, x + 1);

				// Find the maximum and minimum luma around the current fragment
				float lumaMin = fmin(lumaCenter, fmin(fmin(lumaDown, lumaUp), fmin(lumaLeft, lumaRight)));
				float lumaMax = fmax(lumaCenter, fmax(fmax(lumaDown, lumaUp), fmax(lumaLeft, lumaRight)));

				// Compute the delta
				float lumaRange = lumaMax - lumaMin;

				// If the luma variation is lower that a threshold (or if we are in a really dark area), we are not on an edge, don't perform any AA
				if (lumaRange < fmax(EDGE_THRESHOLD_MIN, lumaMax * EDGE_THRESHOLD_MAX))
				{
					continue;
				}

				// Test pixel detection
				//_this->SetPixel(x, y, zCOLOR(255, 0, 0, 0));
				//continue;

				// Query the 4 remaining corners lumas
				float lumaDownLeft = GetCachedIntensity(y - 1, x - 1);
				float lumaDownRight = GetCachedIntensity(y - 1, x + 1);
				float lumaUpLeft = GetCachedIntensity(y + 1, x - 1);
				float lumaUpRight = GetCachedIntensity(y + 1, x + 1);

				// Combine the four edges lumas (using intermediary variables for future computations with the same values)
				float lumaDownUp = lumaDown + lumaUp;
				float lumaLeftRight = lumaLeft + lumaRight;

				// Same for corners
				float lumaLeftCorners = lumaDownLeft + lumaUpLeft;
				float lumaDownCorners = lumaDownLeft + lumaDownRight;
				float lumaRightCorners = lumaDownRight + lumaUpRight;
				float lumaUpCorners = lumaUpRight + lumaUpLeft;

				// Compute an estimation of the gradient along the horizontal and vertical axis
				float edgeHorizontal = fabs(-2.0F * lumaLeft + lumaLeftCorners) + fabs(-2.0F * lumaCenter + lumaDownUp) * 2.0F + fabs(-2.0F * lumaRight + lumaRightCorners);
				float edgeVertical = fabs(-2.0F * lumaUp + lumaUpCorners) + fabs(-2.0F * lumaCenter + lumaLeftRight) * 2.0F + fabs(-2.0F * lumaDown + lumaDownCorners);

				// Is the local edge horizontal or vertical?
				bool isHorizontal = (edgeHorizontal >= edgeVertical);

				// Select the two neighboring texels lumas in the opposite direction to the local edge
				float luma1 = isHorizontal ? lumaDown : lumaLeft;
				float luma2 = isHorizontal ? lumaUp : lumaRight;

				// Compute gradients in this direction
				float gradient1 = luma1 - lumaCenter;
				float gradient2 = luma2 - lumaCenter;

				// Which direction is the steepest?
				bool is1Steepest = fabs(gradient1) >= fabs(gradient2);

				// Gradient in the corresponding direction, normalized
				float gradientScaled = 0.25F * fmax(fabs(gradient1), fabs(gradient2));

				// Choose the step size (one pixel) according to the edge direction
				float stepLength = 1.0F;

				// Average luma in the correct direction
				float lumaLocalAverage = 0.0F;

				if (is1Steepest)
				{
					// Switch the direction
					stepLength = -stepLength;
					lumaLocalAverage = 0.5F * (luma1 + lumaCenter);
				}
				else
				{
					lumaLocalAverage = 0.5F * (luma2 + lumaCenter);
				}

				// Shift UV in the correct direction by half a pixel
				zVEC2 currentUv = zVEC2(x, y);
				if (isHorizontal)
					currentUv[1] += stepLength * 0.5F;
				else
					currentUv[0] += stepLength * 0.5F;

				// Compute offset (for each iteration step) in the right direction
				zVEC2 offset = isHorizontal ? zVEC2(1.0F, 0.0F) : zVEC2(0.0F, 1.0F);

				// Compute UVs to explore on each side of the edge, orthogonally. The QUALITY allows us to step faster.
				zVEC2 uv1 = currentUv - offset;
				zVEC2 uv2 = currentUv + offset;

				// Read the lumas at both current extremities of the exploration segment, and compute the delta wrt to the local average luma
				float lumaEnd1 = GetCachedIntensity(uv1);
				float lumaEnd2 = GetCachedIntensity(uv2);
				lumaEnd1 -= lumaLocalAverage;
				lumaEnd2 -= lumaLocalAverage;

				// If the luma deltas at the current extremities are larger than the local gradient, we have reached the side of the edge
				bool reached1 = fabs(lumaEnd1) >= gradientScaled;
				bool reached2 = fabs(lumaEnd2) >= gradientScaled;
				bool reachedBoth = reached1 && reached2;

				// If the side is not reached, we continue to explore in this direction
				if (!reached1)
					uv1 -= offset;
				if (!reached2)
					uv2 += offset;

				if (!reachedBoth)
				{
					for (int i = 2; i < ITERATIONS; i++)
					{
						// If needed, read luma in 1st direction, compute delta
						if (!reached1)
						{
							lumaEnd1 = GetCachedIntensity(uv1);
							lumaEnd1 = lumaEnd1 - lumaLocalAverage;
						}

						// If needed, read luma in opposite direction, compute delta
						if (!reached2)
						{
							lumaEnd2 = GetCachedIntensity(uv2);
							lumaEnd2 = lumaEnd2 - lumaLocalAverage;
						}

						// If the luma deltas at the current extremities is larger than the local gradient, we have reached the side of the edge
						reached1 = fabs(lumaEnd1) >= gradientScaled;
						reached2 = fabs(lumaEnd2) >= gradientScaled;
						reachedBoth = reached1 && reached2;

						// If the side is not reached, we continue to explore in this direction, with a variable quality
						if (!reached1)
							uv1 -= offset * QUALITY[i];
						if (!reached2)
							uv2 += offset * QUALITY[i];

						// If both sides have been reached, stop the exploration
						if (reachedBoth) { break; }
					}
				}

				// Compute the distances to each extremity of the edge
				float distance1 = isHorizontal ? (x - uv1[0]) : (y - uv1[1]);
				float distance2 = isHorizontal ? (uv2[0] - x) : (uv2[1] - y);

				// In which direction is the extremity of the edge closer?
				bool isDirection1 = distance1 < distance2;
				float distanceFinal = fmin(distance1, distance2);

				// Length of the edge
				float edgeThickness = (distance1 + distance2);

				// UV offset: read in the direction of the closest side of the edge
				float pixelOffset = (-distanceFinal / edgeThickness) + 0.5F;

				// Is the luma at center smaller than the local average?
				bool isLumaCenterSmaller = lumaCenter < lumaLocalAverage;

				// If the luma at center is smaller than at its neighbour, the delta luma at each end should be positive (same variation)
				// (In the direction of the closer side of the edge.)
				bool correctVariation = ((isDirection1 ? lumaEnd1 : lumaEnd2) < 0.0F) != isLumaCenterSmaller;

				// If the luma variation is incorrect, do not offset
				float finalOffset = correctVariation ? pixelOffset : 0.0F;

				// Sub-pixel shifting
				// Full weighted average of the luma over the 3x3 neighborhood
				float lumaAverage = (1.0F / 12.0F) * (2.0F * (lumaDownUp + lumaLeftRight) + lumaLeftCorners + lumaRightCorners);

				// Ratio of the delta between the global average and the center luma, over the luma range in the 3x3 neighborhood
				float subPixelOffset1 = std::clamp(fabs(lumaAverage - lumaCenter) / lumaRange, 0.0F, 1.0F);
				float subPixelOffset2 = (-2.0F * subPixelOffset1 + 3.0F) * subPixelOffset1 * subPixelOffset1;

				// Compute a sub-pixel offset based on this delta
				float subPixelOffsetFinal = subPixelOffset2 * subPixelOffset2 * SUBPIXEL_QUALITY;

				// Pick the biggest of the two offsets
				finalOffset = fmax(finalOffset, subPixelOffsetFinal);

				// Compute the final UV coordinates
				zVEC2 finalUv = zVEC2(x, y);
				if (isHorizontal)
					finalUv[1] += finalOffset * stepLength;
				else
					finalUv[0] += finalOffset * stepLength;

				// Read the color at the new UV coordinates, and use it
				D3DCOLOR finalColor = GetPixel(ddsd, finalUv);

				g_d3dMutex.lock();
				((zCRnd_D3D*)zrenderer)->SetPixel(x, y, zCOLOR(RGB_GETRED(finalColor), RGB_GETGREEN(finalColor), RGB_GETBLUE(finalColor)));
				g_d3dMutex.unlock();
			}
		}
	}

	//0x0051D840 public: void __thiscall zCBspTree::Render(void)
	void __fastcall zCBspTree_Render(zCBspTree* _this, void* vtable);
	CInvoke<void(__thiscall*)(zCBspTree * _this)> Ivk_zCBspTree_Render(GothicMemoryLocations::zCBspTree::Render, &zCBspTree_Render);

	void __fastcall zCBspTree_Render(zCBspTree* _this, void* vtable)
	{
		Ivk_zCBspTree_Render(_this);

		HRESULT hr = S_OK;
		IDirectDrawSurface7* back_buffer = ((zCRnd_D3D*)zrenderer)->xd3d_pfrontBuffer;

		DDSURFACEDESC2 ddsd;
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(DDSURFACEDESC2);

		if (g_fxaa == NULL)
		{
			DDSURFACEDESC2 orig;
			ZeroMemory(&orig, sizeof(orig));
			orig.dwSize = sizeof(DDSURFACEDESC2);
			back_buffer->GetSurfaceDesc(&orig);
			g_width = orig.dwWidth;
			g_height = orig.dwHeight;

			ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT;
			ddsd.dwWidth = g_width;
			ddsd.dwHeight = g_height;

			hr = ((zCRnd_D3D*)zrenderer)->xd3d_pdd7->CreateSurface(&ddsd, &g_fxaa, NULL);
			if (hr != S_OK)
			{
				return;
			}
		}

		if (g_intensityCache == NULL)
		{
			g_intensityCache = new float* [g_height];
			for (int i = 0; i < g_height; ++i)
				g_intensityCache[i] = new float[g_width];
		}

		if (g_threadBuffers == NULL)
		{
			g_threadBuffers = new D3DCOLOR* [g_threadNum];
			for (int i = 0; i < g_threadNum; ++i)
				g_threadBuffers[i] = new D3DCOLOR[g_width];
		}

		auto t1 = std::chrono::high_resolution_clock::now();

		hr = g_fxaa->Blt(0, back_buffer, 0, DDBLT_WAIT, 0);
		if (hr != S_OK)
		{
			return;
		}

		hr = g_fxaa->Lock(NULL, &ddsd, DDLOCK_NOSYSLOCK | DDLOCK_READONLY | DDLOCK_WAIT, NULL);
		if (hr != S_OK)
		{
			return;
		}

		std::vector<std::thread> threads;

		for (int i = 0; i < g_threadNum; i++)
			threads.push_back(std::thread(CalculateIntensityCache, &ddsd, i, g_threadNum));
		for (auto& thread : threads)
			thread.join();
		threads.clear();
		
		for (int i = 0; i < g_threadNum; i++)
			threads.push_back(std::thread(ApplyFxaa, &ddsd, i, g_threadNum));
		for (auto& thread : threads)
			thread.join();
		threads.clear();

		auto t2 = std::chrono::high_resolution_clock::now();

		auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
		std::cout << ms_int.count() << " ms\n";

		g_fxaa->Unlock(NULL);
	}

	void Release()
	{
		if (g_fxaa != 0)
			g_fxaa->Release();
		for (int i = 0; i < g_height; ++i)
			delete[] g_intensityCache[i];
		delete[] g_intensityCache;
		for (int i = 0; i < g_threadNum; ++i)
			delete[] g_threadBuffers[i];
		delete[] g_threadBuffers;
	}

	/*
	// Idee 3: auf eine doppelt so große Surface rendern und am Ende runterskalieren
	int g_width = 1280;
	int g_height = 720;

	// 0x0070E530 private: int __thiscall zCRnd_D3D::XD3D_InitPerDX(long,int,int,int,int)
	int __fastcall zCRnd_D3D_XD3D_InitPerDX(zCRnd_D3D* _this, void* vtable, long flags, int x, int y, int bpp, int id);
	CInvoke<int(__thiscall*)(zCRnd_D3D * _this, long flags, int x, int y, int bpp, int id)> Ivk_zCRnd_D3D_XD3D_InitPerDX(GothicMemoryLocations::zCRnd_D3D::XD3D_InitPerDX, &zCRnd_D3D_XD3D_InitPerDX);

	int __fastcall zCRnd_D3D_XD3D_InitPerDX(zCRnd_D3D* _this, void* vtable, long flags, int x, int y, int bpp, int id)
	{
		return Ivk_zCRnd_D3D_XD3D_InitPerDX(_this, flags, x, y, bpp, id);
	}

	// 0x0070F250 private: int __thiscall zCRnd_D3D::XD3D_InitPort(int,int,int,int,int)
	int __fastcall zCRnd_D3D_XD3D_InitPort(zCRnd_D3D* _this, void* vtable, int x, int y, int bpp, int id, int mode);
	CInvoke<int(__thiscall*)(zCRnd_D3D * _this, int x, int y, int bpp, int id, int mode)> Ivk_zCRnd_D3D_XD3D_InitPort(GothicMemoryLocations::zCRnd_D3D::XD3D_InitPort, &zCRnd_D3D_XD3D_InitPort);

	int __fastcall zCRnd_D3D_XD3D_InitPort(zCRnd_D3D* _this, void* vtable, int x, int y, int bpp, int id, int mode)
	{
		return Ivk_zCRnd_D3D_XD3D_InitPort(_this, x, y, bpp, id, mode);
	}

	// 0x00711F00 private: int __thiscall zCRnd_D3D::XD3D_SetDevice(int,int,int,int,enum zTRnd_ScreenMode)
	int __fastcall zCRnd_D3D_XD3D_SetDevice(zCRnd_D3D* _this, void* vtable, int num, int x, int y, int bpp, zTRnd_ScreenMode mode);
	CInvoke<int(__thiscall*)(zCRnd_D3D * _this, int num, int x, int y, int bpp, zTRnd_ScreenMode mode)> Ivk_zCRnd_D3D_XD3D_SetDevice(GothicMemoryLocations::zCRnd_D3D::XD3D_SetDevice, &zCRnd_D3D_XD3D_SetDevice);

	int __fastcall zCRnd_D3D_XD3D_SetDevice(zCRnd_D3D* _this, void* vtable, int num, int x, int y, int bpp, zTRnd_ScreenMode mode)
	{
		return Ivk_zCRnd_D3D_XD3D_SetDevice(_this, num, x, y, bpp, mode);
	}

	// 0x0071FE30 private: int __thiscall zCRnd_D3D::XD3D_SetMode(int,int,int)
	int __fastcall zCRnd_D3D_XD3D_SetMode(zCRnd_D3D* _this, void* vtable, int x, int y, int bpp);
	CInvoke<int(__thiscall*)(zCRnd_D3D * _this, int x, int y, int bpp)> Ivk_zCRnd_D3D_XD3D_SetMode(GothicMemoryLocations::zCRnd_D3D::XD3D_SetMode, &zCRnd_D3D_XD3D_SetMode);

	int __fastcall zCRnd_D3D_XD3D_SetMode(zCRnd_D3D* _this, void* vtable, int x, int y, int bpp)
	{
		// beeinflusst die tatsächliche größe des fensters, führt aber zu darstellungsfehlern
		//x = g_width;
		//y = g_height;
		int result = Ivk_zCRnd_D3D_XD3D_SetMode(_this, x, y, bpp);
		return result;
	}

	// 0x00712C80 private: int __thiscall zCRnd_D3D::XD3D_CreateWin(int,int,int)
	int __fastcall zCRnd_D3D_XD3D_CreateWin(zCRnd_D3D* _this, void* vtable, int x, int y, int bpp);
	CInvoke<int(__thiscall*)(zCRnd_D3D * _this, int x, int y, int bpp)> Ivk_zCRnd_D3D_XD3D_CreateWin(GothicMemoryLocations::zCRnd_D3D::XD3D_CreateWin, &zCRnd_D3D_XD3D_CreateWin);

	int __fastcall zCRnd_D3D_XD3D_CreateWin(zCRnd_D3D* _this, void* vtable, int x, int y, int bpp)
	{
		// beeinflusst darstellungsfläche des menüs, führt aber zu darstellungsfehlern
		//x = g_width;
		//y = g_height;
		return Ivk_zCRnd_D3D_XD3D_CreateWin(_this, x, y, bpp);
	}

	// 0x00702180 public: static void __cdecl zCView::SetMode(int,int,int,struct HWND__ * *)
	static void __cdecl zCView_SetMode(int x, int y, int bpp, HWND* initContextHandle);
	CInvoke<void(__cdecl*)(int x, int y, int bpp, HWND * initContextHandle)> Ivk_zCView_SetMode(GothicMemoryLocations::zCView::SetMode, &zCView_SetMode);

	static void __cdecl zCView_SetMode(int x, int y, int bpp, HWND* initContextHandle)
	{
		// global galaktisch, überschreibt Werte aus INI/CMD, läuft
		x = g_width * 2;
		y = g_height * 2;
		Ivk_zCView_SetMode(x, y, bpp, initContextHandle);
	}

	// 0x00717150 public: virtual void __thiscall zCRnd_D3D::EndFrame(void)
	void __fastcall zCRnd_D3D_EndFrame(zCRnd_D3D* _this, void* vtable);
	CInvoke<void(__thiscall*)(zCRnd_D3D * _this)> Ivk_zCRnd_D3D_EndFrame(GothicMemoryLocations::zCRnd_D3D::EndFrame, &zCRnd_D3D_EndFrame);

	void __fastcall zCRnd_D3D_EndFrame(zCRnd_D3D* _this, void* vtable)
	{
		HRESULT hr = S_OK;
		IDirectDrawSurface7* back_buffer = _this->xd3d_pfrontBuffer;

		RECT r = RECT();
		r.left = 0;
		r.top = 0;
		r.right = g_width;
		r.bottom = g_height;

		hr = back_buffer->Blt(&r, back_buffer, 0, DDBLT_WAIT, 0);
		if (hr != S_OK)
		{
			Ivk_zCRnd_D3D_EndFrame(_this);
			return;
		}

		Ivk_zCRnd_D3D_EndFrame(_this);
	}
	*/

	/*
	// Idee 4: Up- and Downscale
	IDirectDrawSurface7* fxaa = 0;

	//0x0051D840 public: void __thiscall zCBspTree::Render(void)
	void __fastcall zCBspTree_Render(zCBspTree* _this, void* vtable);
	CInvoke<void(__thiscall*)(zCBspTree * _this)> Ivk_zCBspTree_Render(GothicMemoryLocations::zCBspTree::Render, &zCBspTree_Render);

	void __fastcall zCBspTree_Render(zCBspTree* _this, void* vtable)
	{
		Ivk_zCBspTree_Render(_this);

		if (fxaa == 0)
			return;

		IDirectDrawSurface7* back_buffer = ((zCRnd_D3D*)zrenderer)->xd3d_pfrontBuffer;
		fxaa->Blt(0, back_buffer, 0, DDBLT_WAIT, 0);
		back_buffer->Blt(0, fxaa, 0, DDBLT_WAIT, 0);
	}

	// 0x00717150 public: virtual void __thiscall zCRnd_D3D::EndFrame(void)
	void __fastcall zCRnd_D3D_EndFrame(zCRnd_D3D* _this, void* vtable);
	CInvoke<void(__thiscall*)(zCRnd_D3D * _this)> Ivk_zCRnd_D3D_EndFrame(GothicMemoryLocations::zCRnd_D3D::EndFrame, &zCRnd_D3D_EndFrame);

	void __fastcall zCRnd_D3D_EndFrame(zCRnd_D3D* _this, void* vtable)
	{
		HRESULT hr = S_OK;
		IDirectDrawSurface7* back_buffer = _this->xd3d_pfrontBuffer;

		if (fxaa == 0)
		{
			DDSURFACEDESC2 orig;
			ZeroMemory(&orig, sizeof(orig));
			orig.dwSize = sizeof(DDSURFACEDESC2);
			back_buffer->GetSurfaceDesc(&orig);

			DDSURFACEDESC2 ddsd;
			ZeroMemory(&ddsd, sizeof(ddsd));
			ddsd.dwSize = sizeof(DDSURFACEDESC2);
			ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT;
			ddsd.dwWidth = orig.dwWidth * 1.25;
			ddsd.dwHeight = orig.dwHeight * 1.25;

			hr = _this->xd3d_pdd7->CreateSurface(&ddsd, &fxaa, NULL);
			if (hr != S_OK)
			{
				Ivk_zCRnd_D3D_EndFrame(_this);
				return;
			}
		}

		Ivk_zCRnd_D3D_EndFrame(_this);
	}
	*/
}
