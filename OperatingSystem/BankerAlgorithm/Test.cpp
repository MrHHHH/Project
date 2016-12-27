#include "BankerAlgorithm.h"

void Test1()
{
	int processNum, resourceNum;
	cout << "请输入进程数量：";
	cin >> processNum;
	cout << "请输入资源种类：";
	cin >> resourceNum;

	Banker b(processNum, resourceNum);
	b.Print();
	int* safeArr = new int[processNum];
	if (b.SafetyAlgorithm(safeArr))
	{
		cout << "当前状态安全,安全序列为：";
		for (int i = 0; i < processNum; ++i)
		{
			cout << safeArr[i] << " ";
		}
		cout << endl;
	}
	else
	{
		cout << "当前状态不安全" << endl;
		return;
	}
	
	int* request = new int[resourceNum];//请求资源向量
	cout << "您还要申请分配资源吗？ Y or N" << endl;
	char ch;
	cin >> ch;
	int pNO;
	while (ch == 'y' || ch == 'Y')
	{
		cout << "请输入请求资源的进程号0~" << processNum - 1 << ":";
		cin >> pNO;
		cout << "请输入请求资源的数量" << endl;
		for (int i = 0; i < resourceNum; ++i)
		{
			cin >> request[i];
		}
		if (b.BankerAlgorithm(pNO, request, resourceNum))
		{
			cout << "分配成功，进入T1时刻" << endl;
		}
		else
		{
			cout << "分配失败，回到T0时刻" << endl;
		}
		b.Print();
		cout << "您还要申请分配资源吗？ Y or N" << endl;
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
