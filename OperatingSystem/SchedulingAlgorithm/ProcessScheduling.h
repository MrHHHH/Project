#pragma once
#include <iostream>
#include <string>
using namespace std;

struct PCB
{
	string _name; //������
	int _arriveTime; //����ʱ��
	int _runTime; //����ʱ��
	string _status; //����״̬
	PCB(string name = "", int arriveTime = 0, int runTime = 0)
		:_name(name)
		, _arriveTime(arriveTime)
		, _runTime(runTime)
		, _status("Waiting")
	{}
};


class ProcessScheduling
{
public:
	ProcessScheduling(int num)
		:_size(num)
	{
		_process = new PCB[num]; //��Ž�����Ϣ������
		for (int i = 0; i < num; ++i)
		{
			cout << "�����������������ʱ�䡢����ʱ��" << endl;
			string name;
			int arriveTime;
			int runTime;
			cin >> name >> arriveTime >> runTime;
			PCB tmp(name, arriveTime, runTime);

			//������������
			if (i == 0)
			{
				_process[i] = tmp;
			}
			else
			{
				//����
				int j = i - 1;
				while (j >= 0 && _process[j]._arriveTime > tmp._arriveTime)
				{
					_process[j + 1] = _process[j];
					--j;
				}
				_process[j+1] = tmp;
			}
		}
 	}

	~ProcessScheduling()
	{
		if (_process != NULL)
		{
			delete[] _process;
			_process = NULL;
		}
	}

	void PrintProcessScheduling();
	void Round_Robin(); //ʱ��Ƭ��ת����
protected:
	PCB* _process;
	size_t _size;
};