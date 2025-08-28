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

	// logs are disabled so we might as well
	UE_LOG(LogInit, Display, "Game Engine Initialized.");
	UE_LOG(LogInit, Display, "Engine Version: {}", FApp::GetBuildVersion());
}

void UGameEngine::Start()
{
	::Start(this);

	Game->bAllowUIOpen = true;
	Game->ConstructTitleBar();
}
