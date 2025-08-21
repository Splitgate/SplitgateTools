#include "UObject.h"

#include "UClass.h"
#include "Engine/UGameEngine.h"

static UObject* (*StaticLoadObjectInternal)(UClass*, UObject*, const wchar_t*, const wchar_t*, unsigned int, void*, bool, void*);
static void (*ProcessEvent)(UObject*, UFunction*, void*);

void UObject::Init_PreEngine()
{
	StaticLoadObjectInternal = Memory::FindStringRef(L"Return an object still needing load from StaticLoadObjectInternal %s").FuncStart();
	LOG_ADDRESS(StaticLoadObjectInternal, "StaticLoadObjectInternal");

	::ProcessEvent = Memory::FindStringRef(L"Accessed None").Scan("40 55");
	LOG_ADDRESS(::ProcessEvent, "UObject::ProcessEvent");
}

UObject* StaticLoadObject(UClass* InClass, const wchar_t* InName)
{
	return StaticLoadObjectInternal(InClass, 0, InName, 0, 0, 0, 0, 0);
}

UClass* UObject::StaticClass()
{
	static UClass* OutClass = reinterpret_cast<UClass*>(GEngine->Class->GetSuperMost());
	return OutClass;
}

void UObject::ProcessEvent(UFunction* Func, void* Params)
{
	::ProcessEvent(this, Func, Params);
}
