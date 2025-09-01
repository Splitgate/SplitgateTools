#pragma once
#include "AController.h"

class APlayerState : public AActor
{
public:
	GENERATED_BODY(APlayerState, AActor, "PlayerState", "/Script/Engine")

	PROPERTY(int, PlayerId);
};
