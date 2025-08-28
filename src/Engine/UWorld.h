#pragma once
#include "UObject/UObject.h"
#include "Games/BaseGame.h"

#define GWorld reinterpret_cast<UWorld*>(*Game->World)

// Move to engine types when it exists
enum ENetMode
{
	NM_Standalone,
	NM_DedicatedServer,
	NM_ListenServer,
	NM_Client,
	NM_MAX,
};

class FNetworkNotify {};
class UWorld : public UObject, public FNetworkNotify
{
public:
	static void Init_PreEngine();

	bool ServerTravel(const FString& InURL, bool bAbsolute = false, bool bShouldSkipGameNotify = false);
	ENetMode GetNetMode();
};
