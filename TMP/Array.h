// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once



/**
 * Traits class which gets the unsigned version of an integer type.
 */
using uint8 = uint8_t;
using uint16 = uint16_t;
using uint32 = uint32_t;
using uint64 = uint64_t;

using int8 = int8_t;
using int16 = int16_t;
using int32 = int32_t;
using int64 = int64_t;
#include "ChooseClass.h"
#include "ContainerAllocationPolicies.h"

template <typename T>
struct TMakeUnsigned
{
	static_assert(sizeof(T) == 0, "Unsupported type in TMakeUnsigned<T>.");
};

template <typename T> struct TMakeUnsigned<const          T> { using Type = const          typename TMakeUnsigned<T>::Type; };
template <typename T> struct TMakeUnsigned<      volatile T> { using Type = volatile typename TMakeUnsigned<T>::Type; };
template <typename T> struct TMakeUnsigned<const volatile T> { using Type = const volatile typename TMakeUnsigned<T>::Type; };

template <> struct TMakeUnsigned<int8  > { using Type = uint8; };
template <> struct TMakeUnsigned<uint8 > { using Type = uint8; };
template <> struct TMakeUnsigned<int16 > { using Type = uint16; };
template <> struct TMakeUnsigned<uint16> { using Type = uint16; };
template <> struct TMakeUnsigned<int32 > { using Type = uint32; };
template <> struct TMakeUnsigned<uint32> { using Type = uint32; };
template <> struct TMakeUnsigned<int64 > { using Type = uint64; };
template <> struct TMakeUnsigned<uint64> { using Type = uint64; };

/**
 * Templated dynamic array
 *
 * A dynamically sized array of typed elements.  Makes the assumption that your elements are relocate-able; 
 * i.e. that they can be transparently moved to new memory without a copy constructor.  The main implication 
 * is that pointers to elements in the TArray may be invalidated by adding or removing other elements to the array. 
 * Removal of elements is O(N) and invalidates the indices of subsequent elements.
 *
 * Caution: as noted below some methods are not safe for element types that require constructors.
 *
 **/
template<typename InElementType, typename InAllocatorType>
class TArray
{
	template <typename OtherInElementType, typename OtherAllocator>
	friend class TArray;

public:
	typedef typename InAllocatorType::SizeType SizeType;
	typedef InElementType ElementType;
	typedef InAllocatorType AllocatorType;

	typedef typename TChooseClass<
		AllocatorType::NeedsElementType,
		typename AllocatorType::template ForElementType<ElementType>,
		typename AllocatorType::ForAnyElementType
	>::Result ElementAllocatorType;

private:
	using USizeType = typename TMakeUnsigned<SizeType>::Type;

	static void OnInvalidNum(USizeType NewNum)
	{
		//const TCHAR* ArrayNameSuffix = TEXT("");
		//if constexpr (sizeof(SizeType) == 8)
		//{
		//	ArrayNameSuffix = TEXT("64");
		//}
	}
protected:

	//template<typename ElementType, typename AllocatorType>
	//friend class TIndirectArray;

	ElementAllocatorType AllocatorInstance;
	SizeType             ArrayNum;
	SizeType             ArrayMax;

public:
	/**
	 * Reserves memory such that the array can contain at least Number elements.
	 *
	 * @param Number The number of elements that the array should be able to contain after allocation.
	 * @see Shrink
	 */
	__forceinline void Reserve(SizeType Number)
	{
		if (Number < 0)
		{
			OnInvalidNum((USizeType)Number);
		}
		else if (Number > ArrayMax)
		{
			ResizeTo(Number);
		}
	}
private:
	__forceinline void ResizeTo(SizeType NewMax)
	{
		if (NewMax)
		{
			NewMax = AllocatorCalculateSlackReserve(NewMax);
		}
		if (NewMax != ArrayMax)
		{
			ArrayMax = NewMax;
			AllocatorResizeAllocation(ArrayNum, ArrayMax);
		}
	}

	void AllocatorResizeAllocation(SizeType CurrentArrayNum, SizeType NewArrayMax)
	{
		if constexpr (TAllocatorTraits<AllocatorType>::SupportsElementAlignment)
		{
			AllocatorInstance.ResizeAllocation(CurrentArrayNum, NewArrayMax, sizeof(ElementType), alignof(ElementType));
		}
		else
		{
			AllocatorInstance.ResizeAllocation(CurrentArrayNum, NewArrayMax, sizeof(ElementType));
		}
	}

	SizeType AllocatorCalculateSlackReserve(SizeType NewArrayMax)
	{
		if constexpr (TAllocatorTraits<AllocatorType>::SupportsElementAlignment)
		{
			return AllocatorInstance.CalculateSlackReserve(NewArrayMax, sizeof(ElementType), alignof(ElementType));
		}
		else
		{
			return AllocatorInstance.CalculateSlackReserve(NewArrayMax, sizeof(ElementType));
		}
		return 0;
	}
};



