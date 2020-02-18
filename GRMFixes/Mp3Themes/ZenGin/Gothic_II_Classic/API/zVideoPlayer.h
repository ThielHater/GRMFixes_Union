// Supported with union (c) 2018 Union team

#ifndef __ZVIDEO_PLAYER_H__VER2__
#define __ZVIDEO_PLAYER_H__VER2__

namespace Gothic_II_Classic {

  class zCVideoPlayer {
  public:
    zSTRING mVideoFilename;
    int mLooping;
    int mPaused;
    int mPlaying;
    int mSoundOn;
    float mSoundVolume;
    float mScaleValue;

    void zCVideoPlayer_OnInit()           zCall( 0x0046D850 );
    zSTRING GetVideoFilename()            zCall( 0x0043EA00 );
    zCVideoPlayer()                       zInit( zCVideoPlayer_OnInit() );
    int Play( int )                       zCall( 0x0046DBA0 );
    virtual ~zCVideoPlayer()              zCall( 0x0046D8E0 );
    virtual int OpenVideo( zSTRING )      zCall( 0x0046D930 );
    virtual int CloseVideo()              zCall( 0x0046DAA0 );
    virtual int PlayInit( int )           zCall( 0x0046DCB0 );
    virtual int PlayFrame()               zCall( 0x0046DCF0 );
    virtual int PlayDeinit()              zCall( 0x0046DD00 );
    virtual int Pause()                   zCall( 0x0046DD10 );
    virtual int Unpause()                 zCall( 0x0046DD30 );
    virtual int Stop()                    zCall( 0x0046DD50 );
    virtual int IsPlaying()               zCall( 0x0046D890 );
    virtual int ToggleSound()             zCall( 0x0046D8A0 );
    virtual float SetSoundVolume( float ) zCall( 0x0046D8B0 );
    virtual int PlayGotoNextFrame()       zCall( 0x0046DCD0 );
    virtual int PlayWaitNextFrame()       zCall( 0x0046DCE0 );
  };

} // namespace Gothic_II_Classic

#endif // __ZVIDEO_PLAYER_H__VER2__