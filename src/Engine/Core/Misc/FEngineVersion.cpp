#include "FEngineVersion.h"

#include "Memory/Memory.h"
#include "Globals.h"

static FEngineVersion* (*Current)();

void FEngineVersion::Init_PreGame()
{
	::Current = Memory::FindStringRef(L"Engine Version: %s").ReverseScan("E8", 1).Add(1).Rel32();
	LOG_ADDRESS(::Current, "FEngineVersion::Current");
}

FEngineVersion* FEngineVersion::Current()
{
	static FEngineVersion* EngineVersion = ::Current();
	return EngineVersion;
}
