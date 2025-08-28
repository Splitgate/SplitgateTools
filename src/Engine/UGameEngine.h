#pragma once
#include "UEngine.h"
#include "Games/BaseGame.h"

#define GEngine Game->Engine
class UGameEngine : public UEngine
{
	void Init(IEngineLoop* InEngineLoop);
	void Start();
	
public:
	GENERATED_BODY(UGameEngine, UEngine, "GameEngine", "/Script/Engine")
	
	static void Init_PreEngine();

};

