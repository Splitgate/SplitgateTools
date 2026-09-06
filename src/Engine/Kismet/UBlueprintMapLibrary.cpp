#include "UBlueprintMapLibrary.h"
#include "Engine/Core/UObject/UClass.h"
#include <Memory/Memory.h>

static void (*GenericMap_Add)(const void*, const void*, const void*, const void*);
void UBlueprintMapLibrary::GenericMap_Add(const void* TargetMap, const void* MapProperty, const void* KeyPtr, const void* ValuePtr)
{
	::GenericMap_Add = Memory::Base().Add(0x2E22F30);

	::GenericMap_Add(TargetMap, MapProperty, KeyPtr, ValuePtr);
}

static bool (*GenericMap_Remove)(const void*, const void*, const void*);
bool UBlueprintMapLibrary::GenericMap_Remove(const void* TargetMap, const void* MapProperty, const void* KeyPtr)
{
	::GenericMap_Remove = Memory::Base().Add(0x2E236A0);

	return ::GenericMap_Remove(TargetMap, MapProperty, KeyPtr);
}
