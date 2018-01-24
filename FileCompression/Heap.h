#pragma once
#include <vector>
#include <iostream>
#include <assert.h>

using namespace std;

template <class T>
struct Less
{
	bool operator()(const T& n1, const T& n2)
	{
		return n1 < n2;
	}
};

template <class T>
struct Greater
{
	bool operator()(const T& n1, const T& n2)
	{
		return n1 > n2;
	}
};


//默认小堆
template<class T,class Compare = Less<T>>
class Heap
{
public:
	Heap()
	{}

	Heap(const T* a,size_t sz)
	{
		_a.reserve(sz);
		for (size_t i = 0; i < sz; i++)
		{
			_a.push_back(a[i]);
		}
		for (int i = (_a.size() - 2) / 2; i >= 0; --i)
		{
			_AdjustDown(i);
		}
	}

	size_t Size()
	{
		return _a.size();
	}

	bool Empty()
	{
		return _a.empty();
	}

	void Push(const T& x)
	{
		_a.push_back(x);
		_AdjustUp(_a.size() - 1);
	}

	void Pop()
	{
		//pop堆顶元素
		assert(!_a.empty());
		swap(_a[0], _a[_a.size() - 1]);
		_a.pop_back();
		_AdjustDown(0);
	}

	const T& Top()
	{
		return _a[0];
	}

protected:
	//向下调整,
	void _AdjustDown(int parent)
	{
		Compare com;
		size_t child = parent * 2 + 1;
		while (child < _a.size())
		{
			if (child + 1 < _a.size() && com(_a[child+1], _a[child]))
			{
				child++;
			}
			if (com(_a[child], _a[parent]))
			{
				swap(_a[parent], _a[child]);
				parent = child;
				child = parent * 2 + 1;
			}
			else
			{
				break;
			}
		}
	}

	//向上调整
	void _AdjustUp(int child)
	{
		Compare com;
		int parent = (child - 1) / 2;
		while (parent >= 0)
		{
			if (com(_a[child], _a[parent]))
			{
				swap(_a[parent], _a[child]);
				child = parent;
				parent = (child - 1) / 2;
			}
			else
			{
				break;
			}
		}
	}


protected:
	vector<T> _a;
};


