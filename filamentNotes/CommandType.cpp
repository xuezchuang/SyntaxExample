// test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

//#include <iostream>
#include <utility>
#include <tuple>

#include "filamentNote.h"
#include <iostream>

#define EXPAND(x) x

#define DECL_ARGS_N__(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,X,...) X
#define DECL_ARGS_N(...)\
    EXPAND(DECL_ARGS_N__(0,##__VA_ARGS__,12,11,10,9,8,7,6,5,4,3,2,1,0))

void CommandType()
{
    int a = DECL_ARGS_N(1,2);
	a = DECL_ARGS_N(1, 2, 3);
	a = DECL_ARGS_N(1, 2, 4, 5);
	a = DECL_ARGS_N(1, 2);
	a = DECL_ARGS_N(1, 2);
    int b = 0;
}



#pragma region Test__vfptr

#pragma pack(push, 4)
class baseclass
{
public:
	baseclass() {}
	virtual void t1() {}
	virtual void t2() {}
	int aa = 0;
};
class dbclass : public baseclass
{
public:
	dbclass() {}
	int a = 0;
	virtual void t1() {}

	double b = 0;
};
#pragma pack(pop)
class novirtualclass
{
public:
	novirtualclass() {}
};
void Test__vfptr()
{
	baseclass* b1 = new dbclass();
	baseclass b2;
	dbclass b3;
	novirtualclass n1;
	int a = 0;
}
#pragma endregion Test__vfptr

#pragma region std::forward
class CData
{
public:
	CData() = delete;
	CData(const char* ch) : data(ch)
	{
		std::cout << "CData(const char* ch)" << std::endl;
	}
	CData(const std::string& str) :data(str)
	{
		std::cout << "CData(const std::string& str)" << std::endl;
	}
	CData(std::string&& str) : data(std::move(str))// data(std::forward<std::string>(str))
	{
		std::cout << "CData(std::string&& str)" << std::endl;
	}
	CData(const std::string&& str) : data(std::move(str))// data(std::forward<std::string>(str))
	{
		std::cout << "CData(std::string&& str)" << std::endl;
	}
	~CData()
	{
		std::cout << "~CData()" << std::endl;
	}
private:
	std::string data;
};

template<typename T>
CData* Creator(T&& t)
{
	//T a("a");
	//return new CData(t);// std::forward<T>(t));
	//std::string&& ab = std::forward<T>(t);
	return new CData(std::forward<T>(t));
}
void Test__forward()
{
	{
		const char* value = "hello";
		std::string str1 = "hello";
		std::string str2 = " world";
		CData* p = Creator(str1);
		//CData* p = Creator(std::string(str1));
		//CData* p = Creator(str1 + str2);

		delete p;
	}
	int a = 0;
}

#pragma endregion std::forward


#pragma region remove_reference

//template <class _Ty>
//struct re_reference {
//	using type = _Ty;
//	using _Const_thru_ref_type = const _Ty;
//};


template <class _Ty>
void hello(std::remove_reference_t<_Ty>& _Arg) noexcept
{
	int a = 0;
};

template <class _Ty>
void hello(std::_Const_thru_ref<_Ty>& _Arg) noexcept
{
	int a = 0;
};

template <class _Ty>
void  hello(std::remove_reference_t<_Ty>&& _Arg) noexcept
{
	int a = 0;
};

template <class _Ty>
void hello(std::_Const_thru_ref<_Ty>&& _Arg) noexcept
{
	int a = 0;
};

void Test_remove_reference()
{
	
	CData abc("hello");
	const CData& abd = abc;
	const CData* pabc = &abc;
	//auto tempA1 = re_reference<CData*>::type(pabc);
	//auto tempA2 = re_reference<CData*>::_Const_thru_ref_type(pabc);
	hello<const CData*>(pabc);
	//auto tempB1 = std::move(std::remove_reference<CData*>::_Const_thru_ref_type(pabc));
	//auto tempB2 = std::move(std::remove_reference_t<CData*>(pabc));
	//auto tempB1 = std::forward<const CData*>(pabc);
	//auto tempB = re_reference<CData>(abd)::type;

	



		int a[] = { 1,2,3 };
		decltype(*a) b = a[0];
		std::remove_reference<decltype(*a)>::type c = a[0];
		a[0] = 4;
		int ba = 0;


}

#pragma endregion remove_reference

#pragma region invoke

class classinvoke
{
public:
	classinvoke(){}

//private:
	void hello()
	{
		int a = 0;
	}

};

using namespace std;
#include <iostream>
#include <omp.h>
#include <vector>
//template<typename Data>
//class FrameGraphPass
//{
//protected:
//	FrameGraphPass() {}
//	Data mData;
//public:
//	Data const& getData() const noexcept { return mData; }
//	Data const* operator->() const { return &mData; }
//};
#include <iostream>
using namespace std;
void fun1(int const& a)
{
	//count << a << endl;
}
//void fun(const int& a)
//{
//
//}
//void fun2(int& const a)
//{
//	count << a << endl;
//}
void Test_invoke()
{
	int a = 0;
	fun1(a);
	//fun(a);
	//fun2(a);

	//return 0;

//	int coreNum = omp_get_num_procs();//获得处理器个数
//	vector<vector<int>> vTest;
//	vTest.resize(coreNum);
//	int b = 0;
//#pragma omp parallel for
//	for (int i = 0; i < 800; i++)
//	{
//		int k = omp_get_thread_num();//获得每个线程的ID
//		vTest[k].push_back(i);
//		//return;
//	}
//	b = 0;
//	b++;
//	std::cout << b << std::endl;
	system("pause");

}
#pragma endregion invoke

#pragma region std_unique
//class class_unique
//{
//public:
//	class_unique(CTest_std_unique&& data)
//		:mReferenceData(std::move(data))
//	{
//		int a = 0;
//
//	}
//	std::unique_ptr<CTest_std_unique> mData;
//	CTest_std_unique mReferenceData;
//
//};

class CTest_std_unique
{
public:
	CTest_std_unique()
	{
		a = 2;
	}
	CTest_std_unique(CTest_std_unique& data)
	{
		a = data.a;
		b = data.b;
	}
	CTest_std_unique(CTest_std_unique&& data)
	{
		// 将移动构造函数的参数 data 中的 a 赋值给成员变量 a
		a = data.a;
		// 将移动构造函数的参数 data 中的 b 赋值给成员变量 b
		b = data.b;
		// 将移动构造函数的参数 data 中的 a 和 b 赋值为 0，作为 "移动" 操作的标志
		data.a = 0;
		data.b = 0;
	}
	int a = 0;
	int b = 0;
};
void Test_std_unique()
{
	{
		auto ptemp = std::make_shared<CTest_std_unique>();
		 
		//CTest_std_unique temp2(temp);
		//CTest_std_unique te
		// mp3(std::move(temp));
		//temp3.a = 3;
		int c= 0;

		//addpass([=]()
		//{
		//	std::cout << "a = " << temp.a << std::endl;
		//});
	}
	int a = 0;

}
//void addpass(Execute&& execute)
//{
//	execute();
//	p = new FrameGraphPass<Execute>(std::forward<Execute>(execute));
//	execute();
//	((FrameGraphPass<Execute>*)p)->mExecute();
//	int a = 0;
//}
//void Test_std_unique()
//{
//	{
//		CTest_std_unique temp;
//		CTest_std_unique temp2(temp);
//		CTest_std_unique temp3(CTest_std_unique());
//
//
//		//addpass([=]()
//		//{
//		//	std::cout << "a = " << temp.a << std::endl;
//		//});
//	}
//	int a = 0;
//	
//}
#pragma endregion std_unique
