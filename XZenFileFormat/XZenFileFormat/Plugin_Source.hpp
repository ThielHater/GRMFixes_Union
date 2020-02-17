// Source file
// Multi-platform file can not have `#pragma once`!!!
// For compile plugin in Multiplatform mode - select 'Debug' or 'Release' configuration.

#include <map>
#include <string>
#include "Plugin_Header.h"

namespace NAMESPACE {
	/* Whether we are in an XZEN right now */
	bool g_IsLoadingXZEN = false;

	/* Map of adresses and original assemblercode bytes */
	std::map<unsigned int, unsigned char> g_OriginalZENLoadSaveAsmBytes;
	std::map<unsigned int, unsigned char>& g_ActiveOriginalAsmBytes = g_OriginalZENLoadSaveAsmBytes;

	void ReplaceCodeBytes(const char* bytes, int numBytes, unsigned int addr)
	{
		// Unprotect the given code-area
		DWORD dwProtect;
		VirtualProtect((void*)addr, numBytes, PAGE_EXECUTE_READWRITE, &dwProtect);

		// Add original code to map
		auto it = g_ActiveOriginalAsmBytes.find(addr);
		if (it == g_ActiveOriginalAsmBytes.end())
		{
			for (int i = 0; i < numBytes; i++)
			{
				g_ActiveOriginalAsmBytes[addr + i] = *(unsigned char*)(addr + i);
			}
		}

		// Overwrite the code
		memcpy((void*)addr, bytes, numBytes);
	}

	void ReplaceCodeRange(unsigned char inst, unsigned int start, unsigned int end)
	{
		// Unprotect the given code-area
		DWORD dwProtect;
		VirtualProtect((void*)start, end - start, PAGE_EXECUTE_READWRITE, &dwProtect);

		// Add original code to map
		auto it = g_ActiveOriginalAsmBytes.find(start);
		if (it == g_ActiveOriginalAsmBytes.end())
		{
			for (unsigned int i = start; i <= end; i++)
			{
				g_ActiveOriginalAsmBytes[i] = *(unsigned char*)(i);
			}
		}

		// Write to memory
		memset((void*)start, inst, (end - start) + 1);
	}

	/* Restores the modified bytes from the given map to their original state */
	void RestoreOriginalCodeBytes(const std::map<unsigned int, unsigned char>& originalMap)
	{
		for each (const std::pair<unsigned int, unsigned char> & c in originalMap)
		{
			*(unsigned char*)c.first = c.second;
		}
	}

	void FixZENLoadSave()
	{
		if (!g_OriginalZENLoadSaveAsmBytes.empty())
			return; // Already patched

		// Indices are only 2 bytes in Gothic 1. Patch some bytes to bump that to 4.
		printf("Patching Game...\n");

		// SaveMSH: push 2 -> push 4  -- fwrite-size
		const char* push4 = "\x6A\x04";
		ReplaceCodeBytes(push4, strlen(push4), GothicMemoryLocations::SaveMSH::ASM_INDEX_SIZE_PUSH);
		printf(" - push 2 -> push 4\n");

		// SaveMSH: mov ax, [edx+0Ch] -> mov eax, [edx+0x0C] -- Cast index to 16-bit
		// Also appended a 0x90 (nop) at the end, since the former instruction is only 3 bytes long.
		const char* cast32 = "\x8B\x42\x0C\x90";
		ReplaceCodeBytes(cast32, strlen(cast32), GothicMemoryLocations::SaveMSH::ASM_INDEX_CAST);
		printf(" - mov ax, [edx+0Ch] -> mov eax, [edx+0x0C]\n");

		// LoadMSH: add [esp+214h+var_1FC], 6 -> add [esp+214h+var_1FC], 8 -- Offset to the next vertex/feature-index pair. 
		// Feature-Indices are 4bytes, so with a 2byte vertexindex it gives 6. We need to make it 8.
		const char* offset8 = "\x83\x44\x24\x18\x08";
		ReplaceCodeBytes(offset8, strlen(offset8), GothicMemoryLocations::LoadMSH::ASM_INDEX_STRUCT_SIZE_ADD);
		printf(" - add [esp+214h+var_1FC], 6 -> add [esp+214h+var_1FC], 8\n");

		// LoadMSH: movzx eax, word ptr [edx] -> mov eax, [edx] -- Remove the cast from 32 to 16-bit when getting the index out of the datastructure
		// Also add a NOP, since a regular mov is only 2 bytes instead of 3 for movzx.
		const char* loadCast = "\x8B\x02\x90";
		ReplaceCodeBytes(loadCast, strlen(loadCast), GothicMemoryLocations::LoadMSH::ASM_INDEX_CAST);
		printf(" - movzx eax, word ptr [edx] -> mov eax, [edx]\n");

		// LoadBIN: Mute the annoying warning about a "too large" chunk
		ReplaceCodeRange(0x90, GothicMemoryLocations::LoadBin::ASM_BSP_ERROR_START, GothicMemoryLocations::LoadBin::ASM_BSP_ERROR_END);
		printf(" - Muted \"Too large chunk\"-warning on ZEN-Load.\n");

		// LoadMSH: mov esi, [ecx+2] -> mov esi, [ecx+4] -- Need to offset the feature by 4, not by 2
		const char* featOffset = "\x8B\x71\x04";
		ReplaceCodeBytes(featOffset, strlen(featOffset), GothicMemoryLocations::LoadMSH::ASM_FEATINDEX_OFFSET);
		printf(" - mov esi, [ecx+2] -> mov esi, [ecx+4]\n");

		// LoadMSH: lea edx, [eax+eax*2] -> imul edx, eax, 4 -- In the next line, edx will be multiplyed by 2, so since edx = 3 * edx it becomes 6 * eax in the next line.
		// Replace this with code that multiplies eax by 4 so we get 8 with the next line.
		const char* imul = "\x6B\xD0\x04";
		ReplaceCodeBytes(imul, strlen(imul), GothicMemoryLocations::LoadMSH::ASM_BLOCK_OFFSET_LEA_FIRST);
		printf(" - lea edx, [eax+eax*2] -> imul edx, eax, 4\n");

		printf("Done!\n");
	}

	// 0x00509A40 private: void __thiscall zCArchiverFactory::ReadLineArg(class zSTRING &,class zSTRING &,class zCBuffer *,class zFILE *)
	void __fastcall zCArchiverFactoryEx_ReadLineArg(zCArchiverFactory* _this, void* vtable, zSTRING& line, zSTRING& arg, zCBuffer* buffer, zFILE* file);

	CInvoke<void(__thiscall*)(zCArchiverFactory* _this, zSTRING& line, zSTRING& arg, zCBuffer* buffer, zFILE* file)> Ivk_zCArchiverFactoryEx_ReadLineArg(0x00509A40, &zCArchiverFactoryEx_ReadLineArg);

	void __fastcall zCArchiverFactoryEx_ReadLineArg(zCArchiverFactory* _this, void* vtable, zSTRING& line, zSTRING& arg, zCBuffer* buffer, zFILE* file)
	{
		// Call original function
		Ivk_zCArchiverFactoryEx_ReadLineArg(_this, line, arg, buffer, file);

		std::string ln = line.ToChar();
		std::string ag = arg.ToChar();

		// We are using the "user" argument on top of each ZEN-File to determine if this is an enhanced ZEN.
		// That should be fine, since nobody really cares about that argument.
		if (ln == "user")
			g_IsLoadingXZEN = ag == "XZEN";
	}

	// 0x00525330 public: int __thiscall zCBspTree::LoadBIN(class zCFileBIN &,int)
	int __fastcall zCBspTree_LoadBIN(zCBspTree* _this, void* vtable, zCFileBIN& file, zBOOL skipThisChunk);

	CInvoke<int(__thiscall*)(zCBspTree* _this, zCFileBIN& file, zBOOL skipThisChunk)> Ivk_zCBspTree__LoadBIN(0x00525330, &zCBspTree_LoadBIN);

	int __fastcall zCBspTree_LoadBIN(zCBspTree* _this, void* vtable, zCFileBIN& file, zBOOL skipThisChunk)
	{
		bool& isLoadingXZEN = g_IsLoadingXZEN;

		// Apply fixes if the file is right
		if (isLoadingXZEN)
		{
			printf("Loading an enhanced ZEN. Applying code modifications.\n");
			FixZENLoadSave();
		}

		// Call game function
		int result = Ivk_zCBspTree__LoadBIN(_this, file, skipThisChunk);

		// Reset everything, worldmesh is done
		if (isLoadingXZEN)
		{
			printf("Done loading Mesh! Restoring original code... \n");
			isLoadingXZEN = false;

			// Go back to normal at the start
			RestoreOriginalCodeBytes(g_OriginalZENLoadSaveAsmBytes);

			// Reset this for the next time
			g_OriginalZENLoadSaveAsmBytes.clear();
		}

		return result;
	}
}