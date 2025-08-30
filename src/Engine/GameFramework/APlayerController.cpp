#include "APlayerController.h"
#include "Memory/Memory.h"
#include "Globals.h"

#include "Engine/Core/UObject/UClass.h"

static void (*SetShowMouseCursor)(APlayerController*, bool);

void APlayerController::Init_PostEngine()
{
	::SetShowMouseCursor = Memory::FindStringRef(L"Player bShowMouseCursor Changed, %s -> %s").FuncStart();
	LOG_ADDRESS(::SetShowMouseCursor, "APlayerController::SetShowMouseCursor");
}

void APlayerController::SetShowMouseCursor(bool bShow)
{
	::SetShowMouseCursor(this, bShow);
}

void APlayerController::LocalTravel(FString& FURL)
{
	UFunction* Func = StaticClass()->FindFunction(L"LocalTravel");
	ProcessEvent(Func, &FURL);
}
