#include "FName.h"
#include "Memory/Hook.h"

static void (*ToString)(const FName*, FString&);
static void (*FromWideString)(FName*, const wchar_t*, EFindName);
static void (*FromAnsiString)(FName*, const char*, EFindName);

void FName::Init_PreEngine()
{
	::FromAnsiString = Memory::FindStringRef("RuntimePhysXCooking").Scan(CALL).Add(1).Rel32();
	LOG_ADDRESS(::FromAnsiString, "FName::FName (ansi)");

	for (Memory::Address s : Memory::FindStringRefs(L"TargetViewRotation"))
	{
		if (s.Add(9).Deref<uint8_t>() == 0x4C)
		{
			::FromWideString = s.Scan(CALL).Add(1).Rel32();
			LOG_ADDRESS(::FromWideString, "FName::FName (wide)");
			
			::ToString = s.Scan(CALL, 1).Add(1).Rel32();
			LOG_ADDRESS(::ToString, "FName::ToString");

			break;
		}
	}
}

FString FName::ToString() const
{
	FString Out;
	ToString(Out);
	return Out;
}

void FName::ToString(FString& Out) const
{
	return ::ToString(this, Out);
}

FName::FName(const wchar_t* Name, EFindName FindType)
{
	::FromWideString(this, Name, FindType);
}

FName::FName(const char* Name, EFindName FindType)
{
	::FromAnsiString(this, Name, FindType);
}