// Source file
// Multi-platform file can not have `#pragma once`!!!
// For compile plugin in Multiplatform mode - select 'Debug' or 'Release' configuration.

#include "Plugin_Header.h"

namespace NAMESPACE
{
	// 0x00714B60 public: virtual void __thiscall zCRnd_D3D::DrawPoly(class zCPolygon *)
	void __fastcall zCRnd_D3D_DrawPoly(zCRnd_D3D* _this, void* vtable, zCPolygon* polygon);
	CInvoke<void(__thiscall*)(zCRnd_D3D * _this, zCPolygon * polygon)> Ivk_zCRnd_D3D_DrawPoly(GothicMemoryLocations::zCRnd_D3D::DrawPoly, &zCRnd_D3D_DrawPoly);

	int g_ignoreDaylightTexturesNum = 0;
	zSTRING* g_ignoreDaylightTextures;

	void ReadIgnoreDaylightTextures()
	{
		g_ignoreDaylightTexturesNum = 2;
		g_ignoreDaylightTextures = new zSTRING[g_ignoreDaylightTexturesNum];
		g_ignoreDaylightTextures[0] = zSTRING("TORC_A0.TGA");
		g_ignoreDaylightTextures[1] = zSTRING("CFIR_A0.TGA");
		// delete[] g_ignoreDaylightTexturesNum;
	}

	void __fastcall zCRnd_D3D_DrawPoly(zCRnd_D3D* _this, void* vtable, zCPolygon* polygon)
	{
		zSTRING texName = polygon->material->GetTextureName();

		for (int i = 0; i < g_ignoreDaylightTexturesNum; i++)
		{
			if (texName == g_ignoreDaylightTextures[i])
			{
				zCOLOR dayLightColor(255, 255, 255, 255);
				for (int v = 0; v < polygon->numClipVert; v++)
					polygon->clipFeat[v]->lightDyn = dayLightColor;
				break;
			}
		}

		Ivk_zCRnd_D3D_DrawPoly(_this, polygon);
	}
}
