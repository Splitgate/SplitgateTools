#pragma once
#include "Memory/Hook.h"
#include "Engine/UWorld.h"
#include "UI/TitleBar/UITitleBarEntry.h"
#include "UI/Settings/SettingsTab.h"

class BaseGame
{
public:
	static std::unique_ptr<BaseGame> Create();

	class UGameEngine* Engine;
	class UWorld** World;
	bool bUsesFProperty;
	bool bShouldRenderUI;
	bool bAllowUIOpen;

	static void Init_PreGame();
	virtual void Init_PreEngine();
	virtual void Init_PostEngine();

	virtual void PopulateTitleBarEntries(std::vector<std::unique_ptr<UITitleBarEntry>>& Entries) {}
	virtual void PopulateSettingsTabs(std::vector<std::unique_ptr<SettingsTab>>& Tabs) {}

	virtual void OnUIVisibilityChange(bool bVisible);
};

inline std::unique_ptr<BaseGame> Game;