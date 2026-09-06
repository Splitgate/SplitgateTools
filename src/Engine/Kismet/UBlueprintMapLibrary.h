#pragma once
#include "UObject/UObject.h"

class UBlueprintMapLibrary : public UObject
{
public:
	GENERATED_BODY(UBlueprintMapLibrary, UObject, "BlueprintMapLibrary", "/Script/Engine")

	static void GenericMap_Add(const void* TargetMap, const void* MapProperty, const void* KeyPtr, const void* ValuePtr);
	static bool GenericMap_Remove(const void* TargetMap, const void* MapProperty, const void* KeyPtr);
};
