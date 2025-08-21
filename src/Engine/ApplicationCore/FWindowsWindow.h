#pragma once
#include <Windows.h>
#include "UObject/ObjectMacros.h"

class FWindowsWindow
{
public:
	static void Init_PreEngine();
	
	void Initialize(__int64 Application, __int64 InDefinition, HINSTANCE InHInstance, __int64 InParent);

	PROPERTY(HWND, HWnd);
};
