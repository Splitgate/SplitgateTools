#pragma once

#include "imgui.h"
#include "imgui_internal.h"

#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_dx12.h"
#include "backends/imgui_impl_win32.h"

// Base class for all ImGui windows that we will render
class UIWindowBase
{
	friend class Renderer;

public:

	UIWindowBase(const char* InName, bool InbCanHaveMultiple = false, bool InbIsClosable = true, ImGuiWindowFlags InWindowFlags = 0);
	virtual ~UIWindowBase() {}

protected:

	const char* Name;

	bool bCanHaveMultiple;
	bool bIsClosable;

	ImGuiWindowFlags WindowFlags;

	virtual void Tick() {}
	virtual void Render();
};