#include <iostream>
#include <memory>
#include "../Invoke.h"
using namespace std;






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

int Global_Num = 0;
void TestStaticRowFunc(int a, int b)
{
	Global_Num = a + b;
}



int main()
{
	typedef TDelegate<void(int,int,int)> DelegateTest;

	int tol = 0;

	DelegateTest Delegate;
		
	Delegate.BindLambda([&](int a, int b, int c)
		{
			tol = a + b + c;
		});
	//Delegate.Execute(3, 4, 5);

	Delegate.Execute(3, 4, 5);
	//
	
	//DelegateTest Delegate_Lambda = DelegateTest::CreateLambda([&](int a, int b, int c) {tol = a + b + c; }, 3, 4, 5);


	typedef TDelegate<void(int)> DelegateStaticFunc;

	int nParamB = 4;
	DelegateStaticFunc Delegate_StaticRow = DelegateStaticFunc::CreateStatic(&TestStaticRowFunc, nParamB);

	
	Delegate_StaticRow.Execute(1);

	//typedef TDelegate<void()> DelegateNoParam;
	//DelegateNoParam DelegateInstanceNoparam;
	//DelegateInstanceNoparam.BindLambda([&]
	//	{
	//		tol++;
	//	});
	//DelegateInstanceNoparam.Execute();
	return 0;
}


