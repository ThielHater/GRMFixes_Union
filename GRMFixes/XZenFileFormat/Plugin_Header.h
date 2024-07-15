// Header file
// Multi-platform file can not have `#pragma once`!!!

#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>

namespace NAMESPACE
{
#ifdef GAME
	struct GothicMemoryLocations
	{
		struct LoadBin
		{
			static const int ASM_BSP_ERROR_START = 0x00525452;
			static const int ASM_BSP_ERROR_END = 0x005254D8 - 1;
		};

		struct LoadMSH
		{
			static const int ASM_INDEX_STRUCT_SIZE_ADD = 0x0055826F;
			static const int ASM_BLOCK_OFFSET_LEA_FIRST = 0x005583A2;
			static const int ASM_FEATINDEX_OFFSET = 0x00558226;
			static const int ASM_INDEX_CAST = 0x00558236;
		};

		struct SaveMSH
		{
			static const int ASM_INDEX_CAST = 0x0055704A;
			static const int ASM_INDEX_SIZE_PUSH = 0x0055704E - 1;
		};

		struct zCArchiverFactory
		{
			static const int ReadLineArg = 0x00509A40;
			static const int WriteLine = 0x0050A420;
			static const int WriteLineChar = 0x0050A520;
		};

		struct zCBspTree
		{
			static const int LoadBIN = 0x00525330;
			static const int SaveBIN = 0x005244C0;
		};
	};
#else
	struct GothicMemoryLocations
	{
		struct LoadBin
		{
			static const int ASM_BSP_ERROR_START = 0x004CB1B8;
			static const int ASM_BSP_ERROR_END = 0x004CB259 - 1;
		};

		struct SaveBin
		{
			static const int ASM_BSP_ERROR_START = 0x004CABA3;
			static const int ASM_BSP_ERROR_END = 0x004CAC5E - 1;
		};

		struct LoadMSH
		{
			static const int ASM_INDEX_STRUCT_SIZE_ADD = 0x004FEF3F;
			static const int ASM_BLOCK_OFFSET_LEA_FIRST = 0x004FF06B;
			static const int ASM_FEATINDEX_OFFSET = 0x004FEEFD;
			static const int ASM_INDEX_CAST = 0x004FEF10;
		};

		struct SaveMSH
		{
			static const int ASM_INDEX_CAST = 0x004FDF53;
			static const int ASM_INDEX_SIZE_PUSH = 0x004FDF57;
			static const int ASM_VERTEX_PROC_START = 0x004FCCEC;
			static const int ASM_VERTEX_PROC_END = 0x004FD4C1 - 1;
		};

		struct zCArchiverFactory
		{
			static const int ReadLineArg = 0x004AE740;
			static const int WriteLine = 0x004AF2E0;
			static const int WriteLineChar = 0x004AF3F0;
		};

		struct zCBspTree
		{
			static const int LoadBIN = 0x004CB080;
			static const int SaveBIN = 0x004CA220;
		};
	};
#endif
}