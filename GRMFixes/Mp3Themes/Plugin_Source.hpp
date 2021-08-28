// Source file
// Multi-platform file can not have `#pragma once`!!!
// For compile plugin in Multiplatform mode - select 'Debug' or 'Release' configuration.

#include "Plugin_Header.h"

// TODO: Reset der Transitions beim Laden eines Spielstandes
namespace NAMESPACE
{
	/* Global variables */
	zCSndSys_MSS* g_zCSndSys_MSS = NULL;
	zCMusicSys_DirectMusic* g_zCMusicSys_DirectMusic = NULL;
	float g_MusicVolume = 0.8F;
	std::string g_currentTheme = std::string("");
	int g_currentThemeHandle = NULL;
	std::unordered_map<std::string, int> g_themeHandles = std::unordered_map<std::string, int>();
	std::unordered_map<std::string, Transition> g_transitions = std::unordered_map<std::string, Transition>();
	std::mutex g_themeHandlesMutex;
	std::mutex g_transitionsMutex;

	// 0x004E8380 public: virtual void __thiscall zCSndFX_MSS::SetLooping(int)
	void __fastcall zCSndFX_MSS_SetLooping(zCSndFX_MSS* _this, void* vtable, int loop);
	CInvoke<void(__thiscall*)(zCSndFX_MSS* _this, int loop)> Ivk_zCSndFX_MSS_SetLooping(0x004E8380, &zCSndFX_MSS_SetLooping);

	// 0x004DD530 public: __thiscall zCSndSys_MSS::zCSndSys_MSS(void)
	DWORD __fastcall zCSndSys_MSS_Constructor(zCSndSys_MSS* _this, void* vtable);
	CInvoke<DWORD(__thiscall*)(zCSndSys_MSS* _this)> Ivk_zCSndSys_MSS_Constructor(0x004DD530, &zCSndSys_MSS_Constructor);

	// 0x004E0370 public: virtual class zCSoundFX * __thiscall zCSndSys_MSS::LoadSoundFX(class zSTRING const &)
	zCSoundFX* __fastcall zCSndSys_MSS_LoadSoundFX(zCSndSys_MSS* _this, void* vtable, const zSTRING& fileName);
	CInvoke<zCSoundFX* (__thiscall*)(zCSndSys_MSS* _this, const zSTRING& fileName)> Ivk_zCSndSys_MSS_LoadSoundFX(0x004E0370, &zCSndSys_MSS_LoadSoundFX);

	// 0x004E3220 public: virtual int __thiscall zCSndSys_MSS::PlaySound(class zCSoundFX *,int,int,float,float)
	int __fastcall zCSndSys_MSS_PlaySound(zCSndSys_MSS* _this, void* vtable, zCSoundFX* sfx, int slot, int freq, float vol, float pan);
	CInvoke<int(__thiscall*)(zCSndSys_MSS* _this, zCSoundFX* sfx, int slot, int freq, float vol, float pan)> Ivk_zCSndSys_MSS_PlaySound(0x004E3220, &zCSndSys_MSS_PlaySound);

	// 0x004E4610 public: virtual void __thiscall zCSndSys_MSS::StopSound(int const &)
	void __fastcall zCSndSys_MSS_StopSound(zCSndSys_MSS* _this, void* vtable, const int& sfxHandle);
	CInvoke<void(__thiscall*)(zCSndSys_MSS* _this, const int& sfxHandle)> Ivk_zCSndSys_MSS_StopSound(0x004E4610, &zCSndSys_MSS_StopSound);

	// 0x004E5A20 public: virtual void __thiscall zCSndSys_MSS::UpdateSoundProps(int const &,int,float,float)
	void __fastcall zCSndSys_MSS_UpdateSoundProps(zCSndSys_MSS* _this, void* vtable, const int& sfxHandle, int freq, float vol, float pan);
	CInvoke<void(__thiscall*)(zCSndSys_MSS* _this, const int& sfxHandle, int freq, float vol, float pan)> Ivk_zCSndSys_MSS_UpdateSoundProps(0x004E5A20, &zCSndSys_MSS_UpdateSoundProps);

	// 0x004DA270 public: __thiscall zCMusicSys_DirectMusic::zCMusicSys_DirectMusic(void)
	DWORD __fastcall zCMusicSys_DirectMusic_Constructor(zCMusicSys_DirectMusic* _this, void* vtable);
	CInvoke<DWORD(__thiscall*)(zCMusicSys_DirectMusic* _this)> Ivk_zCMusicSys_DirectMusic_Constructor(0x004DA270, &zCMusicSys_DirectMusic_Constructor);

	// 0x004DCC80 public: virtual void __thiscall zCMusicSys_DirectMusic::SetVolume(float)
	void __fastcall zCMusicSys_DirectMusic_SetVolume(zCMusicSys_DirectMusic* _this, void* vtable, float vol);
	CInvoke<void(__thiscall*)(zCMusicSys_DirectMusic* _this, float vol)> Ivk_zCMusicSys_DirectMusic_SetVolume(0x004DCC80, &zCMusicSys_DirectMusic_SetVolume);

	// 0x004DCBF0 public: virtual void __thiscall zCMusicSys_DirectMusic::Stop(void)
	void __fastcall zCMusicSys_DirectMusic_Stop(zCMusicSys_DirectMusic* _this, void* vtable);
	CInvoke<void(__thiscall*)(zCMusicSys_DirectMusic* _this)> Ivk_zCMusicSys_DirectMusic_Stop(0x004DCBF0, &zCMusicSys_DirectMusic_Stop);

	// 0x004DB850 public: virtual void __thiscall zCMusicSys_DirectMusic::PlayThemeByScript(class zSTRING const &,int,int *)
	void __fastcall zCMusicSys_DirectMusic_PlayThemeByScript(zCMusicSys_DirectMusic* _this, void* vtable, const zSTRING& id, const int manipulate, zBOOL* done);
	CInvoke<void(__thiscall*)(zCMusicSys_DirectMusic* _this, const zSTRING& id, const int manipulate, zBOOL* done)> Ivk_zCMusicSys_DirectMusic_PlayThemeByScript(0x004DB850, &zCMusicSys_DirectMusic_PlayThemeByScript);

	// 0x004DC4E0 public: virtual void __thiscall zCMusicSys_DirectMusic::PlayTheme(class zCMusicTheme *,float const &,enum zTMus_TransType const &,enum zTMus_TransSubType const &)
	void __fastcall zCMusicSys_DirectMusic_PlayTheme(zCMusicSys_DirectMusic* _this, void* vtable, struct zCMusicTheme* theme, const float& volume, const zTMus_TransType& tr, const zTMus_TransSubType& trSub);
	CInvoke<void(__thiscall*)(zCMusicSys_DirectMusic* _this, struct zCMusicTheme* theme, const float& volume, const zTMus_TransType& tr, const zTMus_TransSubType& trSub)> Ivk_zCMusicSys_DirectMusic_PlayTheme(0x004DC4E0, &zCMusicSys_DirectMusic_PlayTheme);

	bool FileExists(const char* fileName)
	{
		std::ifstream infile(fileName);
		return infile.good();
	}

	void AddTransition(std::string fileName, TransitionType type, int handle, FadeMode mode, int duration)
	{
		g_transitionsMutex.lock();

		std::unordered_map<std::string, Transition>::iterator it = g_transitions.find(fileName);

		if (it == g_transitions.end())
		{
			float vol = g_MusicVolume;
			if (mode == FadeMode::In)
				vol = 0.0F;
			else if (mode == FadeMode::Out)
				vol = g_MusicVolume;

			struct Transition trans = {
				type,
				fileName,
				handle,
				mode,
				duration,
				vol
			};

			g_transitions.insert(it, std::pair<std::string, Transition>(fileName, trans));
		}
		else
		{
			//assert(std::is_same(it->second.handle, handle));
			it->second.mode = mode;
			it->second.duration = duration;
			// do not change volume
		}

		g_transitionsMutex.unlock();
	}

	void ProcessTransitions()
	{
		int interval = 100;

		while (true)
		{
			g_transitionsMutex.lock();

			std::unordered_map<std::string, Transition>::iterator it = g_transitions.begin();

			while (it != g_transitions.end())
			{
				float delta = (it->second.duration > 0) ? 1.0F / (it->second.duration / interval) : 1.0F;
				if (it->second.mode == FadeMode::In)
					it->second.volume = std::min(it->second.volume + delta, g_MusicVolume);
				else if (it->second.mode == FadeMode::Out)
					it->second.volume = std::max(it->second.volume - delta, 0.0F);

				if (it->second.type == TransitionType::MilesSoundSystem)
				{
					Ivk_zCSndSys_MSS_UpdateSoundProps(g_zCSndSys_MSS, it->second.handle, zSND_FREQ_DEFAULT, it->second.volume, zSND_PAN_CENTER);
				}
				else if (it->second.type == TransitionType::DirectMusic)
				{
					Ivk_zCMusicSys_DirectMusic_SetVolume(g_zCMusicSys_DirectMusic, it->second.volume);
				}

				if (it->second.duration == 0
					|| (it->second.mode == FadeMode::In && it->second.volume == g_MusicVolume)
					|| (it->second.mode == FadeMode::Out && it->second.volume == 0.0F))
				{
					if (it->second.mode == FadeMode::Out)
					{
						HANDLE con = cmd.GetHandle();
						SetConsoleTextAttribute(con, 2);
						cmd << "Plugin: Stopped ";
						SetConsoleTextAttribute(con, 10);

						if (it->second.type == TransitionType::MilesSoundSystem)
						{
							cmd << "Miles";
							Ivk_zCSndSys_MSS_StopSound(g_zCSndSys_MSS, it->second.handle);
						}
						else if (it->second.type == TransitionType::DirectMusic)
						{
							cmd << "DirectMusic";
							Ivk_zCMusicSys_DirectMusic_Stop(g_zCMusicSys_DirectMusic);
							Ivk_zCMusicSys_DirectMusic_SetVolume(g_zCMusicSys_DirectMusic, g_MusicVolume);
						}

						SetConsoleTextAttribute(con, 2);
						cmd << "!" << endl;
						SetConsoleTextAttribute(con, 8);

						g_themeHandlesMutex.lock();
						g_themeHandles.erase(it->first);
						g_themeHandlesMutex.unlock();
					}

					it = g_transitions.erase(it);
				}
				else
				{
					++it;
				}
			}

			g_transitionsMutex.unlock();

			std::this_thread::sleep_for(std::chrono::milliseconds(interval));
		}
	}

	void __fastcall zCSndFX_MSS_SetLooping(zCSndFX_MSS* _this, void* vtable, int loop)
	{
		Ivk_zCSndFX_MSS_SetLooping(_this, loop);
	}

	DWORD __fastcall zCSndSys_MSS_Constructor(zCSndSys_MSS* _this, void* vtable)
	{
		//assert(std::is_same(g_zCSndSys_MSS, NULL));
		DWORD result = Ivk_zCSndSys_MSS_Constructor(_this);
		g_zCSndSys_MSS = (zCSndSys_MSS*)result;
		std::thread(&ProcessTransitions).detach();
		return result;
	}

	zCSoundFX* __fastcall zCSndSys_MSS_LoadSoundFX(zCSndSys_MSS* _this, void* vtable, const zSTRING& fileName)
	{
		return Ivk_zCSndSys_MSS_LoadSoundFX(_this, fileName);
	}

	int __fastcall zCSndSys_MSS_PlaySound(zCSndSys_MSS* _this, void* vtable, zCSoundFX* sfx, int slot, int freq, float vol, float pan)
	{
		return Ivk_zCSndSys_MSS_PlaySound(_this, sfx, slot, freq, vol, pan);
	}

	void __fastcall zCSndSys_MSS_StopSound(zCSndSys_MSS* _this, void* vtable, const int& sfxHandle)
	{
		Ivk_zCSndSys_MSS_StopSound(_this, sfxHandle);
	}

	void __fastcall zCSndSys_MSS_UpdateSoundProps(zCSndSys_MSS* _this, void* vtable, const int & sfxHandle, int freq, float vol, float pan)
	{
		Ivk_zCSndSys_MSS_UpdateSoundProps(_this, sfxHandle, freq, vol, pan);
	}

	DWORD __fastcall zCMusicSys_DirectMusic_Constructor(zCMusicSys_DirectMusic* _this, void* vtable)
	{
		//assert(std::is_same(g_zCMusicSys_DirectMusic, NULL));
		DWORD result = Ivk_zCMusicSys_DirectMusic_Constructor(_this);
		g_zCMusicSys_DirectMusic = (zCMusicSys_DirectMusic*)result;
		return result;
	}

	void __fastcall zCMusicSys_DirectMusic_SetVolume(zCMusicSys_DirectMusic* _this, void* vtable, float vol)
	{
		g_MusicVolume = vol;
		if (g_zCSndSys_MSS != NULL && g_currentThemeHandle != NULL)
		{
			Ivk_zCSndSys_MSS_UpdateSoundProps(g_zCSndSys_MSS, g_currentThemeHandle, zSND_FREQ_DEFAULT, g_MusicVolume, zSND_PAN_CENTER);
		}
	}

	void __fastcall zCMusicSys_DirectMusic_Stop(zCMusicSys_DirectMusic* _this, void* vtable)
	{
		Ivk_zCMusicSys_DirectMusic_Stop(_this);
	}

	void __fastcall zCMusicSys_DirectMusic_PlayThemeByScript(zCMusicSys_DirectMusic* _this, void* vtable, const zSTRING& id, const int manipulate, zBOOL* done)
	{
		std::string name = std::string(id.ToChar());
		if (name != "")
		{
			HANDLE con = cmd.GetHandle();
			SetConsoleTextAttribute(con, 2);
			cmd << "Plugin: Play script instance ";
			SetConsoleTextAttribute(con, 10);
			cmd << name.c_str();
			SetConsoleTextAttribute(con, 2);
			cmd << "!" << endl;
			SetConsoleTextAttribute(con, 8);
			Ivk_zCMusicSys_DirectMusic_PlayThemeByScript(_this, id, manipulate, done);
		}
	}

	void __fastcall zCMusicSys_DirectMusic_PlayTheme(zCMusicSys_DirectMusic* _this, void* vtable, struct zCMusicTheme* theme, const float& volume, const zTMus_TransType& tr, const zTMus_TransSubType& trSub)
	{
		std::string nextTheme(theme->fileName.ToChar());
		nextTheme = nextTheme.substr(0, nextTheme.find_last_of("."));
		std::transform(nextTheme.begin(), nextTheme.end(), nextTheme.begin(), ::toupper);

		HANDLE con = cmd.GetHandle();
		SetConsoleTextAttribute(con, 2);
		cmd << "Plugin: ";

		if (g_currentTheme != nextTheme && g_zCSndSys_MSS != NULL)
		{
			char exe[MAX_PATH], drive[MAX_PATH], dir[MAX_PATH], wave[MAX_PATH];
			GetModuleFileName(NULL, exe, MAX_PATH);
			_splitpath(exe, drive, dir, NULL, NULL);
			strcpy_s(wave, drive);
			strcat_s(wave, dir);
			strcat_s(wave, "..\\_work\\DATA\\Sound\\SFX\\");
			strcat_s(wave, nextTheme.c_str());
			strcat_s(wave, ".wav");
			_fullpath(wave, wave, MAX_PATH);

			int fadeOut = 3000;
			int fadeIn = 3000;

			zTMus_TransType trType = (tr == zMUS_TR_DEFAULT) ? theme->trType : tr;

			if (trType == zTMus_TransType::zMUS_TR_NONE)
			{
				fadeOut = 500;
				fadeIn = 0;
			}
			else if (trType == zTMus_TransType::zMUS_TR_FILL)
			{
				fadeOut = 1000;
				fadeIn = 2000;
			}

			if (g_currentThemeHandle != NULL)
			{
				std::thread(&AddTransition, g_currentTheme, TransitionType::MilesSoundSystem, g_currentThemeHandle, FadeMode::Out, fadeOut).detach();
			}
			else
			{
				std::thread(&AddTransition, "", TransitionType::DirectMusic, NULL, FadeMode::Out, fadeOut).detach();
			}

			cmd << "Playing ";
			SetConsoleTextAttribute(con, 10);
			cmd << nextTheme.c_str();
			SetConsoleTextAttribute(con, 2);
			cmd << " using ";
			SetConsoleTextAttribute(con, 10);

			if (FileExists(wave))
			{
				cmd << "Miles";

				g_themeHandlesMutex.lock();

				std::unordered_map<std::string, int>::iterator it = g_themeHandles.find(nextTheme);

				if (it == g_themeHandles.end())
				{
					zCSndFX_MSS* soundFx = (zCSndFX_MSS*)Ivk_zCSndSys_MSS_LoadSoundFX(g_zCSndSys_MSS, (nextTheme + ".wav").c_str());
					Ivk_zCSndFX_MSS_SetLooping(soundFx, TRUE);
					g_currentThemeHandle = Ivk_zCSndSys_MSS_PlaySound(g_zCSndSys_MSS, soundFx, zSND_SLOT_NONE, zSND_FREQ_DEFAULT, 0.0F, zSND_PAN_CENTER);
					g_themeHandles.insert(g_themeHandles.end(), std::pair<std::string, int>(nextTheme, g_currentThemeHandle));
				}
				else
				{
					g_currentThemeHandle = it->second;
				}

				g_themeHandlesMutex.unlock();

				std::thread(&AddTransition, nextTheme, TransitionType::MilesSoundSystem, g_currentThemeHandle, FadeMode::In, fadeIn).detach();
			}
			else
			{
				cmd << "DirectMusic";

				g_currentThemeHandle = NULL;
				Ivk_zCMusicSys_DirectMusic_PlayTheme(_this, theme, volume, tr, trSub);
				std::thread(&AddTransition, "", TransitionType::DirectMusic, NULL, FadeMode::In, fadeIn).detach();
			}

			SetConsoleTextAttribute(con, 2);
			cmd << ".." << endl;
			SetConsoleTextAttribute(con, 8);

			g_currentTheme = nextTheme;
		}
		else
		{
			cmd << "Playing ";
			SetConsoleTextAttribute(con, 10);
			cmd << g_currentTheme.c_str();
			SetConsoleTextAttribute(con, 2);
			cmd << " already.." << endl;
			SetConsoleTextAttribute(con, 8);
		}
	}
}
