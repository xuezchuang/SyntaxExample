#include <iostream>
#include <memory>
#include "Function.h"
using namespace std;




void DoSomethingWithConvertingStringsToInts(TFunctionRef<int(int ntest)> Convert)
{
	//std::string hello = "hello";
	int a = 3;
	int n = Convert(a);
}

template <typename FuncType>
class abc;

template <typename Ret,typename... ParamTypes>
class abc<Ret(ParamTypes...)>
{
public:
	template <typename FunctorType>
	abc(FunctorType&& fun)
	{
		int a = 0;
		int b = fun(a);
		int c = 0;
	}

private:
	// A pointer to a function which invokes the call operator on the callable object
	Ret(*Callable)(void*, ParamTypes&...) = nullptr;
	//// A pointer to the callable object
	//void* Ptr = nullptr;
};

class abe
{
public:
	template <typename T>
	abe(T&& fun) :Ptr(Forward<T>(fun))
	{
		int a = 0;
		//Ptr = &fun;
	}
	//void* Ptr = nullptr;
	TFunctionRef<int(int)> Ptr;
};

struct abd
{
	template <typename FunctorType>
	void apply(FunctorType& functor)
	{
		int a = 9;
		functor(a);
	}

	template <typename FunctorType>
	void apply(FunctorType&& functor)
	{
		int a = 9;
		functor(a);
	}

};
void test(abc<int(int n)> a)
{
	//int b = a(4);
}

template <typename T>
struct MyStruct {
	T value;

	template <typename FunctorType>
	void apply(FunctorType functor, std::true_type) {
		value = 1;
	}

	template <typename FunctorType>
	void apply(FunctorType& functor, std::false_type) {
		value = 2;
	}

	template <typename FunctorType>
	void apply(FunctorType&& functor) {
		value = 3;
	}
};

TFunctionRef<int(int)> GetTransform(int nParam)
{
	// Squares number and returns it as a string with the specified prefix
	//return [=](int Num)
	//	{
	//		return nParam * 2 + Num;
	//	};
	auto lambda = [&](int Num) {
		return nParam * 2 + Num;
		};
	return TFunctionRef<int(int)>(lambda);
}
//TFunctionRef GetInvalidTransform()
//{
//	int nParam = 10;
//	auto lambda = [nParam](int Num) {
//		return nParam * 2 + Num;
//		};
//
//	// Return a reference to the lambda (which will go out of scope)
//	return TFunctionRef(lambda);
//}

	

int main() 
{
	/* 1. */
	int a = 0;
	abe abe_b([&](int ntest) {
		a++;
		return ntest + 1;
		});
	abe_b.Ptr(4);

	/* 2. */
	auto lambda_test = [](int Num)
		{
			return Num + 1;
		};
	TFunctionRef<int(int)> functest_a(lambda_test);

	/* 3. ��������������ʹ�����lambdaʧЧ,TFunctionRef����ʧ��.������TFunctionRefʹ�ÿյĹ��캯��,Ҳ����Ϊ������. */
	//TFunctionRef<int(int)> functest_c;
	//{
	//	functest_c = GetTransform(4);
	//}

	/* 4.��ȥ����ճ��TFunction��,������.
	*	����TFunctionRefBase�洢��TFunctionRefCaller�ľ�̬������ַ,�õ�ַ�������Ϊ ���洢�������ͺͲ�������.
	*	TFunctionRef��StorageType�洢����lambda�ĵ�ַ,�����ַ���ⲿ�����,����TFunctionRef�����Ǵ洢lambda�ĺ�����ַ.
	*	TFunction��StorageType�洢�ľ���һ������lambda��(TFunction_OwnedObject T).������Ҫ��̬��ȡ�����Ĵ�С�ͷ����ڴ�.ʵ����=�Ȳ���.
	*		����ʵ��ϸ����δ�о� : ��TFunction�յĹ��캯����,���ȷ�� TFunction_OwnedObject T ������ ?��ΪT��������lambda����
	*		��Bind��ʱ��ſ���ȷ���ⲿlambda������,�����ں���Bind��ʱ��,using OwnedType = TStorageOwnerTypeT<FunctorType, bUnique, !bUseInline>;��ȷ������,�����ڴ�
	* 
	*/


	DoSomethingWithConvertingStringsToInts([](int ntest) 
		{
		return ntest;
		});



	abd Tb;
	auto lambda11 = [](int& value) {
		value *= 2;
		};

	Tb.apply(lambda11);

	Tb.apply([](int& value) {
		value *= 2;
		});



	MyStruct<int> myStruct;
	myStruct.value = 0;

	// ������ͨ���ã�ֵ���ݣ�
	auto lambda1 = [](int& value) {
		value *= 2;
		};
	myStruct.apply(lambda1);
	int ma = myStruct.value;

	auto lambda2 = [](int& value) {
		value += 1;
		};
	myStruct.apply(lambda2);
	int b = myStruct.value;

	myStruct.apply([](int& value) {
		value -= 1;
		});

	int c = myStruct.value;
	int d = 0;
}