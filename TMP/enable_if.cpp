#include <iostream>
#include <memory>
//#include <type_traits>
using namespace std;


template <bool _Test, typename T = void>
struct enable_if_f
{
};

template <typename T>
struct enable_if_f<true, T>
{
	using type = T;
};

template <bool _Test, class _Ty = void>
using enable_if_b = typename enable_if_f<_Test, _Ty>::type;

template <typename T> enable_if_b<is_integral_v<T>>       foo(T) {}  // #1
template <typename T> enable_if_b<is_floating_point_v<T>> foo(T) {}  // #2

//template <typename T> enable_if_t<is_integral_v<T>>       foo2(T) {}  // #1
//template <typename T> enable_if_t<is_floating_point_v<T>> foo2(T) {}  // #2

template <typename... Args>
struct always_true : true_type {};

template <typename... Args>
constexpr bool always_true_v = true;

template <typename T> 
struct S 
{
	//template <typename U>
	//static enable_if_t<is_same_v<T, int>> foo3(U) {}    // #1
	//template <typename U>
	//static enable_if_t<!is_same_v<T, int>> foo3(U) {}    // #2

	template <typename U> 
	static enable_if_t<always_true_v<U> && is_same_v<T, int>> foo4(U) {}    // #1
	template <typename U>
	static enable_if_t<always_true_v<U> && !is_same_v<T, int>> foo4(U) {}    // #2
};


int main() 
{
	//S s(1, 1.0f);     // match #2, output: 0
	foo(1);     // match #1
	foo(1.0f);  // match #2

	//S<int>::foo3(1);
	S<int>::foo4(1);
}