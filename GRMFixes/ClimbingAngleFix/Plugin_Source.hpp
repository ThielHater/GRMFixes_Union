// Source file
// Multi-platform file can not have `#pragma once`!!!
// For compile plugin in Multiplatform mode - select 'Debug' or 'Release' configuration.

#include "Plugin_Header.h"

namespace NAMESPACE
{
	// 0x006405A0 public: void __thiscall oCAniCtrl_Human::SetScriptValues(void)
	void __fastcall oCAniCtrl_Human_SetScriptValues(oCAniCtrl_Human* _this, void* vtable);
	CInvoke<void(__thiscall*)(oCAniCtrl_Human* _this)> Ivk_oCAniCtrl_Human_SetScriptValues(GothicMemoryLocations::oCAniCtrl_Human::SetScriptValues, &oCAniCtrl_Human_SetScriptValues);

	void __fastcall oCAniCtrl_Human_SetScriptValues(oCAniCtrl_Human* _this, void* vtable)
	{
		// Call this first, to initialize the instance
		Ivk_oCAniCtrl_Human_SetScriptValues(_this);

		// Patch climbing angle to Gothic 2
		oTGilValues* gil = (oTGilValues*)GothicMemoryLocations::oTGilValues::Instance;
		gil->CLIMB_GROUND_ANGLE[GIL_HUMAN] = 50;
		gil->CLIMB_HEADING_ANGLE[GIL_HUMAN] = 50;
		gil->CLIMB_HORIZ_ANGLE[GIL_HUMAN] = 50;

		// Call another time, to set our new values
		Ivk_oCAniCtrl_Human_SetScriptValues(_this);
	}
}
