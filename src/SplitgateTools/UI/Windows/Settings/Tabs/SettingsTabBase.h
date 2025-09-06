#pragma once

class SettingsTabBase
{
public:
	SettingsTabBase(const char* InName) : Name(InName) {}
	const char* Name;
	virtual void RenderContent();
};