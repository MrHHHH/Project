#include "ProcessScheduling.h"

void ProcessScheduling::Round_Robin() //时间片轮转调度
{
	int time;//时间片 ,假设初始为2
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
				_process[i]._runTime -= time;  //注意顺序
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
				//删除
				for (int j = i + 1; j < _size; ++j)
				{
					_process[j - 1] = _process[j];
				}
				--_size;

				//i的位置被删掉了，所以不需要++i
			}
		}
		PrintProcessScheduling();

	}
	cout << "进程调度完毕!" << endl << endl;
}

void ProcessScheduling::PrintProcessScheduling()
{

	cout << "******************************************************		" << endl;
	cout << "进程名" << "	" << "到达时间" << "	" << "运行时间" << "	" << "状态" << endl;

	for (int i = 0; i < _size; ++i)
	{
		cout << _process[i]._name << "		";
		cout << _process[i]._arriveTime << "	";
		cout << _process[i]._runTime << "	";
		cout << _process[i]._status << endl;
	}
	cout << endl;
}