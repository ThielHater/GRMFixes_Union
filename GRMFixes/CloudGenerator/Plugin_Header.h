// Header file
// Multi-platform file can not have `#pragma once`!!!

// dynamic namespace
namespace NAMESPACE
{
	struct GothicMemoryLocations
	{
		struct zCTextureFileFormatInternal
		{
			static const int LoadTexture = 0x005C7D10;
		};

		struct zCSkyState
		{
			static const int PresetDay1 = 0x005BBB50;
			static const int PresetDay2 = 0x005BBCD0;
			static const int PresetEvening = 0x005BBDA0;
			static const int PresetNight0 = 0x005BBE70;
			static const int PresetNight1 = 0x005BC070;
			static const int PresetNight2 = 0x005BC1F0;
			static const int PresetDawn = 0x005BC370;
			static const int PresetDay0 = 0x005BB9B0;
		};

		struct zCSkyControler_Outdoor
		{
			static const int CreateDefault = 0x005BDEC0;
			static const int GetStateTextureSearch = 0x005BCFA0;
			static const int ApplyStateTexToLayer = 0x005BCFF0;
			static const int RenderSkyPlane = 0x005BFC30;
			static const int RenderSkyDome = 0x005BFED0;
		};

		struct zCSkyLayer
		{
			static const int RenderSkyLayer = 0x005BED70;
		};

		struct zCRnd_D3D
		{
			static const int EndFrame = 0x00717150;
			static const int XD3D_SetRenderState = 0x007185C0;
			static const int XD3D_SetTextureStageState = 0x00718270;
		};
	};

	void AnalyzeHelper();
}