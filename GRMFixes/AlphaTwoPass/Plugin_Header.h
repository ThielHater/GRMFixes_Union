// Header file
// Multi-platform file can not have `#pragma once`!!!

#include <string>
#include <set>

namespace NAMESPACE
{
	void ReadAlphaTwoPassFile();

	struct GothicMemoryLocations
	{
		struct zCVob
		{
			static const int CalcRenderAlpha = 0x005D5F60;
		};

		struct zCRenderManager
		{
			static const int DrawVertexBuffer = 0x005B2A50;
		};
	};
}