#pragma once
#include <iostream>
#include <vector>
using namespace std;


class Banker
{
public:
	Banker(int processNum, int resourceNum)
		:_processNum(processNum)
		,_resourceNum(resourceNum)
	{
		_Available = new int[resourceNum];
		_Max.resize(processNum);
		_Allocation.resize(processNum);
		_Need.resize(processNum);
		_Work = new int[resourceNum];
		_Finish.resize(processNum);
		
		for (int i = 0; i < _processNum; ++i)
		{
			_Max[i] = new int[_resourceNum];
			_Allocation[i] = new int[_resourceNum];
			_Need[i] = new int[_resourceNum];
		}

		InitArr();
	}

	~Banker()
	{
		delete[] _Available;
		delete[] _Work;
		for (int i = 0; i < _processNum; ++i)
		{
			delete[] _Max[i];
			delete[] _Allocation[i];
			delete[] _Need[i];
		}
	}

	void Print();
	void PrintSafe(int pNO);
	bool SafetyAlgorithm(int* safeArr);
	bool BankerAlgorithm(int pNO, int* request, size_t len);

protected:
	void InitArr();
	bool Arr1LessEqualArr2(int* arr1, int* arr2, size_t len);

protected:
	int _processNum;  //进程数
	int _resourceNum;  //资源数
	int* _Available;  //可利用资源向量
	vector<int*> _Max;  //最大需求矩阵
	vector<int*> _Allocation;  //已分配矩阵
	vector<int*> _Need;    //需求矩阵

	int* _Work;   //工作向量
	vector<bool> _Finish;   //表示进程是否可以执行
};