#pragma once

#include <string>

#include "imgui.h"
#include "imgui_internal.h"

#include "backends/imgui_impl_dx11.h"
#include "backends/imgui_impl_dx12.h"
#include "backends/imgui_impl_win32.h"

// Base class for all ImGui windows that we will render
class UIWindowBase
{
	friend class WindowManager;

public:

	UIWindowBase(std::string InName, bool InbCanHaveMultiple = false, bool InbIsClosable = true, ImGuiWindowFlags InWindowFlags = 0)
	:	OriginalName(InName),
		Name(InName),
		bCanHaveMultiple(InbCanHaveMultiple),
		bIsClosable(InbIsClosable),
		WindowFlags(InWindowFlags) {}
	virtual ~UIWindowBase() = default;

protected:

	// todo: this isnt the best system a lot of string comparison lol
	std::string OriginalName;
	std::string Name;

	bool bCanHaveMultiple;
	bool bIsClosable;

	bool bOpen = true;

	ImGuiWindowFlags WindowFlags;

	virtual void Tick() {}
	virtual void Render();

	virtual void OnOpen() {};
	virtual void OnClose() {};

public:
	void Close() { bOpen = false; }
};