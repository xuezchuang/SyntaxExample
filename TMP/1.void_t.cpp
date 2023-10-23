#include <iostream>
#include <memory>
using namespace std;


//template <typename...>using void_t = void;

// 1.void_t的作用就是如果T类似有type则匹配到true_type.
// 2.void_t匹配无论成功还是失败,均返回void.成功则匹配到true_type,失败则false_type.
// 3.总结:void_t的作用为了特例化模板.
// primary template
template <typename, typename = void> 
struct has_type_member : false_type {};
// partial specialization
template <typename T> 
struct has_type_member<T, void_t<typename T::type>> : true_type {};

template <typename T>
struct has_type_member_v : has_type_member<T> {};

struct myTest
{
	//using type = int;
	using type = myTest;
};
//要检查一个 Metafunction 是否遵守了 Metafunction Convention。也就是说，给一个任意的类型，我们检查它内部是否定义了一个名为 “type” 的名字：
int main()
{
	//只要myTest中有type就可以,这儿的type为void_t中判断即可.
	std::cout << has_type_member<int>::value << std::endl;             // 0, SFINAE
	std::cout << has_type_member<true_type>::value << std::endl;       // 1
	std::cout << has_type_member<myTest>::value << std::endl;          // 1
	system("pause");
}