#pragma once
#include "Memory/Hook.h"

class BaseGame
{
public:
	static std::unique_ptr<BaseGame> Create();

	class UGameEngine* Engine;
	class UWorld** World;
	bool bUsesFProperty;
	bool bShouldRenderUI;
	
	static void Init_PreGame();
	virtual void Init_PreEngine();
	virtual void Init_PostEngine();

	virtual void OnUIVisibilityChange(bool bVisible);
};

inline std::unique_ptr<BaseGame> Game;