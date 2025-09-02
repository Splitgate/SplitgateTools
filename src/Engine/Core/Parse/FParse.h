#pragma once

#include "Engine/Core/Containers/FString.h"

struct FParse
{
	friend class BaseGame;

private:

	static void Init_PreEngine();

public:

	/** Parses a string. */
	static bool Value(const TCHAR* Stream, const TCHAR* Match, FString& Value, bool bShouldStopOnSeparator = true);
};