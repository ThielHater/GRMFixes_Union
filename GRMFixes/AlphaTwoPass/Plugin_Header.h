// Header file
// Multi-platform file can not have `#pragma once`!!!

#include <string>
#include <set>

namespace NAMESPACE
{
	void ReadAlphaTwoPassFile();

	struct GothicMemoryLocations
	{
		struct zCRnd_D3D
		{
			static const int XD3D_SetRenderState = 0x007185C0;
		};

		struct zCTexture
		{
			static const int HasAlpha = 0x005C8860;
		};

		struct zCVob
		{
			static const int Render = 0x005D6090;
		};
	};
}