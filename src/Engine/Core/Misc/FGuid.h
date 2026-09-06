#pragma once
#include "UObject/UObject.h"

enum class EGuidFormats
{
	Digits,
	DigitsWithHyphens,
	DigitsWithHyphensInBraces,
	DigitsWithHyphensInParentheses,
	HexValuesInBraces,
	UniqueObjectGuid,
	Short,
	Base36Encoded,
};

struct FGuid
{
	GENERATED_USTRUCT_BODY("Guid", "/Script/CoreUObject")

	FGuid()
		: A(0)
		, B(0)
		, C(0)
		, D(0)
	{
	}

	explicit FGuid(uint32 InA, uint32 InB, uint32 InC, uint32 InD)
		: A(InA), B(InB), C(InC), D(InD)
	{
	}

	friend bool operator==(const FGuid& X, const FGuid& Y)
	{
		return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) == 0;
	}

	friend bool operator!=(const FGuid& X, const FGuid& Y)
	{
		return ((X.A ^ Y.A) | (X.B ^ Y.B) | (X.C ^ Y.C) | (X.D ^ Y.D)) != 0;
	}

	friend bool operator<(const FGuid& X, const FGuid& Y)
	{
		return	((X.A < Y.A) ? true : ((X.A > Y.A) ? false :
			((X.B < Y.B) ? true : ((X.B > Y.B) ? false :
				((X.C < Y.C) ? true : ((X.C > Y.C) ? false :
					((X.D < Y.D) ? true : ((X.D > Y.D) ? false : false)))))))); //-V583
	}

	uint32& operator[](int32 Index)
	{
		switch (Index)
		{
		case 0: return A;
		case 1: return B;
		case 2: return C;
		case 3: return D;
		}

		return A;
	}

	const uint32& operator[](int32 Index) const
	{
		switch (Index)
		{
		case 0: return A;
		case 1: return B;
		case 2: return C;
		case 3: return D;
		}

		return A;
	}

	void Invalidate()
	{
		A = B = C = D = 0;
	}

	bool IsValid() const
	{
		return ((A | B | C | D) != 0);
	}

	std::string ToString() const
	{
		return ToString(EGuidFormats::Digits);
	}

	std::string ToString(EGuidFormats Format) const;

	static FGuid NewGuid();

	// Parse

public:

	uint32 A;
	uint32 B;
	uint32 C;
	uint32 D;
};
