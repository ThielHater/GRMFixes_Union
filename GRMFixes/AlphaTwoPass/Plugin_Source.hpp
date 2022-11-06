// Source file
// Multi-platform file can not have `#pragma once`!!!
// For compile plugin in Multiplatform mode - select 'Debug' or 'Release' configuration.

#include "Plugin_Header.h"

namespace NAMESPACE
{
	std::set<std::string> g_vobMapTwoPass = std::set<std::string>();

	void ReadAlphaTwoPassFile()
	{
		// Read alpha VOB names from txt
		char exe[MAX_PATH], drive[MAX_PATH], dir[MAX_PATH];
		GetModuleFileName(NULL, exe, MAX_PATH);
		_splitpath(exe, drive, dir, NULL, NULL);

		char alpha[MAX_PATH];
		strcpy_s(alpha, drive);
		strcat_s(alpha, dir);
		strcat_s(alpha, "AlphaTwoPass.txt");

		HANDLE con = cmd.GetHandle();
		SetConsoleTextAttribute(con, 2);
		cmd << "Plugin: ";
		SetConsoleTextAttribute(con, 10);
		cmd << "Reading alpha VOB list..." << endl;
		SetConsoleTextAttribute(con, 8);

		std::ifstream file(alpha);
		std::string line;
		while (std::getline(file, line))
		{
			g_vobMapTwoPass.insert(line);
		}

		SetConsoleTextAttribute(con, 2);
		cmd << "Plugin: ";
		SetConsoleTextAttribute(con, 10);
		if (g_vobMapTwoPass.empty())
			cmd << "File not found or empty, two pass rendering will be applied to all VOBs!" << endl;
		else
			cmd << "Done!" << endl;
		SetConsoleTextAttribute(con, 8);
	}

	// 0x005D5F60 public: void __thiscall zCVob::CalcRenderAlpha(float,class zCVisual * &,float &)
	void __fastcall zCVob_CalcRenderAlpha(zCVob* _this, void* vtable, const zREAL distVobToCam, zCVisual*& activeVisual, zREAL& alpha);
	CInvoke<void(__thiscall*)(zCVob* _this, const zREAL distVobToCam, zCVisual*& activeVisual, zREAL& alpha)> Ivk_zCVob_CalcRenderAlpha(GothicMemoryLocations::zCVob::CalcRenderAlpha, &zCVob_CalcRenderAlpha);

	void __fastcall zCVob_CalcRenderAlpha(zCVob* _this, void* vtable, const zREAL distVobToCam, zCVisual*& activeVisual, zREAL& alpha)
	{
		Ivk_zCVob_CalcRenderAlpha(_this, distVobToCam, activeVisual, alpha);

		if (activeVisual && activeVisual->objectName && g_vobMapTwoPass.find(activeVisual->objectName.ToChar()) != g_vobMapTwoPass.end())
		{
			if (alpha == 1.0f)
				alpha = 0.99f;
		}
	}
	
	// 0x005B2A50 public: void __fastcall zCRenderManager::DrawVertexBuffer(class zCVertexBuffer *,int,int,unsigned short *,unsigned long,class zCMaterial *)
	void __fastcall zCRenderManager_DrawVertexBuffer(zCRenderManager* _this, class zCVertexBuffer* vertexBufferIn, int firstVert, int numVert, zWORD* indexList, zDWORD numIndex, class zCMaterial* material);
	CInvoke<void(__fastcall*)(zCRenderManager* _this, class zCVertexBuffer* vertexBufferIn, int firstVert, int numVert, zWORD* indexList, zDWORD numIndex, class zCMaterial* material)> Ivk_zCRenderManager_DrawVertexBuffer(GothicMemoryLocations::zCRenderManager::DrawVertexBuffer, &zCRenderManager_DrawVertexBuffer);

	void __fastcall zCRenderManager_DrawVertexBuffer(zCRenderManager* _this, class zCVertexBuffer* vertexBufferIn, int firstVert, int numVert, zWORD* indexList, zDWORD numIndex, class zCMaterial* material)
	{
		zTRnd_AlphaBlendFunc alphaFunc = _this->overrideAlphaBlendFunc;
		
		if (_this->overrideAlphaBlendFunc == zRND_ALPHA_FUNC_BLEND && material->texture)
		{
			if (material->texture->hasAlpha)
			{
				// Two Pass Rendering Technique (https://blogs.msdn.microsoft.com/shawnhar/2009/02/18/depth-sorting-alpha-blended-objects/)
				_this->overrideAlphaBlendFunc = zRND_ALPHA_FUNC_BLEND;
				Ivk_zCRenderManager_DrawVertexBuffer(_this, vertexBufferIn, firstVert, numVert, indexList, numIndex, material);
				_this->overrideAlphaBlendFunc = zRND_ALPHA_FUNC_TEST;
				Ivk_zCRenderManager_DrawVertexBuffer(_this, vertexBufferIn, firstVert, numVert, indexList, numIndex, material);
			}
			else
			{
				_this->overrideAlphaBlendFunc = zRND_ALPHA_FUNC_NONE;
				Ivk_zCRenderManager_DrawVertexBuffer(_this, vertexBufferIn, firstVert, numVert, indexList, numIndex, material);
			}	
		}
		else
		{
			Ivk_zCRenderManager_DrawVertexBuffer(_this, vertexBufferIn, firstVert, numVert, indexList, numIndex, material);
		}

		_this->overrideAlphaBlendFunc = alphaFunc;
	}
}
