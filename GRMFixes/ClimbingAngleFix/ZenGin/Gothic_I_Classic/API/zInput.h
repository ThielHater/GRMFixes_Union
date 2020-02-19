// Supported with union (c) 2018 Union team

#ifndef __ZINPUT_H__VER0__
#define __ZINPUT_H__VER0__

#include "zInput_Const.h"

namespace Gothic_I_Classic {

  enum zTInputDevice {
    zINPUT_UNDEF,
    zINPUT_KEYBOARD,
    zINPUT_MOUSE,
    zINPUT_JOYSTICK1,
    zINPUT_JOYSTICK2,
    zINPUT_JOYSTICK3,
    zINPUT_JOYSTICK4
  };

  struct zSKeyMapping {
    unsigned short logicalID;
    zCArray<unsigned short> controlValues;
    int set;

    zSKeyMapping() {}
    ~zSKeyMapping() zCall( 0x004C2F20 );
  };

  struct zSCtrlValue {
    zSTRING name;
    unsigned short value;

    void zSCtrlValue_OnInit()                     zCall( 0x004C7500 );
    ~zSCtrlValue()                                zCall( 0x004C58E0 );
    zSCtrlValue& operator =( zSCtrlValue const& ) zCall( 0x004C71A0 );
    zSCtrlValue()                                 zInit( zSCtrlValue_OnInit() );
  };

  class zCInput {
  public:
    zCArraySort<zSKeyMapping*> mapList;

    void zCInput_OnInit()                                               zCall( 0x004C2820 );
    zCInput()                                                           zInit( zCInput_OnInit() );
    void Bind( unsigned short, zCArray<unsigned short> const& )         zCall( 0x004C2B10 );
    void BindOption( zSTRING, unsigned short, zCArray<unsigned short> ) zCall( 0x004C2D00 );
    void Unbind( unsigned short )                                       zCall( 0x004C2F40 );
    int IsBinded( unsigned short, unsigned short )                      zCall( 0x004C3050 );
    int GetBinding( unsigned short, zCArray<unsigned short>& )          zCall( 0x004C3150 );
    unsigned short GetFirstBindedLogicalKey( unsigned short )           zCall( 0x004C3270 );
    void InitializeControlValues()                                      zCall( 0x004C32E0 );
    void BindKeys( int )                                                zCall( 0x004C5930 );
    static int CompareKeyMaps( void const*, void const* )               zCall( 0x004C2AF0 );
    static unsigned short GetControlValueByName( zSTRING const& )       zCall( 0x004C68D0 );
    static zSTRING GetNameByControlValue( unsigned short )              zCall( 0x004C6980 );
    virtual ~zCInput()                                                  zCall( 0x004C2A50 );
    virtual float GetState( unsigned short )                            zCall( 0x004C2880 );
    virtual int SetState( unsigned short, int )                         zCall( 0x004C2890 );
    virtual int GetToggled( unsigned short )                            zCall( 0x004C28A0 );
    virtual void SetDeviceEnabled( zTInputDevice, int )                 zCall( 0x004C28B0 );
    virtual int GetDeviceEnabled( zTInputDevice )                       zCall( 0x004C28C0 );
    virtual int GetDeviceConnected( zTInputDevice )                     zCall( 0x004C28D0 );
    virtual int KeyPressed( int )                                       zCall( 0x004C28E0 );
    virtual int KeyToggled( int )                                       zCall( 0x004C28F0 );
    virtual int AnyKeyPressed()                                         zCall( 0x004C2900 );
    virtual void ResetRepeatKey( int )                                  zCall( 0x004C2910 );
    virtual unsigned short GetKey( int, int )                           zCall( 0x004C2920 );
    virtual void SetKey( int, int )                                     zCall( 0x004C2930 );
    virtual char GetChar()                                              zCall( 0x004C2940 );
    virtual void ClearKeyBuffer()                                       zCall( 0x004C2950 );
    virtual int GetNumJoysConnected()                                   zCall( 0x004C2960 );
    virtual void SetJoyDigitalEmu( int )                                zCall( 0x004C2970 );
    virtual void SetJoyEnabled( int )                                   zCall( 0x004C2980 );
    virtual float JoyState( int )                                       zCall( 0x004C2990 );
    virtual float JoyState( int, int )                                  zCall( 0x004C29A0 );
    virtual void GetMousePos( float&, float&, float& )                  zCall( 0x004C29B0 );
    virtual int GetMouseButtonPressedLeft()                             zCall( 0x004C29C0 );
    virtual int GetMouseButtonPressedMid()                              zCall( 0x004C29D0 );
    virtual int GetMouseButtonPressedRight()                            zCall( 0x004C29E0 );
    virtual void SetMouseSensitivity( float, float )                    zCall( 0x004C29F0 );
    virtual void GetMouseSensitivity( float&, float& )                  zCall( 0x004C2A00 );
    virtual void SetMouseFlipXY( int, int )                             zCall( 0x004C2A10 );
    virtual void GetMouseFlipXY( int&, int& )                           zCall( 0x004C2A20 );
    virtual void ProcessInputEvents()                                   zPureCall;

    // static properties
    static zCArray<zSCtrlValue>& values;
  };

} // namespace Gothic_I_Classic

#endif // __ZINPUT_H__VER0__