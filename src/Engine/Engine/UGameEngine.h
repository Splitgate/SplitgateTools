#pragma once
#include "UEngine.h"
#include "Engine/UGameInstance.h"
#include "Games/BaseGame.h"

#define GEngine Game->Engine
class UGameEngine : public UEngine
{
	void Init(IEngineLoop* InEngineLoop);
	void Start();
	
public:
	GENERATED_BODY(UGameEngine, UEngine, "GameEngine", "/Script/Engine")
	
	UPROPERTY(UGameInstance*, GameInstance);
	
	static void Init_PreEngine();

	struct VTableOffsets
	{
		static inline int GetMaxTickRate;
	};
};

