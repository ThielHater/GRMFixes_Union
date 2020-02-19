// Supported with union (c) 2018 Union team

#ifndef __OPLAYER_INFO_H__VER3__
#define __OPLAYER_INFO_H__VER3__

#include "zBuffer.h"
#include "zPlayerInfo.h"

namespace Gothic_II_Addon {
  const int MAX_PLAYER_INSTANCES = 5;

  enum TPlayerInstance {
    PC_UNKNOWN,
    PC_HERO,
    PC_FIGHTER,
    PC_THIEF,
    PC_MAGE,
    PC_PSIONIC
  };

  class oCPlayerInfo : public zCPlayerInfo {
  public:
    TPlayerInstance instance;

    void oCPlayerInfo_OnInit()                          zCall( 0x004363E0 );
    oCPlayerInfo()                                      zInit( oCPlayerInfo_OnInit() );
    zSTRING GetInstanceName() const                     zCall( 0x004366D0 );
    TPlayerInstance GetInstance() const                 zCall( 0x00436720 );
    virtual ~oCPlayerInfo()                             zCall( 0x00436570 );
    virtual void Init( zSTRING const&, zSTRING const& ) zCall( 0x00436560 );
    virtual void Reset()                                zCall( 0x00436770 );
    virtual void Pack( zCBuffer& )                      zCall( 0x00436730 );
    virtual void Unpack( zCBuffer& )                    zCall( 0x00436750 );
    virtual void SetInstance( TPlayerInstance )         zCall( 0x00436580 );
    virtual void SetInstanceByName( zSTRING const& )    zCall( 0x00436590 );
  };

} // namespace Gothic_II_Addon

#endif // __OPLAYER_INFO_H__VER3__