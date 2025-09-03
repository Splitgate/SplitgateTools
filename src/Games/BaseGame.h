#pragma once
#include "Memory/Hook.h"
#include "Engine/UWorld.h"

class BaseGame
{
public:
	static std::unique_ptr<BaseGame> Create();

	class UGameEngine* Engine;
	class UWorld** World;
	bool bUsesFProperty;
	bool bShouldRenderUI;
	bool bAllowUIOpen;

	BaseGame();
	
	static void Init_PreGame();
	virtual void Init_PreEngine();
	virtual void Init_PostEngine();

	virtual void ConstructTitleBar();
	virtual void GatherSettingsEntries();

	virtual void OnUIVisibilityChange(bool bVisible);
};

inline std::unique_ptr<BaseGame> Game;