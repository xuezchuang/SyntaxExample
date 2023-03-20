#include <iostream>
#include <memory>
using namespace std;

//template <typename...>using void_t = void;

// primary template
template <typename, typename = void> 
struct has_type_member : false_type {};
// partial specialization
template <typename T> 
struct has_type_member<T, void_t<typename T::type>> : true_type {};

template <typename T>
struct has_type_member_v : has_type_member<T> {};

//template <typename T>
//struct has_type_member_v

//template <typename _Ty>
//constexpr bool has_type_member_v<_Ty> = has_type_member<_Ty, _Ty>;

//template <class _Ty1>
//constexpr bool has_type_member_v = has_type_member(_Ty1);

//要检查一个 Metafunction 是否遵守了 Metafunction Convention。也就是说，给一个任意的类型，我们检查它内部是否定义了一个名为 “type” 的名字：
int main()
{
	//has_type_member<int>;
	//has_type_member<true_type>;
	//has_type_member<type_identity<int>>;
	//std::cout << has_type_member<int> << std::endl;                // 0, SFINAE
	//std::cout << has_type_member_v<true_type> << std::endl;          // 1
	//std::cout << has_type_member_v<type_identity<int>> << std::endl; // 1
}