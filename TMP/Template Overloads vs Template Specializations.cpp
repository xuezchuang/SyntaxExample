#include <iostream>
#include <memory>
using namespace std;


template <typename T> void foo(T);     // #1
template <>           void foo(int*);  // #2
template <typename T> void foo(T*);    // #3

template <typename T> void foo2(T);     // #1
template <typename T> void foo2(T*);    // #3
template <>           void foo2(int*);  // #2


template <typename T> void foo3(T);     // #1
template <>           void foo3(int*);  // #2
template <typename T> void foo3(T*);    // #3
template <>           void foo3(int*);  // #4



int main() 
{
	//Template Overloads vs Template Specializations
	//优先使用Template Overloads,其次是Template Specializations
	foo(new int(1));  // which one will be called? 使用#3

	foo2(new int(1));  // which one will be called?使用#2

	foo3(new int(1));  // which one will be called?使用#4

}