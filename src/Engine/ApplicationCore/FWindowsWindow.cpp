#include "FWindowsWindow.h"

#include "Memory/Hook.h"

#include "Renderer.h"
#include "Misc/FApp.h"

#include "Games/BaseGame.h"

static void (*Initialize)(FWindowsWindow*, __int64, __int64, HINSTANCE, __int64);
void FWindowsWindow::Init_PreEngine()
{
	Memory::Address HWndUse = Memory::FindStringRef(L"Window Creation Failed!").ReverseScan("48 89").Add(3);
	::Initialize = HWndUse.FuncStart();
	LOG_ADDRESS(::Initialize, "FWindowsWindow::Initialize");
	HOOK(Initialize);

	HWnd_Offset = HWndUse.Deref<uint8_t>();
	LOG_OFFSET(HWnd_Offset, "FWindowsWindow::HWnd");
}

void FWindowsWindow::Initialize(__int64 Application, __int64 InDefinition, HINSTANCE InHInstance, __int64 InParent)
{
	::Initialize(this, Application, InDefinition, InHInstance, InParent);

	Renderer::Window = HWnd();

	if (Game && Game->bShouldRenderUI)
	{
		switch (FApp::RHI)
		{
		case EWindowsRHI::D3D11:
			Renderer::SetupDirectX11();
			break;
		case EWindowsRHI::D3D12:
			Renderer::SetupDirectX12();
			break;
		case EWindowsRHI::Unsupported:
			break;
		}
	}
}
