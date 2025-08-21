#include "UFunction.h"

#include "UClass.h"
#include "Engine/GameFramework/APlayerController.h"
#include "Memory/Hook.h"

void UFunction::Init_PostEngine()
{
	//MessageBoxA(0,"before nativefunc finder",0,0);
	//APlayerController::StaticClass();
	//MessageBoxA(0,"after pc staticclass call",0,0);
	//APlayerController::StaticClass()->FindFunction("ServerAcknowledgePossession");
	//MessageBoxA(0,"after findfunc call",0,0);
	//Memory::FindStringRef(L"ServerAcknowledgePossession_Validate").FuncStart();
	//MessageBoxA(0,"after memory tickle",0,0);
	
	NativeFunc_Offset = Memory::FindOffsetByValue(
		APlayerController::StaticClass()->FindFunction("ServerAcknowledgePossession"),
		Memory::FindStringRef(L"ServerAcknowledgePossession_Validate").FuncStart()
	);
	LOG_OFFSET(NativeFunc_Offset, "UFunction::NativeFunc");
}
