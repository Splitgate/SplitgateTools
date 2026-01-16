#pragma once
#include "UObject/UObject.h"

#include "FQuat.h"
#include "FVector.h"
#include "FRotator.h"
#include "Math/UnrealMathSSE.h"

MS_ALIGN(16) struct FTransform
{
	GENERATED_USTRUCT_BODY("Transform", "/Script/CoreUObject")

	VectorRegister Rotation;
	VectorRegister Translation;
	VectorRegister Scale3D;

	FORCEINLINE FQuat GetRotation() const
	{
		FQuat OutRotation;
		VectorStoreAligned(Rotation, &OutRotation);
		return OutRotation;
	}

	FORCEINLINE FVector GetTranslation() const
	{
		FVector OutTranslation;
		VectorStoreFloat3(Translation, &OutTranslation);
		return OutTranslation;
	}

	FORCEINLINE FVector GetScale3D() const
	{
		FVector OutScale3D;
		VectorStoreFloat3(Scale3D, &OutScale3D);
		return OutScale3D;
	}

	void ParseJson(json JsonData)
	{
		if (JsonData.contains("ROTATION"))
		{
			FQuat NewRotation = uppercase_keys(JsonData.at("ROTATION"));
			Rotation = VectorLoadAligned(&NewRotation);
		}

		if (JsonData.contains("TRANSLATION"))
		{
			FVector NewTranslation = uppercase_keys(JsonData.at("TRANSLATION"));
			Translation = VectorLoadFloat3_W0(&NewTranslation);
		}

		if (JsonData.contains("SCALE3D"))
		{
			FVector NewScale3D = uppercase_keys(JsonData.at("SCALE3D"));
			Scale3D = VectorLoadFloat3_W0(&NewScale3D);
		}
	}
};