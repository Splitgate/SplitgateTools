#include "FWindowsDynamicRHI.h"
#include "Memory/Hook.h"

void FWindowsDynamicRHI::Init_PreEngine()
{
#if !D3D12_ALLOWED
	// Temporary for forcing directx 11 to always get chosen as if you did -d3d11 as imgui will not work currently on d3d12
	Memory::Address DX11ForceJMP = Memory::FindStringRef(L"vulkan").Scan("75", 1);
	LOG_ADDRESS(DX11ForceJMP, "DX11ForceJMP");
	if (DX11ForceJMP)
	{
		DX11ForceJMP.Write(JMP8);
	}
#endif
}
