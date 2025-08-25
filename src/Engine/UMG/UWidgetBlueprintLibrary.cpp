#include "UWidgetBlueprintLibrary.h"
#include "Engine/Core/UObject/UClass.h"

void UWidgetBlueprintLibrary::SetInputMode_UIOnly(APlayerController* Target, UObject* InWidgetToFocus, bool bLockMouseToViewport)
{
	struct {
		APlayerController* Target;
		UObject* InWidgetToFocus;
		bool bLockMouseToViewport;
	} p = {
		Target, InWidgetToFocus, bLockMouseToViewport
	};

	UFunction* Func = StaticClass()->FindFunction(L"SetInputMode_UIOnly");
	DefaultObject()->ProcessEvent(Func, &p);
}

void UWidgetBlueprintLibrary::SetInputMode_GameAndUI(APlayerController* Target, UObject* InWidgetToFocus, bool bLockMouseToViewport, bool bHideCursorDuringCapture)
{
	struct {
		APlayerController* Target;
		UObject* InWidgetToFocus;
		bool bLockMouseToViewport;
		bool bHideCursorDuringCapture;
	} p = {
		Target, InWidgetToFocus, bLockMouseToViewport, bHideCursorDuringCapture
	};

	UFunction* Func = StaticClass()->FindFunction(L"SetInputMode_GameAndUI");
	DefaultObject()->ProcessEvent(Func, &p);
}
