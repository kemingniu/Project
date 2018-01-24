#pragma once
#include"Heap.h"
using namespace std;

template<class W>
struct HuffmanTreeNode
{
	W _w; //Ȩֵ
	HuffmanTreeNode<W>* _left; //����
	HuffmanTreeNode<W>* _right;	//�Һ���
	HuffmanTreeNode<W>* _parent;	//�Һ���

	HuffmanTreeNode(const W& x)
		:_w(x)
		,_left(NULL)
		,_right(NULL)
		,_parent(NULL)
	{}

};

template <class W>
class HaffmanTree
{
	typedef HuffmanTreeNode<W> Node;
public:
	HaffmanTree()
		:_root(NULL)
	{}

	HaffmanTree(W* a, size_t n, const W& invalid)
	{
		//������������
		//ÿ��ѡȡ��ǰȨֵ��С�������ڵ㣬����������
		//���ڵ��Ȩֵ��������������Ȩֵ֮�ͣ��ٴν����ڵ�Ż�ԭ����
		//�ظ��˹���
		
		//�����ݶ�����С��,�Լ�д�Ƚϵķº���

		struct WeightCompare
		{
			bool operator() (const Node* left, const Node* right)
			{
				return left->_w < right->_w;
			}
		};

		Heap<Node*,WeightCompare> minHeap;
		for (size_t i = 0; i < n; ++i)
		{
			if(a[i] != invalid)
				minHeap.Push(new Node(a[i]));
		}
		
		//��minHeap��ֻ��һ���ڵ�ʱ�������ĸ��ڵ�
		while (minHeap.Size() > 1)
		{
			//�Ѷ�ÿ�δ�ŵĶ���_w��С�Ľڵ�
			Node* left = minHeap.Top();
			minHeap.Pop();
			Node* right = minHeap.Top();
			minHeap.Pop();

			//�������ڵ���������
			Node* parent = new Node(left->_w + right->_w);
			parent->_left = left;		
			parent->_right = right;
			left->_parent = parent;
			right->_parent = parent;
			minHeap.Push(parent);
		}

		_root = minHeap.Top();
	}

	Node* GetRoot()
	{
		return _root;
	}
protected:
	Node* _root;
};

void TestHuffman()
{
	int a[] = { 2,0,1,5,3,4,6,7,8,9 };
	HaffmanTree<int> a1(a, sizeof(a)/sizeof(a[0]),0);
}