#pragma once
#include "UObject/UObject.h"
#include "Engine/Core/UObject/UClass.h"

#include "Containers/FText.h"
#include "Math/FVector.h"
#include "Math/FTransform.h"
#include "Misc/FDateTime.h"

struct FForgeInteractableSaveData
{
	uint16 ID;
	FTransform Transform;
};

struct FForgeMap
{
	FDateTime Timestamp;
	FString SharingCode;
	FString MapName;
	FText DisplayName;
	TArray<uint8> DisplayImagePixels;
	UObject* DisplayImage;
	TArray<FForgeInteractableSaveData> InteractablesSaveData;
};