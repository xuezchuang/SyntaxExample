// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once
#include "DelegateBase.h"
#include "DelegateInstancesImpl.h"

template <typename DelegateSignature, typename UserPolicy = FDefaultDelegateUserPolicy>
class TDelegate
{
	static_assert("Expected a function signature for the delegate template parameter");
};

template <typename InRetValType, typename... ParamTypes, typename UserPolicy>
class TDelegate<InRetValType(ParamTypes...), UserPolicy> : public TDelegateBase<UserPolicy>
{
	using FuncType = InRetValType(ParamTypes...);
	using DelegateInstanceInterfaceType = IBaseDelegateInstance<FuncType, UserPolicy>;
	typedef InRetValType RetValType;
public:
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
};