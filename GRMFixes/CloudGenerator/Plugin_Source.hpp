// Source file
// Multi-platform file can not have `#pragma once`!!!
// For compile plugin in Multiplatform mode - select 'Debug' or 'Release' configuration.

#include "Plugin_Header.h"
#include "WhiteNoiseGenerator.h"
#include "PerlinNoiseGenerator.h"
#include "ColorHelper.h"
#include <time.h>

namespace NAMESPACE
{
	// TODO: Textur austauschen statt generieren?
	// TODO: Skalierung beeinflussen

	#pragma region Hooks
	//0x005BBB50 public: void __thiscall zCSkyState::PresetDay1(void)
	void __fastcall zCSkyState_PresetDay1(zCSkyState* _this, void* vtable);
	CInvoke<void(__thiscall*)(zCSkyState* _this)> Ivk_zCSkyState_PresetDay1(GothicMemoryLocations::zCSkyState::PresetDay1, &zCSkyState_PresetDay1);

	//0x005BBCD0 public: void __thiscall zCSkyState::PresetDay2(void)
	void __fastcall zCSkyState_PresetDay2(zCSkyState* _this, void* vtable);
	CInvoke<void(__thiscall*)(zCSkyState* _this)> Ivk_zCSkyState_PresetDay2(GothicMemoryLocations::zCSkyState::PresetDay2, &zCSkyState_PresetDay2);

	//0x005BBDA0 public: void __thiscall zCSkyState::PresetEvening(void)
	void __fastcall zCSkyState_PresetEvening(zCSkyState* _this, void* vtable);
	CInvoke<void(__thiscall*)(zCSkyState* _this)> Ivk_zCSkyState_PresetEvening(GothicMemoryLocations::zCSkyState::PresetEvening, &zCSkyState_PresetEvening);

	//0x005BBE70 public: void __thiscall zCSkyState::PresetNight0(void)
	void __fastcall zCSkyState_PresetNight0(zCSkyState* _this, void* vtable);
	CInvoke<void(__thiscall*)(zCSkyState* _this)> Ivk_zCSkyState_PresetNight0(GothicMemoryLocations::zCSkyState::PresetNight0, &zCSkyState_PresetNight0);

	//0x005BC070 public: void __thiscall zCSkyState::PresetNight1(void)
	void __fastcall zCSkyState_PresetNight1(zCSkyState* _this, void* vtable);
	CInvoke<void(__thiscall*)(zCSkyState* _this)> Ivk_zCSkyState_PresetNight1(GothicMemoryLocations::zCSkyState::PresetNight1, &zCSkyState_PresetNight1);

	//0x005BC1F0 public: void __thiscall zCSkyState::PresetNight2(void)
	void __fastcall zCSkyState_PresetNight2(zCSkyState* _this, void* vtable);
	CInvoke<void(__thiscall*)(zCSkyState* _this)> Ivk_zCSkyState_PresetNight2(GothicMemoryLocations::zCSkyState::PresetNight2, &zCSkyState_PresetNight2);

	//0x005BC370 public: void __thiscall zCSkyState::PresetDawn(void)
	void __fastcall zCSkyState_PresetDawn(zCSkyState* _this, void* vtable);
	CInvoke<void(__thiscall*)(zCSkyState* _this)> Ivk_zCSkyState_PresetDawn(GothicMemoryLocations::zCSkyState::PresetDawn, &zCSkyState_PresetDawn);

	//0x005BB9B0 public: void __thiscall zCSkyState::PresetDay0(void)
	void __fastcall zCSkyState_PresetDay0(zCSkyState* _this, void* vtable);
	CInvoke<void(__thiscall*)(zCSkyState* _this)> Ivk_zCSkyState_PresetDay0(GothicMemoryLocations::zCSkyState::PresetDay0, &zCSkyState_PresetDay0);

	// 0x005BDEC0 private: void __thiscall zCSkyControler_Outdoor::CreateDefault(void)
	void __fastcall zCSkyControler_Outdoor_CreateDefault(zCSkyControler_Outdoor* _this, void* vtable);
	CInvoke<void(__thiscall*)(zCSkyControler_Outdoor* _this)> Ivk_zCSkyControler_Outdoor_CreateDefault(GothicMemoryLocations::zCSkyControler_Outdoor::CreateDefault, &zCSkyControler_Outdoor_CreateDefault);

	//0x005BCFF0 private: void __thiscall zCSkyControler_Outdoor::ApplyStateTexToLayer(int,int)
	void __fastcall zCSkyControler_Outdoor_ApplyStateTexToLayer(zCSkyControler_Outdoor* _this, void* vtable, int state, int layer);
	CInvoke<void(__thiscall*)(zCSkyControler_Outdoor* _this, int state, int layer)> Ivk_zCSkyControler_Outdoor_ApplyStateTexToLayer(GothicMemoryLocations::zCSkyControler_Outdoor::ApplyStateTexToLayer, &zCSkyControler_Outdoor_ApplyStateTexToLayer);

	// 0x007185C0 private: int __thiscall zCRnd_D3D::XD3D_SetRenderState(enum _D3DRENDERSTATETYPE,unsigned long)
	//int __fastcall zCRnd_D3D_XD3D_SetRenderState(zCRnd_D3D* _this, void* vtable, zTRnd_TextureStageState state, DWORD value);
	//CInvoke<int(__thiscall*)(zCRnd_D3D* _this, zTRnd_TextureStageState state, DWORD value)> Ivk_zCRnd_D3D_XD3D_SetRenderState(GothicMemoryLocations::zCRnd_D3D::XD3D_SetRenderState, &zCRnd_D3D_XD3D_SetRenderState);

	// 0x00718270 public: virtual int __thiscall zCRnd_D3D::SetTextureStageState(unsigned long,enum zTRnd_TextureStageState,unsigned long)
	int __fastcall zCRnd_D3D_XD3D_SetTextureStageState(zCRnd_D3D* _this, void* vtable, zDWORD stage, zTRnd_TextureStageState state, zDWORD value);
	CInvoke<int(__thiscall*)(zCRnd_D3D* _this, zDWORD stage, zTRnd_TextureStageState state, zDWORD value)> Ivk_zCRnd_D3D_XD3D_SetTextureStageState(GothicMemoryLocations::zCRnd_D3D::XD3D_SetTextureStageState, &zCRnd_D3D_XD3D_SetTextureStageState);

	// 0x005BFC30 private: void __thiscall zCSkyControler_Outdoor::RenderSkyPlane(void)
	void __fastcall zCSkyControler_Outdoor_RenderSkyPlane(zCSkyControler_Outdoor* _this, void* vtable);
	CInvoke<void(__thiscall*)(zCSkyControler_Outdoor* _this)> Ivk_zCSkyControler_Outdoor_RenderSkyPlane(GothicMemoryLocations::zCSkyControler_Outdoor::RenderSkyPlane, &zCSkyControler_Outdoor_RenderSkyPlane);

	// 0x005C7D10 public: virtual int __thiscall zCTextureFileFormatInternal::LoadTexture(class zFILE &,class zCTextureExchange *)
	int __fastcall zCTextureFileFormatInternal_LoadTexture(zCTextureFileFormatInternal* _this, void* vtable, zFILE& inFile, zCTextureExchange* texFile);
	CInvoke<int(__thiscall*)(zCTextureFileFormatInternal* _this, zFILE& inFile, zCTextureExchange* texFile)> Ivk_zCTextureFileFormatInternal_LoadTexture(GothicMemoryLocations::zCTextureFileFormatInternal::LoadTexture, &zCTextureFileFormatInternal_LoadTexture);
#pragma endregion

	void __fastcall zCSkyState_PresetDay1(zCSkyState* _this, void* vtable)
	{
		Ivk_zCSkyState_PresetDay1(_this);
		// time = 0.0;
		// polyColor = zVEC3(255,250,235);
		// fogColor = zVEC3(120,140,180);
		// domeColor1 = zVEC3(255,255,255);
		// domeColor0 = zVEC3(120,140,180);
		// fogDist = 0.05;
		// sunOn = TRUE;
		// cloudShadowOn = FALSE
		// layer[0].texName = "SKYDAY_LAYER1_A0.TGA";
		// layer[0].texAlpha = 215;
		// layer[1].texName = "";
		// layer[1].texAlpha = 255;
	}

	void __fastcall zCSkyState_PresetDay2(zCSkyState* _this, void* vtable)
	{
		Ivk_zCSkyState_PresetDay2(_this);
		// time = 0.25;
		// polyColor = zVEC3(255,250,235);
		// fogColor = zVEC3(120,140,180);
		// domeColor1 = zVEC3(255,255,255);
		// domeColor0 = zVEC3(120,140,180);
		// fogDist = 0.05;
		// sunOn = TRUE;
		// cloudShadowOn = FALSE
		// layer[0].texName = "";
		// layer[0].texAlpha = 0;
		// layer[1].texName = "";
		// layer[1].texAlpha = 255;
	}

	void __fastcall zCSkyState_PresetEvening(zCSkyState* _this, void* vtable)
	{
		Ivk_zCSkyState_PresetEvening(_this);
		// time = 0.3;
		// polyColor = zVEC3(255,185,170);
		// fogColor = zVEC3(180,75,60);
		// domeColor1 = zVEC3(255,255,255);
		// domeColor0 = zVEC3(180,75,65);
		// fogDist = 0.2;
		// sunOn = TRUE;
		// cloudShadowOn = FALSE
		// layer[0].texName = "";
		// layer[0].texAlpha = 128;
		// layer[1].texName = "";
		// layer[1].texAlpha = 128;
	}

	void __fastcall zCSkyState_PresetNight0(zCSkyState* _this, void* vtable)
	{
		Ivk_zCSkyState_PresetNight0(_this);
		// time = 0.35;
		// polyColor = zVEC3(105,105,195);
		// fogColor = zVEC3(20,20,60);
		// domeColor1 = zVEC3(55,55,155);
		// domeColor0 = zVEC3(20,20,60);
		// fogDist = 0.1;
		// sunOn = FALSE;
		// cloudShadowOn = FALSE
		// layer[0].texName = "SKYNIGHT_LAYER0_A0.TGA";
		// layer[0].texAlpha = 255;
		// layer[1].texName = "SKYNIGHT_LAYER1_A0.TGA";
		// layer[1].texAlpha = 0;
	}

	void __fastcall zCSkyState_PresetNight1(zCSkyState* _this, void* vtable)
	{
		Ivk_zCSkyState_PresetNight1(_this);
		// time = 0.5;
		// polyColor = zVEC3(40,60,210);
		// fogColor = zVEC3(5,5,20);
		// domeColor1 = zVEC3(55,55,155);
		// domeColor0 = zVEC3(5,5,20);
		// fogDist = 0.05;
		// sunOn = FALSE;
		// cloudShadowOn = FALSE
		// layer[0].texName = "";
		// layer[0].texAlpha = 255;
		// layer[1].texName = "SKYNIGHT_LAYER1_A0.TGA";
		// layer[1].texAlpha = 215;
	}

	void __fastcall zCSkyState_PresetNight2(zCSkyState* _this, void* vtable)
	{
		Ivk_zCSkyState_PresetNight2(_this);
		// time = 0.65;
		// polyColor = zVEC3(40,60,210);
		// fogColor = zVEC3(5,5,20);
		// domeColor1 = zVEC3(55,55,155);
		// domeColor0 = zVEC3(5,5,20);
		// fogDist = 0.2;
		// sunOn = FALSE;
		// cloudShadowOn = FALSE
		// layer[0].texName = "";
		// layer[0].texAlpha = 255;
		// layer[1].texName = "SKYNIGHT_LAYER0_A0.TGA";
		// layer[1].texAlpha = 0;
	}

	void __fastcall zCSkyState_PresetDawn(zCSkyState* _this, void* vtable)
	{
		Ivk_zCSkyState_PresetDawn(_this);
		// time = 0.7;
		// polyColor = zVEC3(190,160,255);
		// fogColor = zVEC3(80,60,105);
		// domeColor1 = zVEC3(255,255,255);
		// domeColor0 = zVEC3(80,60,105);
		// fogDist = 0.5;
		// sunOn = TRUE;
		// cloudShadowOn = FALSE
		// layer[0].texName = "";
		// layer[0].texAlpha = 128;
		// layer[1].texName = "";
		// layer[1].texAlpha = 128;
	}

	void __fastcall zCSkyState_PresetDay0(zCSkyState* _this, void* vtable)
	{
		Ivk_zCSkyState_PresetDay0(_this);
		// time = 0.75;
		// polyColor = zVEC3(255,250,235);
		// fogColor = zVEC3(120,140,180);
		// domeColor1 = zVEC3(255,255,255);
		// domeColor0 = zVEC3(120,140,180);
		// fogDist = 0.2;
		// sunOn = TRUE;
		// cloudShadowOn = FALSE
		// layer[0].texName = "SKYDAY_LAYER1_A0.TGA";
		// layer[0].texAlpha = 0;
		// layer[1].texName = "SKYDAY_LAYER0_A0.TGA";
		// layer[1].texAlpha = 255;
	}

	int RataDie(int y, int m, int d)
	{
		if (m < 3) y--, m += 12;
		return 365 * y + y / 4 - y / 100 + y / 400 + (153 * m - 457) / 5 + d - 306;
	}

	bool moonChanged = false;

	// TODO: Code entfernen
	void AnalyzeHelper()
	{
		if (gameMan && gameMan->gameSession && gameMan->gameSession->world && gameMan->gameSession->world->activeSkyControler)
		{
			// Note: rainFX.outdoorRainFXWeight steuert Regenwolken, rainFX.timeStartRain und rainFX.timeStopRain steuern Regen
			zCSkyControler_Outdoor* skyController = (zCSkyControler_Outdoor*)gameMan->gameSession->world->skyControlerOutdoor;

			if (!moonChanged)
			{
				time_t t = time(0);
				tm* now = localtime(&t);
				int days = RataDie(now->tm_year + 1900, now->tm_mon + 1, now->tm_mday + 1) - RataDie(1900, 1, 1);
				float moonAge = fmod(days, 29.530588853);
				zSTRING texName;
				if (moonAge < 2.86)
					texName = "MOON_V0.TGA";
				else if (moonAge < 5.72)
					texName = "MOON_V1.TGA";
				else if (moonAge < 9.53)
					texName = "MOON_V2.TGA";
				else if (moonAge < 12.86)
					texName = "MOON_V3.TGA";
				else if (moonAge < 15.24)
					texName = "MOON_V4.TGA";
				else if (moonAge < 17.62)
					texName = "MOON_V5.TGA";
				else if (moonAge < 20.96)
					texName = "MOON_V6.TGA";
				else if (moonAge < 24.29)
					texName = "MOON_V7.TGA";
				else if (moonAge < 27.63)
					texName = "MOON_V8.TGA";
				else if (moonAge < 29.54)
					texName = "MOON_V9.TGA";
				skyController->planets[1].mesh->SharePoly(0)->material->SetTexture(texName);
				//skyController->planets[1].mesh->SharePoly(0)->material->rndAlphaBlendFunc = zRND_ALPHA_FUNC_BLEND;
				skyController->planets[1].mesh->SharePoly(0)->material->color.alpha = 255;
				moonChanged = true;
			}
		}
	}

	zCMesh* CreateSkyDomeMesh(int numDomeLayers)
	{
		zCMesh* boxMesh = zCMesh::Load("skyDome_layer" + zSTRING(numDomeLayers) + ".3ds");

		if (!boxMesh)
			return 0;
		
		float radius = 10.0f;
		float scaleRatio = radius / boxMesh->GetBBox3D().GetSphere3D().radius;
		boxMesh->Scale(scaleRatio, scaleRatio, scaleRatio);
		
		zCMaterial* material = zNEW(zCMaterial);
		material->smooth = TRUE;
		boxMesh->SetMaterial(material);
		zRELEASE(material);

		return boxMesh;
	}
	
	void __fastcall zCSkyControler_Outdoor_CreateDefault(zCSkyControler_Outdoor* _this, void* vtable)
	{
		Ivk_zCSkyControler_Outdoor_CreateDefault(_this);

		/*
		for (int i = 0; i < 8; i++)
		{
			_this->stateList[i]->layer[0].texName = "Default.tga";
		}
		*/

		_this->skyLayer[0].skyPolyMesh = CreateSkyDomeMesh(1);
		_this->skyLayer[0].skyPoly = _this->skyLayer[0].skyPolyMesh->SharePoly(0);
		_this->skyLayer[1].skyPolyMesh = CreateSkyDomeMesh(2);
		_this->skyLayer[1].skyPoly = _this->skyLayer[1].skyPolyMesh->SharePoly(0);
	}

	void __fastcall zCSkyControler_Outdoor_ApplyStateTexToLayer(zCSkyControler_Outdoor* _this, void* vtable, int state, int layer)
	{
		// Note: wenn _this->dayCounter nicht festgehalen wird, ändern sich Textur und Nebelfarbe
		//Ivk_zCSkyControler_Outdoor_ApplyStateTexToLayer(_this, state, layer);

		zCTexture* tex = _this->stateList[state]->layer[layer].tex;

		if (!tex)
		{
			zSTRING texName = _this->stateList[state]->layer[layer].texName;
			tex = zCTexture::Load(texName, 0);
			_this->stateList[state]->layer[layer].tex = tex;
		}

		_this->skyLayer[layer].skyPolyMesh->SharePoly(0)->material->SetTexture(tex);
	}

	bool g_renderingSky = false;

	int __fastcall zCRnd_D3D_XD3D_SetTextureStageState(zCRnd_D3D* _this, void* vtable, zDWORD stage, zTRnd_TextureStageState state, zDWORD value)
	{
		int result = Ivk_zCRnd_D3D_XD3D_SetTextureStageState(_this, stage, state, value);

		if (g_renderingSky)
		{
			result = Ivk_zCRnd_D3D_XD3D_SetTextureStageState(_this, stage, zRND_TSS_COLOROP, zRND_TOP_SELECTARG1);
			result = Ivk_zCRnd_D3D_XD3D_SetTextureStageState(_this, stage, zRND_TSS_COLORARG1, zRND_TA_TEXTURE);
			result = Ivk_zCRnd_D3D_XD3D_SetTextureStageState(_this, stage, zRND_TSS_COLORARG2, zRND_TA_DIFFUSE);
			result = Ivk_zCRnd_D3D_XD3D_SetTextureStageState(_this, stage, zRND_TSS_ALPHAOP, zRND_TOP_SELECTARG1);
			result = Ivk_zCRnd_D3D_XD3D_SetTextureStageState(_this, stage, zRND_TSS_ALPHAARG1, zRND_TA_DIFFUSE);
			result = Ivk_zCRnd_D3D_XD3D_SetTextureStageState(_this, stage, zRND_TSS_ALPHAARG2, zRND_TA_TEXTURE);
		}

		return result;
	}

	const zREAL TIME_KEY_0 = zREAL(0.00F);
	const zREAL TIME_KEY_1 = zREAL(0.25F);
	const zREAL TIME_KEY_2 = zREAL(0.30F);
	const zREAL TIME_KEY_3 = zREAL(0.35F);
	const zREAL TIME_KEY_4 = zREAL(0.50F);
	const zREAL TIME_KEY_5 = zREAL(0.65F);
	const zREAL TIME_KEY_6 = zREAL(0.70F);
	const zREAL TIME_KEY_7 = zREAL(0.75F);

	void RenderSkyLayer(zCSkyLayer* _this, zCSkyState* skyLayerState)
	{
		zCCamera* cam = zCCamera::activeCam;
		zCSkyControler_Outdoor* sky = (zCSkyControler_Outdoor*)gameMan->gameSession->world->skyControlerOutdoor;

		zBOOL night = (_this == &sky->skyLayer[0]) && (sky->masterTime >= TIME_KEY_1) && (sky->masterTime <= TIME_KEY_7);
		
		int	channel = 0;
		if (_this == &sky->skyLayer[1])
			channel = 1;

		zVEC3 c0 = night ? zVEC3(255, 255, 255) : sky->masterState.domeColor1;

		if ((sky->masterTime >= TIME_KEY_3) && (sky->masterTime <= TIME_KEY_5) && (channel == 1))
		{
			c0 = c0 + zVEC3(255, 255, 255);
			c0[0] *= 0.5F;
			c0[1] *= 0.5F;
			c0[2] *= 0.5F;
		}

		zCMaterial* mat = _this->skyPoly->material;
		if (_this->skyPoly->material->color.alpha >= 255)
			mat->rndAlphaBlendFunc = zRND_ALPHA_FUNC_NONE;
		else
			mat->rndAlphaBlendFunc = zRND_ALPHA_FUNC_BLEND;

		zCOLOR skyCol = zCOLOR(c0[0], c0[1], c0[2], 0);

		/*
		for (int i = 0; i < _this->skyPolyMesh->numPoly; i++)
		{
			zCPolygon* p = _this->skyPolyMesh->SharePoly(i);

			for (int j = 0; j < p->polyNumVert; j++)
			{
				zCVertFeature* f = p->feature[j];
				f->lightDyn = skyCol;
				f->lightDyn.alpha = 128;
			}
		}
		*/

		cam->camMatrixInv.SetTranslation(0);
		cam->camMatrix = cam->camMatrixInv.InverseLinTrafo();
		zCCamera::activeCamPos.n[VX] = cam->camMatrixInv[0][3];
		zCCamera::activeCamPos.n[VY] = cam->camMatrixInv[1][3];
		zCCamera::activeCamPos.n[VZ] = cam->camMatrixInv[2][3];
		cam->SetUpFrustum();

		zTRenderContext renderContext;
		memset((void*)&renderContext, 0, sizeof(renderContext));
		renderContext.clipFlags = -1;
		renderContext.cam = zCCamera::activeCam;

		_this->skyPolyMesh->Render(renderContext);
		zrenderer->FlushPolys();
	}

	void __fastcall zCSkyControler_Outdoor_RenderSkyPlane(zCSkyControler_Outdoor* _this, void* vtable)
	{
		g_renderingSky = true;
		//Ivk_zCSkyControler_Outdoor_RenderSkyPlane(_this);
		RenderSkyLayer(&_this->skyLayer[0], _this->skyLayerState[0]);
		//RenderSkyLayer(&_this->skyLayer[1], _this->skyLayerState[1]);
		g_renderingSky = false;
	}
	
	int __fastcall zCTextureFileFormatInternal_LoadTexture(zCTextureFileFormatInternal* _this, void* vtable, zFILE& inFile, zCTextureExchange* texFile)
	{
		int result = Ivk_zCTextureFileFormatInternal_LoadTexture(_this, inFile, texFile);
		
		if (inFile.s_initialized)
		{
			zSTRING fileName = inFile.GetFilename();

			if (fileName == zSTRING("SKYDAY_LAYER0_A0-C"))
			{
				zCTextureConvert* texCvt = zrenderer->CreateTextureConvert();

				zCTextureInfo texInfo;
				texInfo.averageColor = 0;
				texInfo.numMipMap = 1;
				texInfo.refSizeX = _this->texInfo.refSizeX;
				texInfo.refSizeY = _this->texInfo.refSizeY;
				texInfo.sizeX = _this->texInfo.sizeX;
				texInfo.sizeY = _this->texInfo.sizeY;
				texInfo.texFormat = zRND_TEX_FORMAT_RGBA_8888;
				texCvt->SetTextureInfo(texInfo);

				int bufferSize = texInfo.sizeX * texInfo.sizeY * 4;
				void* dbuffer = NULL;
				int destPitchXBytes = 0;
				if (texCvt->GetTextureBuffer(0, dbuffer, destPitchXBytes))
				{
					//WhiteNoiseGenerator* noiseGenerator = new WhiteNoiseGenerator();
					PerlinNoiseGenerator* noiseGenerator = new PerlinNoiseGenerator();
					double* noise = noiseGenerator->GenerateNoise(texInfo.sizeX, texInfo.sizeY);

					for (int y = 0; y < texInfo.sizeY; y++)
					{
						for (int x = 0; x < texInfo.sizeX; x++)
						{
							//double s = noiseGenerator->Turbulence(x, y, 32) / 255;
							double s = noise[y * texInfo.sizeY + x] * 1.4;
							if (s > 1.0)
								s = 1.0;
							hsv hsv = { 28.0f, s, 0.64f };
							rgb rgb = hsv2rgb(hsv);

							((zBYTE*)dbuffer)[y * texInfo.sizeY * 4 + x * 4 + 0] = rgb.b * 255;
							((zBYTE*)dbuffer)[y * texInfo.sizeY * 4 + x * 4 + 1] = rgb.g * 255;
							((zBYTE*)dbuffer)[y * texInfo.sizeY * 4 + x * 4 + 2] = rgb.r * 255;
							((zBYTE*)dbuffer)[y * texInfo.sizeY * 4 + x * 4 + 3] = 255;
						}
					}

					delete noiseGenerator;
				}

				texCvt->ConvertToNewSize(texInfo.sizeX, texInfo.sizeY);
				texCvt->ConvertToNewFormat(_this->texInfo.texFormat);

				zCTextureExchange::CopyContents(texCvt, texFile);
			}
		}

		return result;
	}
}
