// Source file
// Multi-platform file can not have `#pragma once`!!!
// For compile plugin in Multiplatform mode - select 'Debug' or 'Release' configuration.

#include "Plugin_Header.h"
#include "zCArrayDegenerated.h"

namespace NAMESPACE
{
	/* Whether we are in an XZEN right now */
	bool g_IsLoadingXZEN = false;

	/* Map of adresses and original assemblercode bytes */
	std::map<unsigned int, unsigned char> g_OriginalZENLoadSaveAsmBytes;
	std::map<unsigned int, unsigned char>& g_ActiveOriginalAsmBytes = g_OriginalZENLoadSaveAsmBytes;

	// 0x00509A40 private: void __thiscall zCArchiverFactory::ReadLineArg(class zSTRING &,class zSTRING &,class zCBuffer *,class zFILE *)
	void __fastcall zCArchiverFactory_ReadLineArg(zCArchiverFactory* _this, void* vtable, zSTRING& line, zSTRING& arg, zCBuffer* buffer, zFILE* file);
	CInvoke<void(__thiscall*)(zCArchiverFactory* _this, zSTRING& line, zSTRING& arg, zCBuffer* buffer, zFILE* file)> Ivk_zCArchiverFactory_ReadLineArg(GothicMemoryLocations::zCArchiverFactory::ReadLineArg, &zCArchiverFactory_ReadLineArg);

	// 0x00525330 public: int __thiscall zCBspTree::LoadBIN(class zCFileBIN &,int)
	int __fastcall zCBspTree_LoadBIN(zCBspTree* _this, void* vtable, zCFileBIN& file, zBOOL skipThisChunk);
	CInvoke<int(__thiscall*)(zCBspTree* _this, zCFileBIN& file, zBOOL skipThisChunk)> Ivk_zCBspTree_LoadBIN(GothicMemoryLocations::zCBspTree::LoadBIN, &zCBspTree_LoadBIN);

	// 0x0050A520 private: void __thiscall zCArchiverFactory::WriteLine(char const * const,class zCBuffer *,class zFILE *)
	void __fastcall zCArchiverFactory_WriteLineChar(zCArchiverFactory* _this, void* vtable, const char* line, struct zCBuffer* buffer, struct zFILE* file);
	CInvoke<void(__thiscall*)(zCArchiverFactory* _this, const char* line, struct zCBuffer* buffer, struct zFILE* file)> Ivk_zCArchiverFactory_WriteLineChar(GothicMemoryLocations::zCArchiverFactory::WriteLineChar, &zCArchiverFactory_WriteLineChar);

	// 0x0050A420 private: void __thiscall zCArchiverFactory::WriteLine(class zSTRING const&, class zCBuffer*, class zFILE*)
	void __fastcall zCArchiverFactory_WriteLine(zCArchiverFactory* _this, void* vtable, const zSTRING& line, struct zCBuffer* buffer, struct zFILE* file);
	CInvoke<void(__thiscall*)(zCArchiverFactory* _this, const zSTRING& line, struct zCBuffer* buffer, struct zFILE* file)> Ivk_zCArchiverFactory_WriteLine(GothicMemoryLocations::zCArchiverFactory::WriteLine, &zCArchiverFactory_WriteLine);

	// 0x005244C0 public: void __thiscall zCBspTree::SaveBIN(class zCFileBIN &)
	void __fastcall zCBspTree_SaveBIN(zCBspTree* _this, void* vtable, zCFileBIN& file);
	CInvoke<void(__thiscall*)(zCBspTree* _this, zCFileBIN& file)> Ivk_zCBspTree_SaveBIN(GothicMemoryLocations::zCBspTree::SaveBIN, &zCBspTree_SaveBIN);

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

	void __stdcall FasterVertexProcessingHook(zCMesh* mesh, zCArrayDegenerated<zCVertex*>* vertexList, zCArrayDegenerated<zCVertFeature*>* featureList, int numPolys, zCPolygon** polyList)
	{
		HANDLE con = cmd.GetHandle();
		SetConsoleTextAttribute(con, 2);
		cmd << "Plugin: ";
		SetConsoleTextAttribute(con, 10);
		cmd << "Collecting vertices..." << endl;
		SetConsoleTextAttribute(con, 2);

		auto start = std::chrono::system_clock::now();

		// [VERSION 6] Need to overwrite the poiter here with new memory, as this isn't always valid as it seems.
		// It's a minor memory, leak, but since it's only for the spacer, I'm not going to bother
		vertexList->Array = new zCVertex * [1];
		featureList->Array = new zCVertFeature * [1];

		// Need to allocate at least *some* data or PushBack will fail.
		vertexList->Reallocate(1);
		featureList->Reallocate(1);

		if (mesh)
		{
			// Get vertices from mesh
			cmd << "- Got mesh data, generating vertex list... (" << mesh->numVert << " vertices)" << endl;

			// Generate vertexlist
			for (int i = 0; i < mesh->numVert; i++)
				vertexList->PushBackFast(mesh->vertList[i]);
			cmd << "- Done." << endl;

			// Create featurelist from polygons
			cmd << "- Generating feature list from polygons... (" << numPolys << " polygons)" << endl;

			std::vector<zCVertFeature*> featureVec;
			for (int i = 0; i < numPolys; i++)
			{
				if ((i % 10000) == 0)
					cmd << "- " << i << " polygons processed." << endl;

				zCPolygon* p = polyList[i];

				// Iterate over poly-vertices
				for (int j = 0; j < p->polyNumVert; j++)
				{
					// PB for each feature if it is already in that list. It's faster to add them all and then do a std::unique.
					featureVec.push_back(p->feature[j]);
				}
			}

			int numFeat = featureVec.size();
			cmd << "- Got " << featureVec.size() << " features." << endl;

			// Remove duplicates
			std::sort(featureVec.begin(), featureVec.end());
			std::unique(featureVec.begin(), featureVec.end());

			cmd << "- Removed " << numFeat - featureVec.size()  << " duplicates." << endl;

			cmd << "- Copying..." << endl;

			// Copy to the games internal data structure
			featureList->FromVector(featureVec);

			cmd << "- Done." << endl;
		}
		else
		{
			cmd << "- Getting vertex data from shared polygons..." << endl;

			// Get vertices from polygons
			std::vector<zCVertex*> vertexVec;
			std::vector<zCVertFeature*> featureVec;

			for (int i = 0; i < numPolys; i++)
			{
				if ((i % 10000) == 0)
					cmd << "- " << i << " polygons processed." << endl;

				zCPolygon* p = polyList[i];

				// Iterate over poly-vertices
				for (int j = 0; j < p->polyNumVert; j++)
				{
					// PB for each feature if it is already in that list. It's faster to add them all and then do a std::unique.
					vertexVec.push_back(p->vertex[j]);
					featureVec.push_back(p->feature[j]);
				}
			}

			int numVerts = vertexVec.size();
			int numFeat = featureVec.size();
			cmd << "- Got " << vertexVec.size() << " vertices." << endl;
			cmd << "- Got " << featureVec.size() << " features." << endl;

			// Remove duplicates
			std::sort(featureVec.begin(), featureVec.end());
			std::unique(featureVec.begin(), featureVec.end());

			std::sort(vertexVec.begin(), vertexVec.end());
			std::unique(vertexVec.begin(), vertexVec.end());

			cmd << "- Removed " << numVerts - vertexVec.size() << " duplicate vertices and " << numFeat - featureVec.size() << " duplicate features!" << endl;
			cmd << "- Copying..." << endl;

			// Copy to the games internal data structure
			featureList->FromVector(featureVec);
			vertexList->FromVector(vertexVec);

			cmd << "- Done." << endl;
		}

		long time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count();

		cmd << "- Process took " << time << " milliseconds." << endl;

		SetConsoleTextAttribute(con, 8);
	}

	void FixVertexProcessing()
	{
		/*
		mov eax, [ebp + 0xC] # PolyList
		push eax

		mov eax, [ebp + 0x10] # NumPolys
		push eax

		lea eax, [ebp - 0x7C] # FeatList
		push eax

		lea eax, [ebp - 0x58] # VertList
		push eax

		mov eax, [ebp + 0x14] # Mesh
		push eax

		call 0x10203040
		*/

		// Remove old vertex-collection
		ReplaceCodeRange(0x90, GothicMemoryLocations::SaveMSH::ASM_VERTEX_PROC_START, GothicMemoryLocations::SaveMSH::ASM_VERTEX_PROC_END);
		cmd << "- Removed old vertex-collection code." << endl;

		// Patch-code
		unsigned char hookCode[] = { 0x8B, 0x45, 0x0C, 0x50, 0x8B, 0x45, 0x10, 0x50, 0x8D, 0x45, 0x84, 0x50, 0x8D, 0x45, 0xA8, 0x50, 0x8B, 0x45, 0x14, 0x50, 0xFF, 0x15 };

		// Patch the game
		ReplaceCodeBytes((char*)hookCode, ARRAYSIZE(hookCode), GothicMemoryLocations::SaveMSH::ASM_VERTEX_PROC_START);
		cmd << "- Inserted function call to new vertex-collection code." << endl;

		// Put our call-address into the code
		static UINT32 s_fnaddr = (UINT32)FasterVertexProcessingHook;
		UINT32 addr2 = (UINT32)&s_fnaddr;
		ReplaceCodeBytes((char*)&addr2, sizeof(UINT32), GothicMemoryLocations::SaveMSH::ASM_VERTEX_PROC_START + ARRAYSIZE(hookCode));
	}

	/* Indices are only 2 bytes in Gothic 1. Patch some bytes to bump that to 4. */
	void FixZENLoadSave()
	{
		if (!g_OriginalZENLoadSaveAsmBytes.empty())
			return; // Already patched

		HANDLE con = cmd.GetHandle();
		SetConsoleTextAttribute(con, 2);

		// SaveMSH: push 2 -> push 4  -- fwrite-size
		const char* push4 = "\x6A\x04";
		ReplaceCodeBytes(push4, strlen(push4), GothicMemoryLocations::SaveMSH::ASM_INDEX_SIZE_PUSH);
		cmd <<"- push 2 -> push 4" << endl;

		// SaveMSH: mov ax, [edx+0Ch] -> mov eax, [edx+0x0C] -- Cast index to 16-bit
		// Also appended a 0x90 (nop) at the end, since the former instruction is only 3 bytes long.
		const char* cast32 = "\x8B\x42\x0C\x90";
		ReplaceCodeBytes(cast32, strlen(cast32), GothicMemoryLocations::SaveMSH::ASM_INDEX_CAST);
		cmd << "- mov ax, [edx+0Ch] -> mov eax, [edx+0x0C]" << endl;

#ifdef GAME
		// LoadMSH: add [esp+214h+var_1FC], 6 -> add [esp+214h+var_1FC], 8 -- Offset to the next vertex/feature-index pair.
		// Feature-Indices are 4bytes, so with a 2byte vertexindex it gives 6. We need to make it 8.
		const char* offset8 = "\x83\x44\x24\x18\x08";
		ReplaceCodeBytes(offset8, strlen(offset8), GothicMemoryLocations::LoadMSH::ASM_INDEX_STRUCT_SIZE_ADD);
		cmd << "- add [esp+214h+var_1FC], 6 -> add [esp+214h+var_1FC], 8" << endl;

		// LoadMSH: movzx eax, word ptr [edx] -> mov eax, [edx] -- Remove the cast from 32 to 16-bit when getting the index out of the datastructure
		// Also add a NOP, since a regular mov is only 2 bytes instead of 3 for movzx.
		const char* loadCast = "\x8B\x02\x90";
		ReplaceCodeBytes(loadCast, strlen(loadCast), GothicMemoryLocations::LoadMSH::ASM_INDEX_CAST);
		cmd << "- movzx eax, word ptr [edx] -> mov eax, [edx]" << endl;
#else
		// LoadMSH: add ecx, 6 -> add ecx, 8 -- Offset to the next vertex/feature-index pair.
		// Feature-Indices are 4bytes, so with a 2byte vertexindex it gives 6. We need to make it 8.
		const char* offset8 = "\x83\xC1\x08";
		ReplaceCodeBytes(offset8, strlen(offset8), GothicMemoryLocations::LoadMSH::ASM_INDEX_STRUCT_SIZE_ADD);
		cmd << "- add ecx, 6 -> add ecx, 8" << endl;

		// LoadMSH: mov dx, [eax] -> mov edx, [eax] -- Remove the cast from 32 to 16-bit when getting the index out of the datastructure
		// Also add a NOP, since a regular mov is only 2 bytes instead of 3 for mov to dx.
		const char* loadCast = "\x8B\x10\x90";
		ReplaceCodeBytes(loadCast, strlen(loadCast), GothicMemoryLocations::LoadMSH::ASM_INDEX_CAST);
		cmd << "- mov dx, [eax] -> mov edx, [eax]" << endl;

		// SaveBIN: Mute the annoying warning about a "too large" chunk
		ReplaceCodeRange(0x90, GothicMemoryLocations::SaveBin::ASM_BSP_ERROR_START, GothicMemoryLocations::SaveBin::ASM_BSP_ERROR_END);
		cmd << "- Muted \"Too large chunk\"-warning on ZEN-Save." << endl;
#endif

		// LoadBIN: Mute the annoying warning about a "too large" chunk
		ReplaceCodeRange(0x90, GothicMemoryLocations::LoadBin::ASM_BSP_ERROR_START, GothicMemoryLocations::LoadBin::ASM_BSP_ERROR_END);
		cmd << "- Muted \"Too large chunk\"-warning on ZEN-Load." << endl;

		// LoadMSH: mov esi, [ecx+2] -> mov esi, [ecx+4] -- Need to offset the feature by 4, not by 2
		const char* featOffset = "\x8B\x71\x04";
		ReplaceCodeBytes(featOffset, strlen(featOffset), GothicMemoryLocations::LoadMSH::ASM_FEATINDEX_OFFSET);
		cmd << "- mov esi, [ecx+2] -> mov esi, [ecx+4]" << endl;

		// LoadMSH: lea edx, [eax+eax*2] -> imul edx, eax, 4 -- In the next line, edx will be multiplyed by 2, so since edx = 3 * edx it becomes 6 * eax in the next line.
		// Replace this with code that multiplies eax by 4 so we get 8 with the next line.
		const char* imul = "\x6B\xD0\x04";
		ReplaceCodeBytes(imul, strlen(imul), GothicMemoryLocations::LoadMSH::ASM_BLOCK_OFFSET_LEA_FIRST);
		cmd << "- lea edx, [eax+eax*2] -> imul edx, eax, 4" << endl;

#ifndef GAME
		FixVertexProcessing();
#endif

		SetConsoleTextAttribute(con, 8);
	}

	void __fastcall zCArchiverFactory_ReadLineArg(zCArchiverFactory* _this, void* vtable, zSTRING& line, zSTRING& arg, zCBuffer* buffer, zFILE* file)
	{
		// Call original function
		Ivk_zCArchiverFactory_ReadLineArg(_this, line, arg, buffer, file);

		std::string ln = line.ToChar();
		std::string ag = arg.ToChar();

		// We are using the "user" argument on top of each ZEN-File to determine if this is an enhanced ZEN.
		// That should be fine, since nobody really cares about that argument.
		if (ln == "user")
			g_IsLoadingXZEN = ag == "XZEN";
	}

	int __fastcall zCBspTree_LoadBIN(zCBspTree* _this, void* vtable, zCFileBIN& file, zBOOL skipThisChunk)
	{
		bool& isLoadingXZEN = g_IsLoadingXZEN;

		// Apply fixes if the file is right
		if (isLoadingXZEN)
		{
			HANDLE con = cmd.GetHandle();
			SetConsoleTextAttribute(con, 2);
			cmd << "Plugin: ";
			SetConsoleTextAttribute(con, 10);
			cmd << "Loading XZEN! Applying code modifications..." << endl;
			SetConsoleTextAttribute(con, 8);
			FixZENLoadSave();
		}

		// Call game function
		int result = Ivk_zCBspTree_LoadBIN(_this, file, skipThisChunk);

		// Reset everything, worldmesh is done
		if (isLoadingXZEN)
		{
			HANDLE con = cmd.GetHandle();
			SetConsoleTextAttribute(con, 2);
			cmd << "Plugin: ";
			SetConsoleTextAttribute(con, 10);
			cmd << "Done! Restoring original code..." << endl;
			SetConsoleTextAttribute(con, 8);
			isLoadingXZEN = false;

			// Go back to normal at the start
			RestoreOriginalCodeBytes(g_OriginalZENLoadSaveAsmBytes);

			// Reset this for the next time
			g_OriginalZENLoadSaveAsmBytes.clear();
		}

		return result;
	}

	/* Leave a note in the ZEN-File so we know it has been modified data */
	void __fastcall zCArchiverFactory_WriteLineChar(zCArchiverFactory* _this, void* vtable, const char* line, struct zCBuffer* buffer, struct zFILE* file)
	{
#ifdef GAME
		Ivk_zCArchiverFactory_WriteLineChar(_this, line, buffer, file);
#else
		if (file != NULL)
		{
			std::string ln = line;

			// Check if we are currently writing the "user" argument
			if (ln.substr(0, 4) == "user")
			{
				HANDLE con = cmd.GetHandle();
				SetConsoleTextAttribute(con, 2);
				cmd << "Plugin: ";
				SetConsoleTextAttribute(con, 10);
				cmd << "Writing modified user argument.." << endl;
				SetConsoleTextAttribute(con, 8);

				// Write our own user-line
				Ivk_zCArchiverFactory_WriteLineChar(_this, "user XZEN", buffer, file);
			}
			else
			{
				// Just write the line
				Ivk_zCArchiverFactory_WriteLineChar(_this, line, buffer, file);
			}
		}
		else
		{
			Ivk_zCArchiverFactory_WriteLineChar(_this, line, buffer, file);
		}
#endif
	}

	/* Just proxy this to the char* version */
	void __fastcall zCArchiverFactory_WriteLine(zCArchiverFactory* _this, void* vtable, const zSTRING& line, struct zCBuffer* buffer, struct zFILE* file)
	{
		zCArchiverFactory_WriteLineChar(_this, vtable, line.ToChar(), buffer, file);
	}

	/* Only apply fixes while saving the worldmesh */
	void __fastcall zCBspTree_SaveBIN(zCBspTree* _this, void* vtable, zCFileBIN& file)
	{
#ifdef GAME
		Ivk_zCBspTree_SaveBIN(_this, file);
#else
		// Apply fixes if we're saving a world
		HANDLE con = cmd.GetHandle();
		SetConsoleTextAttribute(con, 2);
		cmd << "Plugin: ";
		SetConsoleTextAttribute(con, 10);
		cmd << "Saving XZEN! Applying code modifications..." << endl;
		SetConsoleTextAttribute(con, 8);

		FixZENLoadSave();

		// Call game function
		Ivk_zCBspTree_SaveBIN(_this, file);

		// Go back to default code
		SetConsoleTextAttribute(con, 2);
		cmd << "Plugin: ";
		SetConsoleTextAttribute(con, 10);
		cmd << "Done! Restoring original code..." << endl;
		SetConsoleTextAttribute(con, 8);

		// Go back to normal at the start
		RestoreOriginalCodeBytes(g_OriginalZENLoadSaveAsmBytes);

		// Reset this for the next time
		g_OriginalZENLoadSaveAsmBytes.clear();
#endif
	}
}