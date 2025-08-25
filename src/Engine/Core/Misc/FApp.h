#pragma once
#include <string>
#include "Containers/FString.h"

enum class EWindowsRHI
{
	D3D11,
	D3D12,
	Unsupported
};

class FApp
{
public:
	static void Init_PreGame();

	static inline EWindowsRHI RHI = EWindowsRHI::Unsupported;
	static inline std::string RendererName;
	
	static void SetGraphicsRHI(FString& RHIString);
	static std::string GetBuildVersion();
};