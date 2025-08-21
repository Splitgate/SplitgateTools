#pragma once

#include "imgui.h"
#include "imgui_internal.h"

#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_dx12.h"
#include "backends/imgui_impl_win32.h"

// Base class for all ImGui windows that we will render
class UIElement
{
	UIElement(const char* InWindowName, bool InbIsClosable = true, bool InbIsOpen = true, ImGuiWindowFlags InWindowFlags = 0);
	virtual ~UIElement();

public:

	const char* WindowName;
	bool bIsClosable;
	bool bIsOpen;
	ImGuiWindowFlags WindowFlags;

	virtual void Tick();
	virtual void Render();

private:

	void CloseWindow();

	virtual void OnClose() {};
	virtual void OnOpen() {};
};