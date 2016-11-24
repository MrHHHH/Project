#include "ProcessScheduling.h"

int main()
{
	cout << "请输入进程的数量" << endl;
	int num = 0; //进程数量
	cin >> num;
	ProcessScheduling p1(num);
//	p1.Round_Robin();
//	p1.FCFS();
	p1.SJF();
	system("pause");
	return 0;
}