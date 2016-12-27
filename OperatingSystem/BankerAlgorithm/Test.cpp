#include "BankerAlgorithm.h"

void Test1()
{
	int processNum, resourceNum;
	cout << "���������������";
	cin >> processNum;
	cout << "��������Դ���ࣺ";
	cin >> resourceNum;

	Banker b(processNum, resourceNum);
	b.Print();
	int* safeArr = new int[processNum];
	if (b.SafetyAlgorithm(safeArr))
	{
		cout << "��ǰ״̬��ȫ,��ȫ����Ϊ��";
		for (int i = 0; i < processNum; ++i)
		{
			cout << safeArr[i] << " ";
		}
		cout << endl;
	}
	else
	{
		cout << "��ǰ״̬����ȫ" << endl;
		return;
	}
	
	int* request = new int[resourceNum];//������Դ����
	cout << "����Ҫ���������Դ�� Y or N" << endl;
	char ch;
	cin >> ch;
	int pNO;
	while (ch == 'y' || ch == 'Y')
	{
		cout << "������������Դ�Ľ��̺�0~" << processNum - 1 << ":";
		cin >> pNO;
		cout << "������������Դ������" << endl;
		for (int i = 0; i < resourceNum; ++i)
		{
			cin >> request[i];
		}
		if (b.BankerAlgorithm(pNO, request, resourceNum))
		{
			cout << "����ɹ�������T1ʱ��" << endl;
		}
		else
		{
			cout << "����ʧ�ܣ��ص�T0ʱ��" << endl;
		}
		b.Print();
		cout << "����Ҫ���������Դ�� Y or N" << endl;
		cin >> ch;
	}
	delete[] request;
	delete[] safeArr;
}

int main()
{
	Test1();
	system("pause");
	return 0;
}
