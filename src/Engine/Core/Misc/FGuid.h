#pragma once
#include "UObject/UObject.h"

struct FGuid
{
	GENERATED_USTRUCT_BODY("Guid", "/Script/CoreUObject")
	
	unsigned int A;
	unsigned int B;
	unsigned int C;
	unsigned int D;
	
	friend bool operator==(const FGuid& X, const FGuid& Y) {
		return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) == 0;
	}
	friend bool operator!=(const FGuid& X, const FGuid& Y) {
		return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) != 0;
	}
	bool IsValid() const {
		return ((A | B | C | D) != 0);
	}
};
