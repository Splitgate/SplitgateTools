#pragma once
#include "UGameViewportClient.h"

class IEngineLoop;
class UEngine : public UObject
{
public:
	GENERATED_BODY(UEngine, UObject, "Engine", "/Script/Engine")

	UPROPERTY(UClass*, ConsoleClass);
	UPROPERTY(UGameViewportClient*, GameViewport);
};
