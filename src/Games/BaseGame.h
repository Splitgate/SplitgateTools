#pragma once
#include "Memory/Hook.h"

class BaseGame
{
public:
	static std::unique_ptr<BaseGame> Create();

	class UGameEngine* Engine;
	class UWorld** World;
	bool bUsesFProperty;
	
	static void Init_PreGame();
	virtual void Init_PreEngine();
	virtual void Init_PostEngine();
};

inline std::unique_ptr<BaseGame> Game;