#include "RaceSettingsTab.h"

#include "imgui.h"
#include "Settings.h"

void RaceSettingsTab::RenderContent()
{
	// Old Logic
	//
	//float XMid = ImGui::GetContentRegionAvail().x / 2;
	//
	//ImGui::AlignTextToFramePadding();
	//ImGui::TextWrapped("Countdown Length");
	//
	//ImGui::SameLine(); ImGui::HelpMarker("Sets the race initial countdown limited between 0 and 2147483647");
	//
	//ImGui::SameLine(XMid);
	//
	//// todo: wrapper funcs for rendering these
	//int NewCountdownLength = GSettings.Race.CountdownLength;
	//ImGui::PushItemWidth(XMid);
	//if (ImGui::InputIntClamped("###Countdown Length", &NewCountdownLength, 0, INT_MAX))
	//{
	//	GSettings.Race.CountdownLength = std::max(0, NewCountdownLength);
	//	GSettings.ApplyAndSave();
	//}
	//ImGui::PopItemWidth();

	RenderIntSetting(
		"Countdown Length", "Sets the amount of seconds the race countdown timer will wait before starting",
		&GSettings.Race.CountdownLength,
		0, INT_MAX);

	RenderBoolSetting(
		"Enable Console", "This will enable the usage of the unreal console during races but will disable uploading stats.",
		&GSettings.Race.AllowConsole);
}