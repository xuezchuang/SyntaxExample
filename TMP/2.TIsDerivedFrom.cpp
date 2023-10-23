#include <iostream>
#include <memory>

/** Is type DerivedType inherited from BaseType. */
template<typename DerivedType, typename BaseType>
struct TIsDerivedFrom
{
	// Different size types so we can compare their sizes later.
	typedef char No[1];
	typedef char Yes[2];

	// Overloading Test() s.t. only calling it with something that is
	// a BaseType (or inherited from the BaseType) will return a Yes.
	static Yes& Test(BaseType*);
	static Yes& Test(const BaseType*);
	static No& Test(...);

	// Makes a DerivedType ptr.
	static DerivedType* DerivedTypePtr() { return nullptr; }

public:
	// Test the derived type pointer. If it inherits from BaseType, the Test( BaseType* ) 
	// will be chosen. If it does not, Test( ... ) will be chosen.
	static constexpr bool Value = sizeof(Test(DerivedTypePtr())) == sizeof(Yes);

	static constexpr bool IsDerived = Value;
};

/* ��̬��̬ */
template<typename ObjectType>
class A
{
public:
	A(){}
	void test()
	{
		static_assert(TIsDerivedFrom<ObjectType, A<ObjectType>>::Value, "A must be base of it's ObjectType (see CRTP)");
		ObjectType::hello();
	}
};
class B : public A<B>
{
public:
	B()
	{
		//test();
	}
	std::vector
};

int main()
{
	B a;
	system("pause");
}