#pragma once
#include "AActor.h"
#include "Core/UObject/UClass.h"

class AWorldSettings : public AActor
{
public:
	GENERATED_BODY(AWorldSettings, AActor, "WorldSettings", "/Script/Engine")

	UPROPERTY(float, TimeDilation);
	UPROPERTY(float, MatineeTimeDilation);
	UPROPERTY(float, DemoPlayTimeDilation);

	UPROPERTY(float, MinGlobalTimeDilation);
	UPROPERTY(float, MaxGlobalTimeDilation);

	UPROPERTY(float, MinUndilatedFrameTime);
	UPROPERTY(float, MaxUndilatedFrameTime);

	// Copied from engine
	float GetEffectiveTimeDilation()
	{
		return TimeDilation() * MatineeTimeDilation() * DemoPlayTimeDilation();
	}
};