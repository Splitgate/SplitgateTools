#include "FOnlineFactoryEOSPlus.h"

#include "Memory/Hook.h"

static void* (*CreateSubsystem)(FOnlineFactoryEOSPlus*, void**, FName);
void FOnlineFactoryEOSPlus::Init_PreEngine()
{
	::CreateSubsystem = Memory::FindStringRef(L"EOSPlus failed to initialize!").FuncStart();
	LOG_ADDRESS(::CreateSubsystem, "FOnlineFactoryEOSPlus::CreateSubsystem");
	HOOK(CreateSubsystem);
}

static bool bHasEverFailed = false;
void** FOnlineFactoryEOSPlus::CreateSubsystem(void** ReturnValue, FName InstanceName)
{
	// block online subsystem creation attempts
	if (bHasEverFailed)
	{
		ReturnValue[0] = nullptr;
		ReturnValue[1] = nullptr;
	}
	else
	{	
		::CreateSubsystem(this, ReturnValue, InstanceName);
		if (!*ReturnValue)
			bHasEverFailed = true;
	}
	return ReturnValue;
}
