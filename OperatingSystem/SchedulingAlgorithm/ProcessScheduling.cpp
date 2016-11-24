#include "ProcessScheduling.h"

void ProcessScheduling::Round_Robin() //时间片轮转调度
{
	PrintProcessScheduling();
	int time = _process[0]._arriveTime;
	int timeslice = 2;//时间片 ,假设初始为2
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
				_process[i]._runTime -= timeslice;  //注意顺序
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

				//周转时间和带权周转时间
				_process[i]._cyclingTime = _process[i]._finishTime - _process[i]._arriveTime;
				_process[i]._weightCyclingTime =
					_process[i]._cyclingTime / (double)(_process[i]._finishTime - _process[i]._beginRunTime);

				PrintProcessScheduling();
				//删除
				for (int j = i + 1; j < _size; ++j)
				{
					_process[j - 1] = _process[j];
				}
				--_size;

				//i的位置被删掉了，所以不需要++i
			}
		}
	}
	cout << "进程调度完毕!" << endl << endl;
}

void ProcessScheduling::PrintProcessScheduling()
{

	cout << "******************************************************		" << endl;
	cout << "进程名" << "	" << "到达时间" << "	" << "还需运行时间" << "	" 
		<< "开始时间" <<"	" << "完成时间" << "	" << "状态" << "	" 
		<< "周转时间" << "	" <<"带权周转时间"<<endl;

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