// Source file
// Multi-platform file can not have `#pragma once`!!!
// For compile plugin in Multiplatform mode - select 'Debug' or 'Release' configuration.

#include "Plugin_Header.h"

namespace NAMESPACE
{
	void AdjustMagicBarrier()
	{
		float pointsWorld[38][2] = { { 57939.2f, 1280.28f }, { 55954.4f, 5421.51f }, { 52856.8f, 10047.0f }, { 49451.9f, 14908.2f }, { 44199.8f, 20513.3f }, { 37684.2f, 26271.2f }, { 30434.0f, 31462.4f }, { 25573.6f, 32692.7f }, { 21248.3f, 35176.1f }, { 19450.7f, 35205.0f }, { 16263.1f, 32799.6f }, { 10755.6f, 34744.4f }, { 9736.9f, 37990.5f }, { 8218.6f, 38393.1f }, { 4065.0f, 39018.4f }, { 839.9f, 39079.3f }, { -9312.9f, 38694.2f }, { -26219.2f, 40844.2f }, { -34576.0f, 43032.3f }, { -44458.8f, 43099.2f }, { -49763.7f, 37384.8f }, { -54137.3f, 26761.7f }, { -62089.3f, 21598.1f }, { -66193.7f, 12999.2f }, { -66132.3f, 6204.0f }, { -63855.2f, -5700.8f }, { -59385.1f, -10081.5f }, { -56013.8f, -22393.4f }, { -47250.3f, -28502.0f }, { -37136.5f, -38319.2f }, { -24664.7f, -46687.9f }, { -7860.6f, -48966.6f }, { 4876.6f, -49691.0f }, { 23147.8f, -47875.1f }, { 48722.3f, -39488.8f }, { 55902.4f, -31909.8f }, { 61238.6f, -23412.8f }, { 60230.1f, -6641.9f } };

		HANDLE con = cmd.GetHandle();
		SetConsoleTextAttribute(con, 2);
		cmd << "Plugin: ";
		SetConsoleTextAttribute(con, 10);
		cmd << "Adjusting magic barrier..." << endl;
		SetConsoleTextAttribute(con, 8);
		
		DWORD dwProtect;
		VirtualProtect((void*)GothicMemoryLocations::oCMagFrontier::PointsWorld, sizeof(pointsWorld), PAGE_EXECUTE_READWRITE, &dwProtect);
		memcpy((void*)GothicMemoryLocations::oCMagFrontier::PointsWorld, pointsWorld, sizeof(pointsWorld));
	}
}
