#include "UStruct.h"

#include "Engine/UGameEngine.h"
#include "Math/FVector.h"
#include "Misc/FGuid.h"

void UStruct::Init_PostEngine()
{
	Super_Offset = Memory::FindOffsetByValue(GEngine->Class, FName("Engine"), offsetof(UObject, Name));
	if (Super_Offset == -1) // for games that override UGameEngine
		Super_Offset = Memory::FindOffsetByValue(GEngine->Class, FName("GameEngine"), offsetof(UObject, Name));
	LOG_OFFSET(Super_Offset, "UStruct::Super");

	Children_Offset = Super_Offset + 8;
	ChildProperties_Offset = Children_Offset;
	LOG_OFFSET(Children_Offset, "UStruct::Children");
	
	// If FVector has no children, then this game uses FProperties 
	Game->bUsesFProperty = !FVector::StaticStruct()->Children();
	UE_LOG(LogInit, Display, "Game uses {}Properties", Game->bUsesFProperty ? 'F' : 'U')
	
	// FProperties are in a separate linked list (Children+8)
	if (Game->bUsesFProperty)
		ChildProperties_Offset += 8;
	LOG_OFFSET(ChildProperties_Offset, "UStruct::ChildProperties");

	PropertiesSize_Offset = Memory::FindOffsetByValue(FGuid::StaticStruct(), sizeof(FGuid));
	LOG_OFFSET(PropertiesSize_Offset, "UStruct::PropertiesSize");
}
