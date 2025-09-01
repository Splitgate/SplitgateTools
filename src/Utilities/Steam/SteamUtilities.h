#pragma once
#include <Windows.h>

// Steam files have weird characters
#pragma warning( disable : 4828 )

#include "isteamuser.h"
#include "isteamutils.h"

class Steam
{
	static HMODULE GetSteamAPI()
	{
		return GetModuleHandle(L"steam_api64.dll");
	}

public:

	static inline bool IsReady() { return (GetSteamAPI() != NULL); }

	static ISteamUser* User();
	static ISteamUtils* Utils();
};