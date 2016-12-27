#include"BankerAlgorithm.h"

void Banker::InitArr()  //初始化所有数组
{
	//Available
	cout << "请输入可利用资源向量Available:" << endl;
	for (int i = 0; i < _resourceNum; ++i)
	{
		cin >> _Available[i];
	}
	//Max
	cout << "请输入最大需求矩阵Max:" << endl;
	for (int i = 0; i < _processNum; ++i)
	{
		for (int j = 0; j < _resourceNum; ++j)
		{
			cin >> _Max[i][j];
		}
	}

	//Allocation
	cout << "请输入已分配矩阵Allocation:" << endl;
	for (int i = 0; i < _processNum; ++i)
	{
		for (int j = 0; j < _resourceNum; ++j)
		{
			cin >> _Allocation[i][j];

			//Need
			_Need[i][j] = _Max[i][j] - _Allocation[i][j];

		}
	}
}

void Banker::Print()
{
	cout << "**********************************资源分配表**********************************" << endl;
	cout << "进程名        Max        Allocation        Need        " << endl;
	for (int i = 0; i < _processNum; ++i)
	{
		printf("P[%d]        ", i);
		for (int j = 0; j < _resourceNum; ++j)
		{
			cout << _Max[i][j] << " ";
		}
		cout << "        ";
		for (int j = 0; j < _resourceNum; ++j)
		{
			cout << _Allocation[i][j] << " ";
		}
		cout << "	   ";
		for (int j = 0; j < _resourceNum; ++j)
		{
			cout << _Need[i][j] << " ";
		}
		cout << endl;
	}
	cout << "可用资源数：";
	for (int i = 0; i < _resourceNum; ++i)
	{
		cout << _Available[i] << " ";
	}
	cout << endl;
}

void Banker::PrintSafe(int pNO)
{
	printf("p[%d]:", pNO);
	cout << "        ";
	//打印
	for (int j = 0; j < _resourceNum; ++j)
	{
		cout << _Work[j] << " ";
	}
	cout << "      ";
	for (int j = 0; j < _resourceNum; ++j)
	{
		cout << _Need[pNO][j] <<" ";
	}
	cout << "        ";
	for (int j = 0; j < _resourceNum; ++j)
	{
		cout << _Allocation[pNO][j] << " ";
	}
	cout << "              ";
	for (int j = 0; j < _resourceNum; ++j)
	{
		cout << _Allocation[pNO][j]+_Work[j] << " ";
	}
	cout << "               " << "true";
	cout << endl;
}

bool Banker::SafetyAlgorithm(int* safeArr)  //安全性算法，检查该状态是否安全。参数：取得安全序列
{
	cout << "**********************************安全序列计算过程**********************************" << endl;
	cout << "进程名        Work        Need        Allocation        Work+Allocation        Finish" << endl;

	int falseNum = 0;
	int safeArrIndex = 0;

	//Work赋值
	for (int i = 0; i < _resourceNum; ++i)
	{
		_Work[i] = _Available[i];
	}
	//Finish赋值
	for (int i = 0; i < _processNum; ++i)
	{
		bool status = true;
		for (int j = 0; j < _resourceNum; ++j)
		{
			if (_Need[i][j] != 0)
			{
				status = false;
				++falseNum;
				break;
			}
		}
		if (status == true)
			safeArr[safeArrIndex++] = i;
		_Finish[i] = status;
	}

	//判断当前请求分配后是否安全
	int no = 0;  //no:当前进程编号
	int sign = no;  //设定标志：第一个为false的进程，如果检查一圈都没有可分配的且falseNum>0，则不安全
	bool isVisited = false;

	while (falseNum > 0)
	{
		if (_Finish[no] == false && Arr1LessEqualArr2(_Need[no], _Work, _resourceNum))   //满足
		{
			//打印
			PrintSafe(no);
			//Work = Work + Allocation
			for (int j = 0; j < _resourceNum; ++j)  //j:当前资源
			{
				_Work[j] = _Work[j] + _Allocation[no][j];
			}
			_Finish[no] = true;
			

			safeArr[safeArrIndex++] = no;
			falseNum--;

			if (falseNum == 0)
				return true;

			while (_Finish[no] == true) //找下一个false进程
			{
				++no;
				if (no >= _processNum)
					no -= _processNum;
			}

			sign = no;
			isVisited = false;
		}
		else if (_Finish[no] == true)
		{
			++no;
		}
		else  //false
		{
			if (no == sign && isVisited == true)
				return false;
			else if (no == sign &&isVisited == false)
				isVisited = true;
			
			++no;
		}
		
		if (no >= _processNum)
			no -= _processNum;
	}
	return true;
}

bool Banker::Arr1LessEqualArr2(int* arr1, int* arr2, size_t len) //比较数组对应元素的大小
{
	bool ret = true;
	for (size_t i = 0; i < len; ++i)
	{
		if (arr1[i] > arr2[i])
		{
			ret = false;
			break;
		}
	}
	return ret;
}

bool Banker::BankerAlgorithm(int pNO, int* request, size_t len)
{
	//检查参数
	if (pNO < 0 || pNO >= _processNum)
	{
		cout << "进程编号错误" << endl;
		exit(EXIT_FAILURE);
	}

	//条件1：requst <= need
	if ( !Arr1LessEqualArr2(request, _Need[pNO], len)) //不满足
	{
		return false;
	}

	//条件二：request <= available
	if ( !Arr1LessEqualArr2(request, _Available, len))  //不满足
	{
		return false;
	}

	//将T0时刻Available, Allocation, Need的内容保存起来
	int* available = new int[_resourceNum];
	int** allocation = new int*[_processNum];
	int** need = new int*[_processNum];
	for (int i = 0; i < _resourceNum; ++i)
	{
		available[i] = _Available[i];
	}
	for (int i = 0; i < _processNum; ++i)
	{
		allocation[i] = new int[_resourceNum];
		need[i] = new int[_resourceNum];
		for (int j = 0; j < _resourceNum; ++j)
		{
			allocation[i][j] = _Allocation[i][j];
			need[i][j] = _Need[i][j];
		}
	}

	//试分配
	for (int j = 0; j < _resourceNum; ++j)
	{
		_Available[j] = _Available[j] - request[j];
		_Allocation[pNO][j] = _Allocation[pNO][j] + request[j];
		_Need[pNO][j] = _Need[pNO][j] - request[j];
	}

	//调用安全检查算法
	int* safeArr = new int[_processNum];

	if (!SafetyAlgorithm(safeArr))  //如果不安全，将保存的值重新放回去
	{
		cout << "当前状态不安全" << endl;
		for (int i = 0; i < _resourceNum; ++i)
		{
			_Available[i] = available[i];
		}
		for (int i = 0; i < _processNum; ++i)
		{
			for (int j = 0; j < _resourceNum; ++j)
			{
				_Allocation[i][j] = allocation[i][j];
				_Need[i][j] = need[i][j];
			}
		}
		return false;
	}
	else  //安全，打印安全序列
	{
		cout << "当前状态安全,安全序列为：";
		for (int i = 0; i < _processNum; ++i)
		{
			cout << safeArr[i] << " ";
		}
		cout << endl;
		return true;
	}

	//释放空间
	delete[] safeArr;
	delete[] available;
	for (int i = 0; i < _processNum; ++i)
	{
		delete[] allocation[i];
		delete[] need[i];
	}
	delete[] allocation;
	delete[] need;
}