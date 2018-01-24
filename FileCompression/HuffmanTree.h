#pragma once
#include"Heap.h"
using namespace std;

template<class W>
struct HuffmanTreeNode
{
	W _w; //权值
	HuffmanTreeNode<W>* _left; //左孩子
	HuffmanTreeNode<W>* _right;	//右孩子
	HuffmanTreeNode<W>* _parent;	//右孩子

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
		//构建哈夫曼树
		//每次选取当前权值最小的两个节点，构建子树，
		//父节点的权值等于左右两孩子权值之和，再次将父节点放回原序列
		//重复此过程
		
		//将数据都放入小堆,自己写比较的仿函数

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
		
		//当minHeap中只有一个节点时就是最后的根节点
		while (minHeap.Size() > 1)
		{
			//堆顶每次存放的都是_w最小的节点
			Node* left = minHeap.Top();
			minHeap.Pop();
			Node* right = minHeap.Top();
			minHeap.Pop();

			//构建父节点重新扔入
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