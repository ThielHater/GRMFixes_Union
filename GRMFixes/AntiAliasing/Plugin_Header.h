// Header file
// Multi-platform file can not have `#pragma once`!!!

#include <d3dtypes.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

namespace NAMESPACE
{
	void Release();

	struct GothicMemoryLocations
	{
		struct zCRnd_D3D
		{
			static const int EndFrame = 0x00717150;
			static const int XD3D_InitPerDX = 0x0070E530;
			static const int XD3D_InitPort = 0x0070F250;
			static const int XD3D_SetDevice = 0x00711F00;
			static const int XD3D_SetMode = 0x0071FE30;
			static const int XD3D_CreateWin = 0x00712C80;
		};

		struct zCView
		{
			static const int SetMode = 0x00702180;
		};

		struct zCBspTree
		{
			static const int Render = 0x0051D840;
		};
	};
}