#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;


template<class T>
class ObjectPool
{
	struct BlockNode
	{
		void* _memory; //内存块
		BlockNode* _next;
		size_t _objNum; //内存对象个数

		BlockNode(size_t objNum)
			:_next(NULL)
			, _objNum(objNum)
		{
			_memory = malloc(_itemSize*objNum);
		}

		~BlockNode()
		{
			if (_memory != NULL)
			{
				free(_memory);
				_memory = NULL;
			}
		}
	};
public:
	ObjectPool(size_t initNum = 10, size_t maxNum = 100000)
		:_usedCount(0)
		, _maxNum(maxNum)
		, _lastDelete(NULL)
	{
		_head = _tail = new BlockNode(initNum);
	}

	void Destroy()
	{
		BlockNode* cur = _head;
		while (cur != NULL)
		{
			BlockNode* del = cur;
			cur = cur->_next;
			delete del;
		}

		_head = _tail = NULL;
		_usedCount = 0;
		_lastDelete = NULL;
	}

	~ObjectPool()
	{
		Destroy();
	}

	static size_t GetItemSize()
	{
		return sizeof(T) > sizeof(void*) ? sizeof(T) : sizeof(void*);
	}

	T* New()
	{
		//优先使用以前释放的结点
		if (_lastDelete)
		{
			T* obj = _lastDelete;
			_lastDelete = *((T**)_lastDelete);
			return new(obj)T;
		}

		if (_usedCount >= _tail->_objNum)
		{
			size_t objNum = _tail->_objNum * 2;
			if (objNum > _maxNum)
				objNum = _maxNum;

			_tail->_next = new BlockNode(objNum);
			_tail = _tail->_next;
			_usedCount = 0;
		}
		T* obj =(T*)( (char*)_tail->_memory + (_usedCount*_itemSize) );
		++_usedCount;
		return new(obj)T;
	}

	void Delete(T* ptr)
	{
		//要进行析构，T可能为string等类型的对象
		ptr->~T();

		if (ptr)
		{
			*((T**)ptr) = _lastDelete;
			_lastDelete = ptr;
		}
	}

protected:
	BlockNode* _head;
	BlockNode* _tail;
	size_t _usedCount; //当前块已用对象个数
	size_t _maxNum; //结点申请内存块的最大个数
	static size_t _itemSize; //单个对象的大小
	T* _lastDelete;
};

template<class T>
size_t ObjectPool<T>::_itemSize = ObjectPool<T>::GetItemSize();


///////////////////////////////////////////////////////////
// 测试内存对象池的常规使用和内存对象的重复使用
void TestObjectPool()
{
	vector<string*> v;

	ObjectPool<string> pool;
	for (size_t i = 0; i < 32; ++i)
	{
		v.push_back(pool.New());
		printf("Pool New [%d]: %p\n", i, v.back());
	}

	while (!v.empty())
	{
		pool.Delete(v.back());
		v.pop_back();
	}

	for (size_t i = 0; i < 32; ++i)
	{
		v.push_back(pool.New());
		printf("Pool New [%d]: %p\n", i, v.back());
	}

	v.push_back(pool.New());
}

#include <Windows.h>

// 针对当前的内存对象池进行简单的性能测试
void TestObjectPoolOP()
{
	size_t begin, end;
	vector<string*> v;
	const size_t N = 100000;
	v.reserve(N);

	cout << "pool new/delete===============================" << endl;
	// 反复申请释放5次
	begin = GetTickCount();
	ObjectPool<string> pool;
	for (size_t i = 0; i < N; ++i)
	{
		v.push_back(pool.New());
	}

	while (!v.empty())
	{
		pool.Delete(v.back());
		v.pop_back();
	}

	for (size_t i = 0; i < N; ++i)
	{
		v.push_back(pool.New());
	}

	while (!v.empty())
	{
		pool.Delete(v.back());
		v.pop_back();
	}

	for (size_t i = 0; i < N; ++i)
	{
		v.push_back(pool.New());
	}

	while (!v.empty())
	{
		pool.Delete(v.back());
		v.pop_back();
	}

	for (size_t i = 0; i < N; ++i)
	{
		v.push_back(pool.New());
	}

	while (!v.empty())
	{
		pool.Delete(v.back());
		v.pop_back();
	}

	for (size_t i = 0; i < N; ++i)
	{
		v.push_back(pool.New());
	}

	while (!v.empty())
	{
		pool.Delete(v.back());
		v.pop_back();
	}


	end = GetTickCount();
	cout << "Pool:" << end - begin << endl;

	cout << "new/delete===============================" << endl;
	begin = GetTickCount();

	for (size_t i = 0; i < N; ++i)
	{
		v.push_back(new string);
	}

	while (!v.empty())
	{
		delete v.back();
		v.pop_back();
	}

	for (size_t i = 0; i < N; ++i)
	{
		v.push_back(new string);
	}

	while (!v.empty())
	{
		delete v.back();
		v.pop_back();
	}

	for (size_t i = 0; i < N; ++i)
	{
		v.push_back(new string);
	}

	while (!v.empty())
	{
		delete v.back();
		v.pop_back();
	}

	for (size_t i = 0; i < N; ++i)
	{
		v.push_back(new string);
	}

	while (!v.empty())
	{
		delete v.back();
		v.pop_back();
	}

	for (size_t i = 0; i < N; ++i)
	{
		v.push_back(new string);
	}

	while (!v.empty())
	{
		delete v.back();
		v.pop_back();
	}

	end = GetTickCount();
	cout << "new/delete:" << end - begin << endl;
}