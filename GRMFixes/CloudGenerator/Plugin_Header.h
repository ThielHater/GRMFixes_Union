// Header file
// Multi-platform file can not have `#pragma once`!!!

// dynamic namespace
namespace NAMESPACE
{
	struct GothicMemoryLocations
	{
		struct zCSkyLayer
		{
			static const int SetSkyPolyTexture = 0x005BED60;
			static const int SetSkyBoxTexture = 0x005BED20;
		};

		struct zCTexture
		{
			static const int LoadSingle = 0x005C9730;
		};

		struct zCTextureFileFormatInternal
		{
			static const int LoadTexture = 0x005C7D10;
		};
	};
}