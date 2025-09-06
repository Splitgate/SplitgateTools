#include "MainTitleBarEntry.h"

#include <memory>

#include "Renderer.h"
#include "UI/Windows/SystemVersionWindow.h"
#include "UI/Windows/ToolSettings.h"
#include "UI/Windows/VersionWindow.h"

void MainTitleBarEntry::Render()
{
	// todo rename all thse classes to make them clearer what they are
				
	if (ImGui::MenuItem("Game Info"))
		Renderer::AddWindow<VersionWindow>();

	if (ImGui::MenuItem("Tool Info"))
		Renderer::AddWindow<SystemVersionWindow>();

	if (ImGui::MenuItem("Settings"))
		Renderer::AddWindow<ToolSettings>();

	ImGui::Separator();

	if (ImGui::MenuItem("Quit"))
	{
		exit(0);
	}
}
