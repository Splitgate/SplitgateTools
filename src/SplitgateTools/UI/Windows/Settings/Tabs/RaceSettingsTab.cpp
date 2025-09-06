#include "RaceSettingsTab.h"

#include "imgui.h"
#include "Settings.h"

void RaceSettingsTab::RenderContent()
{
	// todo: wrapper funcs for rendering these
	int NewCountdownLength = GSettings.Race.CountdownLength;
	if (ImGui::InputInt("Countdown Length", &NewCountdownLength))
	{
		GSettings.Race.CountdownLength = std::max(0, NewCountdownLength);
		GSettings.Save();
	}
}
