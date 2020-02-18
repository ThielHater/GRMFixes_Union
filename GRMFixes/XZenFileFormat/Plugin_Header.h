// Header file
// Multi-platform file can not have `#pragma once`!!!

#include <map>
#include <string>

namespace NAMESPACE
{
	struct GothicMemoryLocations
	{
		struct LoadBin
		{
			static const int ASM_BSP_ERROR_START = 0x00525452;
			static const int ASM_BSP_ERROR_END = 0x005254D8 - 1;
		};

		struct SaveMSH
		{
			static const int ASM_INDEX_CAST = 0x0055704A;
			static const int ASM_INDEX_SIZE_PUSH = 0x0055704E - 1;
		};

		struct LoadMSH
		{
			static const int ASM_INDEX_STRUCT_SIZE_ADD = 0x0055826F;
			static const int ASM_BLOCK_OFFSET_LEA_FIRST = 0x005583A2;
			static const int ASM_FEATINDEX_OFFSET = 0x00558226;
			static const int ASM_INDEX_CAST = 0x00558236;
		};
	};
}