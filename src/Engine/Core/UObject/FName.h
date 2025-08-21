#pragma once

#include "Containers/FString.h"

enum EFindName
{
	FNAME_Find,
	FNAME_Add,
	FNAME_Replace_Not_Safe_For_Threading
};

class FName
{
public:
	static void Init_PreEngine();
	
	FString ToString() const;
	void ToString(FString& Out) const;
	
	bool operator==(FName Other) const {
		return Index == Other.Index;
	}
	bool operator!=(FName Other) const {
		return Index != Other.Index;
	}
	bool IsNone() const {
		return Index == 0;
	}

	FName() : Index(0), Number(0) {}
	FName(const wchar_t* Name, EFindName FindType=FNAME_Add);
	FName(const char* Name, EFindName FindType=FNAME_Add);
	
	unsigned int Index;
	unsigned int Number;
};
