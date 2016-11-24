#include "ProcessScheduling.h"


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

void Remove(PCB* arr, size_t size,size_t i)//ɾ��iλ�õ�����
{
	for (int j = i + 1; j < size; ++j)
	{
		arr[j - 1] = arr[j];
	}
}
void ProcessScheduling::Round_Robin() //ʱ��Ƭ��ת����
{
	PrintProcessScheduling();
	int time = _process[0]._arriveTime;
	int timeslice = 2;//ʱ��Ƭ ,�����ʼΪ2
	size_t i = 0;
	while (_size > 0)
	{
		timeslice = 2;
		while (timeslice != 0 && _size != 0)
		{
			if (i == _size)
				i = 0;
			//��ʱ�̸ý���û�е���
			while (_process[i]._arriveTime > time)
			{
				if (_process[0]._arriveTime <= time)
					i = 0;
				else
					++time;
			}
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

				_process[i]._runTime = 0;
				_process[i]._finishTime = time;

				_process[i]._status = FINISH;

				//��תʱ��ʹ�Ȩ��תʱ��
				_process[i]._cyclingTime = _process[i]._finishTime - _process[i]._arriveTime;
				_process[i]._weightCyclingTime =
					_process[i]._cyclingTime / (double)(_process[i]._finishTime - _process[i]._beginRunTime);

				PrintProcessScheduling();
				//ɾ��
				Remove(_process, _size,i);
				--_size;

				//i��λ�ñ�ɾ���ˣ����Բ���Ҫ++i
			}
		}
	}
	cout << "���̵������!" << endl << endl;
}

void ProcessScheduling::FCFS() //�����ȷ�������㷨
{
	PrintProcessScheduling();
	int time = _process[0]._arriveTime;
	while (_size > 0)
	{
		//��ʱ�̸ý���û�е���
		if (_process[0]._arriveTime > time)
			time = _process[0]._arriveTime;
		_process[0]._beginRunTime = time;
		time += _process[0]._runTime;
		_process[0]._finishTime = time;

		_process[0]._runTime = 0;
		_process[0]._status = FINISH;

		//��תʱ��ʹ�Ȩ��תʱ��
		_process[0]._cyclingTime = _process[0]._finishTime - _process[0]._arriveTime;
		_process[0]._weightCyclingTime =
			_process[0]._cyclingTime / (double)(_process[0]._finishTime - _process[0]._beginRunTime);
		
		PrintProcessScheduling();
		//ɾ��
		Remove(_process, _size, 0);
		--_size;
	}

	cout << "���̵������!" << endl << endl;
}

void ProcessScheduling::SJF() //����ҵ���ȵ���
{
	PrintProcessScheduling();
	int time = _process[0]._arriveTime;
	while (_size > 0)
	{
	
		//��ʱ�̸ý���û�е���
		if (_process[0]._arriveTime > time)
			time = _process[0]._arriveTime;

		//�ҵ���С�ģ���0��Ԫ�ؽ���
		size_t i = 0;
		size_t shortIndex = i;
		while (i + 1 < _size && _process[i + 1]._arriveTime <= time)
		{
			if (_process[i]._runTime > _process[i + 1]._runTime)
				shortIndex = i + 1;
			++i;
		}
		swap(_process[0], _process[shortIndex]);

		//����_process[0]
		_process[0]._beginRunTime = time;
		time += _process[0]._runTime;
		_process[0]._finishTime = time;
		_process[0]._runTime = 0;
		_process[0]._status = FINISH;

		//��תʱ��ʹ�Ȩ��תʱ��
		_process[0]._cyclingTime = _process[0]._finishTime - _process[0]._arriveTime;
		_process[0]._weightCyclingTime =
			_process[0]._cyclingTime / (double)(_process[0]._finishTime - _process[0]._beginRunTime);

		PrintProcessScheduling();
		//ɾ��
		Remove(_process, _size, 0);
		--_size;
	}
	cout << "���̵������!" << endl << endl;
}