#pragma once
#include "UObject/FName.h"

// Patch for fixing low fps when not launching through Steam
class FOnlineFactoryEOSPlus
{
public:
	static void Init_PreEngine();
	
	void** CreateSubsystem(void** ReturnValue, FName InstanceName);
};
