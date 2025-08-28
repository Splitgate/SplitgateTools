#include "Splitgate.h"

// Title Bar Entries
#include "UI/World/MapsMenuEntry.h"

Splitgate::Splitgate()
{
	bShouldRenderUI = true;
}

void Splitgate::Init_PreEngine()
{
	BaseGame::Init_PreEngine();	
}

void Splitgate::Init_PostEngine()
{
	BaseGame::Init_PostEngine();
}

void Splitgate::ConstructTitleBar()
{
	new MapsMenuEntry();
}