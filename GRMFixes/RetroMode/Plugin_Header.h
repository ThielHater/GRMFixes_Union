// Header file
// Multi-platform file can not have `#pragma once`!!!

#include <thread>
#include <vector>
#include <Windows.h>

#define GOTHIC1

namespace NAMESPACE
{
	struct GothicMemoryLocations
	{
#ifdef GOTHIC1
		struct zCWorld
		{
			static const int Render = 0x005F3EC0;
		};

		struct oCItem
		{
			static const int RenderItem = 0x00672F70;
		};

		struct zCSndSys_MSS
		{
			static const int InitializeMSS = 0x004DE2B0;
			static const int ASM_SET_SAMPLERATE = 0x004DE8BE;
		};

		struct zCRenderer
		{
			static const int Vid_SetMode = 0x005AE970;
		};
#endif
#ifdef GOTHIC2A
		struct zCWorld
		{
			static const int Render = 0x00621700;
		};

		struct oCItem
		{
			static const int RenderItem = 0x00713AC0;
		};

		struct zCSndSys_MSS
		{
			static const int InitializeMSS = 0x004EB830;
			static const int ASM_SET_SAMPLERATE = 0x004EBE6B;
		};

		struct zCRenderer
		{
			static const int Vid_SetMode = 0x005D3C20;
		};
#endif
	};
}