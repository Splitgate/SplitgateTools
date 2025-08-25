#pragma once
#include "AController.h"

class APlayerController : public AController
{
public:
	GENERATED_BODY(APlayerController, AController, "PlayerController", "/Script/Engine")

	static void Init_PostEngine();

	void SetShowMouseCursor(bool bShow);
};
