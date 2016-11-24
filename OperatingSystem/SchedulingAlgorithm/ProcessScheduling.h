#pragma once
#include <iostream>
#include <string>
using namespace std;

enum Status
{
	READY,
	BLOCK,
	FINISH,
};
struct PCB
{
	string _name; //进程名
	int _arriveTime; //到达(提交)时间
	int _runTime; //服务时间
	int _beginRunTime; //开始运行时间
	int _finishTime; //完成时间
	int _cyclingTime; //周转时间
	double _weightCyclingTime; //带权周转时间
	Status _status; //进程状态

	PCB(string name = "", int arriveTime = 0, int runTime = 0)
		:_name(name)
		, _arriveTime(arriveTime)
		, _runTime(runTime)
		, _beginRunTime(0)
		, _finishTime(0)
		, _cyclingTime(0)
		, _weightCyclingTime(0.0)
		, _status(READY)
	{}
};


class ProcessScheduling
{
public:
	ProcessScheduling()
		:_process(NULL)
		, _size(0)
	{}

	ProcessScheduling(int num)
		:_size(num)
	{
		_process = new PCB[num]; //存放进程信息的数组
		for (int i = 0; i < num; ++i)
		{
			cout << "请输入进程名、到达时间、服务时间" << endl;
			string name;
			int arriveTime;
			int runTime;
			cin >> name >> arriveTime >> runTime;
			PCB tmp(name, arriveTime, runTime);

			//就绪队列排序
			if (i == 0)
			{
				_process[i] = tmp;
			}
			else
			{
				//插入
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
	void Round_Robin(); //时间片轮转调度
protected:
	PCB* _process;
	size_t _size;
};