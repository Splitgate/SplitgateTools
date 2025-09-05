#include "UPortalWarsGameEngine.h"

static Memory::Address GetMaxTickRatePatch;
void UPortalWarsGameEngine::Init_PostEngine()
{
	Memory::Address GetMaxTickRatePtr = Memory::Address(DefaultObject()).Deref()
		.Add(VTableOffsets::GetMaxTickRate);
	if (GetMaxTickRatePtr)
	{
		GetMaxTickRatePatch = GetMaxTickRatePtr.Deref().Scan("48 85 ?? 74").Add(3);
		LOG_ADDRESS(GetMaxTickRatePatch, "UPortalWarsGameEngine::GetMaxTickRate patch address");
	}

//	if (check config for patch)
	{
		ApplyLobbyFPSPatch();
	}
}

void UPortalWarsGameEngine::ApplyLobbyFPSPatch()
{
	if (GetMaxTickRatePatch)
		GetMaxTickRatePatch.Write(0xEB);
}

void UPortalWarsGameEngine::RevertLobbyFPSPatch()
{
	if (GetMaxTickRatePatch)
		GetMaxTickRatePatch.Write(0x74);
}
