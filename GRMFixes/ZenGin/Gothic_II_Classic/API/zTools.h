// Supported with union (c) 2018 Union team

#ifndef __ZTOOLS_H__VER2__
#define __ZTOOLS_H__VER2__

namespace Gothic_II_Classic {
  const int zRAND_MAX = RAND_MAX;
  const float _PI     = 3.14159265358979323846f;

  class zCTimer {
  public:
    float factorMotion;
    float frameTimeFloat;
    float totalTimeFloat;
    float frameTimeFloatSecs;
    float totalTimeFloatSecs;
    unsigned long lastTimer;
    unsigned long frameTime;
    unsigned long totalTime;
    unsigned long minFrameTime;
    unsigned long forcedMaxFrameTime;

    void zCTimer_OnInit()                    zCall( 0x005F2890 );
    zCTimer()                                zInit( zCTimer_OnInit() );
    void ResetTimer()                        zCall( 0x005F28C0 );
    void SetFrameTime( float )               zCall( 0x005F2A10 );
    void SetMaxFPS( unsigned long )          zCall( 0x005F2AA0 );
    void LimitFPS( int )                     zCall( 0x005F2B30 );
    void SetMotionFactor( float )            zCall( 0x0062F8E0 );
    static void S_SetMaxFPS( unsigned long ) zCall( 0x005F2A80 );
    static unsigned long S_GetMaxFPS()       zCall( 0x005F2A90 );
    static void FrameUpdate()                zCall( 0x005F2AB0 );

    // static properties
    static unsigned long& s_forcedMaxFrameTime;
  };

  class zCChecksum {
  public:
    unsigned long checksum;

    zCChecksum() {}
    void BeginChecksum()                                                                zCall( 0x005F2D60 );
    unsigned long EndChecksum()                                                         zCall( 0x005F2D70 );
    void CalcBufferChecksum( unsigned char*, unsigned long )                            zCall( 0x005F2D80 );
    void CalcFileChecksum( zFILE* )                                                     zCall( 0x005F2DA0 );
    void CalcFileChecksum( zSTRING const& )                                             zCall( 0x005F2E00 );
    static unsigned long GetBufferCRC32( unsigned char*, unsigned long, unsigned long ) zCall( 0x005F2C20 );
  };

  class zCChecksum16 {
  public:
    unsigned short checksum;

    zCChecksum16() {}
    void BeginChecksum()                                                                  zCall( 0x005F2FE0 );
    unsigned short EndChecksum()                                                          zCall( 0x005F2FF0 );
    void CalcBufferChecksum( unsigned char*, unsigned long )                              zCall( 0x005F3000 );
    static unsigned short GetBufferCRC16( unsigned char*, unsigned long, unsigned short ) zCall( 0x005F2E90 );
  };

  class zCFPUControler {
  public:
    unsigned long savedControlWord;
    unsigned long defaultControlWord;

    void zCFPUControler_OnInit()          zCall( 0x005F3080 );
    zCFPUControler()                      zInit( zCFPUControler_OnInit() );
    void SaveCurrentControlWord()         zCall( 0x005F30C0 );
    void RestoreSavedControlWord()        zCall( 0x005F30E0 );
    unsigned long GetCurrentControlWord() zCall( 0x005F30F0 );
    void RestoreDefaultControlWord()      zCall( 0x005F3100 );
    void ComputeDefaultControlWord()      zCall( 0x005F3110 );
    void SetControlWord( unsigned long )  zCall( 0x005F3150 );
    void SetPrecision_24()                zCall( 0x005F3170 );
    void SetPrecision_53()                zCall( 0x005F3190 );
    void SetPrecision_64()                zCall( 0x005F31B0 );
    void PrintStatus()                    zCall( 0x005F31D0 );
  };

} // namespace Gothic_II_Classic

#endif // __ZTOOLS_H__VER2__