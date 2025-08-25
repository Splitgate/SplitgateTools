#pragma once
#include <Windows.h>
#include "UObject/ObjectMacros.h"
#include "Containers/FString.h"

class FNetworkVersion
{
public:
	static void Init_PreGame();

	static std::string GetProjectVersion();
};