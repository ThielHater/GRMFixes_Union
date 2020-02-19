// Source file
// Multi-platform file can not have `#pragma once`!!!
// For compile plugin in Multiplatform mode - select 'Debug' or 'Release' configuration.

#include "Plugin_Header.h"

namespace NAMESPACE
{
	bool g_isFixEnabled = false;

	// 0x00578BD0 public: virtual int __thiscall zCModel::Render(struct zTRenderContext &)
	int __fastcall zCModel_Render(zCModel* _this, void* vtable, struct zTRenderContext& ctx);
	CInvoke<int(__thiscall*)(zCModel* _this, struct zTRenderContext& ctx)> Ivk_zCModel_Render(GothicMemoryLocations::zCModel::Render, &zCModel_Render);

	bool FileExists(const char* fileName)
	{
		std::ifstream infile(fileName);
		return infile.good();
	}

	void SetEnabled()
	{
		char exe[MAX_PATH], drive[MAX_PATH], dir[MAX_PATH], mod[MAX_PATH];
		GetModuleFileName(NULL, exe, MAX_PATH);
		_splitpath(exe, drive, dir, NULL, NULL);
		strcpy_s(mod, drive);
		strcat_s(mod, dir);
		strcat_s(mod, "..\\Data\\Carnage_Graphics_Patch.mod");
		_fullpath(mod, mod, MAX_PATH);

		// Is CGP installed?
		g_isFixEnabled = FileExists(mod);
	}
	
	int __fastcall zCModel_Render(zCModel* _this, void* vtable, struct zTRenderContext& ctx)
	{
		if (g_isFixEnabled)
		{
			// Workaround for the new finger-models breaking on some fatness values
			if (_this->fatness >= 2.0f)
				_this->fatness = 0.50f;
			else if (_this->fatness == 1.0f)
				_this->fatness = 0.25f;
			else if (_this->fatness <= -1.0f)
				_this->fatness = -0.25f;
		}

		// Continue rendering...
		return Ivk_zCModel_Render(_this, ctx);
	}
}
