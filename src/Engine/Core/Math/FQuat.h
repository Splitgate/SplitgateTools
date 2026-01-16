#pragma once
#include "UObject/UObject.h"

struct FQuat
{
	GENERATED_USTRUCT_BODY("Quat", "/Script/CoreUObject")

	float X;
	float Y;
	float Z;
	float W;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(FQuat, X, Y, Z, W)
};
