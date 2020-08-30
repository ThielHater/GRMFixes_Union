#include "UnionAfx.h"
#include "Interface_HeaderInclusion.h"


void Game_Entry() {
}

void Game_Init() {
}

void Game_Exit() {
}

void Game_Loop() {
    NAMESPACE::AnalyzeHelper();
}

void Game_SaveBegin() {
}

void Game_SaveEnd() {
}

void LoadBegin() {
}

void LoadEnd() {
}

void Game_LoadBegin_NewGame() {
  LoadBegin();
}

void Game_LoadEnd_NewGame() {
  LoadEnd();
}

void Game_LoadBegin_SaveGame() {
  LoadBegin();
}

void Game_LoadEnd_SaveGame() {
  LoadEnd();
}

void Game_LoadBegin_ChangeLevel() {
  LoadBegin();
}

void Game_LoadEnd_ChangeLevel() {
  LoadEnd();
}

void Game_LoadBegin_Trigger() {
}

void Game_LoadEnd_Trigger() {
}

void Game_Pause() {
}

void Game_Unpause() {
}

void Game_DefineExternals() {
}



// please disable unused functions.
// write 'false' in 'Enabled' macro.
#define AppDefault True
CApplication* lpApplication = CApplication::CreateRefApplication (
  Enabled( AppDefault ) Game_Entry,
  Enabled( AppDefault ) Game_Init,
  Enabled( AppDefault ) Game_Exit,
  Enabled( AppDefault ) Game_Loop,
  Enabled( AppDefault ) Game_SaveBegin,
  Enabled( AppDefault ) Game_SaveEnd,
  Enabled( AppDefault ) Game_LoadBegin_NewGame,
  Enabled( AppDefault ) Game_LoadEnd_NewGame,
  Enabled( AppDefault ) Game_LoadBegin_SaveGame,
  Enabled( AppDefault ) Game_LoadEnd_SaveGame,
  Enabled( AppDefault ) Game_LoadBegin_ChangeLevel,
  Enabled( AppDefault ) Game_LoadEnd_ChangeLevel,
  Enabled( AppDefault ) Game_LoadBegin_Trigger,
  Enabled( AppDefault ) Game_LoadEnd_Trigger,
  Enabled( AppDefault ) Game_Pause,
  Enabled( AppDefault ) Game_Unpause,
  Enabled( AppDefault ) Game_DefineExternals
  );