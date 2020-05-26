// Source file
// Multi-platform file can not have `#pragma once`!!!
// For compile plugin in Multiplatform mode - select 'Debug' or 'Release' configuration.

#include "Plugin_Header.h"
#include "WhiteNoiseGenerator.h"
#include "PerlinNoiseGenerator.h"
#include "ColorHelper.h"

namespace NAMESPACE
{
	// 0x005C7D10 public: virtual int __thiscall zCTextureFileFormatInternal::LoadTexture(class zFILE &,class zCTextureExchange *)
	int __fastcall zCTextureFileFormatInternal_LoadTexture(zCTextureFileFormatInternal* _this, void* vtable, zFILE& inFile, zCTextureExchange* texFile);
	CInvoke<int(__thiscall*)(zCTextureFileFormatInternal* _this, zFILE& inFile, zCTextureExchange* texFile)> Ivk_zCTextureFileFormatInternal_LoadTexture(GothicMemoryLocations::zCTextureFileFormatInternal::LoadTexture, &zCTextureFileFormatInternal_LoadTexture);

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
				if (texCvt->GetTextureBuffer(0, dbuffer, destPitchXBytes)) //erste MipMap
				{
					//WhiteNoiseGenerator* noiseGenerator = new WhiteNoiseGenerator();
					PerlinNoiseGenerator* noiseGenerator = new PerlinNoiseGenerator();
					double* noise = noiseGenerator->GenerateNoise(texInfo.sizeX, texInfo.sizeY);

					for (int y = 0; y < texInfo.sizeY; y++)
					{
						for (int x = 0; x < texInfo.sizeX; x++)
						{
							//double s = noiseGenerator->Turbulence(x, y, 32) / 255;
							double s = noise[y * texInfo.sizeY + x];
							hsv hsv = { 198.0f, s, 0.92f };
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
