#pragma once

#include "imgui.h"
#include "imgui_internal.h"

#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_dx12.h"
#include "backends/imgui_impl_win32.h"

// Base class for all ImGui windows that we will render
class UIElement
{
	friend class Renderer;

protected:

	UIElement(const char* InWindowName, bool InbCanHaveMultiple = false, bool InbIsClosable = true, bool InbIsOpen = true, ImGuiWindowFlags InWindowFlags = 0);
	virtual ~UIElement();

	const char* WindowName;
	char WindowId[256];

	bool bCanHaveMultiple;
	bool bIsClosable;
	bool bIsOpen;
	ImGuiWindowFlags WindowFlags;

	virtual void Tick();
	virtual void Render();
	void CloseWindow();

	virtual void OnClose() {};
	virtual void OnOpen() {};

private:

	void CloseInternal();
};