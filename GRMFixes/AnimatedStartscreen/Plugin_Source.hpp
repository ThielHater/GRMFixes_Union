// Source file
// Multi-platform file can not have `#pragma once`!!!
// For compile plugin in Multiplatform mode - select 'Debug' or 'Release' configuration.

#include "Plugin_Header.h"

namespace NAMESPACE
{
	void Init()
	{
		std::thread animThread(Animate);
		animThread.detach();
	}

	void Animate()
	{
		int curFrame = 0;
		int maxFrame = 0;

		do
		{
			zCTexture* tex = zCTexture::Load(zSTRING("STARTSCREEN_A") + zSTRING(maxFrame) + zSTRING(".TGA"), FALSE);

			zCOLOR avg = tex->GetTextureInfo().averageColor;

			if (avg.dword == 0xff29241d || avg.dword == 0xff800080 || avg.dword == 0) // check for default texture
			{
				maxFrame--;
				break;
			}
			else
			{
				maxFrame++;
			}
		}
		while (true);

		while (maxFrame > 0 && !gameMan->IsGameRunning() && !gameMan->exitGame)
		{
			gameMan->initScreen->InsertBack(zSTRING("STARTSCREEN_A") + zSTRING(curFrame) + zSTRING(".TGA"));

			auto timePoint = std::chrono::steady_clock::now() + std::chrono::milliseconds(40); // 25 fps
			std::this_thread::sleep_until(timePoint);

			if (curFrame < maxFrame)
				curFrame++;
			else
				curFrame = 0;
		}
	}
}