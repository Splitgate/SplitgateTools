#pragma once
#include "UObject/UObject.h"

struct FVector
{
	GENERATED_USTRUCT_BODY("Vector", "/Script/CoreUObject")

	float X;
	float Y;
	float Z;
	
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(FVector, X, Y, Z)
};
