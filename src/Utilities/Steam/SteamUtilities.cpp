#include "SteamUtilities.h"
#include "steam_api_flat.h"

#define NAMEOF(Type) #Type

ISteamUser* Steam::User()
{
    if (IsReady())
    {
        return ((ISteamUser* (__cdecl*)(void))GetProcAddress(GetSteamAPI(), NAMEOF(SteamAPI_SteamUser_v021)))();
    }

    return nullptr;
}

ISteamUtils* Steam::Utils()
{
    if (IsReady())
    {
        return ((ISteamUtils* (__cdecl*)(void))GetProcAddress(GetSteamAPI(), NAMEOF(SteamAPI_SteamUtils_v010)))();
    }

    return nullptr;
}
