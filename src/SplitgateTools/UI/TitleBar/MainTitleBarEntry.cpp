#include "MainTitleBarEntry.h"

#include <memory>

#include "Renderer.h"
#include "UI/WindowManager.h"
#include "UI/Windows/ToolInfoWindow.h"
#include "UI/Windows/SettingsWindow.h"
#include "UI/Windows/GameInfoWindow.h"
#include "UI/Windows/HydraWindow.h"

void MainTitleBarEntry::Render()
{
	// todo rename all thse classes to make them clearer what they are
				
	if (ImGui::MenuItem("Settings"))
		WindowManager::Add<SettingsWindow>();
	
	if (ImGui::MenuItem("Game Info"))
		WindowManager::Add<GameInfoWindow>();

	if (ImGui::MenuItem("Tool Info"))
		WindowManager::Add<ToolInfoWindow>();

	if (ImGui::MenuItem("hydra"))
		WindowManager::Add<HydraWindow>();

	ImGui::Separator();

	if (ImGui::MenuItem("Quit"))
	{
		exit(0);
	}
}
