#include "SteamUtilities.h"
#include "steam_api_flat.h"

#define NAMEOF(Type) #Type

ISteamUser* Steam::SteamUser()
{
    if (IsSteamReady())
    {
        return ((ISteamUser* (__cdecl*)(void))GetProcAddress(GetSteamAPI(), NAMEOF(SteamAPI_SteamUser_v021)))();
    }

    return nullptr;
}

ISteamUtils* Steam::SteamUtils()
{
    if (IsSteamReady())
    {
        return ((ISteamUtils* (__cdecl*)(void))GetProcAddress(GetSteamAPI(), NAMEOF(SteamAPI_SteamUtils_v010)))();
    }

    return nullptr;
}
