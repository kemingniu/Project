#pragma once
#include "alloc.h"
using namespace std;

template<bool thread, int inst>
class DefaultAllocTemplate {
private:
	union Obj{
		Obj* _next;
		char _client_data[1]; //底层优化
	};

	enum {ALIGN = 8};
	enum {MAX_BYTES = 128};
	enum {NFREE_LIST = MAXBYTE / ALIGN};

	static Obj* _freelist[NFREE_LIST];
	
	static char *_startFree;
	static char *_endFree;
	static size_t _heapSize;

public:
	static size_t GetIndex(size_t n) {
		return (n + ALIGN - 1) / ALIGN - 1;
	}

	static size_t RoundUp(size_t n) {
		return (n + ALIGN - 1)&(~(ALIGN - 1));
	}

	static char* ChunkAlloc(size_t n, size_t& nobjs) {
		char *result;
		size_t totalBytes = nobjs * n;
		size_t leftBytes = _endFree - _startFree;

		if (leftBytes >= totalBytes) //足够开20个内存块
		{
			result = _startFree;
			_startFree += totalBytes;
			return (result);
		}

		//足够一块，但是不够20块，更新nobjs
		else if (leftBytes >= n) {
			nobjs = leftBytes / n;
			totalBytes = n * nobjs;
			result = _startFree;
			_startFree += totalBytes;
			return (result);
		}
		//内存池剩余空间连一个内存块大小都无法提供	
		else 
		{
			//将剩余大小挂到自由链表中
			if (leftBytes > 0) {
				Obj **myfreelist = _freelist + GetIndex(leftBytes);
				
				((Obj*)_startFree)->_next = *myfreelist;
				*myfreelist = (Obj*)_startFree;
			}
			size_t BytesToGet = totalBytes * 2 + RoundUp(_heapSize >> 4);
			_startFree = (char*)malloc(BytesToGet);
			
			//系统内存吃紧，开辟不出来
			if (_startFree == NULL) {
				//先尝试去后续取内存块，归还给狭义内存池，进行回调
				Obj **myfreelist, *p;
				for (int i = n; i <= MAXBYTE; i += ALIGN) {
					myfreelist = _freelist + GetIndex(i);
					p = *myfreelist;
					if (0 != p) {
						*myfreelist = p->_next;
						_startFree = (char*)p;
						_endFree = _startFree + i;
						return (ChunkAlloc(n, nobjs));
					}
				}

				//此时自由链表也无可用内存块，调用一级空间配置器
				_endFree = 0;
				_startFree = (char*)malloc_alloc::Allocate(BytesToGet);
			}
			_heapSize += BytesToGet;
			_endFree = _startFree + BytesToGet;
			
			return ChunkAlloc(n, nobjs);
		}
	}

	static void* Refill(size_t n) {
		size_t nobjs = 20;
		char *chunk = ChunkAlloc(n, nobjs);
		if (nobjs == 1)
			return chunk;

		//将开辟好的挂到自由链表上,返回1块
		Obj ** myfreelist;
		Obj *result;
		Obj *cur_obj, *next_obj;

		myfreelist = _freelist + GetIndex(n);
		result = (Obj*)chunk;
		*myfreelist = next_obj = (Obj*)(chunk + n);

		//将剩余节点串到自由链表中
		for (int i = 1; ; i++) {
			cur_obj = next_obj;
			next_obj = (Obj*)((char*)next_obj + n);
			if (nobjs - 1 == i) {
				cur_obj->_next = 0;
				break;
			}
			else {
				cur_obj->_next = next_obj;
			}
		}
		return result;
	}

	static void* Allocate(size_t n) {
		if (n > MAXBYTE) {
			return malloc_alloc::Allocate(n);
		}

		Obj ** myfreelist;
		Obj * result;
		myfreelist = _freelist + GetIndex(n);
		result = *myfreelist;
		if (result == NULL) {
			void *r = Refill(RoundUp(n));
			return r;
		}
		*myfreelist = result->_next;
		return (result);
	}

	static void DeAllocate(void *p, size_t n) {
		if (n > MAXBYTE)
			return MallocAllocTemplate<0>::Deallocate(p);

		size_t index = GetIndex(n);
		((Obj*)p)->_next = _freelist[index];
		_freelist[index] = (Obj*)p;
	}
};

template<bool threads, int inst>
typename DefaultAllocTemplate<threads, inst>::Obj*
DefaultAllocTemplate<threads, inst>::_freelist[NFREE_LIST] = { 0 };

template<bool threads, int inst>
char* DefaultAllocTemplate<threads, inst>::_startFree = NULL;

template<bool threads, int inst>
char* DefaultAllocTemplate<threads, inst>::_endFree = NULL;

template<bool threads, int inst>
size_t DefaultAllocTemplate<threads, inst>::_heapSize = 0;


template<class T, class Alloc>
class SimpleAlloc {
public:
	static T* Allocate(size_t n) {
		return 0 == n ? 0 : (T*)Alloc::Allocate(n * sizeof(T));
	}

	static T* Allocate(void) {
		return (T*)Alloc::Allocate(sizeof(T));
	}

	static void DeAllocate(T* p, size_t n) {
		if (0 != n)
			Alloc::DeAllocate(p, sizeof(T));
	}

	static void DeAllocate(T* p) {
		Alloc::DeAllocate(p, sizeof(T));
	}
};

void TestAlloc() {
	int i = 100000;
	while (--i) {
		DefaultAllocTemplate<0, 0>::Allocate(100);
	}
	system("pause");
}

