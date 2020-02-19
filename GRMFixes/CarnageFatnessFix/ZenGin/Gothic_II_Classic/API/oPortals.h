// Supported with union (c) 2018 Union team

#ifndef __OPORTALS_H__VER2__
#define __OPORTALS_H__VER2__

namespace Gothic_II_Classic {

  class oCPortalRoom {
  public:
    zSTRING portalName;
    zSTRING ownerNpc;
    int ownerGuild;

    void oCPortalRoom_OnInit()                 zCall( 0x00712970 );
    void oCPortalRoom_OnInit( zSTRING const& ) zCall( 0x00712AF0 );
    oCPortalRoom()                             zInit( oCPortalRoom_OnInit() );
    oCPortalRoom( zSTRING const& a0 )          zInit( oCPortalRoom_OnInit( a0 ));
    ~oCPortalRoom()                            zCall( 0x00712CC0 );
    oCNpc* GetOwnerNpc()                       zCall( 0x00712D40 );
    int GetOwnerGuild()                        zCall( 0x00712D90 );
    zSTRING& GetPortalName()                   zCall( 0x00712DA0 );
    int IsPortalMyRoom( oCNpc* )               zCall( 0x00712DB0 );
    void SavePortalData( zCArchiver& )         zCall( 0x00713010 );
    void LoadPortalData( zCArchiver& )         zCall( 0x00713050 );
  };

  class oCPortalRoomManager {
  public:
    zSTRING* oldPlayerPortal;
    zSTRING* curPlayerPortal;
    oCPortalRoom* oldPlayerRoom;
    oCPortalRoom* curPlayerRoom;
    zCArraySort<oCPortalRoom*> portals;

    void oCPortalRoomManager_OnInit()                        zCall( 0x00713100 );
    oCPortalRoomManager()                                    zInit( oCPortalRoomManager_OnInit() );
    void CleanUp()                                           zCall( 0x00713130 );
    ~oCPortalRoomManager()                                   zCall( 0x00713180 );
    void AssignPortalToNpc( zSTRING const&, zSTRING const& ) zCall( 0x00713220 );
    void AssignPortalToGuild( zSTRING const&, int )          zCall( 0x00713450 );
    int GetPortalRoomIndex( zSTRING const& )                 zCall( 0x00713650 );
    int IsPlayerInMyRoom( oCNpc* )                           zCall( 0x007137F0 );
    int IsNSCInPlayerRoom( oCNpc* )                          zCall( 0x00713830 );
    int WasPlayerInMyRoom( oCNpc* )                          zCall( 0x00713870 );
    int IsPortalMyRoom( zSTRING const&, oCNpc* )             zCall( 0x007138A0 );
    oCNpc* GetCurPlayerPortalRoomOwner()                     zCall( 0x00713A00 );
    int GetCurPlayerPortalRoomGuild()                        zCall( 0x00713A60 );
    oCNpc* GetFormerPlayerPortalRoomOwner()                  zCall( 0x00713A70 );
    int GetFormerPlayerPortalRoomGuild()                     zCall( 0x00713AD0 );
    int HasPlayerChangedPortalRoom()                         zCall( 0x00713AE0 );
    oCNpc* GetCurNpcPortalRoomOwner( oCNpc* )                zCall( 0x00713B30 );
    int GetCurNpcPortalRoomGuild( oCNpc* )                   zCall( 0x00713BD0 );
    void ShowDebugInfo()                                     zCall( 0x00713C10 );
    void ShowPortalInfo()                                    zCall( 0x00714250 );
    void SavePortalData( zCArchiver& )                       zCall( 0x007145E0 );
    void LoadPortalData( zCArchiver& )                       zCall( 0x00714650 );
    static int SortPortals( void const*, void const* )       zCall( 0x00713090 );
  };

} // namespace Gothic_II_Classic

#endif // __OPORTALS_H__VER2__