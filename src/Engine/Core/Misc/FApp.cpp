#include "FApp.h"
#include "Memory/Hook.h"

#include "Strings/Strings.h"
#include "Renderer.h"

static const wchar_t* (*GetBuildVersion)();
static void (*SetGraphicsRHI)(FString&);

void FApp::Init_PreGame()
{
	auto GetBuildVersionCall = Memory::FindStringRef(L"GameSessionID").Scan("E8", 1).Add(1).Rel32();
	if (GetBuildVersionCall)
	{
		::GetBuildVersion = GetBuildVersionCall.Scan("E9").Add(1).Rel32();
	}
	LOG_ADDRESS(::GetBuildVersion, "FApp::GetBuildVersion");

	::SetGraphicsRHI = Memory::FindStringRef(L"RequiredOpenGL").ReverseScan("E8", 3).Add(1).Rel32();
	LOG_ADDRESS(::SetGraphicsRHI, "FApp::SetGraphicsRHI");
	HOOK(SetGraphicsRHI);
}

void FApp::SetGraphicsRHI(FString& RHIString)
{
	UE_LOG(LogRHI, Display, "Attempting to setup with {}", RHIString.ToString());

	std::wstring_view RHIName = *RHIString;
	
	if (RHIName == L"DirectX 12")
		RHI = EWindowsRHI::D3D12;
	else if (RHIName == L"DirectX 11")
		RHI = EWindowsRHI::D3D11;

	if (RHI == EWindowsRHI::Unsupported)
		UE_LOG(LogRHI, Warning, "Unsupported GraphicsRHI in use: {}", RHIString.ToString());

	::SetGraphicsRHI(RHIString);
}

std::string FApp::GetBuildVersion()
{
	static std::string BuildVer = WideToString(::GetBuildVersion());
	return BuildVer;
}
