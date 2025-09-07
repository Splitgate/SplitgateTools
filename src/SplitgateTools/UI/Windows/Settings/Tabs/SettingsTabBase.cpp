#include "SettingsTabBase.h"

#include "UI/Windows/Settings/SettingsWindow.h"
#include "UI/Helpers/ImHelpers.h"

#include <format>

void SettingsTabBase::RenderContent()
{
	ImGui::Text("sir this is a base class");
}

bool SettingsTabBase::RenderIntSetting(const char* Name, const char* Description, int* v, int min, int max, int step, int step_fast, int flags)
{
	std::string FormattedName = std::format("###{}", Name);
	float XMid = ImGui::GetContentRegionAvail().x / 2;

	ImGui::AlignTextToFramePadding();
	ImGui::TextWrapped(Name);

	ImGui::SameLine(); ImGui::HelpMarker(Description);

	ImGui::SameLine(XMid);

	ImGui::PushItemWidth(XMid);
	bool bReturnValue = ImGui::InputIntClamped("###Countdown Length", v, min, max, step, step_fast, flags);
	ImGui::PopItemWidth();

	return bReturnValue;
}
