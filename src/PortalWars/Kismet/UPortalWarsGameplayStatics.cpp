#include "UPortalWarsGameplayStatics.h"
#include "Engine/Core/UObject/UClass.h"
#include <Globals.h>

#include "Engine/UWorld.h"

UPortalWarsSaveGame* UPortalWarsGameplayStatics::GetSaveGameForLocalPlayer(ULocalPlayer* LocalPlayer)
{
	struct {
		ULocalPlayer* LocalPlayer;
		UPortalWarsSaveGame* ReturnValue;
	} p = {
		LocalPlayer
	};

	UFunction* Func = StaticClass()->FindFunction(L"GetSaveGameForLocalPlayer");
	DefaultObject()->ProcessEvent(Func, &p);

	return p.ReturnValue;
}
