#include "UGameEngine.h"
#include "Games/BaseGame.h"

#include "Core/Misc/FApp.h"

static void (*Init)(UGameEngine*, IEngineLoop*);
static void (*Start)(UGameEngine*);

void UGameEngine::Init_PreEngine()
{
	::Init = Memory::FindPattern("E8 ?? ?? ?? ?? 48 8B CF E8 ?? ?? ?? ?? 33 ?? 48 8B C8 4C 8B").FuncStart();
	LOG_ADDRESS(::Init, "UGameEngine::Init");
	HOOK(Init);

	::Start = Memory::FindStringRef(L"Starting Game.").FuncStart();
	LOG_ADDRESS(::Start, "UGameEngine::Start");
	HOOK(Start);

	Memory::Address GetMaxTickRatePtr = Memory::FindStringRef(L"GETMAXTICKRATE").ScanVirtualCall().Add(2);
	if (GetMaxTickRatePtr)
	{
		VTableOffsets::GetMaxTickRate = GetMaxTickRatePtr.Deref<int>();
		LOG_OFFSET(VTableOffsets::GetMaxTickRate, "UGameEngine::GetMaxTickRate");
	}
	else
	{
		UE_LOG(LogInit, Warning, "Failed to find {}", "UGameEngine::GetMaxTickRate");
	}
}

void UGameEngine::Init(IEngineLoop* InEngineLoop)
{
	::Init(this, InEngineLoop);

	Game->Engine = this;
	Game->Init_PostEngine();

	// no point hooking SetupInitialLocalPlayer for this since it gets called around this time anyway
	UGameViewportClient* Viewport = GameViewport();
	if (Viewport)
		Viewport->ViewportConsole() = NewObject<UConsole>(Viewport, ConsoleClass());

//	UE_LOG(LogInit, Display, "Game Engine Initialized.");
//	UE_LOG(LogInit, Display, "Engine Version: {}", FApp::GetBuildVersion());
}

void UGameEngine::Start()
{
	::Start(this);

	Game->bAllowUIOpen = true;
	Game->ConstructTitleBar();
}
