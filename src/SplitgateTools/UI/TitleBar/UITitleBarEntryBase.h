#pragma once

#include "imgui.h"
#include "imgui_internal.h"

#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_dx12.h"
#include "backends/imgui_impl_win32.h"

class UITitleBarEntryBase
{
	friend class Renderer;

public:

	UITitleBarEntryBase(const char* InName = "Default Entry", ImGuiWindowFlags InFlags = 0);
	virtual ~UITitleBarEntryBase();

protected:

	const char* Name;
	ImGuiWindowFlags Flags;

	// Called every frame
	virtual void Tick();

	// Called when the menu is currently open, no other times.
	virtual void Render();
};