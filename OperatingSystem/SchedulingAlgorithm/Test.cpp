#include "ProcessScheduling.h"

int main()
{
	cout << "��������̵�����" << endl;
	int num = 0; //��������
	cin >> num;
	ProcessScheduling p1(num);
	p1.PrintProcessScheduling();
	p1.Round_Robin();
	system("pause");
	return 0;
}