#pragma once
#include "UObject/UObject.h"

struct FRotator
{
	GENERATED_USTRUCT_BODY("Rotator", "/Script/CoreUObject")

	float Pitch;
	float Yaw;
	float Roll;
};
