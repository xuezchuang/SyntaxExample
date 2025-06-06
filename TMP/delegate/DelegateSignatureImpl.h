// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "DelegateBase.h"
#include "DelegateInstancesImpl.h"

template <typename DelegateSignature, typename UserPolicy = FDefaultDelegateUserPolicy>
class TDelegate
{
	static_assert("Expected a function signature for the delegate template parameter");
};



template <typename T>
using TIdentity_T = typename TIdentity<T>::Type;
typedef decltype(nullptr)		TYPE_OF_NULLPTR;

template <typename InRetValType, typename... ParamTypes, typename UserPolicy>
class TDelegate<InRetValType(ParamTypes...), UserPolicy> : public TDelegateBase<UserPolicy>
{
	using FuncType = InRetValType(ParamTypes...);
	using DelegateInstanceInterfaceType = IBaseDelegateInstance<FuncType, UserPolicy>;
	typedef InRetValType RetValType;
public:

	/**
	* Static: Creates a C++ lambda delegate
	* technically this works for any functor types, but lambdas are the primary use case
	*/
	template<typename FunctorType, typename... VarTypes>
	inline static TDelegate<RetValType(ParamTypes...), UserPolicy> CreateLambda(FunctorType&& InFunctor, VarTypes&&... Vars)
	{
		TDelegate<RetValType(ParamTypes...), UserPolicy> Result;
		new (Result) TBaseFunctorDelegateInstance<FuncType, UserPolicy, typename TRemoveReference<FunctorType>::Type, std::decay_t<VarTypes>...>(Forward<FunctorType>(InFunctor), Forward<VarTypes>(Vars)...);
		return Result;
	}

	/**
	* Static: Creates a raw C++ pointer global function delegate
	*/
	template <typename... VarTypes>
	inline static TDelegate<RetValType(ParamTypes...), UserPolicy> CreateStatic(typename TIdentity<RetValType (*)(ParamTypes..., std::decay_t<VarTypes>...)>::Type InFunc, VarTypes&&... Vars)
	{
		TDelegate<RetValType(ParamTypes...), UserPolicy> Result;
		new (Result) TBaseStaticDelegateInstance<FuncType, UserPolicy, std::decay_t<VarTypes>...>(InFunc, Forward<VarTypes>(Vars)...);
		return Result;
	}

	/**
	* Static: Binds a C++ lambda delegate
	* technically this works for any functor types, but lambdas are the primary use case
	*/
	template<typename FunctorType, typename... VarTypes>
	inline void BindLambda(FunctorType&& InFunctor, VarTypes&&... Vars)
	{
		new (*this)TBaseFunctorDelegateInstance<FuncType, UserPolicy, typename TRemoveReference<FunctorType>::Type, std::decay_t<VarTypes>...>(Forward<FunctorType>(InFunctor), Forward<VarTypes>(Vars)...);
	}
	using Super = TDelegateBase<UserPolicy>;
	using Super::GetDelegateInstanceProtected;
	/**
	 * Execute the delegate.
	 *
	 * If the function pointer is not valid, an error will occur. Check IsBound() before
	 * calling this method or use ExecuteIfBound() instead.
	 *
	 * @see ExecuteIfBound
	 */
	RetValType Execute(ParamTypes... Params) const
	{
		DelegateInstanceInterfaceType* LocalDelegateInstance = GetDelegateInstanceProtected();
		return LocalDelegateInstance->Execute(Params...);
	}

	/**
	 * Returns a pointer to the correctly-typed delegate instance.
	 */
	DelegateInstanceInterfaceType* GetDelegateInstanceProtected() const
	{
		return (DelegateInstanceInterfaceType*)Super::GetDelegateInstanceProtected();
	}

	/**
	* Default constructor
	*/
	inline TDelegate()
	{
	}

	/**
	* 'Null' constructor
	*/
	inline TDelegate(TYPE_OF_NULLPTR)
	{
	}

	TDelegate& operator=(const TDelegate& Other)
	{
		if (&Other != this)
		{
			// this down-cast is OK! allows for managing invocation list in the base class without requiring virtual functions
			DelegateInstanceInterfaceType* OtherInstance = Other.GetDelegateInstanceProtected();

			if (OtherInstance != nullptr)
			{
				OtherInstance->CreateCopy(*this);
			}
			else
			{
				//Unbind();
			}
		}

		return *this;
	}

	inline TDelegate(const TDelegate& Other)
	{
		*this = Other;
	}
};