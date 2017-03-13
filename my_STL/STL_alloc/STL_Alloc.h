#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <iostream>
using namespace std;
// Trace ����

#define __DEBUG__

FILE* fOut = fopen("trace.log", "w");

static string GetFileName(const string& path)
{
	char ch = '/';

#ifdef _WIN32
	ch = '\\';
#endif

	size_t pos = path.rfind(ch);
	if (pos == string::npos)
		return path;
	else
		return path.substr(pos + 1);
}
// ���ڵ���׷�ݵ�trace log
inline static void __trace_debug(const char* function,
	const char * filename, int line, char* format, ...)
{
	// ��ȡ�����ļ�
#ifdef __DEBUG__
	// ������ú�������Ϣ
	fprintf(stdout, "�� %s:%d��%s", GetFileName(filename).c_str(), line, function);
	fprintf(fOut, "�� %s:%d��%s", GetFileName(filename).c_str(), line, function);

	// ����û����trace��Ϣ
	va_list args;
	va_start(args, format);
	vfprintf(stdout, format, args);
	vfprintf(fOut, format, args);
	va_end(args);
#endif
}

#define __TRACE_DEBUG(...)  \
	__trace_debug(__FUNCTION__, __FILE__, __LINE__, __VA_ARGS__);

/////////////////////////////////////////////////////////////////////////
//һ���ռ�������

//����instΪԤ������ʱ����
template<int inst>
class __MallocAllocTemplate
{
private:
	//�����ڴ治�������ĺ���
	//oom: out of memory
	static void *OomMalloc(size_t);
	static void *OomRealloc(void *, size_t);
	
	//�����ڴ�ʧ��ʱ�������ľ������ָ�� 
	static void(*__mallocAllocOomHandler)();
public:

	//�ռ����:һ���ռ�������ֱ��ʹ��malloc
	static void *Allocate(size_t n)
	{
		__TRACE_DEBUG("(n:%u)\n", n);
		void *result = malloc(n);
		if (result == 0)
			result = OomMalloc(n);
		return result;
	}

	//�ռ��ͷţ�һ���ռ�������ֱ��ʹ��free
	static void Deallocate(void *p, size_t /*n*/)
	{
		__TRACE_DEBUG("(p:%p)\n", p);
		free(p);
	}

	//���·���ռ䣺һ���ռ�������ֱ�ӵ���realloc
	static void *Reallocate(void *p, size_t /*oldSize*/, size_t newSize)
	{
		void *result = realloc(p, newSize);
		if (result == 0)
			result = OomRealloc(p, newSize);
		return result;
	}

	
	static void(*SetMallocHandler(void *f()))()
	{
		void(*old)() = __mallocAllocOomHandler;
		__mallocAllocOomHandler = f;
		return old;
	}
};

template<int inst>
void(*__MallocAllocTemplate<inst>::__mallocAllocOomHandler)() = 0;

template<int inst>
void *__MallocAllocTemplate<inst>::OomMalloc(size_t n)
{
	void(*myMallocHandler)();
	void *result;

	while (1)  //һֱ���� �ͷţ����٣��ͷţ��ٿ���....
	{
		myMallocHandler = __mallocAllocOomHandler;
		if (myMallocHandler == 0)
		{
			cerr << "out of memory" << endl;
			exit(1);
		}

		(*myMallocHandler)(); //��ͼ�ͷ��ڴ�
		result = malloc(n);   //�����ڴ�
		if (result)
			return result;
	}
}

template<int inst>
void *__MallocAllocTemplate<inst>::OomRealloc(void *p, size_t n)
{
	void(*myMallocHandler)();
	void * result;
	while (1)
	{
		myMallocHandler = __mallocAllocOomHandler;
		if (myMallocHandler == 0)
		{
			throw::bad_alloc;
		}

		(*myMallocHandler)();
		result = realloc(p, n);
		if (result)
			return result;
	}
}

typedef __MallocAllocTemplate<0> MallocAlloc;

/////////////////////////////////////////////////////////////////////////
//�����ռ�������

//����ö�٣�����������������һ�ַ�ʽ
enum {__ALIGN = 8}; //С��������ϵ��߽磨���м����
enum {__MAX_BYTES = 128};   //��������ֵ
enum {__NFREELISTS = __MAX_BYTES/__ALIGN};  //free_list �Ľ�����

//��һ���������ڶ��߳������ �ڶ�������Ԥ������ʱ����
template<bool threads, int inst>
class __DefaultAllocTemplate
{
private:
	union Obj
	{
		union Obj *freeListLink;   //ָ����ͬ��ʽ����һ��Obj
		char clientData[1];        //ָ��ʵ������
	};

public:

	//�ռ����
	static void *Allocate(size_t n) //nΪ�ֽ���
	{
		__TRACE_DEBUG("(n: %u)\n", n);
		//1.�����������__MAX_BYTES������һ���ռ�������
		//2.���򣬽�������ռ��������л�ȡ
		if (n > (size_t)__MAX_BYTES)
			return MallocAlloc::Allocate(n);
		
		//��������ռ�������
		//1.�����Ӧ���������е�λ��û�пռ䣬��Refill���
		//2.��������������У���ֱ�ӷ���һ�������ڴ�
		//ps:���Ϊ���̣߳���Ҫ���м���
		size_t index = FreeListIndex(n);
		Obj* head = freeList[index];
		
		if (head == NULL)
		{
			//û���ҵ����õ�freeList,�������freeList
			return Refill(ROUND_UP(n));
		}
		else
		{
			__TRACE_DEBUG("��������ȡ�ڴ�:_freeList[%d]\n", index);

			freeList[index] = head->freeListLink;  //ͷɾ��������
			return head;
		}
	}

	//�ռ����
	static void Deallocate(void *p, size_t n)  //nΪ�ֽ���
	{
		__TRACE_DEBUG("(p:%p, n: %u)\n", p, n);
		//1.�����������__MAX_BYTES,����һ���ռ�������
		//2.���򣬵��ö����ռ�������
		if (n > (size_t)__MAX_BYTES)
			MallocAlloc::Deallocate(p, n);
		
		//��������ռ�������
		//ps:���̻߳�����Ҫ���Ǽ���
		size_t index = FreeListIndex(n);
		Obj *head = freeList[index];

		Obj *tmp = (Obj*)p;
		tmp->freeListLink = head; //ͷ����������
		freeList[index] = tmp;
	}

	//���·���ռ�
	static void *Reallocate(void *p, size_t oldSize, size_t newSize)
	{
		//1.�����������__MAX_BYTES,����һ���ռ�������
		//2.���򣬵��ö����ռ�������
		if (oldSize > (size_t)__MAX_BYTES && newSize > (size_t)__MAX_BYTES)
			MallocAlloc::Reallocate(p, oldSize, newSize);

		//��������ռ�������
		//1.����¾ɵĴ�С��ͬһ���䣬����Ҫ���·��䣬ֱ�ӷ���
		//2.�������·��䲢��������
		if (ROUND_UP(oldSize) == ROUND_UP(newSize))
			return p;

		void *result = Allocate(newSize);
		memcpy(result, p, (oldSize <= newSize ? oldSize : newSize));
		Deallocate(p, oldSize);
		return result;
	}
private:
	//���������С��ѡ��freeList��λ�ã����ض�Ӧ�±�
	static size_t FreeListIndex(size_t bytes)
	{
		return (bytes + __ALIGN - 1) / __ALIGN - 1;
	}

	//����ڴ����freeList�С�����һ����СΪn�Ķ��󣬲����ܼ����СΪn���������鵽freeList��
	static void *Refill(size_t n)
	{
		__TRACE_DEBUG("(n:%u)\n", n);
		int nobjs = 20;
		char* chunk = ChunkAlloc(n, nobjs);

		if (nobjs == 1)
			return chunk;

		Obj *result, *cur;
		result = (Obj*)chunk;  //����Ҫ���ص�����
		//��ʣ��Ŀռ����ӵ�����������
		cur = (Obj*)(chunk + n);
		size_t index = FreeListIndex(n);
		freeList[index] = cur;

		for (int i = 2; i <= nobjs; ++i)
		{
			cur->freeListLink = (Obj*)(chunk + n*i);
			cur = cur->freeListLink;
		}
		cur->freeListLink = NULL;
		return result;
	}

	//��ȡ����ڴ棬����ȡ��nobjs������
	static char *ChunkAlloc(size_t size, int& nobjs)
	{
		__TRACE_DEBUG("(size: %u, nobjs: %d)\n", size, nobjs);

		size_t bytesNeed = size*nobjs;
		size_t bytesLeft = endFree - startFree;  //�ڴ��ʣ��ռ�
		char *result;
		//1.�ڴ���ڴ��㹻��ֱ�Ӵ��ڴ����ȡ(bytesLeft >= bytesNeed)
		//2.�ڴ���ڴ治ȫ�����������ٹ�һ������Ĵ�С(bytesLeft >= size)
		//3.�ڴ���ڴ治��һ������Ĵ�С�����ϵͳ�������ڴ浽�ڴ����(bytesLeft < size)

		if (bytesLeft >= bytesNeed)
		{
			__TRACE_DEBUG("�ڴ�����ڴ��㹻����%d������\n", nobjs);
			result = startFree;
			startFree += bytesNeed;
			return result;
		}
		else if (bytesLeft >= size)
		{
			__TRACE_DEBUG("�ڴ�����ڴ治������%d������ֻ�ܷ���%d������\n", nobjs, bytesLeft / size);
			result = startFree;
			nobjs = bytesLeft / size; //ʣ��ռ���Է������ĸ���
			startFree += (nobjs*size);
			return result;
		}
		else
		{
			//����ڴ���л���С���ڴ棬����ͷ�嵽���ʵ�freeList
			if (bytesLeft > 0)
			{
				size_t index = FreeListIndex(bytesLeft);
				Obj *tmp = (Obj*)startFree;
				tmp->freeListLink = freeList[index];
				freeList[index] = tmp;
				startFree = NULL;
				__TRACE_DEBUG("���ڴ����ʣ��Ŀռ䣬�����freeList[%d]\n", index);
			}

			//��ϵͳ�������ڴ�
			size_t bytesToGet = bytesNeed * 2 + ROUND_UP(heapSize >> 4);
			startFree = (char*)malloc(bytesToGet);
			__TRACE_DEBUG("�ڴ�ؿռ䲻�㣬ϵͳ�ѷ���%u bytes�ڴ�\n", bytesToGet)
			//�����ϵͳ���з����ڴ�ʧ�ܣ����Ե����������и���Ľ���з���
			if (startFree == NULL)
			{
				__TRACE_DEBUG("ϵͳ�������㹻������֮�£�ֻ�ܵ����������п���\n");
				for (int i = size; i <= __MAX_BYTES; i += __ALIGN)
				{
					size_t index = FreeListIndex(i);
					Obj *head = freeList[index];

					if (head != NULL)
					{
						startFree = (char*)head;
						freeList[index] = head->freeListLink;
						endFree = startFree + i;
						return ChunkAlloc(size, nobjs);
					}
				}
				endFree = 0;
				//����������Ҳû���ڴ棬���ٵ�һ���������з����ڴ�
				//��Ϊһ���������п����������ڴ洦���п��ܻ���䵽�ڴ�
				__TRACE_DEBUG("ϵͳ�Ѻ��������������ڴ棬һ�������������һ������\n");
				startFree = (char*)MallocAlloc::Allocate(bytesToGet);
			}
			heapSize += bytesToGet;
			endFree = startFree + bytesToGet;
			return ChunkAlloc(size, nobjs);
		}
	}

	//��bytes�ϵ���8�ı���
	static size_t ROUND_UP(size_t bytes)
	{
		return (bytes + __ALIGN - 1) & (~(__ALIGN - 1));
	}
private:
	static Obj *volatile freeList[__NFREELISTS];  //��������volatile�������β�ͬ�̷߳��ʺ��޸ĵı���
	static char *startFree;  //�ڴ�صĿ�ʼλ��
	static char *endFree;    //�ڴ�صĽ���λ��
	static size_t heapSize;  //��ϵͳ�ѷ�����ܴ�С
};

//��ʼ����̬����
template <bool threads, int inst>
char *__DefaultAllocTemplate<threads, inst>::startFree = 0;
template <bool threads, int inst>
char *__DefaultAllocTemplate<threads, inst>::endFree = 0;
template <bool threads, int inst>
size_t __DefaultAllocTemplate<threads, inst>::heapSize = 0;
template <bool threads, int inst>
typename __DefaultAllocTemplate<threads, inst>::Obj *volatile __DefaultAllocTemplate<threads, inst>::freeList[__NFREELISTS] = 
	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

#   define __NODE_ALLOCATOR_THREADS true
#ifdef __USE_MALLOC
typedef MallocAlloc Alloc; //��allocΪһ���ռ�������
#else
typedef __DefaultAllocTemplate<__NODE_ALLOCATOR_THREADS, 0> Alloc;//��AllocΪ�����ռ�������
#endif // __USE_MALLOC

template<class T, class _Alloc>
class SimpleAlloc
{
public:
	static T *Allocate(size_t n)  //nΪ�������
	{
		return (n == 0) ? 0 : (T*)_Alloc::Allocate(n*sizeof(T));
	}

	static T *Allocate()
	{
		return (T*)_Alloc::Allocate(sizeof(T));
	}

	static void Deallocate(T *p, size_t n)
	{
		if (n != 0)
		{
			_Alloc::Deallocate(p, n*sizeof(T));
		}
	}

	static void Deallocate(T *p)
	{
		_Alloc::Deallocate(p, sizeof(T));
	}
};


//ͨ��__TRACE_DEBUG���׺в���

//�������ⳡ�� 
void TestAlloc1()
{
	cout<<"�����ڴ�ؿռ䲻�����"<<endl;   
	char *p1=SimpleAlloc<char,Alloc>::Allocate(8);
	char *p2 = SimpleAlloc<char, Alloc>::Allocate(8);
	cout << "�����ڴ�ؿռ䲻�㣬ϵͳ�ѽ��з���" << endl;
	char *p3 = SimpleAlloc<char, Alloc>::Allocate(12); 
}

//����ϵͳ���ڴ�ľ��ĳ��� 
void TestAlloc2()
{
	cout<<"����ϵͳ���ڴ�ľ�"<<endl;
	SimpleAlloc<char, Alloc>::Allocate(1024 * 1024 * 1024);    //SimpleAlloc<char, Alloc>::Allocate(1024*1024*1024);
	SimpleAlloc<char, Alloc>::Allocate(1024 * 1024);
//���ò��ԣ�˵��ϵͳ����С���ڴ���������Ǻ�ǿ�ġ�   
	for(int i= 0;i< 100000; ++i)    
	{
		char *p1=SimpleAlloc<char,Alloc>::Allocate(128);
	}
}