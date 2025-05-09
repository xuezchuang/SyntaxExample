#pragma once
#include "GenericPlatform.h"
template <typename T> struct TRemoveReference { typedef T Type; };
template <typename T> struct TRemoveReference<T& > { typedef T Type; };
template <typename T> struct TRemoveReference<T&&> { typedef T Type; };

/**
* Forward will cast a reference to an rvalue reference.
* This is UE's equivalent of std::forward.
*/
template <typename T>
FORCEINLINE T&& Forward(typename TRemoveReference<T>::Type& Obj)
{
	return (T&&)Obj;
}

template <typename T>
FORCEINLINE T&& Forward(typename TRemoveReference<T>::Type&& Obj)
{
	return (T&&)Obj;
}

/**
* Used to declare an untyped array of data with compile-time alignment.
* It needs to use template specialization as the MS_ALIGN and GCC_ALIGN macros require literal parameters.
*/
template<int32 Size, uint32 Alignment>
struct TAlignedBytes
{
	alignas(Alignment) uint8 Pad[Size];
};