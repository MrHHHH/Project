#include"BankerAlgorithm.h"

void Banker::InitArr()  //��ʼ����������
{
	//Available
	cout << "�������������Դ����Available:" << endl;
	for (int i = 0; i < _resourceNum; ++i)
	{
		cin >> _Available[i];
	}
	//Max
	cout << "����������������Max:" << endl;
	for (int i = 0; i < _processNum; ++i)
	{
		for (int j = 0; j < _resourceNum; ++j)
		{
			cin >> _Max[i][j];
		}
	}

	//Allocation
	cout << "�������ѷ������Allocation:" << endl;
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
	cout << "**********************************��Դ�����**********************************" << endl;
	cout << "������        Max        Allocation        Need        " << endl;
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
	cout << "������Դ����";
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
	//��ӡ
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

bool Banker::SafetyAlgorithm(int* safeArr)  //��ȫ���㷨������״̬�Ƿ�ȫ��������ȡ�ð�ȫ����
{
	cout << "**********************************��ȫ���м������**********************************" << endl;
	cout << "������        Work        Need        Allocation        Work+Allocation        Finish" << endl;

	int falseNum = 0;
	int safeArrIndex = 0;

	//Work��ֵ
	for (int i = 0; i < _resourceNum; ++i)
	{
		_Work[i] = _Available[i];
	}
	//Finish��ֵ
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

	//�жϵ�ǰ���������Ƿ�ȫ
	int no = 0;  //no:��ǰ���̱��
	int sign = no;  //�趨��־����һ��Ϊfalse�Ľ��̣�������һȦ��û�пɷ������falseNum>0���򲻰�ȫ
	bool isVisited = false;

	while (falseNum > 0)
	{
		if (_Finish[no] == false && Arr1LessEqualArr2(_Need[no], _Work, _resourceNum))   //����
		{
			//��ӡ
			PrintSafe(no);
			//Work = Work + Allocation
			for (int j = 0; j < _resourceNum; ++j)  //j:��ǰ��Դ
			{
				_Work[j] = _Work[j] + _Allocation[no][j];
			}
			_Finish[no] = true;
			

			safeArr[safeArrIndex++] = no;
			falseNum--;

			if (falseNum == 0)
				return true;

			while (_Finish[no] == true) //����һ��false����
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

bool Banker::Arr1LessEqualArr2(int* arr1, int* arr2, size_t len) //�Ƚ������ӦԪ�صĴ�С
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
	//������
	if (pNO < 0 || pNO >= _processNum)
	{
		cout << "���̱�Ŵ���" << endl;
		exit(EXIT_FAILURE);
	}

	//����1��requst <= need
	if ( !Arr1LessEqualArr2(request, _Need[pNO], len)) //������
	{
		return false;
	}

	//��������request <= available
	if ( !Arr1LessEqualArr2(request, _Available, len))  //������
	{
		return false;
	}

	//��T0ʱ��Available, Allocation, Need�����ݱ�������
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

	//�Է���
	for (int j = 0; j < _resourceNum; ++j)
	{
		_Available[j] = _Available[j] - request[j];
		_Allocation[pNO][j] = _Allocation[pNO][j] + request[j];
		_Need[pNO][j] = _Need[pNO][j] - request[j];
	}

	//���ð�ȫ����㷨
	int* safeArr = new int[_processNum];

	if (!SafetyAlgorithm(safeArr))  //�������ȫ���������ֵ���·Ż�ȥ
	{
		cout << "��ǰ״̬����ȫ" << endl;
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
	else  //��ȫ����ӡ��ȫ����
	{
		cout << "��ǰ״̬��ȫ,��ȫ����Ϊ��";
		for (int i = 0; i < _processNum; ++i)
		{
			cout << safeArr[i] << " ";
		}
		cout << endl;
		return true;
	}

	//�ͷſռ�
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