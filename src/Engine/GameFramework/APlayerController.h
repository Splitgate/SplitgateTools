#pragma once
#include "AController.h"
#include "Engine/ULocalPlayer.h"

class APlayerController : public AController
{
public:
	GENERATED_BODY(APlayerController, AController, "PlayerController", "/Script/Engine")

	static void Init_PostEngine();

	PROPERTY(ULocalPlayer*, Player);

	void SetShowMouseCursor(bool bShow);
	void LocalTravel(FString& FURL);
};
