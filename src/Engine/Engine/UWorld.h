#pragma once
#include "UObject/UObject.h"
#include "UObject/UClass.h"

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
	GENERATED_BODY(UWorld, UObject, "World", "/Script/Engine")

	static void Init_PreEngine();

	UPROPERTY(class UGameInstance*, OwningGameInstance);

	bool ServerTravel(const FString& InURL, bool bAbsolute = false, bool bShouldSkipGameNotify = false);
	ENetMode GetNetMode();

	class APlayerController* GetFirstLocalPlayerController();

	class AWorldSettings* GetWorldSettings();
};
