#include "ForgeTitleBarEntry.h"

#include "Renderer.h"
#include "UI/WindowManager.h"
#include "UI/Windows/Forge/ForgeMapsWindow.h"

void ForgeTitleBarEntry::Render()
{
	if (ImGui::MenuItem("Maps"))
		WindowManager::Add<ForgeMapsWindow>();

	if (ImGui::MenuItem("My Maps"))
		WindowManager::Add<ForgeMapsWindow>();
}
