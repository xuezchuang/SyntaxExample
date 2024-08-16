#pragma once
#include "DelegateInstanceInterface.h"

template <typename FuncType, typename UserPolicy, typename... VarTypes>
class TCommonDelegateInstanceState : IBaseDelegateInstance<FuncType, UserPolicy>
{
public:
	template <typename... InVarTypes>
	explicit TCommonDelegateInstanceState(InVarTypes&&... Vars)
		//: Payload(Forward<InVarTypes>(Vars)...)
		//, Handle (FDelegateHandle::GenerateNewHandle)
	{
		int b = 0;
	}

	//FDelegateHandle GetHandle() const final
	//{
	//	return Handle;
	//}

protected:
	//// Payload member variables (if any).
	//TTuple<VarTypes...> Payload;

	//// The handle of this delegate
	//FDelegateHandle Handle;
};

/**
 * Implements a delegate binding for C++ functors, e.g. lambdas.
 */
template <typename FuncType, typename UserPolicy, typename FunctorType, typename... VarTypes>
class TBaseFunctorDelegateInstance;


class FTest
{
public:
	FTest()
	{
		int a = 0;
	}
	FTest(FTest&& other) noexcept 
	{
		a = other.a;
	}

	// 需要显式定义拷贝构造函数
	FTest(const FTest& other) 
	{
		a = other.a;
	}

	// 需要显式定义拷贝赋值运算符
	FTest& operator=(const FTest& other)
	{
		if (this != &other)
		{
			a = other.a;
		}
		return *this;
	}

	// 移动赋值运算符
	FTest& operator=(FTest&& other) noexcept 
	{
		if (this != &other)
		{
			a = other.a;
		}
		return *this;
	}
	int a = 0;
};

template <typename RetValType, typename... ParamTypes, typename UserPolicy, typename FunctorType, typename... VarTypes>
class TBaseFunctorDelegateInstance<RetValType(ParamTypes...), UserPolicy, FunctorType, VarTypes...> : public TCommonDelegateInstanceState<RetValType(ParamTypes...), UserPolicy, VarTypes...>
{
private:
	static_assert(std::is_same_v<FunctorType, typename TRemoveReference<FunctorType>::Type>, "FunctorType cannot be a reference");

	using Super            = TCommonDelegateInstanceState<RetValType(ParamTypes...), UserPolicy, VarTypes...>;
	using DelegateBaseType = typename UserPolicy::FDelegateExtras;

public:
	template <typename InFunctorType, typename... InVarTypes>
	explicit TBaseFunctorDelegateInstance(InFunctorType&& InFunctor, InVarTypes&&... Vars)
		: Super  (Forward<InVarTypes>(Vars)...)
		, Functor(Forward<InFunctorType>(InFunctor))
	{
		int a = 0;
	}

	const void* GetObjectForTimerManager() const final
	{
		return nullptr;
	}

	int GetBoundProgramCounterForTimerManager() const final
	{
		return 0;
	}

	// Deprecated
	bool HasSameObject(const void* UserObject) const final
	{
		// Functor Delegates aren't bound to a user object so they can never match
		return false;
	}

	bool IsSafeToExecute() const final
	{
		// Functors are always considered safe to execute!
		return true;
	}

public:
	// IBaseDelegateInstance interface
	void CreateCopy(DelegateBaseType& Base) const final
	{
		new (Base) TBaseFunctorDelegateInstance(*this);
	}

	RetValType Execute(ParamTypes... Params) const final
	{
		return Functor(Params...);// this->Payload.ApplyAfter(Functor, Params...);
	}

	bool ExecuteIfSafe(ParamTypes... Params) const final
	{
		// Functors are always considered safe to execute!
		//(void)this->Payload.ApplyAfter(Functor, Params...);

		return true;
	}

	template< class T >
	struct TRemoveConst
	{
		typedef T Type;
	};
	template< class T >
	struct TRemoveConst<const T>
	{
		typedef T Type;
	};
	//Struct_Copy jjj;
	FTest m_a;
private:
	// C++ functor
	// We make this mutable to allow mutable lambdas to be bound and executed.  We don't really want to
	// model the Functor as being a direct subobject of the delegate (which would maintain transivity of
	// const - because the binding doesn't affect the substitutability of a copied delegate.
	mutable typename TRemoveConst<FunctorType>::Type Functor;
};
