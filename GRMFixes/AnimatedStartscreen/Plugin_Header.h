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
	void Init();
	void Animate();

	struct GothicMemoryLocations
	{
		struct CGameManager
		{
			static const int PlayVideo = 0x00429B80;
		};
	};
}