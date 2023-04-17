

#include <iostream>
//#include <windows.h>
#include <afxtempl.h>

double mathDotNormal(double xyz1[2], double xyz2[2])
{
	double dblDot = xyz1[0] * xyz2[0] + xyz1[1] * xyz2[1] /*+ xyz1[2] * xyz2[2]*/;
	if (dblDot > 1.)dblDot = 1.;
	if (dblDot < -1.)dblDot = -1.;
	return dblDot;
}

double mathDot(double xyz1[2], double xyz2[2])
{
	double d1 = xyz1[0] * xyz1[0] + xyz1[1] * xyz1[1];
	double d2 = xyz2[0] * xyz2[0] + xyz2[1] * xyz2[1];
	for (int i = 0; i < 2; i++)
	{
		xyz1[i] = xyz1[i] / d1;
		xyz2[i] = xyz2[i] / d2;
	}
	return mathDotNormal(xyz1, xyz2);
}
//
//double mathAcos(double x)
//{
//	if (x < -1)x = -1.;
//	if (x > 1)x = 1.;
//	return acos(x);
//};
//
//double mathCrossAngle(double Vector1[3], double Vector2[3])
//{
//	double d1 = Vector1[0] * Vector1[0] + Vector1[1] * Vector1[1] + Vector1[2] * Vector1[2];
//	double d2 = Vector2[0] * Vector2[0] + Vector2[1] * Vector2[1] + Vector2[2] * Vector2[2];
//	d1 = sqrt(d1);
//	d2 = sqrt(d2);
//	double dTol = 1.0e-10;
//	if (fabs(d1) < dTol) { return 0; }
//	if (fabs(d2) < dTol) { return 0; }
//	double NV1[3], NV2[3];
//	for (int i = 0; i < 3; i++)
//	{
//		NV1[i] = Vector1[i] / d1;
//		NV2[i] = Vector2[i] / d2;
//	}
//
//	double m_pi = 4.0 * atan(1.0);
//	double m_trang = 180. / m_pi;
//	double v = fabs(mathDot(NV1, NV2));
//	double r = mathAcos(v);
//	return r * m_trang;
//}
//
//#include "ADArray.hpp"

#include <vector>

struct _UMD_RC_PM_UNIT
{
	double dPhi;
	double dXn;
	double dPn;
	double dMny;
	double dMnz;
	double desi;
	void Initialize()
	{
		dPhi = 0.0;
		dXn = 0.0;
		dPn = 0.0;
		dMny = 0.0;
		dMnz = 0.0;
		desi = 0.0;
	}
	_UMD_RC_PM_UNIT() {}
	_UMD_RC_PM_UNIT(const _UMD_RC_PM_UNIT& rData) { *this = rData; }
	_UMD_RC_PM_UNIT& operator = (const _UMD_RC_PM_UNIT& rData)
	{
		dPhi = rData.dPhi;
		dXn = rData.dXn;
		dPn = rData.dPn;
		dMny = rData.dMny;
		dMnz = rData.dMnz;
		desi = rData.desi;

		return *this;
	}

	~_UMD_RC_PM_UNIT()
	{

		int a = 0;
	}
};

struct _UMD_RC_PM
{
	double dRotate;// 吝赋绵 雀傈阿(Radian)
	double dDmax;
	double dDeff;
	//_UMD_RC_PM_UNIT Pmemin;
	//CArray<int, int&> arPmUnit;

	void Initialize()
	{
		dRotate = dDmax = dDeff = 0.0;
		//Pmemin.Initialize();
		//arPmUnit.RemoveAll();
	}
	_UMD_RC_PM() {}
	_UMD_RC_PM(const _UMD_RC_PM& PmData) { *this = PmData; }
	_UMD_RC_PM& operator = (const _UMD_RC_PM& PmData)
	{
		Initialize();
		dRotate = PmData.dRotate;
		dDmax = PmData.dDmax;
		dDeff = PmData.dDeff;
		//Pmemin = PmData.Pmemin;
		//arPmUnit.Copy(PmData.arPmUnit);

		return *this;
	}
	~_UMD_RC_PM()
	{

		int a = 0;
	}
};

using namespace std;
struct TRealPMData
{
	// 绵仿捞 弥措老锭
	//ADArray<int> arLcomData;
	std::vector<int> arLcomData;
	//std::deque<_UMD_RC_PM_UNIT> arLcomData;

	TRealPMData() { Init(); }

	TRealPMData(const TRealPMData& PmData)
	{
		*this = PmData;
		//arLcomData._Myproxy->_Mycont->_Myproxy = arLcomData._Myproxy;
		 //PmData.arLcomData._Myproxy->_Mycont  ->_Myproxy = arLcomData._Myproxy;
		 //_Container_base12* pError = const_cast<_Container_base12*>(PmData.arLcomData._Myproxy->_Mycont);
		 //pError->_Myproxy = PmData.arLcomData._Myproxy;
		//for(int i = 0;i < PmData.arLcomData.size();i++)
		//{
		//	arLcomData[i] = PmData.arLcomData[i];
		//}
		//
		//int a =0;
	}
	TRealPMData& operator = (const TRealPMData& PmData)
	{
		//arLcomData.RemoveAll();
		//arLcomData.Copy(PmData.arLcomData);
		arLcomData = PmData.arLcomData;
		return *this;
	}

	void Init()
	{
		//arLcomData.RemoveAll();
	}
};

class CDgnCalcColumn
{
public:
	CDgnCalcColumn() {}

	BOOL test(_UMD_RC_PM& a);

	//ADArray<_UMD_RC_PM> m_aTest;
};
BOOL CDgnCalcColumn::test(_UMD_RC_PM& a)
{
	//a = m_aTest[0];
	return TRUE;
}

struct _m_Container_base12;

struct _m_Container_proxy
{
	_m_Container_base12* _Mycont;

};

struct _m_Container_base12
{
	_m_Container_proxy* _Myproxy;
};


class aa
{

public:
	aa()
	{


	}
	_m_Container_base12	 mcon;
	//_m_Container_proxy *_Myproxy;
	~aa()
	{
	}
};


int main()
{

	CArray<aa> aTest;
	{
		aa mma;
		mma.mcon._Myproxy = new _m_Container_proxy();
		mma.mcon._Myproxy->_Mycont = &mma.mcon;
		//mma._Myproxy=mma.mcon._Myproxy;
		aTest.Add(mma);
		aTest.Add(mma);

	}
	//aa bbh = aTest[0];
	//*(aTest[0].p) = 3;


	////std::deque<int> _bstlTest;
	//
	//{
	//	std::deque<int> _stlTest;
	//	_stlTest.push_back(3);
	//	_stlTest.push_back(5);
	//	_bstlTest = _stlTest;
	//}
	//int bb = _bstlTest[0];
	//CArray<TRealPMData, TRealPMData&> atest;
	CArray<TRealPMData> atest;
	{
		TRealPMData c;
		//c.arLcomData.SetSize(3);
		c.arLcomData.resize(3);
		c.arLcomData[0] = 3;
		c.arLcomData[1] = 4;
		c.arLcomData[2] = 5;
		atest.Add(c);

		//c.arLcomData.resize(2);

		atest.Add(c);
		int a = 4;
	}
	//atest.SetSize(2);
	//int a = atest[0].arLcomData[0];
	TRealPMData aba;
	aba = atest[0];
	//for(auto it: atest[0])
	//for (std::vector<int>::iterator it = atest[0].arLcomData.begin(); it != atest[0].arLcomData.end(); it++)
	{

		int a = 0;
	}

	CDgnCalcColumn Dgntest;
	//Dgntest.m_aTest.SetSize(2);
	//_UMD_RC_PM aa;// = 0;
	//Dgntest.test(aa);

	//double v1[3] = { 0,0,1.0 };
	//double v2[3] = { 1,0,2.0 };
	//double dangle = mathCrossAngle(v1, v2);

	//double v1[2] = { 1,0,};
	//double v2[2] = { 1,-1};
	////0,-1 : -1
	////1,-1 : -0.5
	////1,-1 : 0
	////1,1 : 0.5
	////0,: 1.0

	////
	//double dangle = mathDot(v1, v2);



	std::cout << "Hello World!\n";
}

//CDN
class CDgnCalcBase_RCDesign_Base
{
}
class CDgnCalcBase_RCDesign_USD : public CDgnCalcBase_RCDesign_Base
{
}
class CDgnCalcBase_RC_Column_Base_USD_LSD : public CDgnCalcBase_RCDesign_USD
{
}
class  CDgnCalcBase_RC_Column_LSD : public CDgnCalcBase_RC_Column_Base_USD_LSD
{
}

class CDgnCalc_CH_RCSC_Column_JTG : public CDgnCalcBase_RC_Column_LSD
{

};

//GSD
class CDgnCalcBase_SRC_Column : public CDgnCalcBase_RC_Column_Base_USD_LSD
{
}

class CDgnCalc_GSD_SRC_LSD : public CDgnCalcBase_SRC_Column
{
}

//BLD双偏压柱验算

class CDgnCalc_CH_RCS_Column : public CDgnCalcBase_RC_Column_LSD
{

};

//钢筋
struct _UMD_RC_COL_MBAR_DATA_EC
{
}
struct _UMD_RC_COL_RBAR_EC
{
	double dDcDgn;  // to Calc. Req.RbarArea.
	int nHoopType;  // 1:Tied 2:Spiral
	//
	_UMD_RC_COL_MBAR_DATA_EC  MainRbarData;
	//_UMD_RC_COL_SBAR_DATA_EC  SubRbarData;
}