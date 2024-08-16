#include <iostream>
#include <memory>
using namespace std;

template <typename T> struct TRemoveReference { typedef T Type; };
template <typename T> struct TRemoveReference<T& > { typedef T Type; };
template <typename T> struct TRemoveReference<T&&> { typedef T Type; };

template <typename T> struct TRemovePointer { typedef T Type; };
template <typename T> struct TRemovePointer<T*> { typedef T Type; };

/**
 * Forward will cast a reference to an rvalue reference.
 * This is UE's equivalent of std::forward.
 */
template <typename T>
T&& Forward(typename TRemoveReference<T>::Type& Obj)
{
	return (T&&)Obj;
}

template <typename T>
T&& Forward(typename TRemoveReference<T>::Type&& Obj)
{
	return (T&&)Obj;
}

template <typename FuncType, typename... ArgTypes>
auto Invoke(FuncType&& Func, ArgTypes&&... Args) //-> decltype(Forward<FuncType>(Func)(Forward<ArgTypes>(Args)...))
{
	return Forward<FuncType>(Func)(Forward<ArgTypes>(Args)...);
}


template <typename Functor, typename FuncType>
struct TFunctionRefCaller;

template <typename Functor, typename Ret, typename... ParamTypes>
struct TFunctionRefCaller<Functor, Ret(ParamTypes...)>
{
	static Ret Call(void* Obj, ParamTypes&... Params)
	{
		return Invoke(*(Functor*)Obj, Forward<ParamTypes>(Params)...);
	}
};

template <typename Functor, typename... ParamTypes>
struct TFunctionRefCaller<Functor, void(ParamTypes...)>
{
	static void Call(void* Obj, ParamTypes&... Params)
	{
		Invoke(*(Functor*)Obj, Forward<ParamTypes>(Params)...);
	}
};

#include "DelegateSignatureImpl.h"




int main()
{
	typedef TDelegate<void(int,int,int)> DelegateTest;

	int tol = 0;
	DelegateTest Delegate_b;
	{
		DelegateTest Delegate;
		
		Delegate.BindLambda([&](int a, int b, int c)
			{
				tol = a + b + c;
			});
		//Delegate.Execute(3, 4, 5);

		Delegate_b = Delegate;
	}

	Delegate_b.Execute(3, 4, 5);

	//typedef TDelegate<void()> DelegateNoParam;
	//DelegateNoParam DelegateInstanceNoparam;
	//DelegateInstanceNoparam.BindLambda([&]
	//	{
	//		tol++;
	//	});
	//DelegateInstanceNoparam.Execute();
	return 0;
}


