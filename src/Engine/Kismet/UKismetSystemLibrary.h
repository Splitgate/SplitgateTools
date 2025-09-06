#pragma once
#include "UObject/UObject.h"
#include "UObject/FPrimaryAssetId.h"

class UKismetSystemLibrary : public UObject
{
public:
	GENERATED_BODY(UKismetSystemLibrary, UObject, "KismetSystemLibrary", "/Script/Engine")

	static void GetPrimaryAssetIdList(FPrimaryAssetType PrimaryAssetType, TArray<FPrimaryAssetId>& OutPrimaryAssetIdList);
	static UObject* GetObjectFromPrimaryAssetId(FPrimaryAssetId PrimaryAssetId);

	static void ExecuteConsoleCommand(const FString& Command, class APlayerController* SpecificPlayer = nullptr);
};
