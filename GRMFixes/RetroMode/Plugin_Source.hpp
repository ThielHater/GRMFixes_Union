// Source file
// Multi-platform file can not have `#pragma once`!!!
// For compile plugin in Multiplatform mode - select 'Debug' or 'Release' configuration.

#include "Plugin_Header.h"

namespace NAMESPACE
{
	bool g_initialized = false;
	bool g_renderingItem = false;
	int g_pixelationFactor = 6;
	int g_bitsPerChannel = 5;
	int g_width = 0;
	int g_height = 0;
	IDirectDrawSurface7* g_retroSurface = NULL;
	int g_numThreads = fmax(1, std::thread::hardware_concurrency() - 1);

	// 0x00672F70 public: void __thiscall oCItem::RenderItem(class zCWorld *,class zCViewBase *,float)
	void __fastcall oCItem_RenderItem(oCItem* _this, void* vtable, zCWorld* pWorld, zCViewBase* pView, float angle);
	CInvoke<void(__thiscall*)(oCItem* _this, zCWorld* pWorld, zCViewBase* pView, float angle)> Ivk_oCItem_RenderItem(GothicMemoryLocations::oCItem::RenderItem, &oCItem_RenderItem);

	// 0x005F3EC0 public: void __thiscall zCWorld::Render(class zCCamera &)
	void __fastcall zCWorld_Render(zCWorld* _this, void* vtable, zCCamera& cam);
	CInvoke<void(__thiscall*)(zCWorld* _this, zCCamera& cam)> Ivk_zCWorld_Render(GothicMemoryLocations::zCWorld::Render, &zCWorld_Render);

	// 0x004DE2B0 public: void __thiscall zCSndSys_MSS::InitializeMSS(void)
	void __fastcall zCSndSys_MSS_InitializeMSS(zCSndSys_MSS* _this, void* vtable);
	CInvoke<void(__thiscall*)(zCSndSys_MSS* _this)> Ivk_zCSndSys_MSS_InitializeMSS(GothicMemoryLocations::zCSndSys_MSS::InitializeMSS, &zCSndSys_MSS_InitializeMSS);

	void ReplaceCodeBytes(const char* bytes, int numBytes, unsigned int addr)
	{
		// Unprotect the given code-area
		DWORD dwProtect;
		VirtualProtect((void*)addr, numBytes, PAGE_EXECUTE_READWRITE, &dwProtect);

		// Overwrite the code
		memcpy((void*)addr, bytes, numBytes);
	}

	void NearestNeighborScale(LPDIRECTDRAWSURFACE7 srcSurface, DDSURFACEDESC2 srcDesc, LPDIRECTDRAWSURFACE7 destSurface, DDSURFACEDESC2 destDesc, int srcWidth, int srcHeight, int destWidth, int destHeight, int startY, int endY, bool fakePalette)
	{
		float scaleX = (float)srcWidth / destWidth;
		float scaleY = (float)srcHeight / destHeight;
		int bitShift = 8 - g_bitsPerChannel;

		for (int y = startY; y < endY; ++y)
		{
			for (int x = 0; x < destWidth; ++x)
			{
				int srcX = (int)(x * scaleX);
				int srcY = (int)(y * scaleY);

				srcX = fmin(srcX, srcWidth - 1);
				srcY = fmin(srcY, srcHeight - 1);

				DWORD* srcPixel = (DWORD*)((BYTE*)srcDesc.lpSurface + srcY * srcDesc.lPitch + srcX * sizeof(DWORD));
				DWORD* destPixel = (DWORD*)((BYTE*)destDesc.lpSurface + y * destDesc.lPitch + x * sizeof(DWORD));

				if (fakePalette && bitShift > 0)
				{
					DWORD srcColor = *srcPixel;
					BYTE alpha = (srcColor >> 24) & 0xFF;
					BYTE red = (srcColor >> 16) & 0xFF;
					BYTE green = (srcColor >> 8) & 0xFF;
					BYTE blue = srcColor & 0xFF;

					red = (red >> bitShift) << bitShift;
					green = (green >> bitShift) << bitShift;
					blue = (blue >> bitShift) << bitShift;
					DWORD destColor = (alpha << 24) | (red << 16) | (green << 8) | blue;

					*destPixel = destColor;
				}
				else
				{
					*destPixel = *srcPixel;
				}
			}
		}
	}

	void MultithreadedNearestNeighborScale(LPDIRECTDRAWSURFACE7 srcSurface, LPDIRECTDRAWSURFACE7 destSurface, int srcWidth, int srcHeight, int destWidth, int destHeight, bool fakePalette)
	{
		std::vector<std::thread> threads;
		int rowsPerThread = destHeight / g_numThreads;

		DDSURFACEDESC2 srcDesc, destDesc;
		srcDesc.dwSize = sizeof(srcDesc);
		destDesc.dwSize = sizeof(destDesc);

		srcSurface->Lock(NULL, &srcDesc, DDLOCK_WAIT, NULL);
		destSurface->Lock(NULL, &destDesc, DDLOCK_WAIT, NULL);

		for (int i = 0; i < g_numThreads; ++i)
		{
			int startY = i * rowsPerThread;
			int endY = (i == g_numThreads - 1) ? destHeight : startY + rowsPerThread;
			threads.emplace_back([=]()
			{
				NearestNeighborScale(srcSurface, srcDesc, destSurface, destDesc, srcWidth, srcHeight, destWidth, destHeight, startY, endY, fakePalette);
			});
		}

		for (auto& thread : threads)
		{
			thread.join();
		}

		srcSurface->Unlock(NULL);
		destSurface->Unlock(NULL);
	}

	void __fastcall oCItem_RenderItem(oCItem* _this, void* vtable, zCWorld* pWorld, zCViewBase* pView, float angle)
	{
		g_renderingItem = true;
		Ivk_oCItem_RenderItem(_this, pWorld, pView, angle);
		g_renderingItem = false;
	}

	void __fastcall zCWorld_Render(zCWorld* _this, void* vtable, zCCamera& cam)
	{
		Ivk_zCWorld_Render(_this, cam);

		if (g_renderingItem)
			return;

		if (!g_initialized)
		{
			char exe[MAX_PATH], drive[MAX_PATH], dir[MAX_PATH];
			GetModuleFileName(NULL, exe, MAX_PATH);
			_splitpath(exe, drive, dir, NULL, NULL);

			char iniFile[MAX_PATH];
			strcpy_s(iniFile, drive);
			strcat_s(iniFile, dir);
			strcat_s(iniFile, "RetroMode.ini");

			g_pixelationFactor = GetPrivateProfileInt("SETTINGS", "pixelationFactor", g_pixelationFactor, iniFile);
			g_bitsPerChannel = GetPrivateProfileInt("SETTINGS", "bitsPerChannel", g_bitsPerChannel, iniFile);

			DDSURFACEDESC2 ddsdBackBuffer, ddsdHalfSizedSurface;

			ddsdBackBuffer.dwSize = sizeof(ddsdBackBuffer);

			((zCRnd_D3D*)zrenderer)->xd3d_pfrontBuffer->Lock(NULL, &ddsdBackBuffer, DDLOCK_WAIT, NULL);
			((zCRnd_D3D*)zrenderer)->xd3d_pfrontBuffer->Unlock(NULL);
			g_width = ddsdBackBuffer.dwWidth;
			g_height = ddsdBackBuffer.dwHeight;

			ZeroMemory(&ddsdHalfSizedSurface, sizeof(ddsdHalfSizedSurface));
			ddsdHalfSizedSurface.dwSize = sizeof(ddsdHalfSizedSurface);
			ddsdHalfSizedSurface.dwFlags = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH;
			ddsdHalfSizedSurface.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_SYSTEMMEMORY;
			ddsdHalfSizedSurface.dwWidth = g_width / g_pixelationFactor;
			ddsdHalfSizedSurface.dwHeight = g_height / g_pixelationFactor;

			zCRnd_D3D::xd3d_pdd7->CreateSurface(&ddsdHalfSizedSurface, &g_retroSurface, NULL);

			g_initialized = true;
		}

		if (g_initialized)
		{
			MultithreadedNearestNeighborScale(((zCRnd_D3D*)zrenderer)->xd3d_pfrontBuffer, g_retroSurface, g_width, g_height, g_width / g_pixelationFactor, g_height / g_pixelationFactor, true);
			MultithreadedNearestNeighborScale(g_retroSurface, ((zCRnd_D3D*)zrenderer)->xd3d_pfrontBuffer, g_width / g_pixelationFactor, g_height / g_pixelationFactor, g_width, g_height, false);
		}
	}

	void __fastcall zCSndSys_MSS_InitializeMSS(zCSndSys_MSS* _this, void* vtable)
	{
		// force sample rate to 11025 Hz
		const char* mov = "\xB8\x11\x2B\x00\x00\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90"; // mov eax, 00002B11
		ReplaceCodeBytes(mov, 15, GothicMemoryLocations::zCSndSys_MSS::ASM_SET_SAMPLERATE);

		Ivk_zCSndSys_MSS_InitializeMSS(_this);
	}
}
