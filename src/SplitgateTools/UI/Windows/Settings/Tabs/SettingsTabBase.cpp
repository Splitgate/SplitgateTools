#include "SettingsTabBase.h"
#include "UI/Windows/Settings/SettingsWindow.h"

#include <format>
#include <Settings.h>

void SettingsTabBase::RenderContent()
{
	ImGui::Text("sir this is a base class");
}

void SettingsTabBase::RenderSettingsText(const char* Name, const char* Description)
{
	float XMid = ImGui::GetContentRegionAvail().x / 2;

	ImGui::AlignTextToFramePadding();
	ImGui::TextWrapped(Name);

	ImGui::SameLine(); 
	
	if (Description)
		ImGui::HelpMarker(Description);

	ImGui::SameLine(XMid);
}

bool SettingsTabBase::RenderIntSetting(const char* Name, const char* Description, int* v, int min, int max, int step, int step_fast, ImGuiInputTextFlags flags)
{
	std::string FormattedName = std::format("###{}", Name);
	float XMid = ImGui::GetContentRegionAvail().x / 2;

	RenderSettingsText(Name, Description);

	ImGui::PushItemWidth(XMid);
	bool bReturnValue = ImGui::InputIntClamped(FormattedName.c_str(), v, min, max, step, step_fast, flags);
	ImGui::PopItemWidth();

	if (bReturnValue)
		GSettings.ApplyAndSave();

	return bReturnValue;
}

bool SettingsTabBase::RenderFloatSetting(const char* Name, const char* Description, float* v, float v_min, float v_max, float v_speed, const char* format, ImGuiSliderFlags flags)
{
	std::string FormattedName = std::format("###{}", Name);
	float XMid = ImGui::GetContentRegionAvail().x / 2;

	RenderSettingsText(Name, Description);

	ImGui::PushItemWidth(XMid);
	bool bReturnValue = ImGui::DragFloat(FormattedName.c_str(), v, v_speed, v_min, v_max, format, flags);
	ImGui::PopItemWidth();

	if (bReturnValue)
		GSettings.ApplyAndSave();

	return bReturnValue;
}

bool SettingsTabBase::RenderFloat2Setting(const char* Name, const char* Description, float v[2], float v_min, float v_max, float v_speed, const char* format, ImGuiSliderFlags flags)
{
	std::string FormattedName = std::format("###{}", Name);
	float XMid = ImGui::GetContentRegionAvail().x / 2;

	RenderSettingsText(Name, Description);

	ImGui::PushItemWidth(XMid);
	bool bReturnValue = ImGui::DragFloat2(FormattedName.c_str(), v, v_speed, v_min, v_max, format, flags);
	ImGui::PopItemWidth();

	if (bReturnValue)
		GSettings.ApplyAndSave();

	return bReturnValue;
}

bool SettingsTabBase::RenderFloat3Setting(const char* Name, const char* Description, float v[3], float v_min, float v_max, float v_speed, const char* format, ImGuiSliderFlags flags)
{
	std::string FormattedName = std::format("###{}", Name);
	float XMid = ImGui::GetContentRegionAvail().x / 2;

	RenderSettingsText(Name, Description);

	ImGui::PushItemWidth(XMid);
	bool bReturnValue = ImGui::DragFloat3(FormattedName.c_str(), v, v_speed, v_min, v_max, format, flags);
	ImGui::PopItemWidth();

	if (bReturnValue)
		GSettings.ApplyAndSave();

	return bReturnValue;
}

bool SettingsTabBase::RenderFloat4Setting(const char* Name, const char* Description, float v[4], float v_min, float v_max, float v_speed, const char* format, ImGuiSliderFlags flags)
{
	std::string FormattedName = std::format("###{}", Name);
	float XMid = ImGui::GetContentRegionAvail().x / 2;

	RenderSettingsText(Name, Description);

	ImGui::PushItemWidth(XMid);
	bool bReturnValue = ImGui::DragFloat4(FormattedName.c_str(), v, v_speed, v_min, v_max, format, flags);
	ImGui::PopItemWidth();

	if (bReturnValue)
		GSettings.ApplyAndSave();

	return bReturnValue;
}

bool SettingsTabBase::RenderBoolSetting(const char* Name, const char* Description, bool* v)
{
	std::string FormattedName = std::format("###{}", Name);
	float XMid = ImGui::GetContentRegionAvail().x / 2;

	RenderSettingsText(Name, Description);

	ImGui::PushItemWidth(XMid);
	bool bReturnValue = ImGui::Checkbox(FormattedName.c_str(), v);
	ImGui::PopItemWidth();

	if (bReturnValue)
		GSettings.ApplyAndSave();

	return bReturnValue;
}
