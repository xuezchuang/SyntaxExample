

int main()
{
	CStringA strA;
	strA = "《建筑结构可靠度设计统一标准》";
	int nA = strA.GetLength();

	CStringW strW;
	strW = _T("《建筑结构可靠度设计统一标准》");
	int nW = strW.GetLength();
}

