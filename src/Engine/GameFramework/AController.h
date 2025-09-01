#pragma once
#include "AActor.h"

class AController : public AActor
{
public:
	GENERATED_BODY(AController, AActor, "Controller", "/Script/Engine")

	PROPERTY(class APlayerState*, PlayerState);
};
