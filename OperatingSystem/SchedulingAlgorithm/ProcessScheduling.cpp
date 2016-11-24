#include "ProcessScheduling.h"

void ProcessScheduling::Round_Robin() //ʱ��Ƭ��ת����
{
	PrintProcessScheduling();
	int time = _process[0]._arriveTime;
	int timeslice = 2;//ʱ��Ƭ ,�����ʼΪ2
	size_t i = 0;
	while (_size != 0)
	{
		timeslice = 2;
		while (timeslice != 0 && _size != 0)
		{
			if (i == _size)
				i = 0;

			if (_process[i]._runTime > timeslice)
			{
				if (_process[i]._status == READY)
					_process[i]._beginRunTime = time;
				_process[i]._runTime -= timeslice;  //ע��˳��
				time += timeslice;
				timeslice = 0;

				_process[i]._status = BLOCK;
				++i;

			}
			else // <=
			{
				if (_process[i]._status == READY)
					_process[i]._beginRunTime = time;

				timeslice -= _process[i]._runTime;
				time += _process[i]._runTime;

				_process[i]._runTime  = 0;
				_process[i]._finishTime = time;

				_process[i]._status = FINISH;

				//��תʱ��ʹ�Ȩ��תʱ��
				_process[i]._cyclingTime = _process[i]._finishTime - _process[i]._arriveTime;
				_process[i]._weightCyclingTime =
					_process[i]._cyclingTime / (double)(_process[i]._finishTime - _process[i]._beginRunTime);

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
	}
	cout << "���̵������!" << endl << endl;
}

void ProcessScheduling::PrintProcessScheduling()
{

	cout << "******************************************************		" << endl;
	cout << "������" << "	" << "����ʱ��" << "	" << "��������ʱ��" << "	" 
		<< "��ʼʱ��" <<"	" << "���ʱ��" << "	" << "״̬" << "	" 
		<< "��תʱ��" << "	" <<"��Ȩ��תʱ��"<<endl;

	for (int i = 0; i < _size; ++i)
	{
		cout << _process[i]._name << "	";
		cout << _process[i]._arriveTime << "		";
		cout << _process[i]._runTime << "		";
		cout << _process[i]._beginRunTime << "		";
		cout << _process[i]._finishTime << "		";
		if (_process[i]._status == READY)
			cout << "READY" << "	";
		else if (_process[i]._status == BLOCK)
			cout << "BLOCK" << "	";
		else
			cout << "FINISH" << "	";
		cout << _process[i]._cyclingTime << "		";
		cout << _process[i]._weightCyclingTime<< "		";

		cout << endl;
	}
	cout << endl;
}