#include "ProcessScheduling.h"

int main()
{
	cout << "��������̵�����" << endl;
	int num = 0; //��������
	cin >> num;
	ProcessScheduling p1(num);
//	p1.Round_Robin();
//	p1.FCFS();
	p1.SJF();
	system("pause");
	return 0;
}