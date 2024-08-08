#include <iostream>
#include <memory>

struct A
{
	int a;
	int b;
	int c[6];
	//A()
	//{
	//	std::cout << "A Construct" << std::endl;
	//};
	~A()
	{
		std::cout << "A DeConstruct" << std::endl;
	}
	A() = default;
	A(A&&) = default;
	A(const A&) = default;
	A& operator=(A&&) = default;
};

template <typename T> struct TRemoveReference { typedef T Type; };
template <typename T> struct TRemoveReference<T& > { typedef T Type; };
template <typename T> struct TRemoveReference<T&&> { typedef T Type; };

template <typename T>
typename TRemoveReference<T>::Type&& MoveTemp(T&& Obj)
{
	typedef typename TRemoveReference<T>::Type CastType;

	return (CastType&&)Obj;
}

int main()
{
	
	//A* p12 = new A;
	void* ptr = new char[1024*1024];
	{
		A mTestA;
		mTestA.a = 3;
		mTestA.b = 4;
		mTestA.c[0] = 5;
		//*(A*)ptr = std::move(mTestA);
		*(A*)ptr = MoveTemp(mTestA);
		//*(A*)ptr = mTestA;
	}
	return 0;

}