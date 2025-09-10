#pragma once
#include "UI/Helpers/ImHelpers.h"

class SettingsTabBase
{
public:
	SettingsTabBase(const char* InName) : Name(InName) {}

	const char* Name;
	virtual void RenderContent();

private:

	static void RenderSettingsText(const char* Name, const char* Description = nullptr);

protected:

	static bool RenderIntSetting(const char* Name, const char* Description, int* v, int min = 0, int max = 0, int step = 1, int step_fast = 100, ImGuiInputTextFlags flags = 0);
	static bool RenderFloatSetting(const char* Name, const char* Description, float* v, float v_min = 0.0f, float v_max = 0.0f, float v_speed = 1.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	static bool RenderFloat2Setting(const char* Name, const char* Description, float v[2], float v_min = 0.0f, float v_max = 0.0f, float v_speed = 1.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	static bool RenderFloat3Setting(const char* Name, const char* Description, float v[3], float v_min = 0.0f, float v_max = 0.0f, float v_speed = 1.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	static bool RenderFloat4Setting(const char* Name, const char* Description, float v[4], float v_min = 0.0f, float v_max = 0.0f, float v_speed = 1.0f, const char* format = "%.3f", ImGuiSliderFlags flags = 0);
	static bool RenderBoolSetting(const char* Name, const char* Description, bool* v);
};