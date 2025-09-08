#pragma once

class SettingsTabBase
{
public:
	SettingsTabBase(const char* InName) : Name(InName) {}

	const char* Name;
	virtual void RenderContent();

protected:

	static bool RenderIntSetting(const char* Name, const char* Description, int* v, int min = 0, int max = 0, int step = 1, int step_fast = 100, int flags = 0);
};