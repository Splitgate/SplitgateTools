#pragma once
#include <Windows.h>

class UObject;
class UStruct;
class UClass;
class UFunction;

// Example Usage: GENERATED_BODY(AGameStateBase, AActor, "GameStateBase", "/Script/Engine")
#define GENERATED_BODY(Clss, OuterClss, ClassName, Module) private: typedef Clss ThisClass; typedef OuterClss Super; public: static const wchar_t* StaticPackage() { return TEXT(Module##"."##ClassName); }; static inline UClass* StaticClass() { return StaticLoadObject<UClass>(StaticPackage()); }; static Clss* DefaultObject() { return (Clss*)StaticLoadObject<UObject>(TEXT(Module##".Default__"##ClassName)); }
#define GENERATED_USTRUCT_BODY(StructName, Module) static const wchar_t* StaticPackage() { return TEXT(Module##"."##StructName); }; static inline UStruct* StaticStruct() { return StaticLoadObject<UStruct>(StaticPackage()); }

#define UNPACK(...) __VA_ARGS__

#define PROPERTY(Type, Name) \
static inline int Name##_Offset; \
template<typename T = Type> \
inline T& Name() { return *reinterpret_cast<T*>(uintptr_t(this) + Name##_Offset); }

#define UPROPERTY(Type, Name) \
template<typename T = Type> \
inline T& Name() { \
	static int Offset = Class->FindOffset(#Name); \
	return Get<T>(Offset); \
} 

enum EObjectFlags
{
	// Do not add new flags unless they truly belong here. There are alternatives.
	// if you change any the bit of any of the RF_Load flags, then you will need legacy serialization
	RF_NoFlags						= 0x00000000,	///< No flags, used to avoid a cast

	// This first group of flags mostly has to do with what kind of object it is. Other than transient, these are the persistent object flags.
	// The garbage collector also tends to look at these.
	RF_Public					=0x00000001,	///< Object is visible outside its package.
	RF_Standalone				=0x00000002,	///< Keep object around for editing even if unreferenced.
	RF_MarkAsNative					=0x00000004,	///< Object (UField) will be marked as native on construction (DO NOT USE THIS FLAG in HasAnyFlags() etc)
	RF_Transactional			=0x00000008,	///< Object is transactional.
	RF_ClassDefaultObject		=0x00000010,	///< This object is its class's default object
	RF_ArchetypeObject			=0x00000020,	///< This object is a template for another object - treat like a class default object
	RF_Transient				=0x00000040,	///< Don't save object.

	// This group of flags is primarily concerned with garbage collection.
	RF_MarkAsRootSet					=0x00000080,	///< Object will be marked as root set on construction and not be garbage collected, even if unreferenced (DO NOT USE THIS FLAG in HasAnyFlags() etc)
	RF_TagGarbageTemp			=0x00000100,	///< This is a temp user flag for various utilities that need to use the garbage collector. The garbage collector itself does not interpret it.

	// The group of flags tracks the stages of the lifetime of a uobject
	RF_NeedInitialization		=0x00000200,	///< This object has not completed its initialization process. Cleared when ~FObjectInitializer completes
	RF_NeedLoad					=0x00000400,	///< During load, indicates object needs loading.
	RF_KeepForCooker			=0x00000800,	///< Keep this object during garbage collection because it's still being used by the cooker
	RF_NeedPostLoad				=0x00001000,	///< Object needs to be postloaded.
	RF_NeedPostLoadSubobjects	=0x00002000,	///< During load, indicates that the object still needs to instance subobjects and fixup serialized component references
	RF_NewerVersionExists		=0x00004000,	///< Object has been consigned to oblivion due to its owner package being reloaded, and a newer version currently exists
	RF_BeginDestroyed			=0x00008000,	///< BeginDestroy has been called on the object.
	RF_FinishDestroyed			=0x00010000,	///< FinishDestroy has been called on the object.

	// Misc. Flags
	RF_BeingRegenerated			=0x00020000,	///< Flagged on UObjects that are used to create UClasses (e.g. Blueprints) while they are regenerating their UClass on load (See FLinkerLoad::CreateExport()), as well as UClass objects in the midst of being created
	RF_DefaultSubObject			=0x00040000,	///< Flagged on subobjects that are defaults
	RF_WasLoaded				=0x00080000,	///< Flagged on UObjects that were loaded
	RF_TextExportTransient		=0x00100000,	///< Do not export object to text form (e.g. copy/paste). Generally used for sub-objects that can be regenerated from data in their parent object.
	RF_LoadCompleted			=0x00200000,	///< Object has been completely serialized by linkerload at least once. DO NOT USE THIS FLAG, It should be replaced with RF_WasLoaded.
	RF_InheritableComponentTemplate = 0x00400000, ///< Archetype of the object can be in its super class
	RF_DuplicateTransient		=0x00800000,	///< Object should not be included in any type of duplication (copy/paste, binary duplication, etc.)
	RF_StrongRefOnFrame			=0x01000000,	///< References to this object from persistent function frame are handled as strong ones.
	RF_NonPIEDuplicateTransient	=0x02000000,	///< Object should not be included for duplication unless it's being duplicated for a PIE session
	RF_Dynamic					=0x04000000,	///< Field Only. Dynamic field - doesn't get constructed during static initialization, can be constructed multiple times
	RF_WillBeLoaded				=0x08000000,	///< This object was constructed during load and will be loaded shortly
	RF_HasExternalPackage		=0x10000000,	///< This object has an external package assigned and should look it up when getting the outermost package
};