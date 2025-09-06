#include "UKismetSystemLibrary.h"
#include "Engine/Core/UObject/UClass.h"
#include <Globals.h>

#include "Engine/UWorld.h"

void UKismetSystemLibrary::GetPrimaryAssetIdList(FPrimaryAssetType PrimaryAssetType, TArray<FPrimaryAssetId>& OutPrimaryAssetIdList)
{
	struct {
		FPrimaryAssetType       PrimaryAssetType;
		TArray<FPrimaryAssetId> OutPrimaryAssetIdList;
	} p;
	p.PrimaryAssetType = PrimaryAssetType;

	UFunction* Func = StaticClass()->FindFunction(L"GetPrimaryAssetIdList");
	DefaultObject()->ProcessEvent(Func, &p);

	OutPrimaryAssetIdList = p.OutPrimaryAssetIdList;
}

UObject* UKismetSystemLibrary::GetObjectFromPrimaryAssetId(FPrimaryAssetId PrimaryAssetId)
{
	struct {
		FPrimaryAssetId PrimaryAssetId;
		UObject* ReturnValue;
	} p;
	p.PrimaryAssetId = PrimaryAssetId;

	UFunction* Func = StaticClass()->FindFunction(L"GetObjectFromPrimaryAssetId");
	DefaultObject()->ProcessEvent(Func, &p);

	return p.ReturnValue;
}
