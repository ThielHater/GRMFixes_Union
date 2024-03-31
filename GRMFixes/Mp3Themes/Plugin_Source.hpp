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

	// public: __thiscall zCSndSys_MSS::zCSndSys_MSS(void)
	DWORD __fastcall zCSndSys_MSS_Constructor(zCSndSys_MSS* _this, void* vtable);
	CInvoke<DWORD(__thiscall*)(zCSndSys_MSS* _this)> Ivk_zCSndSys_MSS_Constructor(GothicMemoryLocations::zCSndSys_MSS::Constructor, &zCSndSys_MSS_Constructor);

	// public: virtual void __thiscall zCSndSys_MSS::StopAllSounds(void)
	void __fastcall zCSndSys_MSS_StopAllSounds(zCSndSys_MSS* _this, void* vtable);
	CInvoke<void(__thiscall*)(zCSndSys_MSS* _this)> Ivk_zCSndSys_MSS_StopAllSounds(GothicMemoryLocations::zCSndSys_MSS::StopAllSounds, &zCSndSys_MSS_StopAllSounds);

	// public: virtual void __thiscall zCSndSys_MSS::UpdateSoundProps(int const &,int,float,float)
	void __fastcall zCSndSys_MSS_UpdateSoundProps(zCSndSys_MSS* _this, void* vtable, const int& sfxHandle, int freq, float vol, float pan);
	CInvoke<void(__thiscall*)(zCSndSys_MSS* _this, const int& sfxHandle, int freq, float vol, float pan)> Ivk_zCSndSys_MSS_UpdateSoundProps(GothicMemoryLocations::zCSndSys_MSS::UpdateSoundProps, &zCSndSys_MSS_UpdateSoundProps);

	// public: __thiscall zCMusicSys_DirectMusic::zCMusicSys_DirectMusic(void)
	DWORD __fastcall zCMusicSys_DirectMusic_Constructor(zCMusicSys_DirectMusic* _this, void* vtable);
	CInvoke<DWORD(__thiscall*)(zCMusicSys_DirectMusic* _this)> Ivk_zCMusicSys_DirectMusic_Constructor(GothicMemoryLocations::zCMusicSys_DirectMusic::Constructor, &zCMusicSys_DirectMusic_Constructor);

	// public: virtual void __thiscall zCMusicSys_DirectMusic::SetVolume(float)
	void __fastcall zCMusicSys_DirectMusic_SetVolume(zCMusicSys_DirectMusic* _this, void* vtable, float vol);
	CInvoke<void(__thiscall*)(zCMusicSys_DirectMusic* _this, float vol)> Ivk_zCMusicSys_DirectMusic_SetVolume(GothicMemoryLocations::zCMusicSys_DirectMusic::SetVolume, &zCMusicSys_DirectMusic_SetVolume);

	// public: virtual void __thiscall zCMusicSys_DirectMusic::PlayThemeByScript(class zSTRING const &,int,int *)
	void __fastcall zCMusicSys_DirectMusic_PlayThemeByScript(zCMusicSys_DirectMusic* _this, void* vtable, const zSTRING& id, const int manipulate, zBOOL* done);
	CInvoke<void(__thiscall*)(zCMusicSys_DirectMusic* _this, const zSTRING& id, const int manipulate, zBOOL* done)> Ivk_zCMusicSys_DirectMusic_PlayThemeByScript(GothicMemoryLocations::zCMusicSys_DirectMusic::PlayThemeByScript, &zCMusicSys_DirectMusic_PlayThemeByScript);

	// public: virtual void __thiscall zCMusicSys_DirectMusic::PlayTheme(class zCMusicTheme *,float const &,enum zTMus_TransType const &,enum zTMus_TransSubType const &)
	void __fastcall zCMusicSys_DirectMusic_PlayTheme(zCMusicSys_DirectMusic* _this, void* vtable, struct zCMusicTheme* theme, const float& volume, const zTMus_TransType& tr, const zTMus_TransSubType& trSub);
	CInvoke<void(__thiscall*)(zCMusicSys_DirectMusic* _this, struct zCMusicTheme* theme, const float& volume, const zTMus_TransType& tr, const zTMus_TransSubType& trSub)> Ivk_zCMusicSys_DirectMusic_PlayTheme(GothicMemoryLocations::zCMusicSys_DirectMusic::PlayTheme, &zCMusicSys_DirectMusic_PlayTheme);

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
							g_zCSndSys_MSS->StopSound(it->second.handle);
						}
						else if (it->second.type == TransitionType::DirectMusic)
						{
							cmd << "DirectMusic";
							g_zCMusicSys_DirectMusic->Stop();
							g_zCMusicSys_DirectMusic->SetVolume(g_MusicVolume);
						}

						SetConsoleTextAttribute(con, 2);
						cmd << "!" << endl;
						SetConsoleTextAttribute(con, 8);

						g_themeHandlesMutex.lock();
						g_themeHandles.erase(it->first);
						if (g_currentTheme == it->second.fileName)
						{
							g_currentTheme = "";
							g_currentThemeHandle = NULL;
						}
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

	DWORD __fastcall zCSndSys_MSS_Constructor(zCSndSys_MSS* _this, void* vtable)
	{
		//assert(std::is_same(g_zCSndSys_MSS, NULL));
		DWORD result = Ivk_zCSndSys_MSS_Constructor(_this);
		g_zCSndSys_MSS = (zCSndSys_MSS*)result;
		std::thread(&ProcessTransitions).detach();
		return result;
	}

	void __fastcall zCSndSys_MSS_StopAllSounds(zCSndSys_MSS* _this, void* vtable)
	{
		if (g_currentThemeHandle != NULL)
		{
			std::thread(&AddTransition, g_currentTheme, TransitionType::MilesSoundSystem, g_currentThemeHandle, FadeMode::Out, 0).detach();
		}
		Ivk_zCSndSys_MSS_StopAllSounds(_this);
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

		if (g_currentTheme != nextTheme)
		{
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

			char searchResult[MAX_PATH];

			if (vdf_searchfile((text)(nextTheme + ".wav").c_str(), searchResult))
			{
				cmd << "Miles";

				if (g_zCSndSys_MSS == NULL)
				{
					cmd << " but handle is not yet valid." << endl;
					return;
				}

				g_themeHandlesMutex.lock();

				std::unordered_map<std::string, int>::iterator it = g_themeHandles.find(nextTheme);

				if (it == g_themeHandles.end())
				{
					zCSndFX_MSS* soundFx = (zCSndFX_MSS*)g_zCSndSys_MSS->LoadSoundFX((nextTheme + ".wav").c_str());
					soundFx->SetLooping(TRUE);
					g_currentThemeHandle = g_zCSndSys_MSS->PlaySound(soundFx, zSND_SLOT_NONE, zSND_FREQ_DEFAULT, 0.0F, zSND_PAN_CENTER);
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
