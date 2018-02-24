#pragma once
#include <iostream>
#include <Windows.h>

using namespace std;

#define THROW_BAD_ALLOC  0;

//һ���ռ����������򵥷�װmalloc,free
//����ڴ���ڴ�

template <int inst>//Ԥ������
class MallocAllocTemplate {
public:
	static void* Allocate(size_t n) {
		void *result = malloc(n);
		if (0 == result)
		//mallocʧ�ܣ�OomMalloc���д������
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
	
	//ͨ���ú�����ָ���Լ�����ڴ治��Ĵ�����


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

		//���������û��Զ��������ڴ溯����ֱ��malloc�ɹ�
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

		//���������û��Զ��������ڴ溯����ֱ��realloc�ɹ�
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
		cout << "����Ŭ���ͷ�����" << endl;
}

void Test() {
	malloc_alloc::SetMallocHandler(TryToFree);
	//����ʧ�ܵĲ��������������û��Զ����ͷ��ڴ溯��
	//int *p = (int*)malloc_alloc::Allocate(-1);
	//malloc_alloc::Deallocate(p);

	//����ʹ��
	int *p = (int*)malloc_alloc::Allocate(sizeof(int) * 5);
	for (int i= 0; i < 5; ++i) {
		p[i] = i;
		cout << p[i] << endl;
	}
	malloc_alloc::Deallocate(p);
	
	system("pause");

}

