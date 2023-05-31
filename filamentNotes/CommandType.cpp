//#include <iostream>
#include <utility>
#include <tuple>
#include <Windows.h>

#include "filamentNote.h"
#include <iostream>

#define EXPAND(x) x

#define DECL_ARGS_N__(_0,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,X,...) X
#define DECL_ARGS_N(...)\
    EXPAND(DECL_ARGS_N__(0,##__VA_ARGS__,12,11,10,9,8,7,6,5,4,3,2,1,0))


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
	//void (baseclass:: * ptrNonStaticFun)() = &baseclass::t1;
	auto it = &baseclass::t1;
	//auto it2 = &dbclass::t1;
	(b1->*it)();
	//(b1->*it2)();
	//fun_ptr func = decltype(&baseclass::t1);
	//baseclass b2;
	//dbclass b3;
	//novirtualclass n1;
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
		// 将移动构造函数的参数 data 中的 a 和 b 赋值为 0,作为 "移动" 操作的标志
		data.a = 0;
		data.b = 0;
	}
	int a = 0;
	int b = 0;
};
void Test_std_unique()
{
	typedef std::shared_ptr<int> intPtr;
	intPtr ntest = std::make_shared<int>(3);
	vector<intPtr> aTest;
	//aTest.push_back(ntest);
	aTest.push_back(std::make_shared<int>(3));
	//aTest.emplace_back(ntest);
	//aTest.emplace_back(2);
	return;
	{
		//auto ptemp = std::make_shared<CTest_std_unique>();
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


#pragma region TestMITC
#define TRUSS 0
#define FRAME 1

#define TRUSS_NODE	0
#define FRAME_NODE	1

#define FORCE_ON_NODE	0
#define LATERAL_FORCE	1
#define LATERAL_UNIFORM_PRESSURE	2
#define MOMENT_ON_A_POINT			3
#define LATERAL_LINEARLY_PRESSURE	4
#define AXIAL_PRESSURE				5
#define AXIAL_FORCE					6
#define MOMENT_ON_BEAM				7
#define TEMPERATURE			8
#define SUPPORT_MOVE		9

#define DIRECT_X		0
#define DIRECT_Y		1
#define DIRECT_R		2

struct Material
{
	double dE;		//弹性模量
	double dMu;		//泊松比
	double dAlpha;	//线膨胀系数
};

struct Section
{
	double dA;		//横截面面积
	double dIz;		//横截面惯性矩
	double dH;		//横截面高
};

struct Node
{
	int iType;			//节点类型
	double dX, dY;		//节点坐标
	int iaDOFIndex[3];	//节点自由度编号
};

struct Element
{
	int iType;		//单元类型号
	int iaNode[2];	//单元两端节点编号
	int iSection;	//单元截面索引号
	int iMaterial;	//单元材料索引号
	double dLength;	//单元长度
	double dSin, dCos;	//单元局部坐标 x轴与整体坐标 x 轴的夹 角的正余弦
	double daEndInterForce[6];	//单元杆端力向量
};

struct Load
{
	int iType;		//载荷类型
	int iDirect;	//载荷作用方向:0—X向,1—Y向,2—R 向 
	double dValue;	//载荷值
	int iLoadedElem;	//载荷作用的单元号
	int iLoadedNode;	//载荷作用的节点号
	double dPosition;	//载荷作用的位置或分布长度
	double dT0,dT1;		//杆上下表面温度变化值
};

struct ConstrainedNode
{
	int iNode;	//受约束节点号
	int iaConstrainedDOF[3];//受约束节点自由度特性:0—位移未知, 1—位移已知,
	//10000+节点号 N—与节点 N 的同类自由度耦合
};

int DOFIndexCalcu(int& iBuf0,int nTotalNode,int nConstrainedNode,ConstrainedNode* pConsNode,Node* pNode);
void ElementDOFCalcu(int nTotalElem,Node* pNod,Element* pElem,int** pElemDOF);
void BandAndDiagCalcu(int nTotalElem, int nTotalDOF, Element* pElem, int** pElemDof, int* pDiaq);
double** TwoArrayDoubAlloc(int nRow,int nCol);
int** TwoArrayIntAlloc(int nRow,int nCol);
template <class T>
void TwoArrayFree(int nRow,T** pdi);
template <class T>
void MatrixZeroize(int nRow,int nCol,T** pT);
template <class T>
void VectorZeroize(int n,T* pT);
void MatrixMultiply(int nRow,int nCol1,int nCol,double** pA,double** pB,double** pC); 
void MatrixVectorMultiply(int nRow,int nCol,double** pA,double* pB,double* pC);
void MatrixTrans(int nRow,int nCol,double** pA,double** pAT); 
void LengthSinCosCalcu(int nTotalElem,Element* pElem,Node* pNode);
void TrussElemStiffCalcu(ofstream& fout1, Element* pElem, Material* pMate, Section* pSect, double** pKe);
void FrameElemStiffCalcu(ofstream&fout1,Element* pElem,Material* pMate,Section* pSect,double** pKe);
void GKAssembly(int nTotalDOF,int nTotalElem,Element* pElem,Node* pNode,Material* pMate,Section* pSect,int* pDiag,double* pGK); 
bool LDLTSolve(int nRow,int* pDiag,double* pGK,double* pB);																																																																																																						 
void FixedEndForceCalcu(Element* pElem, Material* pMate, Section* pSect, Load* pLoad, double* pFixedEndF, int i);
void LoadVectorAssembly(int nLoad, int nTotalDOF, int nFreeDOF, int* pDiag, double* pGK, Element* pElem, Material* pMate, Section* pSect,
						Load*pLoad, Node* pNode, double* pLoadVect, double* pDisp); 
void ElementEndForceInit(int nTotalElem, Element* pElem);            
double GetElementInGK(int nRow, int iRow, int iCol, int* pDiag, double* pGK);
void LoadVectorModify(int nTotalDOF, int nFreeDOF, int* pDiag, double* pGK, double* pDisp, double* pLoadVect);
void InternalForceCalcu(int nTotalElem, Element* pElem, Node* pNode, double* pDisp);
void TrussInternalForceCalcu(Element* pElem,Node* pNode,double* pDisp,double** pKe);

void FrameInternalForceCalcu(Element* pElem,Node* pNode,double* pDisp,double** pKe);
void SupportReactionCalcu(int nTotalDOF,int nFreeDOF,int* pDiag,double* pGK,double* pDisp,double* pLoadVect);
void NodeDisplOutput(ofstream& fout,int nTotalNode,Node* pNode,double* pDisp);
void EndInternalForceOutput(ofstream& fout0,int nTotalElem,Element* pElem);
void SupportReactionOutput(ofstream& fout0,int nConstrainedNode,ConstrainedNode* pConsNode,Node* pNode,double* pLoadVect);

//#include "stdafx.h"
#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include <iostream>
#include <fstream>
#include <iomanip>
//#include "barsystem06.h"


void TestMITC()
{
	int nTotalNode;			 //节点总数
	int nConstrainedNode;	 //受约束节点总数
	int nTotalElem;			 //单元总数
	int nMaterialType;		 //材料种类数
	int nSectionType;		 //截面几何特性种类数
	int nLoad;				 //载荷总数
	int nTotalDOF;			 //总自由度数
	int nFreeDOF;			 //独立自由度数
	int i;					 //循环控制变量
	int iBuf;				 //

	ifstream fin0("test05.txt");//文件输入流对象,原始数据文件
	if(!fin0)
	{
		cout << "原始数据文件打开失败!" << endl;
		exit(-1);
	}
	ofstream fout0("Results.dat");//文件输出流对象,计算结果数据文件 
	if(!fout0)
	{
		cout<<"计算结果输出文件打开失败!"<<endl;
		exit(-1);
	}
	fin0 >> nTotalNode >> nConstrainedNode >> nTotalElem >> nMaterialType >> nSectionType >> nLoad;//输入总控数据
	//内存分配------------------------------------------------
	Node* pNode=new Node[nTotalNode];
	ConstrainedNode* pConsNode=new ConstrainedNode[nConstrainedNode]; 
	Element* pElem=new Element[nTotalElem];
	Material* pMate=new Material[nMaterialType];
	Section* pSect=new Section[nSectionType];
	Load* pLoad=new Load[nLoad];
	int** pElemDOF=TwoArrayIntAlloc(nTotalElem,6);  
	//--------------------------------------------------- 
	//读入结构描述数据
	for(i = 0; i < nTotalNode; i++)          //读入结点数据
		fin0 >> (pNode + i)->iType >> (pNode + i)->dX >> (pNode + i)->dY;
	for(i = 0; i < nConstrainedNode; i++)     //读入受约束结点数据
		fin0 >> (pConsNode + i)->iNode >> (pConsNode + i)->iaConstrainedDOF[0] >> (pConsNode + i)->iaConstrainedDOF[1] >> (pConsNode + i)->iaConstrainedDOF[2];
	for(i = 0; i < nTotalElem; i++)          //读入单元数据
		fin0 >> (pElem + i)->iType >> (pElem + i)->iaNode[0] >> (pElem + i)->iaNode[1] >> (pElem + i)->iSection >> (pElem + i)->iMaterial;
	for(i = 0; i < nMaterialType; i++)        //读入材料数据
		fin0 >> (pMate + i)->dE >> (pMate + i)->dMu >> (pMate + i)->dAlpha;
	for(i = 0; i < nSectionType; i++)         //读入截面数据
		fin0 >> (pSect + i)->dA >> (pSect + i)->dIz >> (pSect + i)->dH;

	for(i = 0; i < nLoad; i++)               //读入载荷数据
	{
		fin0 >> (pLoad + i)->iType >> (pLoad + i)->iDirect >> (pLoad + i)->dValue >> (pLoad + i)->iLoadedElem >> (pLoad + i)->iLoadedNode >>
			(pLoad + i)->dPosition >> (pLoad + i)->dT0 >> (pLoad + i)->dT1;
	}
	//------------------------------------------------------
	cout.setf(ios::right);
	cout << endl << endl;
	cout << setw(14) << "总控数据:" << endl << endl;
	cout << setw(14) << "结点总数:" << setw(10) << nTotalNode << endl
		<< setw(14) << "受约束结点数:" << setw(10) << nConstrainedNode << endl;
	cout << setw(14) << "单元总数:" << setw(10) << nTotalElem << endl << setw(14) << "材料总数:" << setw(10) << nMaterialType << endl;
	cout << setw(14) << "截面种类数:" << setw(10) << nSectionType << endl << setw(14) << "载荷总数:" << setw(10) << nLoad << endl << endl;
	cout << "=============================================" << endl;
	cout << setw(10) << "结点数据:" << endl << endl;
	cout << setw(10) << "结点类型" << setw(10) << "X" << setw(10) << "Y" << endl;
	for(i = 0; i < nTotalNode; i++)         //
	{
		cout << setw(10) << (pNode + i)->iType << setw(10) << (pNode + i)->dX << setw(10) << (pNode + i)->dY << endl << endl;
		cout << setw(16) << "受约束结点数据:" << endl << endl;
		cout << setw(12) << "受约束结点号" << setw(10) << "X向特征数" << setw(10) << "Y向特征数" << setw(10) << "R向特征数" << endl;
	}
	for(i = 0; i < nConstrainedNode; i++)         //
	{
		cout << setw(12) << (pConsNode + i)->iNode << setw(10) << (pConsNode + i)->iaConstrainedDOF[0];
		cout << setw(10) << (pConsNode + i)->iaConstrainedDOF[1] << setw(10) << (pConsNode + i)->iaConstrainedDOF[2] << endl << endl;
		cout << setw(10) << "单元数据:" << endl << endl;
		cout << setw(12) << "单元型号" << setw(12) << "始端结点号" << setw(12) << "终端结点号" << setw(12) << "截面型号" << setw(12) << "材料索引号" << endl;
	}
	for(i = 0; i < nTotalElem; i++)         //
	{
		cout << setw(12) << (pElem + i)->iType
			<< setw(12) << (pElem + i)->iaNode[0]
			<< setw(12) << (pElem + i)->iaNode[1]
			<< setw(12) << (pElem + i)->iSection
			<< setw(12) << (pElem + i)->iMaterial << endl;
		cout << endl;
		cout << setw(10) << "材料数据:" << endl << endl;
		cout << setw(12) << "弹性模量" << setw(12) << "泊松比" << setw(12) << "线胀系数" << endl;
	}
	for(i = 0; i < nMaterialType; i++)     //
	{
		cout << setw(12) << (pMate + i)->dE
			<< setw(12) << (pMate + i)->dMu
			<< setw(12) << (pMate + i)->dAlpha << endl;
		cout << endl;

		cout << setw(14) << "截面几何特征:" << endl << endl;
		cout << setw(12) << "面积" << setw(12) << "惯性矩" << setw(12) << "截面高" << endl;
	}
	for(i = 0; i < nSectionType; i++)     //
	{
		cout << setw(12) << (pSect + i)->dA
			<< setw(12) << (pSect + i)->dIz
			<< setw(12) << (pSect + i)->dH << endl;
		cout << endl;

		cout << setw(10) << "载荷数据:" << endl << endl;
		cout << setw(10) << "类型号" << setw(10) << "方向" << setw(10) << "载荷值"
			<< setw(10) << "单元号" << setw(10) << "结点号"
			<< setw(10) << "位置" << setw(10) << "温度 T0" << setw(10) << "温度 T1" << endl;
	}
	for(i = 0; i < nLoad; i++)           //
	{
		cout << setw(10) << (pLoad + i)->iType
			<< setw(10) << (pLoad + i)->iDirect
			<< setw(10) << (pLoad + i)->dValue
			<< setw(10) << (pLoad + i)->iLoadedElem
			<< setw(10) << (pLoad + i)->iLoadedNode
			<< setw(10) << (pLoad + i)->dPosition
			<< setw(10) << (pLoad + i)->dT0 << setw(10) << (pLoad + i)->dT1 << endl;
	}
	//原始数据输出到文件---------------------------------------
	fout0.setf(ios::right);
	fout0 << endl << endl;
	fout0 << setw(14) << "总控数据:" << endl << endl;
	fout0 << setw(14) << "结点总数:" << setw(10) << nTotalNode << endl
		<< setw(14) << "受约束结点数:" << setw(10) << nConstrainedNode << endl
		<< setw(14) << "单元总数:" << setw(10) << nTotalElem << endl <<
		setw(14) << "材料总数:" << setw(10) << nMaterialType << endl <<
		setw(14) << "截面种类数:" << setw(10) << nSectionType << endl <<
		setw(14) << "载荷总数:" << setw(10) << nLoad << endl << endl;
	fout0 << "=============================================" << endl;
	fout0 << setw(10) << "结点数据:" << endl << endl;
	fout0 << setw(10) << "结点类型" << setw(10) << "X" << setw(10) << "Y" << endl;
	for(i = 0; i < nTotalNode; i++)
	{
		fout0 << setw(10) << (pNode + i)->iType
			<< setw(10) << (pNode + i)->dX
			<< setw(10) << (pNode + i)->dY << endl << endl;
	}
	fout0<<setw(16)<<"受约束结点数据:"<<endl<<endl;
	fout0 << setw(12) << "受约束结点号"
		<< setw(10) << "X向特征数"
		<< setw(10) << "Y向特征数"
		<< setw(10) << "R向特征数" << endl;

	for(i = 0; i < nConstrainedNode; i++)         //
	{
		fout0 << setw(12) << (pConsNode + i)->iNode
			<< setw(10) << (pConsNode + i)->iaConstrainedDOF[0] 
			<< setw(10) << (pConsNode + i)->iaConstrainedDOF[1] 
			<< setw(10) << (pConsNode + i)->iaConstrainedDOF[2] << endl;
	}
	fout0<<endl;

	fout0 << setw(10) << "单元数据:" << endl << endl;
	fout0 << setw(12) << "单元型号" << setw(12) << "始端结点号" << setw(12) << "终端结点号" << setw(12) << "截面型号" << setw(12) << "材料索引号" << endl;
	for(i = 0; i < nTotalElem; i++)
	{
		fout0 << setw(12) << (pElem + i)->iType
			<< setw(12) << (pElem + i)->iaNode[0]
			<< setw(12) << (pElem + i)->iaNode[1]
			<< setw(12) << (pElem + i)->iSection
			<< setw(12) << (pElem + i)->iMaterial << endl;
	}
	fout0 << endl;

	fout0 << setw(10) << "材料数据:" << endl << endl;
	fout0 << setw(12) << "弹性模量" << setw(12) << "泊松比" << setw(12) << "线胀系数" << endl;
	for(i = 0; i < nMaterialType; i++)
	{
		fout0 << setw(12) << (pMate + i)->dE
			<< setw(12) << (pMate + i)->dMu
			<< setw(12) << (pMate + i)->dAlpha << endl;
	}
	fout0 << endl;

	fout0 << setw(14) << "截面几何特征:" << endl << endl;
	fout0 << setw(12) << "面积" << setw(12) << "惯性矩" << setw(12) << "截面高" << endl;
	for(i = 0; i < nSectionType; i++)
	{
		fout0 << setw(12) << (pSect + i)->dA
			<< setw(12) << (pSect + i)->dIz
			<< setw(12) << (pSect + i)->dH << endl;
	}
	fout0<<endl;

	fout0 << setw(10) << "载荷数据:" << endl << endl;
	fout0<<setw(10)<<"类型号"<<setw(10)<<"方向"<<setw(10)<<"载荷值"
	<<setw(10)<<"单元号"<<setw(10)<<"结点号"
		<< setw(10) << "位置" << setw(10) << "温度 T0" << setw(10) << "温度 T1" << endl;
	for(i = 0; i < nLoad; i++)
	{
		fout0 << setw(10) << (pLoad + i)->iType
			<< setw(10) << (pLoad + i)->iDirect
			<< setw(10) << (pLoad + i)->dValue
			<< setw(10) << (pLoad + i)->iLoadedElem
			<< setw(10) << (pLoad + i)->iLoadedNode
			<< setw(10) << (pLoad + i)->dPosition
			<< setw(10) << (pLoad + i)->dT0 << setw(10) << (pLoad + i)->dT1 << endl;
	}
	//------------------------------------------------------ 
	LengthSinCosCalcu(nTotalElem,pElem,pNode);
	//计算总自由度数  节点自由度和单元定位向量------------------------    
	nTotalDOF = DOFIndexCalcu(nFreeDOF, nTotalNode, nConstrainedNode, pConsNode, pNode);
	ElementDOFCalcu(nTotalElem, pNode, pElem, pElemDOF);
	//------------------------------------------------------
	int* pDiag = new int[nTotalDOF];           //存放主元地址

	BandAndDiagCalcu(nTotalElem, nTotalDOF, pElem, pElemDOF, pDiag); //计算半带宽和主元地址
	TwoArrayFree(nTotalElem, pElemDOF);         //释放单元定位向量数组的内存

	iBuf = pDiag[nTotalDOF - 1] + 1;//计算带内元数总数
	double* pGK = new double[iBuf];//一维变带宽存放总刚度矩阵的下三角部分

	double* pLoadVect=new double[nTotalDOF];	//存放载荷向量
	double* pDisp=new double[nTotalDOF];		//存放位移向量

	VectorZeroize(iBuf, pGK);				//总刚置零 
	VectorZeroize(nTotalDOF, pLoadVect);    //总载荷向量置零         
	VectorZeroize(nTotalDOF, pDisp);		//总位移向量置零             
	ElementEndForceInit(nTotalElem,pElem);  //初始化单元杆端力向量
	//装配总刚度矩阵和总载荷向量-----------------------------------
	GKAssembly(nTotalDOF, nTotalElem, pElem, pNode, pMate, pSect, pDiag, pGK); //组装总刚
	LoadVectorAssembly(nLoad, nTotalDOF, nFreeDOF, pDiag, pGK, pElem, pMate, pSect, pLoad, pNode, pLoadVect, pDisp);//组装总载荷向量 
	LDLTSolve(nFreeDOF, pDiag, pGK, pDisp);//解平衡方程求位移   
	InternalForceCalcu(nTotalElem, pElem, pNode, pDisp);//计算杆件内力          
	SupportReactionCalcu(nTotalDOF, nFreeDOF, pDiag, pGK, pDisp, pLoadVect);//计算支座反力
	NodeDisplOutput(fout0, nTotalNode, pNode, pDisp);//输出节点位移            
	EndInternalForceOutput(fout0, nTotalElem, pElem);//输出杆件内力 
	SupportReactionOutput(fout0, nConstrainedNode, pConsNode, pNode, pLoadVect);//输出支座反力
	//释放内存
	delete[]pNode;
	delete[]pConsNode;
	delete[]pElem;
	delete[]pMate;
	delete[]pSect;
	delete[]pLoad;
	delete[]pDiag;
	delete[]pGK;
	delete[]pLoadVect;
	delete[]pDisp;
	//关闭文件
	fin0.close();

	fout0.close();
	system("pause");
}
//------------------------------------------------------
int DOFIndexCalcu(int& iBuf0,int nTotalNode,int nConstrainedNode, ConstrainedNode* pConsNode,Node* pNode)
{
	int i,j,k;
	int iBuf;//总自由度数
	for(i = 0; i < nTotalNode; i++)
		for(j = 0; j < 3; j++)
			(pNode + i)->iaDOFIndex[j] = 0;//将各节点自由度编号置零
	for(i = 0; i < nConstrainedNode; i++)
	{
		iBuf = (pConsNode + i)->iNode;//受约束节点号
		for(j = 0; j < 3; j++)
			pNode[iBuf].iaDOFIndex[j] = (pConsNode + i)->iaConstrainedDOF[j];
	}
	iBuf=0;
	for(i=0;i<nTotalNode;i++)
	{
		if((pNode + i)->iType == FRAME_NODE)
		{
			for(j = 0; j < 3; j++)
			{
				//对刚架节点的未知独立自由度编号
				if((pNode + i)->iaDOFIndex[j] == 0)
					(pNode + i)->iaDOFIndex[j] = iBuf++;
			}
		}
		else
		{
			for(j = 0; j < 2; j++)
			{
				//对桁架节点的未知独立自由度编号
				if((pNode + i)->iaDOFIndex[j] == 0)
					(pNode + i)->iaDOFIndex[j] =iBuf++;
			}
		}
	}
	iBuf0 = iBuf;//未知独立自由度数
	for(i = 0; i < nTotalNode; i++)
	{
		if((pNode+i)->iType==FRAME_NODE)
		{
			for(j = 0; j < 3; j++)
			{
				//对刚架节点的已知独立自由度编号
				if((pNode + i)->iaDOFIndex[j] == -1)
					(pNode + i)->iaDOFIndex[j] = iBuf++;
			}
		}
		else
		{
			for(j = 0; j < 2; j++)
			{
				//对桁架节点的已知独立自由度编号
				if((pNode + i)->iaDOFIndex[j] == -1)
					(pNode + i)->iaDOFIndex[j] = iBuf++;
			}
		}
	}
	for(i = 0; i < nTotalNode; i++)
	{
		if((pNode + i)->iType == FRAME_NODE)
		{
			for(j=0;j<3;j++)
			{
				if((pNode+i)->iaDOFIndex[j]>=10000)//对刚架从节点自由度编号
				{
					k = (pNode + i)->iaDOFIndex[j] - 10000;
					(pNode + i)->iaDOFIndex[j] = (pNode + k)->iaDOFIndex[j];
				}
			}
		}
		else
		{
			for(j = 0; j < 2; j++)
			{
				if((pNode + i)->iaDOFIndex[j] >= 10000) //对桁架从节点自由度编号
				{
					k = (pNode + i)->iaDOFIndex[j] - 10000;
					(pNode + i)->iaDOFIndex[j] = (pNode + k)->iaDOFIndex[j];
				}
			}
		}
	}
	return iBuf;
}
//计算单元定位向量------------------------------------------ 
void ElementDOFCalcu(int nTotalElem,Node* pNode,Element* pElem,int** pElemDOF)
{
	int iNode0, iNode1;//单元两端节点号
	int i, j;
	for(i = 0; i < nTotalElem; i++)
	{
		iNode0 = (pElem + i)->iaNode[0];
		iNode1 = (pElem + i)->iaNode[1];
		if((pElem+i)->iType==TRUSS) //对桁架单元
		{
			for(j = 0; j < 2; j++)
			{
				pElemDOF[i][j] = (pNode + iNode0)->iaDOFIndex[j];
				pElemDOF[i][j + 2] = (pNode + iNode1)->iaDOFIndex[j];
			}
		}
		else //对刚架单元
		{
			for(j = 0; j < 3; j++)
			{
				pElemDOF[i][j] = (pNode + iNode0)->iaDOFIndex[j];
				pElemDOF[i][j + 3] = (pNode + iNode1)->iaDOFIndex[j];
			}
		}
	}
}

void  BandAndDiagCalcu(int nTotalElem,int nTotalDOF,Element* pElem,int** pElemDOF,int* pDiag)
{
	int iMinDOF;
	int iBuf;
	int iDOFIndex;
	int i, j;

	for(i = 0; i < nTotalDOF; i++)//各行半带宽置 1
		pDiag[i] = 1;
	for(i = 0; i < nTotalElem; i++)
	{
		iMinDOF = pElemDOF[i][0];
		if((pElem + i)->iType == TRUSS)
		{
			for(j = 0; j < 4; j++)  //从桁架单元的 4个节点位移编号中选出最小号
			{
				if(pElemDOF[i][j] < iMinDOF)
					iMinDOF = pElemDOF[i][j];
			}
		}
		else
		{
			for(j = 0; j < 6; j++)	//从刚架单元的 6个节点位移编号中选出最小号
			{
				if(pElemDOF[i][j] < iMinDOF)
					iMinDOF = pElemDOF[i][j];
			}
		}
		if((pElem + i)->iType == TRUSS)
		{
			for(j = 0; j < 4; j++)
			{
				iDOFIndex = pElemDOF[i][j];
				iBuf = iDOFIndex - iMinDOF + 1;//计算半带宽
				if(iBuf > pDiag[iDOFIndex])
					pDiag[iDOFIndex] = iBuf;
			}
		}
		else
		{
			for(j = 0; j < 6; j++)
			{
				iDOFIndex = pElemDOF[i][j];
				iBuf = iDOFIndex - iMinDOF + 1; //计算半带宽
				if(iBuf > pDiag[iDOFIndex])
					pDiag[iDOFIndex] = iBuf;
			}
		}
	}
	pDiag[0]=0;
	for(i = 1; i < nTotalDOF; i++)
		pDiag[i] = pDiag[i] + pDiag[i - 1];
}

//双精度二维数组内存分配--------------------------------------
double** TwoArrayDoubAlloc(int nRow,int nCol)
{
	double** pd=new double* [nRow];
	if(!pd)
	{
		cout << "内存分配失败!" << endl;
		exit(-1);
	}
	for(int i = 0; i < nRow; i++)
	{
		pd[i] = new double[nCol];//申请列数
		if(!pd[i])
		{
			cout << "内存分配失败!" << endl; exit(-1);
		}
	}
	return pd;
}

//整型二维数组内存分配--------------------------------------- 
int** TwoArrayIntAlloc(int nRow,int nCol)
{
	int** pi=new int* [nRow];//申请行数
	if(!pi)
	{
		cout << "内存分配失败!" << endl;
		exit(-1);
	}
	for(int i = 0; i < nRow; i++)
	{
		pi[i] = new int[nCol];//申请列数
		if(!pi[i])
		{
			cout << "内存分配失败!" << endl;
			exit(-1);
		}
	}
	return pi;
}

//二维数组内存释放------------------------------------------
template <class T>
void TwoArrayFree(int nRow, T** pdi)
{
	for(int i = 0; i < nRow; i++)//回收列空间
		delete[] pdi[i];
	delete[] pdi;//回收行空间
}

//矩阵置零------------------------------------------------
template <class T>
void MatrixZeroize(int nRow, int nCol, T** pT)
{
	for(int i = 0; i < nRow; i++)
		for(int j = 0; j < nCol; j++)
			pT[i][j] = 0;
}

//向量置零------------------------------------------------
template <class T>
void VectorZeroize(int n, T* pT)
{
	for(int i = 0; i < n; i++)
		pT[i] = 0;
}
//两矩阵相乘-----------------------------------------------
void  MatrixMultiply(int nRow, int nCol1, int nCol, double** pA, double** pB, double** pC)
{
	double dTemp;
	for(int i = 0; i < nRow; i++)
		for(int j = 0; j < nCol; j++)
		{
			dTemp = 0.0;
			for(int k = 0; k < nCol1; k++)
				dTemp += pA[i][k] * pB[k][j];
			pC[i][j] = dTemp;
		}
}

//矩阵左乘向量--------------------------------------------- 
void MatrixVectorMultiply(int nRow, int nCol, double** pA, double* pB, double* pC)
{
	double dTemp;
	for(int i = 0; i < nRow; i++)
	{
		dTemp = 0.0;
		for(int j = 0; j < nCol; j++)
			dTemp += pA[i][j] * pB[j];
		pC[i] = dTemp;
	}
}

//矩阵转置------------------------------------------------ 
void MatrixTrans(int nRow, int nCol, double** pA, double** pAT)
{
	for(int i = 0; i < nRow; i++)
		for(int j = 0; j < nCol; j++)
			pAT[j][i] = pA[i][j];
}

//计算杆单元长度和方向余弦------------------------------------ 
void LengthSinCosCalcu(int nTotalElem, Element* pElem, Node* pNode)
{
	int i;
	int iNode0, iNode1;
	double dDeltaX, dDeltaY;
	double dX0, dY0, dX1, dY1;

	for(i = 0; i < nTotalElem; i++)
	{
		iNode0 = (pElem + i)->iaNode[0];
		iNode1 = (pElem + i)->iaNode[1];
		dX0 = (pNode + iNode0)->dX;
		dY0 = (pNode + iNode0)->dY;
		dX1 = (pNode + iNode1)->dX;
		dY1 = (pNode + iNode1)->dY;
		dDeltaX = dX1 - dX0;
		dDeltaY = dY1 - dY0;
		(pElem + i)->dLength = sqrt(dDeltaX * dDeltaX + dDeltaY * dDeltaY); (pElem + i)->dSin = dDeltaY / (pElem + i)->dLength;
		(pElem + i)->dCos = dDeltaX / (pElem + i)->dLength;
	}
}
//计算桁架单元刚度矩阵-------------------------------------       
void TrussElemStiffCalcu(ofstream& fout1, Element* pElem, Material* pMate, Section* pSect, double** pKe)
{
	int i, j;
	double dBuf, dBuf1;
	double dE, dA, dLength, dSin, dCos;
	int iSectType, iMateType;
	iSectType = pElem->iSection;
	iMateType = pElem->iMaterial;
	dA = (pSect + iSectType)->dA;
	dE = (pMate + iMateType)->dE;
	dLength = pElem->dLength;
	MatrixZeroize(4, 4, pKe);

	dBuf = dE * dA / dLength;
	pKe[0][0] = pKe[2][2] = dBuf;
	pKe[0][2] = pKe[2][0] = -dBuf;
	for(i = 0; i < 4; i++)
		for(j = 0; j < 4; j++)
			fout1.write((char*)&pKe[i][j], sizeof(double));//将局部坐标系单刚记盘转换到整体坐标系

	dSin = pElem->dSin;

	dCos = pElem->dCos;
	dBuf1 = dCos * dCos * dBuf;
	pKe[0][0] = pKe[2][2] = dBuf1;
	pKe[0][2] = pKe[2][0] = -dBuf1;
	dBuf1 = dSin * dSin * dBuf;
	pKe[1][1] = pKe[3][3] = dBuf1;
	pKe[1][3] = pKe[3][1] = -dBuf1;
	dBuf1 = dCos * dSin * dBuf;
	pKe[1][0] = pKe[0][1] = pKe[2][3] = pKe[3][2] = dBuf1;
	pKe[0][3] = pKe[3][0] = pKe[1][2] = pKe[2][1] = -dBuf1;
}

//计算刚架单元刚度矩阵----------------------------------------
void FrameElemStiffCalcu(ofstream& fout1, Element* pElem, Material* pMate, Section* pSect, double** pKe)
{
	int i, j;
	double dBuf, dLength;
	double dE;
	double dA, dIz, dSin, dCos;
	int iSectType, iMateType;
	double** pT = TwoArrayDoubAlloc(6, 6);
	double** pTT = TwoArrayDoubAlloc(6, 6);
	double** pTemp = TwoArrayDoubAlloc(6, 6);

	iSectType = pElem->iSection;
	iMateType = pElem->iMaterial;
	dA = (pSect + iSectType)->dA;
	dIz = (pSect + iSectType)->dIz;
	dE = (pMate + iMateType)->dE;
	dLength = pElem->dLength;

	MatrixZeroize(6, 6, pKe);
	dBuf = dE * dA / dLength;
	pKe[0][0] = pKe[3][3] = dBuf;
	pKe[0][3] = pKe[3][0] = -dBuf;
	dBuf = dLength * dLength * dLength;
	dBuf = 12.0 * dE * dIz / dBuf;
	pKe[1][1] = pKe[4][4] = dBuf;

	pKe[1][4] = pKe[4][1] = -dBuf;
	dBuf = dLength * dLength;
	dBuf = 6.0 * dE * dIz / dBuf;
	pKe[1][2] = pKe[2][1] = pKe[1][5] = pKe[5][1] = dBuf;
	pKe[2][4] = pKe[4][2] = pKe[4][5] = pKe[5][4] = -dBuf;
	dBuf = 4.0 * dE * dIz / dLength;
	pKe[2][2] = pKe[5][5] = dBuf;
	pKe[5][2] = pKe[2][5] = dBuf / 2.0;
	for(i = 0; i < 6; i++)
		for(j = 0; j < 6; j++)
			fout1.write((char*)&pKe[i][j], sizeof(double));//将局部坐标系单刚记盘转换到整体坐标系
	dSin = pElem->dSin;
	dCos = pElem->dCos;
	MatrixZeroize(6, 6, pT);
	pT[2][2] = pT[5][5] = 1.0;
	pT[0][0] = pT[1][1] = pT[3][3] = pT[4][4] = dCos;
	pT[0][1] = pT[3][4] = dSin;
	pT[1][0] = pT[4][3] = -dSin;

	MatrixTrans(6, 6, pT, pTT);
	MatrixMultiply(6, 6, 6, pTT, pKe, pTemp);
	MatrixMultiply(6, 6, 6, pTemp, pT, pKe);

	TwoArrayFree(6, pT);
	TwoArrayFree(6, pTT);
	TwoArrayFree(6, pTemp);
}

bool LDLTSolve(int nRow, int* pDiag, double* pGK, double* pB)
{
	int i;
	int j, k;
	int iBegin, jBegin, iAdd_ii, iAdd_jj, iAdd_kk;
	int iRow, nHalfBand;
	double   dBuf;

	for(i = 1; i < nRow; i++)
	{
		iBegin = i - pDiag[i] + pDiag[i - 1] + 1;
		iAdd_ii = pDiag[i];
		for(j = iBegin + 1; j < i + 1; j++)
		{
			jBegin = j - pDiag[j] + pDiag[j - 1] + 1;
			if(jBegin < iBegin) jBegin = iBegin;
			iAdd_jj = pDiag[j];
			dBuf = 0.0;
			for(k = jBegin; k < j; k++)
			{
				if(pGK[pDiag[k]] == 0.0)
				{
					cout<<"总刚系数有错!"<<endl;
					return false;
				}
				dBuf += pGK[iAdd_ii - i + k] * pGK[iAdd_jj - j + k] / pGK[pDiag[k]];
			}
			pGK[iAdd_ii - i + j] -= dBuf;
		}
	}
	if(pGK[0] == 0.0)
	{
		cout<<"总刚系数有错!"<<endl;
		return false;
	}
	pB[0] = pB[0] / pGK[0];
	for(i = 1; i < nRow; i++)
	{
		dBuf = 0.0;
		iBegin = i - pDiag[i] + pDiag[i - 1] + 1;
		iAdd_ii = pDiag[i];
		for(j = iBegin; j < i; j++)
		{
			dBuf += pB[j] * pGK[iAdd_ii - i + j];
		}
		pB[i] -= dBuf;

		if(pGK[iAdd_ii] == 0.0)
		{
			cout<<"总刚系数有错!"<<endl;
			return false;
		}
		pB[i] /= pGK[iAdd_ii];
	}
	nHalfBand = 1;
	iRow = nRow - 1;
	for(i = nRow - 2; i >= 0; i--)
	{
		nHalfBand++;
		while(nHalfBand > (pDiag[iRow] - pDiag[iRow - 1])) {
			nHalfBand--;
			iRow--;
		}
		dBuf = 0.0;
		iAdd_ii = pDiag[i];
		for(j = 1; j < nHalfBand; j++)
		{
			if(j < (pDiag[j + i] - pDiag[j + i - 1]))
			{
				if(pGK[iAdd_ii] == 0.0)
				{
					cout<<"总刚系数有错!"<<endl;
					return false;
				}
				iAdd_kk = pDiag[j + i];
				dBuf += pGK[iAdd_kk - j] * pB[j + i] / pGK[iAdd_ii];
			}
		}
		pB[i] -= dBuf;
	}
	return true;
}

//装配总刚度矩阵-------------------------------------------     
void GKAssembly(int nTotalDOF, int nTotalElem, Element* pElem, Node* pNode, Material* pMate, Section* pSect, int* pDiag, double* pGK)
{
	int i, j, m, iNode0, iNode1, GKi, GKj, GKij;
	double** pKe0 = TwoArrayDoubAlloc(4, 4);
	double** pKe1 = TwoArrayDoubAlloc(6, 6);
	int iaDOFIndex[6];

	ofstream fout1("ElemStiff.dat", ios::binary);
	if(!fout1)
	{
		cout << "单元刚度矩阵输出文件打开失败!" << endl;
		exit(-1);
	}

	for(m = 0; m < nTotalElem; m++)
	{
		switch((pElem + m)->iType)
		{
			case TRUSS:
				TrussElemStiffCalcu(fout1, pElem + m, pMate, pSect, pKe0); iNode0 = (pElem + m)->iaNode[0];
				iNode1 = (pElem + m)->iaNode[1];
				for(i = 0; i < 2; i++)
				{
					iaDOFIndex[i] = (pNode + iNode0)->iaDOFIndex[i];   iaDOFIndex[i + 2] = (pNode + iNode1)->iaDOFIndex[i];
				}
				for(i = 0; i < 4; i++)
				{
					GKi = iaDOFIndex[i];
					for(j = 0; j < 4; j++)
					{
						GKj = iaDOFIndex[j];
						if(GKi >= GKj)
						{
							GKij = pDiag[GKi] - GKi + GKj;
							pGK[GKij] += pKe0[i][j];
						}
					}
				}
				break;

			case FRAME:
				FrameElemStiffCalcu(fout1, pElem + m, pMate, pSect, pKe1); iNode0 = (pElem + m)->iaNode[0];
				iNode1 = (pElem + m)->iaNode[1];
				for(i = 0; i < 3; i++)         //形成单元定位向量
				{
					iaDOFIndex[i] = (pNode + iNode0)->iaDOFIndex[i];   iaDOFIndex[i + 3] = (pNode + iNode1)->iaDOFIndex[i];
				}
				for(i = 0; i < 6; i++)
				{
					GKi = iaDOFIndex[i];
					for(j = 0; j < 6; j++)
					{
						GKj = iaDOFIndex[j];
						if(GKi >= GKj)
						{
							GKij = pDiag[GKi] - GKi + GKj;
							pGK[GKij] += pKe1[i][j];
						}
					}
				}
		}
	}

	TwoArrayFree(4, pKe0);
	TwoArrayFree(6, pKe1);
	fout1.close();
}
//装配载荷向量---------------------------------------------
void LoadVectorAssembly(int nLoad, int nTotalDOF, int nFreeDOF, int* pDiag, double* pGK, Element* pElem, Material* pMate,
					   Section* pSect, Load* pLoad, Node* pNode, double* pLoadVect, double* pDisp)
{
	int i, vi;
	int    iLoadType, iNode0, iNode1, iXDOFIndex, iYDOFIndex, iRDOFIndex,iLoadedElem;
	double* pFixedEndF = new double[6];
	int iaDOFIndex[6];

	for(i = 0; i < nLoad; i++)
	{
		iLoadType = (pLoad + i)->iType;
		switch(iLoadType)
		{
			case SUPPORT_MOVE://处理支座位移
				iNode0 = (pLoad + i)->iLoadedNode;
				if((pLoad + i)->iDirect == DIRECT_X)
				{
					iXDOFIndex = (pNode + iNode0)->iaDOFIndex[0]; pDisp[iXDOFIndex] += (pLoad + i)->dValue;
				}
				else if((pLoad + i)->iDirect == DIRECT_Y)
				{
					iYDOFIndex = (pNode + iNode0)->iaDOFIndex[1]; pDisp[iYDOFIndex] += (pLoad + i)->dValue;
				}
				else
				{
					iRDOFIndex = (pNode + iNode0)->iaDOFIndex[2]; pDisp[iRDOFIndex] += (pLoad + i)->dValue;
				}
				LoadVectorModify(nTotalDOF, nFreeDOF, pDiag, pGK, pDisp, pLoadVect); break;
			case FORCE_ON_NODE://处理节点力
				iNode0 = (pLoad + i)->iLoadedNode;
				if((pLoad + i)->iDirect == DIRECT_X)
				{
					iXDOFIndex = (pNode + iNode0)->iaDOFIndex[0]; pLoadVect[iXDOFIndex] += (pLoad + i)->dValue;
				}
				else if((pLoad + i)->iDirect == DIRECT_Y)
				{
					iYDOFIndex = (pNode + iNode0)->iaDOFIndex[1]; pLoadVect[iYDOFIndex] += (pLoad + i)->dValue;
				}
				else
				{

					iRDOFIndex = (pNode + iNode0)->iaDOFIndex[2];
					pLoadVect[iRDOFIndex] += (pLoad + i)->dValue;
				}
				break;
			case LATERAL_FORCE:				//处理梁上横向节点力
			case LATERAL_UNIFORM_PRESSURE:	//处理梁上横向均布力
			case MOMENT_ON_A_POINT:			//处理梁上集中力矩
			case LATERAL_LINEARLY_PRESSURE:	//处理梁上横向三角形分布力
			case AXIAL_PRESSURE:			//处理梁上轴向均布力
			case AXIAL_FORCE:				//处理梁上轴上集中力
			case MOMENT_ON_BEAM:			//处理梁上分布力矩
			case TEMPERATURE:				//处理梁上温度载荷
				FixedEndForceCalcu(pElem, pMate, pSect, pLoad + i, pFixedEndF, i);
				iLoadedElem = (pLoad + i)->iLoadedElem;
				iNode0 = (pElem + iLoadedElem)->iaNode[0];
				iNode1 = (pElem + iLoadedElem)->iaNode[1];
				for(int j = 0; j < 3; j++) //形成单元定位向量
				{
					iaDOFIndex[j] = (pNode + iNode0)->iaDOFIndex[j]; 
					iaDOFIndex[j + 3] = (pNode + iNode1)->iaDOFIndex[j];
				}
				for(int j = 0; j < 6; j++)
				{
					vi = iaDOFIndex[j];
					pLoadVect[vi] += pFixedEndF[j];
				}
				break;
		}
	}
	for(i = 0; i < nFreeDOF; i++)
		pDisp[i] = pLoadVect[i];

	delete[]pFixedEndF;
}
//计算单元固端力-------------------------------------------  
void FixedEndForceCalcu(Element* pElem,Material* pMate,Section* pSect,Load* pLoad,double* pFixedEndF,int i)
{
	int iMateType, iSectType, iLoadType, iLoadedElem;
	double dE, dAlpha;
	double dA, dIz, dH;
	double dt0, dt1, dBuf;
	double da, dQ, dc, dg, dl, db, ds;

	double** pT = TwoArrayDoubAlloc(6, 6);
	double** pTT = TwoArrayDoubAlloc(6, 6);
	double* pTemp = new double[6];

	double dXi = 0;
	double dXj = 0;
	double dYi = 0;
	double dYj = 0;
	double dMi = 0;
	double dMj = 0;

	iLoadedElem = pLoad->iLoadedElem;
	iLoadType = pLoad->iType;

	da = pLoad->dPosition;
	dl = (pElem + iLoadedElem)->dLength;
	dQ = pLoad->dValue;
	dc = da / dl;
	dg = dc * dc;
	db = dl - da;
	switch(iLoadType)
	{
		case LATERAL_FORCE:
			ds = db / dl;
			dYi = -dQ * ds * ds * (1.0 + 2.0 * dc);
			dYj = -dQ * dg * (1.0 + 2.0 * ds);
			dMi = -dQ * ds * ds * da;
			dMj = dQ * db * dg;
			break;
		case LATERAL_UNIFORM_PRESSURE:
			ds = dQ * da * 0.5;
			dYi = -ds * (2.0 - 2.0 * dg + dc * dg);
			dYj = -ds * dg * (2.0 - dc);

			ds = ds * da / 6.0;
			dMi = -ds * (6.0 - 8.0 * dc + 3.0 * dg);
			dMj = ds * dc * (4.0 - 3.0 * dc);
			break;
		case MOMENT_ON_A_POINT:
			ds = db / dl;
			dYi = -6.0 * dQ * dc * ds / dl;
			dYj = -dYi;
			dMi = dQ * ds * (2.0 - 3.0 * ds);
			dMj = dQ * dc * (2.0 - 3.0 * dc);
			break;
		case LATERAL_LINEARLY_PRESSURE:
			ds = dQ * da * 0.25;
			dYi = -ds * (2.0 - 3.0 * dg + 1.6 * dg * dc);
			dYj = -ds * dg * (3.0 - 1.6 * dc);
			ds *= da;
			dMi = ds * (2.0 - 3.0 * dc + 1.2 * dg) / 1.5;
			dMj = -ds * dc * (1.0 - 0.5 * dc);
			break;
		case AXIAL_PRESSURE:
			dXi = -dQ * da * (1.0 - 0.5 * dc);
			dXj = -0.5 * dQ * dc * da;
			break;
		case AXIAL_FORCE:

			dXj = -dQ * dc;
			break;
		case MOMENT_ON_BEAM:
			ds = db / dl;
			dYi = -dQ * dg * (3.0 * ds + dc);
			dYj = -dYi;
			ds *= db / dl;
			dMi = -dQ * ds * da;
			dMj = dQ * dg * db;
			break;
		case TEMPERATURE:
			iMateType = (pElem + iLoadedElem)->iMaterial;
			dE = (pMate + iMateType)->dE;
			dAlpha = (pMate + iMateType)->dAlpha;

			iSectType = (pElem + iLoadedElem)->iSection;
			dA = (pSect + iSectType)->dA;
			dIz = (pSect + iSectType)->dIz;
			dH = (pSect + iSectType)->dH;
			dt0 = pLoad->dT0;
			dt1 = pLoad->dT1;
			dBuf = 0.5 * dAlpha * (dt0 + dt1) * dE * dA;
			dXi = dBuf;
			dXj = -dBuf;
			dBuf = dAlpha * (dt1 - dt0) * dE * dIz / dH;
			dMi = dBuf;
			dMj = -dBuf;
			break;
	}

	(pElem + iLoadedElem)->daEndInterForce[1] = dYi;
	(pElem + iLoadedElem)->daEndInterForce[4] = dYj;
	(pElem + iLoadedElem)->daEndInterForce[2] = dMi;
	(pElem + iLoadedElem)->daEndInterForce[5] = dMj;

	VectorZeroize(6, pTemp);
	pTemp[0] = 0.0;
	pTemp[1] = -dYi;
	pTemp[2] = -dMi;
	pTemp[3] = 0.0;
	pTemp[4] = -dYj;
	pTemp[5] = -dMj;

	MatrixZeroize(6, 6, pT);
	pT[2][2] = pT[5][5] = 1.0;
	pT[0][0] = pT[1][1] = pT[3][3] = pT[4][4] = (pElem + iLoadedElem)->dCos;
	pT[0][1] = pT[3][4] = (pElem + iLoadedElem)->dSin;
	pT[1][0] = pT[4][3] = -(pElem + iLoadedElem)->dSin;

	MatrixTrans(6, 6, pT, pTT);
	MatrixVectorMultiply(6, 6, pTT, pTemp, pFixedEndF);

	TwoArrayFree(6, pT);
	TwoArrayFree(6, pTT);

	delete[]pTemp;
}

//初始化单元杆端力向量--------------------------------------- 
void ElementEndForceInit(int nTotalElem,Element* pElem)
{
	for(int i=0;i<nTotalElem;i++)
		VectorZeroize(6,(pElem+i) ->daEndInterForce);
}
//取总刚度矩阵带内元素----------------------------------------
double GetElementInGK(int nRow, int iRow, int iCol, int* pDiag, double* pGK)
{
	int iBuf;
	int nHalfBand;
	double dBuf;
	if(iRow > nRow || iCol > nRow)
	{
		cout << "行或列越界!" << endl;
		dBuf = 0.0;
		return dBuf;
	}
	if(iRow >= iCol)
	{
		if(iRow == 0)
			nHalfBand = 1;
		else
			nHalfBand = pDiag[iRow] - pDiag[iRow - 1];
		iBuf = iRow - iCol;
		if(nHalfBand <= iBuf)
		{
			dBuf = 0.0;
			return dBuf;
		}
		else
			return pGK[pDiag[iRow] - iBuf];
	}
	else
	{
		nHalfBand = pDiag[iCol] - pDiag[iCol - 1];

		iBuf = iCol - iRow;
		if(nHalfBand <= iBuf)
		{
			dBuf = 0.0;
			return dBuf;
		}
		else
			return pGK[pDiag[iCol] - iBuf];
	}
}
//加载支座位移-------------------------------------------       
void LoadVectorModify(int nTotalDOF, int nFreeDOF, int* pDiag, double* pGK, double* pDisp, double* pLoadVect)
{
	int i, j;
	double dBuf;
	for(i = nFreeDOF; i < nTotalDOF; i++)	//在受约束自由度范围内搜索
	{
		if(pDisp[i] != 0.0)	//若有支座位移
		{
			for(j = 0; j < nFreeDOF; j++)
			{
				dBuf = GetElementInGK(nTotalDOF, i, j, pDiag, pGK); if(dBuf != 0.0)
					pLoadVect[j] -= dBuf * pDisp[i];//修改载荷向量
			}
		}
	}
}
//计算单元杆端力----------------------------------------       
void InternalForceCalcu(int nTotalElem, Element* pElem, Node* pNode, double* pDisp)
{
	int i, j, e;
	double** pKe0 = TwoArrayDoubAlloc(4, 4);
	double** pKe1 = TwoArrayDoubAlloc(6, 6);
	ifstream fin1("ElemStiff.dat", ios::binary);
	if(!fin1)
	{
		cout << "单元刚度矩阵输入文件打开失败!" << endl;
		exit(-1);
	}
	for(e = 0; e < nTotalElem; e++)
	{
		if((pElem + e)->iType == TRUSS)
		{
			for(i = 0; i < 4; i++)
				for(j = 0; j < 4; j++)
					fin1.read((char*)&pKe0[i][j], sizeof(double)); TrussInternalForceCalcu(pElem + e, pNode, pDisp, pKe0);
		}
		else
		{
			for(i = 0; i < 6; i++)
				for(j = 0; j < 6; j++)
					fin1.read((char*)&pKe1[i][j], sizeof(double));       FrameInternalForceCalcu(pElem + e, pNode, pDisp, pKe1);
		}
	}
	TwoArrayFree(4, pKe0);
	TwoArrayFree(6, pKe1);
	fin1.close();
	//if(remove("ElemStiff.dat"))
	//{
	//	cout << "删除文件错!" << endl;
	//	exit(-1);
	//}
}
//计算桁架单元内力----------------------------------------    
void TrussInternalForceCalcu(Element* pElem, Node* pNode, double* pDisp, double** pKe)
{
	int i;
	int iNode0, iNode1;
	int iaDOFIndex[4];
	double daEndDisp[4];
	double daLocalDisp[4];
	double daEndForce[4];

	double** pT = TwoArrayDoubAlloc(4, 4);
	iNode0 = pElem->iaNode[0];
	iNode1 = pElem->iaNode[1];
	for(i = 0; i < 2; i++)
	{
		iaDOFIndex[i] = (pNode + iNode0)->iaDOFIndex[i];
		iaDOFIndex[i + 2] = (pNode + iNode1)->iaDOFIndex[i];
	}
	for(i = 0; i < 4; i++)
		daEndDisp[i] = pDisp[iaDOFIndex[i]];

	MatrixZeroize(4, 4, pT);
	pT[0][0] = pT[1][1] = pT[2][2] = pT[3][3] = pElem->dCos;
	pT[0][1] = pT[2][3] = pElem->dSin;
	pT[1][0] = pT[3][2] = -pElem->dSin;
	MatrixVectorMultiply(4, 4, pT, daEndDisp, daLocalDisp);

	MatrixVectorMultiply(4, 4, pKe, daLocalDisp, daEndForce);
	for(i = 0; i < 2; i++)
	{
		pElem->daEndInterForce[i] += daEndForce[i];
		pElem->daEndInterForce[i + 3] += daEndForce[i + 2];
	}
	TwoArrayFree(4, pT);
}
//计算刚架单元杆端内力---------------------------------------   
void FrameInternalForceCalcu(Element* pElem, Node* pNode, double* pDisp, double** pKe)
{
	int i;
	int iNode0, iNode1;
	int iaDOFIndex[6];
	double daEndDisp[6];
	double daLocalDisp[6];
	double daEndForce[6];

	double** pT = TwoArrayDoubAlloc(6, 6);
	iNode0 = pElem->iaNode[0];

	iNode1 = pElem->iaNode[1];
	for(i = 0; i < 3; i++)
	{
		iaDOFIndex[i] = (pNode + iNode0)->iaDOFIndex[i];
		iaDOFIndex[i + 3] = (pNode + iNode1)->iaDOFIndex[i];
	}
	for(i = 0; i < 6; i++)
		daEndDisp[i] = pDisp[iaDOFIndex[i]];
	MatrixZeroize(6, 6, pT);
	pT[2][2] = pT[5][5] = 1.0;
	pT[0][0] = pT[1][1] = pT[3][3] = pT[4][4] = pElem->dCos;
	pT[0][1] = pT[3][4] = pElem->dSin;
	pT[1][0] = pT[4][3] = -pElem->dSin;
	MatrixVectorMultiply(6, 6, pT, daEndDisp, daLocalDisp);
	MatrixVectorMultiply(6, 6, pKe, daLocalDisp, daEndForce);

	for(i = 0; i < 6; i++)
		pElem->daEndInterForce[i] += daEndForce[i];

	TwoArrayFree(6, pT);
}
//计算支座反力---------------------------------------------       
void SupportReactionCalcu(int   nTotalDOF, int nFreeDOF, int* pDiag, double* pGK, double* pDisp, double* pLoadVect)
{
	int i, j;
	double dBuf;
	for(i = nFreeDOF; i < nTotalDOF; i++)
	{
		pLoadVect[i] = -pLoadVect[i];
		for(j = 0; j < nTotalDOF; j++)
		{
			dBuf = GetElementInGK(nTotalDOF, i, j, pDiag, pGK);
			if(dBuf != 0.0)
				pLoadVect[i] += dBuf * pDisp[j];
		}
	}
}
//输出结点位移---------------------------------------------
void NodeDisplOutput(ofstream& fout, int nTotalNode, Node* pNode, double* pDisp) 
{
	int i;
	int iDOFXIndex, iDOFYIndex, iDOFRIndex;
	cout << setiosflags(ios::right);
	cout << endl << endl;
	cout << setw(48) << "====================          节      点      位      移 ================================" << endl;
	cout << endl;
	cout << setw(15) << "结点号" << setw(15) << "X方向" << setw(15) << "Y方向" << setw(15) << "R方向"
		<< endl;

	fout << setiosflags(ios::right);
	fout << endl << endl;
	fout << setw(48) << "====================           节     点     位     移 ================================" << endl;
	fout << endl;
	fout << setw(15) << "结点号" << setw(15) << "X方向" << setw(15) << "Y方向" << setw(15) << "R方向"
		<< endl;

	cout << setiosflags(ios::scientific);
	cout.precision(2);
	fout << setiosflags(ios::scientific);
	fout.precision(2);
	for(i = 0; i < nTotalNode; i++)
	{
		iDOFXIndex = (pNode + i)->iaDOFIndex[0];
		iDOFYIndex = (pNode + i)->iaDOFIndex[1];
		iDOFRIndex = (pNode + i)->iaDOFIndex[02];
		if((pNode + i)->iType == TRUSS)
		{

			cout << setw(15) << i;
			cout << setw(15) << pDisp[iDOFXIndex]
				<< setw(15) << pDisp[iDOFYIndex]
				<< setw(15) << "" << endl;
			fout << setw(15) << i;

			fout << setw(15) << pDisp[iDOFXIndex]
				<< setw(15) << pDisp[iDOFYIndex]
				<< setw(15) << "" << endl;
		}
		else
		{
			cout << setw(15) << i;
			cout << setw(15) << pDisp[iDOFXIndex]
				<< setw(15) << pDisp[iDOFYIndex]
				<< setw(15) << pDisp[iDOFRIndex] << endl;

			fout << setw(15) << i;
			fout << setw(15) << pDisp[iDOFXIndex]
				<< setw(15) << pDisp[iDOFYIndex]
				<< setw(15) << pDisp[iDOFRIndex] << endl;
		}
	}
}
//输出杆端内力------------------------------------------- 
void EndInternalForceOutput(ofstream& fout, int nTotalElem, Element* pElem)
{
	int e;
	cout << endl;
	cout << setw(48) << "====================           杆     端     内     力 ==========================================" << endl << endl;
	cout << setw(10) << "单元号" << setw(10) << "始端轴力" << setw(10) << "始端剪力" << setw(10) << "始端弯矩"
		<< setw(10) << "终端轴力" << setw(10) << "终端剪力" << setw(10) << "终端弯矩"
		<< endl;

	fout << endl;
	fout << setw(48) << "====================           杆     端     内     力 ==========================================" << endl << endl;
	fout << setw(10) << "单元号" << setw(10) << "始端轴力" << setw(10) << "始端剪力" << setw(10) << "始端弯矩"
		<< setw(10) << "终端轴力" << setw(10) << "终端剪力" << setw(10) << "终端弯矩"
		<< endl;
	cout.setf(ios::fixed, ios::floatfield);
	fout.setf(ios::fixed, ios::floatfield);

	for(e = 0; e < nTotalElem; e++)
	{
		if((pElem + e)->iType == 0)
		{
			cout << setw(10) << e;
			cout << setw(10) << (pElem + e)->daEndInterForce[0]
				<< setw(10) << "" << setw(10) << ""
				<< setw(10) << (pElem + e)->daEndInterForce[3]
				<< setw(10) << "" << setw(10) << ""
				<< endl;
			fout << setw(10) << e;

			fout << setw(10) << (pElem + e)->daEndInterForce[0]
				<< setw(10) << "" << setw(10) << ""
				<< setw(10) << (pElem + e)->daEndInterForce[3]
				<< setw(10) << "" << setw(10) << ""
				<< endl;
		}
		else
		{
			cout << setw(10) << e;
			cout.setf(ios::fixed);
			cout << setprecision(3);
			cout << setw(10) << (pElem + e)->daEndInterForce[0]
				<< setw(10) << (pElem + e)->daEndInterForce[1]
				<< setw(10) << (pElem + e)->daEndInterForce[2]
				<< setw(10) << (pElem + e)->daEndInterForce[3]
				<< setw(10) << (pElem + e)->daEndInterForce[4]
				<< setw(10) << (pElem + e)->daEndInterForce[5]
				<< endl;

			fout << setw(10) << e;
			fout.setf(ios::fixed);
			fout << setprecision(3);
			fout << setw(10) << (pElem + e)->daEndInterForce[0]
				<< setw(10) << (pElem + e)->daEndInterForce[1]
				<< setw(10) << (pElem + e)->daEndInterForce[2]

				<< setw(10) << (pElem + e)->daEndInterForce[3]
				<< setw(10) << (pElem + e)->daEndInterForce[4]
				<< setw(10) << (pElem + e)->daEndInterForce[5]
				<< endl;
		}
	}
}
//输出支座反力--------------------------------------------     
void SupportReactionOutput(ofstream& fout, int nConstrainedNode, ConstrainedNode* pConsNode, Node* pNode, double* pLoadVect)
{
	int i;
	int iNode, iDOFXIndex, iDOFYIndex, iDOFRIndex;
	cout << endl;
	cout << setw(48) << "====================           支     座     反     力 ====================" << endl << endl;
	cout << setw(12) << "支座节点号" << setw(12) << "X向反力" << setw(12) << "Y向反力"
		<< setw(12) << "R向反力" << endl;
	fout << endl;
	fout << setw(48) << "====================           支     座     反     力 ====================" << endl << endl;
	fout << setw(12) << "支座节点号" << setw(12) << "X向反力" << setw(12) << "Y向反力"
		<< setw(12) << "R向反力" << endl;
	for(i = 0; i < nConstrainedNode; i++)
	{
		iNode = (pConsNode + i)->iNode;
		iDOFXIndex = (pNode + iNode)->iaDOFIndex[0];
		iDOFYIndex = (pNode + iNode)->iaDOFIndex[1];
		iDOFRIndex = (pNode + iNode)->iaDOFIndex[2];
		if((pConsNode + i)->iaConstrainedDOF[0] == -1 || (pConsNode + i)->iaConstrainedDOF[1] == -1 || (pConsNode + i)->iaConstrainedDOF[2] == -1)
		{
			if((pNode + iNode)->iType == TRUSS)
			{
				cout << setw(12) << iNode
					<< setw(12) << pLoadVect[iDOFXIndex]
					<< setw(12) << pLoadVect[iDOFYIndex]

					<< setw(12) << ""
					<< endl;

				fout << setw(12) << iNode
					<< setw(12) << pLoadVect[iDOFXIndex]
					<< setw(12) << pLoadVect[iDOFYIndex]
					<< setw(12) << ""
					<< endl;
			}
			else
			{
				cout << setw(12) << iNode
					<< setw(12) << pLoadVect[iDOFXIndex]
					<< setw(12) << pLoadVect[iDOFYIndex]
					<< setw(12) << pLoadVect[iDOFRIndex]
					<< endl;

				fout << setw(12) << iNode
					<< setw(12) << pLoadVect[iDOFXIndex]
					<< setw(12) << pLoadVect[iDOFYIndex]
					<< setw(12) << pLoadVect[iDOFRIndex]
					<< endl;
			}
		}
	}
}

#pragma endregion TestMITC

