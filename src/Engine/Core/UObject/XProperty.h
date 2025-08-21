#pragma once
#include "FName.h"
#include "ObjectMacros.h"

// Wrapper classes that support FProperty and UProperty simultaneously

class XPropertyClass
{
public:
	static void Init_PostEngine();
	
	PROPERTY(FName, Name);
};

class XProperty
{
public:
	static void Init_PostEngine();
	
	PROPERTY(XPropertyClass*, Class);
	PROPERTY(XProperty*, Next);
	PROPERTY(FName, Name);

	PROPERTY(int, ArrayDim);
	PROPERTY(int, ElementSize);
	PROPERTY(int, Offset);

	PROPERTY(void*, Inner);
};


struct XBoolProperty
{
	unsigned char FieldSize;
	unsigned char ByteOffset;
	unsigned char ByteMask;
	unsigned char FieldMask;
};
struct XMapProperty
{
	XProperty* KeyProperty;
	XProperty* ValueProperty;
};
struct XEnumProperty
{
	XProperty* UnderlyingProp;
	class UEnum* Enum;
};