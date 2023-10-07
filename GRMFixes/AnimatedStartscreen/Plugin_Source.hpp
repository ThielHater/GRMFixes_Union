// Source file
// Multi-platform file can not have `#pragma once`!!!
// For compile plugin in Multiplatform mode - select 'Debug' or 'Release' configuration.

#include "Plugin_Header.h"
#include "bink.h"

namespace NAMESPACE
{
    #define zRES_CACHE_LOCKED 2

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

        zSTRING rootDir = zoptions->GetDirString(DIR_ROOT);
        zSTRING videoDir = zoptions->GetDirString(DIR_VIDEO);
        zSTRING videoFile = zSTRING("STARTSCREEN.BIK");

        BinkSetSoundSystem(BinkOpenWaveOut, 0);

        BINK* bink = BinkOpen(rootDir + videoDir + videoFile, 0);
        if (!bink)
            return;

        if (bink->NumTracks > 0)
        {
            BinkSetSoundOnOff(bink, 1);
            BinkSetVolume(bink, 0, 65535);
        }

        zCTexture* tex = zrenderer->CreateTexture();
        gameMan->initScreen->InsertBack(tex);

        while (!gameMan->IsGameRunning() && !gameMan->exitGame)
        {
            // TODO: How to stop music earlier?
            if (bink->NumTracks > 0)
                zmusic->Stop();

            BinkDoFrame(bink);
            BinkNextFrame(bink);

            // wait until it is time to show the frame
            while (true)
            {
                if (!BinkWait(bink))
                    break;
            }

            zCTextureConvert* texConv = zrenderer->CreateTextureConvert();

            zCTextureInfo texInfo;
            // Gothic and Bink have to use the same format!
            // As RGB textures are not supported in Gothic, this may be the best option.
            texInfo.texFormat = zRND_TEX_FORMAT_RGB_565;
            texInfo.sizeX = bink->Width;
            texInfo.sizeY = bink->Height;
            texInfo.numMipMap = 1;
            texConv->SetTextureInfo(texInfo);

            texConv->Lock(zRES_CACHE_LOCKED);

            BYTE* texBuffer;
            int pitch; // width * bytes per pixel
            texConv->GetTextureBuffer(0, (void*&)texBuffer, pitch);

            BinkCopyToBuffer(bink, texBuffer, pitch, bink->Height, 0, 0, BINKCOPYALL | BINKSURFACE565);

            zCTextureExchange::CopyContents(texConv, tex);

            texConv->Unlock();
            delete texConv;

            if (bink && bink->FrameNum > bink->Frames)
                BinkGoto(bink, 1, 0);
        }

        BinkClose(bink);
    }
}