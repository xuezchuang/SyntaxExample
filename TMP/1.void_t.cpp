#include <iostream>
#include <memory>
using namespace std;


//template <typename...>using void_t = void;

// 1.void_t�����þ������T������type��ƥ�䵽true_type.
// 2.void_tƥ�����۳ɹ�����ʧ��,������void.�ɹ���ƥ�䵽true_type,ʧ����false_type.
// 3.�ܽ�:void_t������Ϊ��������ģ��.
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
//Ҫ���һ�� Metafunction �Ƿ������� Metafunction Convention��Ҳ����˵����һ����������ͣ����Ǽ�����ڲ��Ƿ�����һ����Ϊ ��type�� �����֣�
int main()
{
	//ֻҪmyTest����type�Ϳ���,�����typeΪvoid_t���жϼ���.
	std::cout << has_type_member<int>::value << std::endl;             // 0, SFINAE
	std::cout << has_type_member<true_type>::value << std::endl;       // 1
	std::cout << has_type_member<myTest>::value << std::endl;          // 1
	system("pause");
}