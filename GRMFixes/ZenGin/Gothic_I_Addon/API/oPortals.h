// Supported with union (c) 2018 Union team

#ifndef __OPORTALS_H__VER1__
#define __OPORTALS_H__VER1__

namespace Gothic_I_Addon {

  class oCPortalRoom {
  public:
    zSTRING portalName;
    zSTRING ownerNpc;
    int ownerGuild;

    void oCPortalRoom_OnInit()                 zCall( 0x00700790 );
    void oCPortalRoom_OnInit( zSTRING const& ) zCall( 0x00700910 );
    oCPortalRoom()                             zInit( oCPortalRoom_OnInit() );
    oCPortalRoom( zSTRING const& a0 )          zInit( oCPortalRoom_OnInit( a0 ));
    ~oCPortalRoom()                            zCall( 0x00700AD0 );
    oCNpc* GetOwnerNpc()                       zCall( 0x00700B70 );
    int GetOwnerGuild()                        zCall( 0x00700BD0 );
    zSTRING& GetPortalName()                   zCall( 0x00700BE0 );
    int IsPortalMyRoom( oCNpc* )               zCall( 0x00700BF0 );
    void SavePortalData( zCArchiver& )         zCall( 0x00700DC0 );
    void LoadPortalData( zCArchiver& )         zCall( 0x00700E00 );
  };

  class oCPortalRoomManager {
  public:
    zSTRING* oldPlayerPortal;
    zSTRING* curPlayerPortal;
    oCPortalRoom* oldPlayerRoom;
    oCPortalRoom* curPlayerRoom;
    zCArraySort<oCPortalRoom*> portals;

    void oCPortalRoomManager_OnInit()                        zCall( 0x00700EB0 );
    oCPortalRoomManager()                                    zInit( oCPortalRoomManager_OnInit() );
    void CleanUp()                                           zCall( 0x00700EE0 );
    ~oCPortalRoomManager()                                   zCall( 0x00700F30 );
    void AssignPortalToNpc( zSTRING const&, zSTRING const& ) zCall( 0x00700FF0 );
    void AssignPortalToGuild( zSTRING const&, int )          zCall( 0x00701240 );
    int GetPortalRoomIndex( zSTRING const& )                 zCall( 0x00701480 );
    int IsPlayerInMyRoom( oCNpc* )                           zCall( 0x00701670 );
    int WasPlayerInMyRoom( oCNpc* )                          zCall( 0x007016B0 );
    int IsPortalMyRoom( zSTRING const&, oCNpc* )             zCall( 0x007016E0 );
    oCNpc* GetCurPlayerPortalRoomOwner()                     zCall( 0x00701830 );
    int GetCurPlayerPortalRoomGuild()                        zCall( 0x00701890 );
    oCNpc* GetFormerPlayerPortalRoomOwner()                  zCall( 0x007018A0 );
    int GetFormerPlayerPortalRoomGuild()                     zCall( 0x00701900 );
    int HasPlayerChangedPortalRoom()                         zCall( 0x00701910 );
    void ShowDebugInfo()                                     zCall( 0x00701960 );
    void ShowPortalInfo()                                    zCall( 0x00701FA0 );
    void SavePortalData( zCArchiver& )                       zCall( 0x00702380 );
    void LoadPortalData( zCArchiver& )                       zCall( 0x007023F0 );
    static int SortPortals( void const*, void const* )       zCall( 0x00700E40 );
  };

} // namespace Gothic_I_Addon

#endif // __OPORTALS_H__VER1__