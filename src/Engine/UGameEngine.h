#pragma once
#include "UEngine.h"
#include "Games/BaseGame.h"

#define GEngine Game->Engine
class UGameEngine : public UEngine
{
	void Init(IEngineLoop* InEngineLoop);
	
public:
	GENERATED_BODY(UGameEngine, UEngine, "GameEngine", "/Script/Engine")
	
	static void Init_PreEngine();

};

