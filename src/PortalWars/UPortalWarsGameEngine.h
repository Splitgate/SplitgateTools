#pragma once
#include "Engine/UGameEngine.h"

#undef GEngine
#define GEngine reinterpret_cast<UPortalWarsGameEngine*>(Game->Engine)
class UPortalWarsGameEngine : public UGameEngine
{
public:
	GENERATED_BODY(UPortalWarsGameEngine, UGameEngine, "PortalWarsGameEngine", "/Script/PortalWars")
	
	static void Init_PostEngine();

	static void ApplyLobbyFPSPatch();
	static void RevertLobbyFPSPatch();
};
