#pragma once
#include <deque>

template<class _Ty, class ARG_TYPE = const _Ty&> 
class ADArray
{
public:
	ADArray(void)
	{

	}

	ADArray(const ADArray& src)
	{
		*this = src;
	}

	ADArray(long _Count)
	{
		m_dqData.resize(_Count);
	}

	ADArray(long _Count,ARG_TYPE _Val )
	{
		m_dqData._Construct_n(_Count,_Val);
	}
	ADArray& operator=(const ADArray& src)
	{
		if (this != &src)
		{
			m_dqData = src.m_dqData;
		}

		return *this;
	}

	~ADArray(void)
	{

	}
public:

	long GetSize() const
	{
		return (long)m_dqData.size();
	}
	long GetCount() const
	{
		return (long)m_dqData.size();
	}

	long GetUpperBound() const
	{
		return (long)(m_dqData.size()-1);
	}
	void SetSize(long nNewSize)
	{
		m_dqData.resize(nNewSize);
	}
	BOOL IsEmpty() const
	{
		return m_dqData.empty() ? TRUE:FALSE;
	}
	void FreeExtra()
	{
		m_dqData.shrink_to_fit();
	}
	void RemoveAll()
	{
		m_dqData.clear();
	}
	
	const _Ty& GetAt(long nIndex) const
	{
		return m_dqData.at(nIndex);
	}

	 _Ty& GetAt(long nIndex)
	{
		return m_dqData.at(nIndex);
	}

	void SetAt(long nIndex, ARG_TYPE newElement)
	{
		m_dqData.at(nIndex) = newElement;
	}

	const _Ty& ElementAt(long nIndex) const
	{
		return m_dqData.at(nIndex);
	}

	_Ty& ElementAt(long nIndex)
	{
		return m_dqData.at(nIndex);
	}

// 	const TYPE* GetData() const;
// 	TYPE* GetData();
//
//	void SetAtGrow(SEC_INT nIndex, ARG_TYPE newElement);
	long Add(ARG_TYPE newElement)
	{
		m_dqData.push_back(newElement);

		return GetUpperBound();
	}
	long Append(const ADArray& src)
	{
		m_dqData.push_back(src.m_dqData.begin(),src.m_dqData.end());

		return GetUpperBound();
	}
	void Copy(const ADArray& src)
	{
		*this = src;
	}

	const _Ty& operator[](long nIndex) const
	{
		return m_dqData[nIndex];
	}

	_Ty& operator[](long nIndex)
	{
		return m_dqData[nIndex];
		//return m_dqData.at(nIndex);
	}

	void RemoveAt(long nIndex, long nCount = 1)
	{
		m_dqData.erase(m_dqData.begin()+nIndex,m_dqData.begin()+nIndex+nCount);
	}

	void InsertAt(long nIndex, ARG_TYPE newElement, long nCount = 1)
	{
		m_dqData.insert(m_dqData.begin()+nIndex,nCount,newElement);
	}

	void InsertAt(long nStartIndex, const ADArray& arNewArray)
	{
		m_dqData.insert(m_dqData.begin()+nStartIndex,arNewArray.m_dqData);
	}

protected:
	std::deque<_Ty> m_dqData;
};

typedef ADArray<double,double>             ADArrayd  ;
typedef ADArray<int,int>                   ADArrayi  ;
typedef ADArray<unsigned int,unsigned int> ADArrayui ;
typedef ADArray<long,long>                 ADArrayl  ;
typedef ADArray<short,short>               ADArrays  ;
//typedef ADArray<CString,const CString&>    ADArrayString;

