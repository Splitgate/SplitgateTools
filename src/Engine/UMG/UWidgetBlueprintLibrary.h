#pragma once
#include "UObject/UObject.h"

class UWidgetBlueprintLibrary : public UObject
{
public:
	GENERATED_BODY(UWidgetBlueprintLibrary, UObject, "WidgetBlueprintLibrary", "/Script/UMG")

	static void SetInputMode_UIOnly(class APlayerController* Target, UObject* InWidgetToFocus = nullptr, bool bLockMouseToViewport = false);

	static void SetInputMode_GameAndUI(class APlayerController* Target, UObject* InWidgetToFocus = nullptr, bool bLockMouseToViewport = false, bool bHideCursorDuringCapture = true);
};
