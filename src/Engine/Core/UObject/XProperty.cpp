#include "XProperty.h"

#include "UObject.h"
#include "UClass.h"
#include "Engine/GameFramework/AGameStateBase.h"
#include "Games/BaseGame.h"
#include "Math/FVector.h"

void XPropertyClass::Init_PostEngine()
{
	Name_Offset = Game->bUsesFProperty ? 0 : offsetof(UObject, Name);
}

void XProperty::Init_PostEngine()
{
	Class_Offset = Game->bUsesFProperty ? 8 : offsetof(UObject, Class);

	Name_Offset = Memory::FindOffsetByValue(FVector::StaticStruct()->ChildProperties(), FName("X"));
	Next_Offset = Memory::FindOffsetByValue(FVector::StaticStruct()->ChildProperties(), FName("Y"), Name_Offset);

	XProperty* PlayerArray_Prop = AGameStateBase::StaticClass()->FindProperty("PlayerArray");

	ArrayDim_Offset = Memory::FindOffsetByValue(PlayerArray_Prop, 1);
	ElementSize_Offset = Memory::FindOffsetByValue(PlayerArray_Prop, 0x10);

	Offset_Offset = Memory::FindOffsetByValue(AActor::StaticClass()->ChildProperties(), sizeof(UObject));
	Inner_Offset = Memory::FindOffsetByValue(PlayerArray_Prop, "PlayerArray", Name_Offset);
}
