#pragma once

template <typename T> struct TRemoveReference { typedef T Type; };
template <typename T> struct TRemoveReference<T& > { typedef T Type; };
template <typename T> struct TRemoveReference<T&&> { typedef T Type; };

template <typename T>
typename TRemoveReference<T>::Type&& MoveTemp(T&& Obj)
{
	typedef typename TRemoveReference<T>::Type CastType;

	return (CastType&&)Obj;
}