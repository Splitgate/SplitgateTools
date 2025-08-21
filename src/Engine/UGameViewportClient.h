#pragma once
#include "UConsole.h"

class UGameViewportClient : public UObject
{
public:
	GENERATED_BODY(UGameViewportClient, UObject, "GameViewportClient", "/Script/Engine")
	
	UPROPERTY(UConsole*, ViewportConsole);
};
