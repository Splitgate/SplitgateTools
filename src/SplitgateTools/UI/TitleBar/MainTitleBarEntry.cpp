#include "MainTitleBarEntry.h"

#include <memory>

#include "Renderer.h"
#include "UI/System/SystemVersionWindow.h"
#include "UI/System/ToolSettings.h"
#include "UI/System/VersionWindow.h"

void MainTitleBarEntry::Render()
{
	// todo rename all thse classes to make them clearer what they are
				
	if (ImGui::MenuItem("Game Info"))
	{
		Renderer::UIElements.push_back(std::make_unique<VersionWindow>());
	}

	if (ImGui::MenuItem("Tool Info"))
	{
		Renderer::UIElements.push_back(std::make_unique<SystemVersionWindow>());
	}

	if (ImGui::MenuItem("Settings"))
	{
		Renderer::UIElements.push_back(std::make_unique<ToolSettings>());
	}

	ImGui::Separator();

	if (ImGui::MenuItem("Quit"))
	{
		exit(0);
	}
}
