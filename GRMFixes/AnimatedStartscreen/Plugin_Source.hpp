// Source file
// Multi-platform file can not have `#pragma once`!!!
// For compile plugin in Multiplatform mode - select 'Debug' or 'Release' configuration.

#include "Plugin_Header.h"
#include "bink.h"

namespace NAMESPACE
{
    #define zRES_CACHE_LOCKED 2

    // 0x00429B80 public: int __thiscall CGameManager::PlayVideo(class zSTRING,int)
    int __fastcall CGameManager_PlayVideo(CGameManager* _this, void* vtable, zSTRING videoFile, int param);
    CInvoke<int(__thiscall*)(CGameManager* _this, zSTRING videoFile, int param)> Ivk_CGameManager_PlayVideo(GothicMemoryLocations::CGameManager::PlayVideo, &CGameManager_PlayVideo);

    // 0x004DB850 public: virtual void __thiscall zCMusicSys_DirectMusic::PlayThemeByScript(class zSTRING const &,int,int *)
    void __fastcall zCMusicSys_DirectMusic_PlayThemeByScript(zCMusicSys_DirectMusic* _this, void* vtable, const zSTRING& id, const int manipulate, zBOOL* done);
    CInvoke<void(__thiscall*)(zCMusicSys_DirectMusic* _this, const zSTRING& id, const int manipulate, zBOOL* done)> Ivk_zCMusicSys_DirectMusic_PlayThemeByScript(GothicMemoryLocations::zCMusicSys_DirectMusic::PlayThemeByScript, &zCMusicSys_DirectMusic_PlayThemeByScript);

    bool g_PauseVideo = false;
    BINK* g_bink = NULL;

    void Init()
    {
        std::thread animThread(Animate);
        animThread.detach();
    }

    void Animate()
    {
        HMODULE dllBink = GetModuleHandleA("binkw32");
        if (!dllBink)
            return;

        PFN_BinkOpen BinkOpen = (PFN_BinkOpen)GetProcAddress(dllBink, "_BinkOpen@8");
        PFN_BinkClose BinkClose = (PFN_BinkClose)GetProcAddress(dllBink, "_BinkClose@4");
        PFN_BinkGoto BinkGoto = (PFN_BinkGoto)GetProcAddress(dllBink, "_BinkGoto@12");
        PFN_BinkDoFrame BinkDoFrame = (PFN_BinkDoFrame)GetProcAddress(dllBink, "_BinkDoFrame@4");
        PFN_BinkNextFrame BinkNextFrame = (PFN_BinkNextFrame)GetProcAddress(dllBink, "_BinkNextFrame@4");
        PFN_BinkWait BinkWait = (PFN_BinkWait)GetProcAddress(dllBink, "_BinkWait@4");
        PFN_BinkCopyToBuffer BinkCopyToBuffer = (PFN_BinkCopyToBuffer)GetProcAddress(dllBink, "_BinkCopyToBuffer@28");
        PFN_BinkOpenWaveOut BinkOpenWaveOut = (PFN_BinkOpenWaveOut)GetProcAddress(dllBink, "_BinkOpenWaveOut@4");
        PFN_BinkSetSoundSystem BinkSetSoundSystem = (PFN_BinkSetSoundSystem)GetProcAddress(dllBink, "_BinkSetSoundSystem@8");
        PFN_BinkSetSoundOnOff BinkSetSoundOnOff = (PFN_BinkSetSoundOnOff)GetProcAddress(dllBink, "_BinkSetSoundOnOff@8");
        PFN_BinkSetVolume BinkSetVolume = (PFN_BinkSetVolume)GetProcAddress(dllBink, "_BinkSetVolume@8");
        PFN_BinkPause BinkPause = (PFN_BinkPause)GetProcAddress(dllBink, "_BinkPause@8");

        zSTRING rootDir = zoptions->GetDirString(DIR_ROOT);
        zSTRING videoDir = zoptions->GetDirString(DIR_VIDEO);
        zSTRING videoFile = zSTRING("STARTSCREEN.BIK");

        BinkSetSoundSystem(BinkOpenWaveOut, 0);

        g_bink = BinkOpen(rootDir + videoDir + videoFile, 0);
        if (!g_bink)
            return;

        if (g_bink->NumTracks > 0)
        {
            BinkSetSoundOnOff(g_bink, 1);
            BinkSetVolume(g_bink, 0, 65535);
        }

        zCTexture* tex = zrenderer->CreateTexture();
        gameMan->initScreen->InsertBack(tex);

        while (!gameMan->IsGameRunning() && !gameMan->exitGame)
        {
            if (g_PauseVideo)
            {
                BinkPause(g_bink, 1);
                continue;
            }
            else
            {
                BinkPause(g_bink, 0);
            }

            BinkDoFrame(g_bink);
            BinkNextFrame(g_bink);

            // wait until it is time to show the frame
            while (true)
            {
                if (!BinkWait(g_bink))
                    break;
            }

            if (!zrenderer)
                break;

            zCTextureConvert* texConv = zrenderer->CreateTextureConvert();

            if (!texConv)
                break;

            zCTextureInfo texInfo;
            // Gothic and Bink have to use the same format!
            // As RGB textures are not supported by Gothic, this option yields best quality.
            texInfo.texFormat = zRND_TEX_FORMAT_RGB_565;
            texInfo.sizeX = g_bink->Width;
            texInfo.sizeY = g_bink->Height;
            texInfo.numMipMap = 1;
            texConv->SetTextureInfo(texInfo);

            texConv->Lock(zRES_CACHE_LOCKED);

            BYTE* texBuffer;
            int pitch; // width * bytes per pixel
            texConv->GetTextureBuffer(0, (void*&)texBuffer, pitch);

            if (!texBuffer)
                break;

            BinkCopyToBuffer(g_bink, texBuffer, pitch, g_bink->Height, 0, 0, BINKCOPYALL | BINKSURFACE565);

            /*
            // use zRND_TEX_FORMAT_RGB_888 and BINKSURFACE24R then convert to DXT1 - works but is too slow
            zCTextureInfo texInfoNew;
            texInfoNew.texFormat = zRND_TEX_FORMAT_DXT1;
            texInfoNew.sizeX = bink->Width;
            texInfoNew.sizeY = bink->Height;
            texInfoNew.numMipMap = 1;
            ((zCTexConGeneric*)texConv)->ConvertTextureFormat(texInfoNew);
            */

            zCTextureExchange::CopyContents(texConv, tex);

            texConv->Unlock();
            delete texConv;

            if (g_bink && g_bink->FrameNum > g_bink->Frames)
                BinkGoto(g_bink, 1, 0);
        }

        BinkClose(g_bink);
    }

    int __fastcall CGameManager_PlayVideo(CGameManager* _this, void* vtable, zSTRING videoFile, int param)
    {
        g_PauseVideo = true;
        int result = Ivk_CGameManager_PlayVideo(_this, videoFile, param);
        g_PauseVideo = false;
        return result;
    }

    void __fastcall zCMusicSys_DirectMusic_PlayThemeByScript(zCMusicSys_DirectMusic* _this, void* vtable, const zSTRING& id, const int manipulate, zBOOL* done)
    {
        if (g_bink && g_bink->NumTracks > 0 && id == zSTRING("SYS_Menu"))
            return;
        Ivk_zCMusicSys_DirectMusic_PlayThemeByScript(_this, id, manipulate, done);
    }
}