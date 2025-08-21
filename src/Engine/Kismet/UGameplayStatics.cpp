#include "UGameplayStatics.h"
#include "Engine/Core/UObject/UClass.h"

UObject* UGameplayStatics::SpawnObject(UClass* ObjectClass, UObject* Outer)
{
	struct {
		UClass* ObjectClass;
		UObject* Outer;
		UObject* ReturnValue;
	} p = {
		ObjectClass, Outer
	};
	
	UFunction* Func = StaticClass()->FindFunction(L"SpawnObject");
	DefaultObject()->ProcessEvent(Func, &p);

	return p.ReturnValue;
}
