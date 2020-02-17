#include "UnionAfx.h"

// multi-platform source inclusion.
// Add your SOURCE files (.h, .hpp or .inl) here or create same interface file.
#ifdef __G1
#define NAMESPACE Gothic_I_Classic
#define CurrentEngine Engine_G1
#include "Plugin_Source.hpp"
#endif

#ifdef __G1A
#define NAMESPACE Gothic_I_Addon
#define CurrentEngine Engine_G1A
#include "Plugin_Source.hpp"
#endif

#ifdef __G2
#define NAMESPACE Gothic_II_Classic
#define CurrentEngine Engine_G2
#include "Plugin_Source.hpp"
#endif

#ifdef __G2A
#define NAMESPACE Gothic_II_Addon
#define CurrentEngine Engine_G2A
#include "Plugin_Source.hpp"
#endif