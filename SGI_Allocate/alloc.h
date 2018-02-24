#pragma once
#include <iostream>
#include <Windows.h>

using namespace std;

#define THROW_BAD_ALLOC  0;

//一级空间配置器，简单封装malloc,free
//针对于大块内存

template <int inst>//预留参数
class MallocAllocTemplate {
public:
	static void* Allocate(size_t n) {
		void *result = malloc(n);
		if (0 == result)
		//malloc失败，OomMalloc中有处理机制
			result = OomMalloc(n);

		return result;
	}

	static void Deallocate(void *p) {
		free(p);
	}

	static void* Reallocate(void *p, size_t new_size) {
		void *result = realloc(p, new_size);
		if (0 == result)
			result = Oom_Realloc(p, new_size);
		
		return result;
	}
	
	//通过该函数，指定自己针对内存不足的处理函数


	static void(*SetMallocHandler(void (*f) () )) (){
		void(*old)() = MallocAllocOomHandler;
		MallocAllocOomHandler = f;
		return (old);
	}

private:
	static void(*MallocAllocOomHandler)();
	static void *OomMalloc(size_t n) {
		void(*MyAllocOomHandler)();
		void *result;

		//反复调用用户自定义清理内存函数，直到malloc成功
		for (;;) {
			MyAllocOomHandler = MallocAllocOomHandler;
			if (MyAllocOomHandler == 0)
				throw THROW_BAD_ALLOC;
			(*MyAllocOomHandler)();
			result = malloc(n);
			if (result != NULL)
				return result;
		}
	}

	static void *OomRealloc(void *p, size_t n) {
		void(*MyAllocOomHandler)();
		void *result;

		//反复调用用户自定义清理内存函数，直到realloc成功
		for (;;) {
			MyAllocOomHandler = MallocAllocOomHandler;
			if (MyAllocOomHandler == 0)
				throw THROW_BAD_ALLOC;
			(*MyAllocOomHandler)();
			result = realloc(n);
			if (result != NULL)
				return result;
		}
	}
};

template <int inst>
void(*MallocAllocTemplate<inst>::MallocAllocOomHandler)() = 0;


typedef MallocAllocTemplate<0> malloc_alloc;

void TryToFree() {
	for (;;)
		cout << "正在努力释放内容" << endl;
}

void Test() {
	malloc_alloc::SetMallocHandler(TryToFree);
	//申请失败的测试用例，调用用户自定义释放内存函数
	//int *p = (int*)malloc_alloc::Allocate(-1);
	//malloc_alloc::Deallocate(p);

	//正常使用
	int *p = (int*)malloc_alloc::Allocate(sizeof(int) * 5);
	for (int i= 0; i < 5; ++i) {
		p[i] = i;
		cout << p[i] << endl;
	}
	malloc_alloc::Deallocate(p);
	
	system("pause");

}

