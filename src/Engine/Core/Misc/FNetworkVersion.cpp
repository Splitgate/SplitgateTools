#include "FNetworkVersion.h"
#include "Memory/Hook.h"
#include "Strings/Strings.h"

static FString* (*GetProjectVersion_Internal)();
void FNetworkVersion::Init_PreGame()
{
	GetProjectVersion_Internal = Memory::FindStringRef(L"Set ProjectVersion to %s. Version Checksum will be recalculated on next use.")
		.FuncStart().Scan("E8").Add(1).Rel32();
	LOG_ADDRESS(::GetProjectVersion_Internal, "FNetworkVersion::GetProjectVersion_Internal");
}

std::string FNetworkVersion::GetProjectVersion()
{
	FString* WideBuildVer = ::GetProjectVersion_Internal();

	std::string BuildVer = WideToString(WideBuildVer->Data);
	return BuildVer;
}
