// Header file
// Multi-platform file can not have `#pragma once`!!!

#include <algorithm>
#include <mutex>
#include <unordered_map>

// dynamic namespace
namespace NAMESPACE
{
	enum FadeMode
	{
		Out = 0,
		In = 1,
	};

	enum TransitionType
	{
		MilesSoundSystem = 0,
		DirectMusic = 1,
	};

	struct Transition
	{
		TransitionType type;
		std::string fileName;
		int handle;
		FadeMode mode;
		int duration;
		float volume;
	};

#define GOTHIC2A

	struct GothicMemoryLocations
	{
#ifdef GOTHIC1
		struct zCSndSys_MSS
		{
			static const int Constructor = 0x004DD530;
			static const int StopAllSounds = 0x004E46D0;
			static const int UpdateSoundProps = 0x004E5A20;
		};

		struct zCMusicSys_DirectMusic
		{
			static const int Constructor = 0x004DA270;
			static const int SetVolume = 0x004DCC80;
			static const int PlayThemeByScript = 0x004DB850;
			static const int PlayTheme = 0x004DC4E0;
		};
#endif
#ifdef GOTHIC2A
		struct zCSndSys_MSS
		{
			static const int Constructor = 0x004EAAF0;
			static const int StopAllSounds = 0x004F23C0;
			static const int UpdateSoundProps = 0x004F3970;
		};

		struct zCMusicSys_DirectMusic
		{
			static const int Constructor = 0x004E7450;
			static const int SetVolume = 0x004E9FD0;
			static const int PlayThemeByScript = 0x004E8AB0;
			static const int PlayTheme = 0x004E9820;
		};
#endif
	};
}