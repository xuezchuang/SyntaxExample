#include <iostream>
#include <memory>
using namespace std;
template <typename T, typename U>
struct S 
{ 
	S(T a, U b)
	{
		std::cout << is_same_v<decltype(b), float>;
	}
};  // #1

template <typename T>
struct S<T, float> 
{
	S(T a, T b) 
	{ 
		std::cout << is_same_v<decltype(b), float>;
	}
};  // #2

template <>
struct S<int, int> 
{ 
	S(int a, int b) 
	{ 
		std::cout << is_same_v<decltype(b), float>;
	} 
};  // #3

int main() 
{
	S s(1, 1.0f);     // match #2, output: 0

}