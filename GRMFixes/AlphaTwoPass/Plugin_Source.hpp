// Source file
// Multi-platform file can not have `#pragma once`!!!
// For compile plugin in Multiplatform mode - select 'Debug' or 'Release' configuration.

#include "Plugin_Header.h"

namespace NAMESPACE
{
	std::unordered_map<std::string, int> g_vobMapTwoPass = std::unordered_map<std::string, int>();
	std::unordered_map<void*, bool> g_twoPassHelper = std::unordered_map<void*, bool>();
	int g_vobRenderPass = 0;
	
	// 0x007185C0 private: int __thiscall zCRnd_D3D::XD3D_SetRenderState(enum _D3DRENDERSTATETYPE,unsigned long)
	int __fastcall zCRnd_D3D_XD3D_SetRenderState(zCRnd_D3D* _this, void* vtable, D3DRENDERSTATETYPE state, DWORD value);
	CInvoke<int(__thiscall*)(zCRnd_D3D* _this, D3DRENDERSTATETYPE state, DWORD value)> Ivk_zCRnd_D3D_XD3D_SetRenderState(GothicMemoryLocations::zCRnd_D3D::XD3D_SetRenderState, &zCRnd_D3D_XD3D_SetRenderState);

	// 0x005C8860 public: virtual int __thiscall zCTexture::HasAlpha(void)
	int __fastcall zCTexture_HasAlpha(zCTexture* _this, void* vtable);
	CInvoke<int(__thiscall*)(zCTexture* _this)> Ivk_zCTexture_HasAlpha(GothicMemoryLocations::zCTexture::HasAlpha, &zCTexture_HasAlpha);

	// 0x005D6090 public: virtual int __fastcall zCVob::Render(struct zTRenderContext &)
	int __fastcall zCVob_Render(zCVob* _this, struct zTRenderContext& ctx);
	CInvoke<int(__fastcall*)(zCVob* _this, struct zTRenderContext& ctx)> Ivk_zCVob_Render(GothicMemoryLocations::zCVob::Render, &zCVob_Render);

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
			g_vobMapTwoPass[line] = 1;
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

	int __fastcall zCRnd_D3D_XD3D_SetRenderState(zCRnd_D3D* _this, void* vtable, D3DRENDERSTATETYPE state, DWORD value)
	{
		Ivk_zCRnd_D3D_XD3D_SetRenderState(_this, state, value);

		// Two Pass Rendering Technique (https://blogs.msdn.microsoft.com/shawnhar/2009/02/18/depth-sorting-alpha-blended-objects/)
		if (g_vobRenderPass == 1)
		{
			Ivk_zCRnd_D3D_XD3D_SetRenderState(_this, D3DRENDERSTATETYPE::D3DRENDERSTATE_ALPHATESTENABLE, true);
			Ivk_zCRnd_D3D_XD3D_SetRenderState(_this, D3DRENDERSTATETYPE::D3DRENDERSTATE_ALPHAFUNC, D3DCMP_GREATEREQUAL);
			Ivk_zCRnd_D3D_XD3D_SetRenderState(_this, D3DRENDERSTATETYPE::D3DRENDERSTATE_ALPHAREF, 160);
			Ivk_zCRnd_D3D_XD3D_SetRenderState(_this, D3DRENDERSTATETYPE::D3DRENDERSTATE_ALPHABLENDENABLE, false);
			Ivk_zCRnd_D3D_XD3D_SetRenderState(_this, D3DRENDERSTATETYPE::D3DRENDERSTATE_ZENABLE, true);
		}
		else if (g_vobRenderPass == 2)
		{
			Ivk_zCRnd_D3D_XD3D_SetRenderState(_this, D3DRENDERSTATETYPE::D3DRENDERSTATE_ALPHATESTENABLE, true);
			Ivk_zCRnd_D3D_XD3D_SetRenderState(_this, D3DRENDERSTATETYPE::D3DRENDERSTATE_ALPHAFUNC, D3DCMP_LESS);
			Ivk_zCRnd_D3D_XD3D_SetRenderState(_this, D3DRENDERSTATETYPE::D3DRENDERSTATE_ALPHAREF, 160);
			Ivk_zCRnd_D3D_XD3D_SetRenderState(_this, D3DRENDERSTATETYPE::D3DRENDERSTATE_ALPHABLENDENABLE, true);
			Ivk_zCRnd_D3D_XD3D_SetRenderState(_this, D3DRENDERSTATETYPE::D3DRENDERSTATE_SRCBLEND, D3DBLEND_SRCALPHA);
			Ivk_zCRnd_D3D_XD3D_SetRenderState(_this, D3DRENDERSTATETYPE::D3DRENDERSTATE_DESTBLEND, D3DBLEND_INVSRCALPHA);
			Ivk_zCRnd_D3D_XD3D_SetRenderState(_this, D3DRENDERSTATETYPE::D3DRENDERSTATE_ZENABLE, true);
			Ivk_zCRnd_D3D_XD3D_SetRenderState(_this, D3DRENDERSTATETYPE::D3DRENDERSTATE_ZWRITEENABLE, false);
		}

		return S_OK;
	}
	
	int __fastcall zCTexture_HasAlpha(zCTexture* _this, void* vtable)
	{
		if (g_vobRenderPass > 0)
			return 1;
		else
			return Ivk_zCTexture_HasAlpha(_this);
	}

	int __fastcall zCVob_Render(zCVob* _this, struct zTRenderContext& ctx)
	{
		bool useTwoPass = false;
		int result = 0;

		auto it = g_twoPassHelper.find(_this);

		if (it == g_twoPassHelper.end())
		{
			if (!g_vobMapTwoPass.empty() && _this->visual && _this->visual->objectName
				&& g_vobMapTwoPass.find(_this->visual->objectName.ToChar()) != g_vobMapTwoPass.end())
			{
				useTwoPass = true;
			}

			g_twoPassHelper.insert(it, std::pair<void*, bool>(_this, useTwoPass));
		}
		else
		{
			useTwoPass = it->second;
		}

		g_vobRenderPass = 1;
		result = Ivk_zCVob_Render(_this, ctx);

		if (useTwoPass)
		{
			g_vobRenderPass = 2;
			result = Ivk_zCVob_Render(_this, ctx);
		}

		g_vobRenderPass = 0;
		return result;
	}
}
