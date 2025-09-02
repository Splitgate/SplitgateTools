#include "FParse.h"

#include "Memory/Memory.h"
#include "Globals.h"

static bool (*Value_FString)(const TCHAR*, const TCHAR*, FString&, bool);

void FParse::Init_PreEngine()
{
	Value_FString = Memory::FindStringRef(L"GameClassName=").Scan("E8").Add(1).Rel32();
	LOG_ADDRESS(Value_FString, "FParse::Value (FString)");
}

bool FParse::Value(const TCHAR* Stream, const TCHAR* Match, FString& Value, bool bShouldStopOnSeparator)
{
	if (Value_FString)
		return Value_FString(Stream, Match, Value, bShouldStopOnSeparator);

	return false;
}
