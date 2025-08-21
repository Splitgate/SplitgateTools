#pragma once
#include "UObject/UObject.h"
#include "Games/BaseGame.h"

#define GWorld (*Game->World) // todo: move to UWorld.h when it's been made

class FNetworkNotify {};
class UWorld : public UObject, public FNetworkNotify
{
public:
	static void Init_PreEngine();
};
