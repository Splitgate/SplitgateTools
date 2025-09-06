#pragma once

#include "imgui.h"
#include "imgui_internal.h"

#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_dx12.h"
#include "backends/imgui_impl_win32.h"

class UITitleBarEntry
{
	friend class Renderer;

public:

	UITitleBarEntry(const char* InName = "Default Entry", ImGuiWindowFlags InFlags = 0);
	virtual ~UITitleBarEntry();

protected:

	const char* Name;
	ImGuiWindowFlags Flags;

	// Called every frame
	virtual void Tick();

	// Called when the menu is currently open, no other times.
	virtual void Render();
};