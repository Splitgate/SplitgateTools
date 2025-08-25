#include "Renderer.h"
#include "UI/UIElement.h"
#include "Memory/Hook.h"
#include "Memory/Memory.h"

// UI Elements
#include "UI/System/VersionWindow.h"
#include "UI/System/SystemVersionWindow.h"

// Games
#include "Games/BaseGame.h"

// DirectX includes
#include <dxgi.h>
#include <dxgi1_4.h>

#include <d3d11.h>
#include <d3d12.h>

// ImGui includes
#include "imgui.h"
#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_dx12.h"
#include "backends/imgui_impl_win32.h"
#include "imgui_internal.h"

// DirectX libraries
#pragma comment(lib, "dxgi.lib") // this made the dll chunky but we need it
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d12.lib")

////////////////////////////////////////////////////////////////////////////////////////
/// DirectX 11 implementation
////////////////////////////////////////////////////////////////////////////////////////

bool Renderer::SetupDirectX11()
{
	IDXGISwapChain* SwapChain;
	ID3D11Device* Device;
	ID3D11DeviceContext* Context;
	DXGI_SWAP_CHAIN_DESC SwapChainDescription = {};
	D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;

	SwapChainDescription.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	SwapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDescription.SampleDesc.Count = 1;
	SwapChainDescription.BufferCount = 1;
	SwapChainDescription.OutputWindow = Window;
	SwapChainDescription.Windowed = true;
	
	HRESULT Result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, 0, &FeatureLevel, 1, D3D11_SDK_VERSION, &SwapChainDescription, &SwapChain, &Device, nullptr, &Context);
	if (FAILED(Result))
	{
		UE_LOG(LogRHI, Error, "Could not initialize D3D11 device and swapchain! UI will not function.");
		return false;
	}

	PVOID* VTable = *reinterpret_cast<PVOID**>(SwapChain);
	PVOID PresentFunc = VTable[DX11::PresentIndex];
	PVOID ResizeBuffersFunc = VTable[DX11::ResizeBuffersIndex];

	DX11::Present = decltype(DX11::Present)(PresentFunc);
	DX11::ResizeBuffers = decltype(DX11::ResizeBuffers)(ResizeBuffersFunc);

	// i am gui
	ImGui_Setup();

	Detour(DX11::Present, DX11::Present_Hook);
	Detour(DX11::ResizeBuffers, DX11::ResizeBuffers_Hook);

	// Release each thingy ma bobby
	SwapChain->Release();
	Device->Release();
	Context->Release();

	return true;
}

inline HRESULT Renderer::DX11::Present_Hook(IDXGISwapChain* Instance, UINT SyncInterval, UINT Flags)
{
	static float Width = 0;
	static float Height = 0;

	// Poll and handle messages (inputs, window resize, etc...)
	static MSG Msg;
	ZeroMemory(&Msg, sizeof(MSG));
	if (PeekMessage(&Msg, Window, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	// Initialize devices if need be
	if (!DX11::Device)
	{
		Instance->GetDevice(__uuidof(DX11::Device), reinterpret_cast<PVOID*>(&DX11::Device));
		DX11::Device->GetImmediateContext(&DX11::DeviceContext);

		ID3D11Texture2D* Target = nullptr;
		Instance->GetBuffer(0, __uuidof(Target), reinterpret_cast<PVOID*>(&Target));

		DX11::Device->CreateRenderTargetView(Target, nullptr, &DX11::CurrentRenderTargetView);
		Target->Release();

		ID3D11Texture2D* Buffer = nullptr;
		Instance->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<PVOID*>(&Buffer));

		D3D11_TEXTURE2D_DESC desc = {};
		Buffer->GetDesc(&desc);

		if (!Width) DXShared::PreviousWindowFunc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(Window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(DXShared::WndProc_Hook)));

		Width = static_cast<float>(desc.Width);
		Height = static_cast<float>(desc.Height);
		Buffer->Release();

		// Handle application-specific workload.
		ImGui_ImplWin32_Init(Window);
		ImGui_ImplDX11_Init(DX11::Device, DX11::DeviceContext);
		ImGui_ImplDX11_CreateDeviceObjects();
	}

	// Time to draw
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	//if (bShowDemo)
	//{
	//	ImGui::ShowDemoWindow(&bShowDemo);
	//}

	ImGui_DrawAll();
	ImGui_VisbilityAlpha();

	ImGui::Render();
	DeviceContext->OMSetRenderTargets(1, &DX11::CurrentRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Call Original
	return DX11::Present(Instance, SyncInterval, Flags);
}

HRESULT Renderer::DX11::ResizeBuffers_Hook(IDXGISwapChain* Instance, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	ImGui_ImplDX11_Shutdown();

	DX11::CurrentRenderTargetView->Release();
	DX11::DeviceContext->Release();
	DX11::Device->Release();
	DX11::Device = nullptr;

	return DX11::ResizeBuffers(Instance, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

////////////////////////////////////////////////////////////////////////////////////////
/// DirectX 12 implementation
////////////////////////////////////////////////////////////////////////////////////////

bool Renderer::SetupDirectX12()
{
	D3D_FEATURE_LEVEL MinimumFeatureLevel = D3D_FEATURE_LEVEL_12_0;

	HRESULT FactoryResult = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&DX12::Factory);
	if (FAILED(FactoryResult))
	{
		UE_LOG(LogRHI, Error, "Could not create DXGI Factory! UI will not function.");
		return false;
	}

	HRESULT DeviceResult = D3D12CreateDevice(nullptr, MinimumFeatureLevel, __uuidof(ID3D12Device), (void**)&DX12::Device);
	if (FAILED(DeviceResult))
	{
		UE_LOG(LogRHI, Error, "Could not initialize D3D12 device! UI will not function.");
		return false;
	}

	D3D12_COMMAND_QUEUE_DESC QueueDescription;
	QueueDescription.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	QueueDescription.Priority = 0;
	QueueDescription.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	QueueDescription.NodeMask = 0;

	HRESULT CommandQueueResult = DX12::Device->CreateCommandQueue(&QueueDescription, __uuidof(ID3D12CommandQueue), (void**)&DX12::CommandQueue);
	if (FAILED(CommandQueueResult))
	{
		UE_LOG(LogRHI, Error, "Could not create D3D12 command queue! UI will not function.");
		return false;
	}

	HRESULT CommandAllocatorResult = DX12::Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, __uuidof(ID3D12CommandAllocator), (void**)&DX12::CommandAllocator);
	if (FAILED(CommandAllocatorResult))
	{
		UE_LOG(LogRHI, Error, "Could not create D3D12 command allocator! UI will not function.");
		return false;
	}

	HRESULT CommandListResult = DX12::Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, DX12::CommandAllocator, nullptr, __uuidof(ID3D12GraphicsCommandList), (void**)&DX12::CommandList);
	if (FAILED(CommandListResult))
	{
		UE_LOG(LogRHI, Error, "Could not create D3D12 command list! UI will not function.");
		return false;
	}

	DXGI_MODE_DESC BufferDescription;
	BufferDescription.Width = 0;
	BufferDescription.Height = 0;
	BufferDescription.RefreshRate.Numerator = 60;
	BufferDescription.RefreshRate.Denominator = 1;
	BufferDescription.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	BufferDescription.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	BufferDescription.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC SwapChainDescription = {};
	SwapChainDescription.BufferDesc = BufferDescription;
	// MSAA Sample count
	SwapChainDescription.SampleDesc.Count = 1;
	SwapChainDescription.SampleDesc.Quality = 0;
	SwapChainDescription.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDescription.BufferCount = 2;
	SwapChainDescription.OutputWindow = Window;
	SwapChainDescription.Windowed = true;
	SwapChainDescription.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SwapChainDescription.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// TODO: This fails, so no dx12 swap chain is made and no UI will occur...
	HRESULT SwapChainResult = DX12::Factory->CreateSwapChain(DX12::CommandQueue, &SwapChainDescription, &DX12::SwapChain);
	if (FAILED(SwapChainResult))
	{
		UE_LOG(LogRHI, Error, "Could not create D3D12 swapchain! UI will not function.");
		return false;
	}

	//PVOID* VTable = *reinterpret_cast<PVOID**>(DX12::SwapChain);
	uint64_t* MethodsTable = (uint64_t*)::calloc(150, sizeof(uint64_t));
	memcpy(MethodsTable, *(uint64_t**)DX12::Device, 44 * sizeof(uint64_t));
	memcpy(MethodsTable + 44, *(uint64_t**)DX12::CommandQueue, 19 * sizeof(uint64_t));
	memcpy(MethodsTable + 44 + 19, *(uint64_t**)DX12::CommandAllocator, 9 * sizeof(uint64_t));
	memcpy(MethodsTable + 44 + 19 + 9, *(uint64_t**)DX12::CommandQueue, 60 * sizeof(uint64_t));
	memcpy(MethodsTable + 44 + 19 + 9 + 60, *(uint64_t**)DX12::SwapChain, 18 * sizeof(uint64_t));

	uint64_t PresentFunc = MethodsTable[DX12::PresentIndex];
	uint64_t ExecuteCommandListsFunc = MethodsTable[DX12::ExecuteCommandListsIndex];
	uint64_t ResizeBuffersFunc = MethodsTable[DX12::ResizeBuffersIndex];

	DX12::Present = decltype(DX12::Present)(PresentFunc);
	DX12::ExecuteCommandLists = decltype(DX12::ExecuteCommandLists)(ExecuteCommandListsFunc);
	DX12::ResizeBuffers = decltype(DX12::ResizeBuffers)(ResizeBuffersFunc);

	// i am gui
	ImGui_Setup();

	Detour(DX12::Present, DX12::Present_Hook);
	Detour(DX12::ExecuteCommandLists, DX12::ExecuteCommandLists_Hook);
	Detour(DX12::ResizeBuffers, DX12::ResizeBuffers_Hook);

	// Release each thingy ma bobby
	DX12::SwapChain->Release();
	DX12::Device->Release();
	DX12::CommandQueue->Release();
	//DX12::CommandAllocator->Release();
	DX12::CommandList->Release();
	DX12::Factory->Release();

	return true;
}

inline HRESULT Renderer::DX12::Present_Hook(IDXGISwapChain* Instance, UINT SyncInterval, UINT Flags)
{
	static float Width = 0;
	static float Height = 0;

	// Poll and handle messages (inputs, window resize, etc...)
	static MSG Msg;
	ZeroMemory(&Msg, sizeof(MSG));
	if (PeekMessage(&Msg, Window, 0U, 0U, PM_REMOVE))
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}

	// Time to draw
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui_DrawAll();
	ImGui_VisbilityAlpha();

	ImGui::Render();

	//UINT FrameIndex = DX12::Swapchain3->GetCurrentBackBufferIndex();
	//
	//D3D12_RESOURCE_BARRIER Barrier;
	//Barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//Barrier.Transition.pResource = DX12::BackBuffer[FrameIndex];
	//Barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	//Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//DX12::CommandList->Reset(DX12::CommandAllocator[FrameIndex], nullptr);
	//DX12::CommandList->ResourceBarrier(1, &Barrier);
	//DX12::CommandList->OMSetRenderTargets(1, &CurrentFrameContext.main_render_target_descriptor, FALSE, nullptr);
	//DX12::CommandList->SetDescriptorHeaps(1, &DX12::SrvHeapDesc);
	//
	//ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), DX12::CommandList);
	//
	//Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	//DX12::CommandList->ResourceBarrier(1, &Barrier);
	//DX12::CommandList->Close();
	//DX12::CommandQueue->ExecuteCommandLists(1, (ID3D12CommandList**)&DX12::CommandList);
	//DX12::Swapchain3->Release();
	//
	//UE_LOG(LogInit, Warning, "DX12 Present Hook");

	return DX12::Present(Instance, SyncInterval, Flags);
}

inline HRESULT Renderer::DX12::ExecuteCommandLists_Hook(ID3D12CommandQueue* Queue, UINT NumCommandLists, ID3D12CommandList* CommandLists)
{
	if (!DX12::CommandQueue && Queue->GetDesc().Type == D3D12_COMMAND_LIST_TYPE_DIRECT) 
		DX12::CommandQueue = Queue;

	return DX12::ExecuteCommandLists(Queue, NumCommandLists, CommandLists);
}

HRESULT Renderer::DX12::ResizeBuffers_Hook(IDXGISwapChain3* Instance, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	ImGui_ImplDX12_Shutdown();

	return DX12::ResizeBuffers(Instance, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

////////////////////////////////////////////////////////////////////////////////////////
/// DirectX Shared implementations
////////////////////////////////////////////////////////////////////////////////////////

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT Renderer::DXShared::WndProc_Hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_KEYUP && wParam == VK_F1)
	{
		ShowUI = !ShowUI;
		
		if (Game)
			Game->OnUIVisibilityChange(ShowUI);
	}

	if (ShowUI)
		ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
	else
		return CallWindowProc(DXShared::PreviousWindowFunc, hWnd, uMsg, wParam, lParam);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////////////
/// Renderer
////////////////////////////////////////////////////////////////////////////////////////

void Renderer::ImGui_Setup()
{
	ImGui::CreateContext();

	ImGuiIO& IO = ImGui::GetIO();
	IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // | ImGuiConfigFlags_NoMouseCursorChange; // | ImGuiConfigFlags_ViewportsEnable;
	// For context, ImGuiConfigFlags_NoMouseCursorChange will fix the mouse flickering in-game but means there will be no mouse in-game unless we set the unreal mouse visible
	// possible fix could be calling ResetToDefaultPointerInputSettings when the ui opens to make the cursor visible, resetting it however to be invisible i am unsure of
}

static inline int HACK_InitialHide = 0;
static inline int HACK_InitialHideMax = 50;

void Renderer::ImGui_DrawAll()
{
	// This hack hides the UI for the first 50 frames due to a current issue where UI starts visible then vanishes after a few frames
	if (HACK_InitialHide <= HACK_InitialHideMax)
	{
		++HACK_InitialHide;
		return;
	}

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("[Splitgate]"))
		{
			if (ImGui::MenuItem("Game Info"))
			{
				new VersionWindow();
			}

			if (ImGui::MenuItem("Tool Info"))
			{
				new SystemVersionWindow();
			}

			//if (ImGui::MenuItem("ImGui Demo"))
			//{
			//	bShowDemo = !bShowDemo;
			//}

			ImGui::Separator();
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Assets"))
		{
			ImGui::SeparatorText("Levels");
		
			if (ImGui::MenuItem("Playable Maps"))
			{
			}
		
			if (ImGui::MenuItem("All Maps"))
			{
			}
		
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Weapons"))
		{
			ImGui::SeparatorText("Levels");

			if (ImGui::MenuItem("Weapon List"))
			{
			}
		
			if (ImGui::MenuItem("Weapon Modifications"))
			{
			}
		
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	for (auto UIElement : UIElements)
	{
		if (!UIElement) continue;

		UIElement->Tick();

		ImGui::Begin(UIElement->WindowName,
			(UIElement->bIsClosable ? &UIElement->bIsOpen : nullptr), UIElement->WindowFlags | ImGuiWindowFlags_NoCollapse);
		
		UIElement->Render();

		ImGui::End();
	}
}

void Renderer::ImGui_VisbilityAlpha()
{
	// Fade ui in / out (thank you very much winver...)
	auto& Style = ImGui::GetStyle();
	if (bUseFade)
	{
		if (ShowUI && Style.Alpha < VisibleAlpha)
		{
			Style.Alpha += ImGui::GetIO().DeltaTime * OpacityFadeSpeed;
		}
		else if (!ShowUI && Style.Alpha > HiddenAlpha)
		{
			Style.Alpha -= ImGui::GetIO().DeltaTime * OpacityFadeSpeed;
		}
		else
		{
			// snap to rounded alpha values in case of floating point inprecisions
			Style.Alpha = ShowUI ? VisibleAlpha : HiddenAlpha;
		}
	}
	else // disabled fade, snaps to and from
	{
		Style.Alpha = ShowUI ? VisibleAlpha : HiddenAlpha;
	}
}