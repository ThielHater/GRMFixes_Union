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
	IDirectDrawSurface7* fxaa = 0;
	char* surfaceData = 0;
	float* intensityCache = 0;

	inline float GetIntensity(WORD red, WORD green, WORD blue)
	{
		float intens = (0.299F * red + 0.587F * green + 0.114F * blue);
		return intens;
	}

	// 0x00717150 public: virtual void __thiscall zCRnd_D3D::EndFrame(void)
	void __fastcall zCRnd_D3D_EndFrame(zCRnd_D3D* _this, void* vtable);
	CInvoke<void(__thiscall*)(zCRnd_D3D * _this)> Ivk_zCRnd_D3D_EndFrame(GothicMemoryLocations::zCRnd_D3D::EndFrame, &zCRnd_D3D_EndFrame);

	void __fastcall zCRnd_D3D_EndFrame(zCRnd_D3D* _this, void* vtable)
	{
		HRESULT hr = S_OK;
		IDirectDrawSurface7* back_buffer = _this->xd3d_pfrontBuffer;

		DDSURFACEDESC2 ddsd;
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(DDSURFACEDESC2);

		if (fxaa == 0 || fxaa->IsLost())
		{
			DDSURFACEDESC2 orig;
			ZeroMemory(&orig, sizeof(orig));
			orig.dwSize = sizeof(DDSURFACEDESC2);
			back_buffer->GetSurfaceDesc(&orig);

			ddsd.dwFlags = DDSD_WIDTH | DDSD_HEIGHT;
			ddsd.dwWidth = orig.dwWidth;
			ddsd.dwHeight = orig.dwHeight;

			hr = _this->xd3d_pdd7->CreateSurface(&ddsd, &fxaa, NULL);
			if (hr != S_OK)
			{
				Ivk_zCRnd_D3D_EndFrame(_this);
				return;
			}
		}

		hr = fxaa->Blt(0, back_buffer, 0, DDBLT_WAIT, 0);
		if (hr != S_OK)
		{
			Ivk_zCRnd_D3D_EndFrame(_this);
			return;
		}

		fxaa->Lock(NULL, &ddsd, DDLOCK_NOSYSLOCK | DDLOCK_READONLY, NULL);

		if (surfaceData == 0)
		{
			surfaceData = new char[ddsd.dwHeight * ddsd.lPitch];
		}

		//memcpy(surfaceData, ddsd.lpSurface, ddsd.dwHeight * ddsd.lPitch);

		if (intensityCache == 0)
		{
			intensityCache = new float[ddsd.dwWidth * ddsd.dwHeight];
		}

		int pixelWidth = (ddsd.ddpfPixelFormat.dwRGBBitCount / 8);

		for (int x = 0; x < ddsd.dwWidth; x++)
		{
			for (int y = 0; y < ddsd.dwHeight; y++)
			{
				DWORD offset = y * ddsd.lPitch + x * pixelWidth;
				D3DCOLOR color = *((LPDWORD)((DWORD)ddsd.lpSurface + offset)); // surfaceData <=> ddsd.lpSurface
				intensityCache[y * ddsd.dwWidth + x] = GetIntensity(RGB_GETRED(color), RGB_GETGREEN(color), RGB_GETBLUE(color));
			}
		}

		zCOLOR red = zCOLOR(255, 0, 0);

		for (int x = 1; x < ddsd.dwWidth - 1; x++)
		{
			for (int y = 1; y < ddsd.dwHeight - 1; y++)
			{
				// rgb2luma = GetIntensity / textureOffset = GetPixelColor
				float lumaCenter = intensityCache[y * ddsd.dwWidth + x];

				float lumaDown = intensityCache[(y - 1) * ddsd.dwWidth + x];
				float lumaUp = intensityCache[(y + 1) * ddsd.dwWidth + x];
				float lumaLeft = intensityCache[y * ddsd.dwWidth + (x - 1)];
				float lumaRight = intensityCache[y * ddsd.dwWidth + (x + 1)];

				float lumaMin = fmin(lumaCenter, fmin(fmin(lumaDown, lumaUp), fmin(lumaLeft, lumaRight)));
				float lumaMax = fmax(lumaCenter, fmax(fmax(lumaDown, lumaUp), fmax(lumaLeft, lumaRight)));

				float lumaRange = lumaMax - lumaMin;

				if (lumaRange > 32)
				{
					_this->SetPixel(x, y, red);
					//SetPixelColor(x, y, red);
				}
			}
		}

		fxaa->Unlock(NULL);

		Ivk_zCRnd_D3D_EndFrame(_this);
	}

	void ReleaseFxaaSurface()
	{
		if (fxaa != 0)
			fxaa->Release();
		delete surfaceData;
		delete[] intensityCache;
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
