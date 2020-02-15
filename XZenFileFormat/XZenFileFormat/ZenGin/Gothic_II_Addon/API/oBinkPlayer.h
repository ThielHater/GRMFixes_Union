// Supported with union (c) 2018 Union team

#ifndef __OBINK_PLAYER_H__VER3__
#define __OBINK_PLAYER_H__VER3__

#include "zBinkPlayer.h"
#include "zRenderer.h"

namespace Gothic_II_Addon {

  class oCBinkPlayer : public zCBinkPlayer {
  public:
    int extendedKeys;
    int disallowInputHandling;

    void oCBinkPlayer_OnInit()       zCall( 0x00422BD0 );
    oCBinkPlayer()                   zInit( oCBinkPlayer_OnInit() );
    virtual ~oCBinkPlayer()          zCall( 0x00422C10 );
    virtual int OpenVideo( zSTRING ) zCall( 0x00423780 );
    virtual int PlayInit( int )      zCall( 0x00422C20 );
    virtual int PlayFrame()          zCall( 0x00422C90 );
    virtual int PlayDeinit()         zCall( 0x00422C80 );
    virtual int PlayHandleEvents()   zCall( 0x00422CA0 );
  };

} // namespace Gothic_II_Addon

#endif // __OBINK_PLAYER_H__VER3__