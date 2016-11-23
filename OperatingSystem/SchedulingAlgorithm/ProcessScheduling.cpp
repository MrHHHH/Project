#include "ProcessScheduling.h"

void ProcessScheduling::Round_Robin() //ʱ��Ƭ��ת����
{
	int time;//ʱ��Ƭ ,�����ʼΪ2
	size_t i = 0;
	while (_size != 0)
	{
		time = 2;
		while (time != 0 && _size!= 0)
		{
			if (i == _size)
				i = 0;

			if (_process[i]._runTime > time)
			{
				_process[i]._runTime -= time;  //ע��˳��
				time = 0;
				_process[i]._status = "Runing";
				PrintProcessScheduling();
				++i;
				_process[i]._status = "Waiting";

			}
			else // <=
			{
				time -= _process[i]._runTime;
				_process[i]._runTime = 0;
				_process[i]._status = "Runing";

				PrintProcessScheduling();
				//ɾ��
				for (int j = i + 1; j < _size; ++j)
				{
					_process[j - 1] = _process[j];
				}
				--_size;

				//i��λ�ñ�ɾ���ˣ����Բ���Ҫ++i
			}
		}
		PrintProcessScheduling();

	}
	cout << "���̵������!" << endl << endl;
}

void ProcessScheduling::PrintProcessScheduling()
{

	cout << "******************************************************		" << endl;
	cout << "������" << "	" << "����ʱ��" << "	" << "����ʱ��" << "	" << "״̬" << endl;

	for (int i = 0; i < _size; ++i)
	{
		cout << _process[i]._name << "		";
		cout << _process[i]._arriveTime << "	";
		cout << _process[i]._runTime << "	";
		cout << _process[i]._status << endl;
	}
	cout << endl;
}