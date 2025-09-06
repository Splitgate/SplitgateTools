#pragma once

#include "Globals.h"

// Need this included in the header for FrameContext::D3D12_CPU_DESCRIPTOR_HANDLE
#include "d3d12.h"
#include "dxgi1_4.h"

#include "UI/UITitleBarEntry.h"

class Renderer
{
public:

	static inline std::vector<class UIElement*> UIElements;
	static inline std::vector<std::unique_ptr<UITitleBarEntry>> TitleBarEntries;
	

	// Shared between all renderers
	static inline HWND Window;

	// ImGui top level controls
	static inline bool ShowUI = false;
	static inline bool bUseFade = true;
	static inline bool bShowDemo = false;

	static inline float Opacity = 1.f;
	static inline float OpacityFadeSpeed = 8.f; // Original Value: 3
	static inline float VisibleAlpha = 0.95f;
	static inline float HiddenAlpha = 0.0f;

	class DX11
	{
	public:
		// VTable indexes, shouldn't really change
		static inline int PresentIndex = 0x8;
		static inline int ResizeBuffersIndex = 0xD;

		// DirectX Functions
		static inline HRESULT(*Present)(struct IDXGISwapChain*, UINT, UINT);
		static inline HRESULT(*ResizeBuffers)(struct IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);

		// DirectX Variables
		static inline struct ID3D11Device* Device;
		static inline struct ID3D11DeviceContext* DeviceContext;
		static inline struct ID3D11RenderTargetView* CurrentRenderTargetView;

		// DirectX Hooks
		static inline HRESULT Present_Hook(struct IDXGISwapChain* Instance, UINT SyncInterval, UINT Flags);
		static HRESULT ResizeBuffers_Hook(struct IDXGISwapChain* Instance, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
	};

	class DX12
	{
	public:
		struct FrameContext {
			ID3D12CommandAllocator* command_allocator;
			ID3D12Resource* main_render_target_resource;
			D3D12_CPU_DESCRIPTOR_HANDLE main_render_target_descriptor;
		};

		// VTable indexes, shouldn't really change
		static inline int PresentIndex = 0x8C;
		static inline int ExecuteCommandListsIndex = 0x36;
		static inline int ResizeBuffersIndex = 0x92;

		// DirectX Functions
		static inline HRESULT(*Present)(struct IDXGISwapChain*, UINT, UINT);
		static inline HRESULT(*ExecuteCommandLists)(ID3D12CommandQueue*, UINT, ID3D12CommandList*);
		static inline HRESULT(*ResizeBuffers)(struct IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT);

		// DirectX Variables
		static inline struct IDXGIFactory* Factory;
		static inline ID3D12Device* Device;
		static inline ID3D12CommandQueue* CommandQueue;
		static inline ID3D12CommandAllocator* CommandAllocator;
		static inline ID3D12GraphicsCommandList* CommandList;
		static inline struct IDXGISwapChain* SwapChain;
		static inline struct IDXGISwapChain3* Swapchain3;
		static inline struct DXGI_SWAP_CHAIN_DESC SwapDesc;

		static inline struct ID3D12DescriptorHeap* RtvHeapDesc;
		static inline struct ID3D12DescriptorHeap* SrvHeapDesc;

		static inline std::vector<FrameContext> FrameContexts;

		static inline bool IsReady = true;

		// DirectX Hooks
		static inline HRESULT Present_Hook(struct IDXGISwapChain* Instance, UINT SyncInterval, UINT Flags);
		static inline HRESULT ExecuteCommandLists_Hook(ID3D12CommandQueue* Queue, UINT NumCommandLists, ID3D12CommandList* CommandLists);
		static HRESULT ResizeBuffers_Hook(struct IDXGISwapChain3* Instance, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);
	};

	class DXShared
	{
	public:
		// Shared WndProc for input detection
		static inline WNDPROC PreviousWindowFunc;

		// Shared WndProc Hook
		static LRESULT CALLBACK WndProc_Hook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};

	static bool SetupDirectX11();
	static bool SetupDirectX12();

	static void ImGui_Setup();
	static void ImGui_DrawAll();
	static void ImGui_VisbilityAlpha();
};
