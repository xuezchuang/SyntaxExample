

int main()
{
	CStringA strA;
	strA = "�������ṹ�ɿ������ͳһ��׼��";
	int nA = strA.GetLength();

	CStringW strW;
	strW = _T("�������ṹ�ɿ������ͳһ��׼��");
	int nW = strW.GetLength();
}

