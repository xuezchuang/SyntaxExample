#include <iostream>
#include <memory>
using namespace std;

template <typename T, typename U> 
struct S {};                  // #1
template <typename T>
struct S<T, typename T::value_type> 
{};   // #2



int main()
{
	//Substitution Failure Is Not An Error

	S<int, int>();          // SFINAE in #2, select #1
	S<true_type, bool>();   // select #2
	S<true_type, int>();    // select #1

}