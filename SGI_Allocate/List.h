#pragma once
#include "alloc.h"
#include "DefaultAlloc.h"


template<class T>
struct __ListNode
{
	__ListNode<T>* _next;
	__ListNode<T>* _prev;
	T _data;

	__ListNode(const T& x)
		:_data(x)
		, _next(NULL)
		, _prev(NULL)
	{}
};

template<class T, class Ref, class Ptr>
struct __ListIterator
{
	typedef __ListNode<T> Node;
	typedef __ListIterator<T, Ref, Ptr> Self;

	Node* _node;


	__ListIterator(Node* x)
		:_node(x)
	{}

	Ref operator*()
	{
		return _node->_data;
	}

	Ptr operator->()
	{
		//return &_node->_data;
		return &(operator*());
	}

	bool operator != (const Self& s)
	{
		return _node != s._node;
	}

	bool operator==(const Self& s)
	{
		return _node == s->_node;
	}

	Self& operator++()
	{
		_node = _node->_next;
		return *this;
	}

	Self operator++(int)
	{
		Self tmp(*this);
		_node = _node->_next;
		return tmp;
	}

	Self& operator--()
	{
		_node = _node->_prev;
		return *this;
	}

	Self operator--(int)
	{
		Self tmp(*this);
		_node = _node->_prev;
		return tmp;
	}
};

template<class T, class alloc = DefaultAllocTemplate<0, 0>>
class List
{
	typedef __ListNode<T> Node;
	typedef SimpleAlloc<Node, alloc> ListNodeAllocator;
public:
	typedef __ListIterator<T, T&, T*> Iterator;
	typedef __ListIterator<T, const T&, const T*> ConstIterator;

	Node* BuyNode(const T& x)
	{
		Node* node = ListNodeAllocator::Allocate();
		new(node)Node(x);
		return node;
	}

	void DestoryNode(Node* node)
	{
		node->~Node();
		ListNodeAllocator::DeAllocate(node);
	}

	List()
	{
		_head = BuyNode(T());
		_head->_next = _head;
		_head->_prev = _head;
	}

	~List()
	{
		Clear();
		//delete _head;
		DestoryNode(_head);
		_head = NULL;
	}

	void Clear()
	{
		Iterator it = Begin();
		while (it != End())
		{
			Node* del = it._node;
			++it;
			//delete del;
			DestoryNode(del);
		}

		_head->_next = _head;
		_head->_prev = _head;
	}

	Iterator Begin()
	{
		return _head->_next;
	}

	Iterator End()
	{
		return _head;
	}

	ConstIterator Begin() const
	{
		return _head->_next;
	}

	ConstIterator End() const
	{
		return _head;
	}

	void PushBack(const T& x)
	{
		Insert(End(), x);
	}

	void PushFront(const T& x)
	{
		Insert(Begin(), x)
	}

	void PopBack()
	{
		Erase(--End());
	}

	void PopFront()
	{
		Erase(Begin());
	}

	void Insert(Iterator pos, const T& x)
	{
		Node* cur = pos._node;
		Node* prev = cur->_prev;

		Node* tmp = BuyNode(x);

		prev->_next = tmp;
		tmp->_prev = prev;

		tmp->_next = cur;
		cur->_prev = tmp;
	}

	Iterator Erase(Iterator& pos)
	{
		assert(pos._node != _head);

		Node* cur = pos._node;
		Node* prev = cur->_prev;
		Node* next = cur->_next;

		prev->_next = next;
		next->_prev = prev;
		//delete cur;
		DestoryNode(cur);

		pos = prev;

		return next;
	}

protected:
	Node* _head;
};


void TestList()
{
	List<int> l;
	l.PushBack(1);
	l.PushBack(2);
	l.PushBack(3);
	l.PushBack(4);

	List<int>::Iterator rit = l.Begin();
	while (rit != l.End())
	{
		cout << *rit << " ";
		++rit;
	}
	cout << endl;
}
