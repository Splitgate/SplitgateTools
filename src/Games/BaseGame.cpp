#include "BaseGame.h"

#include "Splitgate/Splitgate.h"

#include "FMemory.h"
#include "Parse/FParse.h"
#include "Misc/FApp.h"
#include "Engine/UGameEngine.h"
#include "Engine/UWorld.h"
#include "Engine/RHI/FWindowsDynamicRHI.h"
#include "Engine/ApplicationCore/FWindowsWindow.h"
#include "Engine/Core/Misc/FNetworkVersion.h"
#include "Engine/Core/Misc/FEngineVersion.h"
#include "UObject/FWeakObjectPtr.h"
#include "UObject/UClass.h"
#include "UObject/UFunction.h"
#include "UObject/UStruct.h"
#include "Engine/IPlatformFilePak.h"

#include "Engine/GameFramework/APlayerController.h"
#include "Engine/UMG/UWidgetBlueprintLibrary.h"

std::unique_ptr<BaseGame> BaseGame::Create()
{
	Init_PreGame();

	//auto KeyPatchJmp = Memory::FindPattern("48 8D 0D ?? ?? ?? 00 E9 ?? ?? ?? ?? CC CC CC CC 48");
	//if (MissingSignature)
	//{
	//	MissingSignature.Write("EB 37");
	//	UE_LOG(LogInit, Display, "Patched IO Store Signing checks");
	//}

	Memory::Address SignatureCheck = Memory::FindPattern("0F 8F ?? ?? ?? ?? 45 33 C0 33 C9 41 8D 50 20 E8", 1);
	if (SignatureCheck)
	{
		SignatureCheck.FuncStart().Write(RET);
		UE_LOG(LogInit, Display, "Patched I/O Store signing checks");
	}
	else
	{
		UE_LOG(LogInit, Display, "Failed to patched I/O Store signing checks");
	}

	UE_LOG(LogInit, Display, "Attempting to find GInternalProjectName");
	
	const wchar_t* GInternalProjectName = Memory::FindStringRef(L"'{0}' must exist and contain a DefaultEngine.ini.")
		.ReverseScan("66 44").Add(4).Rel32();
	LOG_ADDRESS(GInternalProjectName, "GInternalProjectName");

	std::wstring_view ProjectName = TEXT("None");
	if (GInternalProjectName != nullptr) 
		ProjectName = GInternalProjectName;
	
	if (ProjectName == L"PortalWars")
	{
#if WITH_SERVER_CODE
		UE_LOG(LogInit, Success, "Starting a Splitgate Dedicated Server"); 
		return std::make_unique<Splitgate>(); // TO:DO actually add this eventually
#else
		UE_LOG(LogInit, Success, "Game Detected: Splitgate");
		return std::make_unique<Splitgate>();
#endif
	}

	// Add handlers for past builds if needed potentially, always keep "Splitgate" as the latest version of the game.

	// Fallback so features may still work on unsupported games
	UE_LOG(LogInit, Warning, "Unsupported game! Expect things to break");
	return std::make_unique<BaseGame>(); 
}

BaseGame::BaseGame()
{
	ConstructTitleBar();
}

void BaseGame::Init_PreGame()
{
	FApp::Init_PreGame();
	FNetworkVersion::Init_PreGame();
	FEngineVersion::Init_PreGame();
}

void BaseGame::Init_PreEngine()
{
	FWindowsDynamicRHI::Init_PreEngine();
	FWindowsWindow::Init_PreEngine();
	FMemory::Init_PreEngine();

	FName::Init_PreEngine();
	FWeakObjectPtr::Init_PreEngine();
	
	FParse::Init_PreEngine();

	UObject::Init_PreEngine();
	IPlatformFilePak::Init_PreEngine();
	UGameEngine::Init_PreEngine();
	UWorld::Init_PreEngine();
}

void BaseGame::Init_PostEngine()
{
	UStruct::Init_PostEngine();
	
	XPropertyClass::Init_PostEngine();
	XProperty::Init_PostEngine();
	
	UClass::Init_PostEngine();
	UFunction::Init_PostEngine();

	APlayerController::Init_PostEngine();
}

void BaseGame::ConstructTitleBar()
{
}

void BaseGame::PopulateSettingsTabs(std::vector<std::unique_ptr<SettingsTab>>& Tabs)
{
}

void BaseGame::OnUIVisibilityChange(bool bVisible)
{
	if (!World || !GWorld) return;

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GWorld, 0);
	if (PlayerController)
	{
		if (bVisible)
			UWidgetBlueprintLibrary::SetInputMode_UIOnly(PlayerController);
		else
			UWidgetBlueprintLibrary::SetInputMode_GameAndUI(PlayerController, nullptr, true, false);
	}
}
