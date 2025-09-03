#include "APortalWarsGameMode.h"

void APortalWarsGameMode::Init_PreEngine()
{
    Memory::Address PortalWarsRaceGameModeCtr = Memory::FindStringRef("RACE");
    if (PortalWarsRaceGameModeCtr)
    {
        Memory::Address CountdownTimeUsage = PortalWarsRaceGameModeCtr.Scan("C7 83").Add(2);
        if (CountdownTimeUsage)
        {
            ModeOffsets::CountdownTime = CountdownTimeUsage.Deref();
        }
    }
}

int& APortalWarsGameMode::CountdownTime()
{
    return Get<int>(ModeOffsets::CountdownTime);
}