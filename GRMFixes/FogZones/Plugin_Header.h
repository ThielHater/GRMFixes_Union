// Header file
// Multi-platform file can not have `#pragma once`!!!

namespace NAMESPACE
{
	struct mySkyState
	{
		int start;
		int duration;
		zVEC3 color;
	};

	struct GothicMemoryLocations
	{
		struct zCRnd_D3D
		{
			static const int BeginFrame = 0x00716F60;
		};

		struct zCSkyControler_Outdoor
		{
			static const int SetFarZ = 0x005C0540;
		};
	};
}