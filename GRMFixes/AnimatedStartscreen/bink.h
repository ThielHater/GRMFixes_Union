// minified bink.h from Bink SDK 1.5.21

typedef struct BINKRECT {
	int Left, Top, Width, Height;
} BINKRECT;

#define BINKMAXDIRTYRECTS 8

typedef struct BINK {
	unsigned int Width; // Width (1 based, 640 for example)
	unsigned int Height; // Height (1 based, 480 for example)
	unsigned int Frames; // Number of frames (1 based, 100 = 100 frames)
	unsigned int FrameNum; // Frame to *be* displayed (1 based)
	unsigned int LastFrameNum; // Last frame decompressed or skipped (1 based)

	unsigned int FrameRate; // Frame Rate Numerator
	unsigned int FrameRateDiv; // Frame Rate Divisor (frame rate=numerator/divisor)

	unsigned int ReadError; // Non-zero if a read error has ocurred
	unsigned int OpenFlags; // flags used on open
	unsigned int BinkType; // Bink flags

	unsigned int Size; // size of file
	unsigned int FrameSize; // The current frame's size in bytes
	unsigned int SndSize; // The current frame sound tracks' size in bytes

	BINKRECT FrameRects[BINKMAXDIRTYRECTS]; // Dirty rects from BinkGetRects
	int NumRects;

	unsigned int PlaneNum; // which set of planes is current
	void* YPlane[2]; // pointer to the uncompressed Y (Cr and Cr follow)
	void* APlane[2]; // decompressed alpha plane (if present)
	unsigned int YWidth; // widths and heights of the video planes
	unsigned int YHeight;
	unsigned int UVWidth;
	unsigned int UVHeight;

	void* MaskPlane; // pointer to the mask plane (Ywidth/16*Yheight/16)
	unsigned int MaskPitch; // Mask Pitch
	unsigned int MaskLength; // total length of the mask plane

	unsigned int LargestFrameSize; // Largest frame size
	unsigned int InternalFrames; // how many frames were potentially compressed

	int NumTracks; // how many tracks

	unsigned int Highest1SecRate; // Highest 1 sec data rate
	unsigned int Highest1SecFrame; // Highest 1 sec data rate starting frame

	int Paused; // is the bink movie paused?

	unsigned int BackgroundThread; // handle to background thread
} BINK;

#define BINKCOPYALL 0x80000000L // copy all pixels (not just changed)

#define BINKSURFACE8P    0
#define BINKSURFACE24    1
#define BINKSURFACE24R   2
#define BINKSURFACE32    3
#define BINKSURFACE32R   4
#define BINKSURFACE32A   5
#define BINKSURFACE32RA  6
#define BINKSURFACE4444  7
#define BINKSURFACE5551  8
#define BINKSURFACE555   9
#define BINKSURFACE565  10
#define BINKSURFACE655  11
#define BINKSURFACE664  12
#define BINKSURFACEYUY2 13
#define BINKSURFACEUYVY 14
#define BINKSURFACEYV12 15
#define BINKSURFACEMASK 15

typedef BINK* (WINAPI* PFN_BinkOpen)(const char* filePath, unsigned int flags);
typedef void (WINAPI* PFN_BinkClose)(void* bink);
typedef void (WINAPI* PFN_BinkGoto)(void* bink, unsigned int frame, int flags);
typedef unsigned int (WINAPI* PFN_BinkDoFrame)(void* bink);
typedef void (WINAPI* PFN_BinkNextFrame)(void* bink);
typedef int (WINAPI* PFN_BinkWait)(void* bink);
typedef int (WINAPI* PFN_BinkCopyToBuffer)(void* bink, void* dest, int destPitch, unsigned int destHeight, unsigned int destX, unsigned int destY, unsigned int flags);
typedef void* (WINAPI* PFN_BinkOpenWaveOut)(unsigned int flags);
typedef int (WINAPI* PFN_BinkSetSoundSystem)(void* open, HANDLE param);
typedef unsigned int (WINAPI* PFN_BinkSetSoundOnOff)(void* bink, int onOff);
typedef void (WINAPI* PFN_BinkSetVolume)(void* bink, unsigned int trackId, int volume);
typedef void (WINAPI* PFN_BinkPause)(void* bink, int pause);
