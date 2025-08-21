#include "UClass.h"
#include "Memory/Hook.h"

static UFunction* (*FindFunctionByName)(UClass*, FName, int);
void UClass::Init_PostEngine()
{
	FindFunctionByName = Memory::FindStringRef(L"Failed to find function %s in %s").FuncStart()
		.Scan({ offsetof(UObject, Class), CALL }).Add(2).Rel32();
	LOG_ADDRESS(FindFunctionByName, "UClass::FindFunctionByName");
}

UFunction* UClass::FindFunction(FName InName)
{
	return ::FindFunctionByName(this, InName, 1); // EIncludeSuperFlag::IncludeSuper
}
