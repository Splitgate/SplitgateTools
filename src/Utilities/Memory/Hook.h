#pragma once
#include "Globals.h"
#include <detours.h>

static void* GetMemberFuncPtr(auto Func) { return reinterpret_cast<void*&>(Func); }

template<typename T>
static void Detour(T& Target, void* Hook)
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());
	DetourAttach(&(void*&)Target, Hook);
	DetourTransactionCommit();
}
template<typename T>
static void Detour(T&& Target, void* Hook)
{
	Detour(Target, Hook);
}

#define MEMBER_FUNC(a) GetMemberFuncPtr(&a)

#define HOOK(Func) Detour(::Func, MEMBER_FUNC(Func))

#define UHOOK(Name) StaticClass()->CreateDefaultObject()->FindFunction(#Name)->Func = MEMBER_FUNC(exec##Name)
#define UHOOK_CUSTOM(Name, OverrideClass, OverrideName) OverrideClass::StaticClass()->CreateDefaultObject()->FindFunction(#Name)->Func = MEMBER_FUNC(exec##OverrideName)

#define VHOOK(Func, Offset) ::Func = Memory::DetourVFT(StaticClass()->CreateDefaultObject(), Offset, MEMBER_FUNC(Func))
#define VHOOK_CUSTOM(Func, Offset, OverrideClass) ::Func = Memory::DetourVFT(CDO<OverrideClass>(), Offset, MEMBER_FUNC(Func))

#define UNOVERRIDE(Old, New, Offset) Memory::DetourVFT(CDO<Old>(), Offset, Memory::GetVirtualFunction(CDO<New>(), Offset))

static int Return0() { return 0; }
static int Return1() { return 1; }
static int Return2() { return 2; }
