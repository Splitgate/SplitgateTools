#pragma once

#include "FMemory.h"
#include "CoreTypes.h"

template <int32_t NumElements>
class TInlineAllocator
{
public:
	template <int32_t Size, int32_t Alignment>
	struct alignas(Alignment) TAlligendBytes
	{
		uint8_t Pad[Size];
	};

	template <typename ElementType>
	struct TTypeCompatibleBytes : public TAlligendBytes<sizeof(ElementType), alignof(ElementType)>
	{
	};

public:
	template <typename ElementType>
	class ForElementType
	{
		friend class TBitArray;

	public:
		TTypeCompatibleBytes<ElementType> InlineData[NumElements];

		ElementType* SecondaryData;

	public:

		FORCEINLINE int32_t NumInlineBytes() const
		{
			return sizeof(ElementType) * NumElements;
		}
		FORCEINLINE int32_t NumInlineBits() const
		{
			return NumInlineBytes() * 8;
		}

		FORCEINLINE ElementType& operator[](int32_t Index)
		{
			return *(ElementType*)(&InlineData[Index]);
		}
		FORCEINLINE const ElementType& operator[](int32_t Index) const
		{
			return *(ElementType*)(&InlineData[Index]);
		}

		FORCEINLINE void operator=(void* InElements)
		{
			SecondaryData = reinterpret_cast<ElementType*>(InElements);
		}

		FORCEINLINE ElementType& GetInlineElement(int32_t Index)
		{
			return *(ElementType*)(&InlineData[Index]);
		}
		FORCEINLINE const ElementType& GetInlineElement(int32_t Index) const
		{
			return *(ElementType*)(&InlineData[Index]);
		}
		FORCEINLINE ElementType& GetSecondaryElement(int32_t Index)
		{
			return SecondaryData[Index];
		}
		FORCEINLINE const ElementType& GetSecondaryElement(int32_t Index) const
		{
			return SecondaryData[Index];
		}
	};
};