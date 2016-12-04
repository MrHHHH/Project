#pragma once

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
ObjectPool<T>::_itemSize = ObjectPool<T>::GetGetItemSize();