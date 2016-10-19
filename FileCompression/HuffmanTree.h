/*************************************************************************
	> File Name: HuffmanTree.h
	> Author: XH
	> Mail: X_H_fight@163.com 
	> Created Time: Sat 15 Oct 2016 01:48:58 PM CST
 ************************************************************************/
#pragma once
#include "Heap.h"

template <class T>
struct HuffmanTreeNode
{
	T _weight;
	HuffmanTreeNode<T>* _left;
	HuffmanTreeNode<T>* _right;
	HuffmanTreeNode<T>* _parent;

	HuffmanTreeNode(const T& w)
		:_weight(w)
		 ,_left(NULL)
		 ,_right(NULL)
		 ,_parent(NULL)
	{}
};

template <class T>
class HuffmanTree
{
	typedef HuffmanTreeNode<T> Node;
public:
	HuffmanTree()
	:_root(NULL)
	{}

	HuffmanTree(T* arr, size_t size, const T& invalid)
	{
		struct NodeLess
		{
			bool operator()(const Node* node1, const Node* node2) const
			{
				return node1->_weight < node2->_weight;
			}
		};

		Heap<Node*, NodeLess> minHeap;
		//将出现次数不为0的字符push进堆里
		for(int i=0; i<256; ++i)
		{
			if(arr[i]._count != 0)
			{
				Node* node = new Node(arr[i]);
				minHeap.Push(node);
			}
		}
		
		//建树
		while(minHeap.Size() > 1)
		{
			Node* left = minHeap.Top();
			minHeap.Pop();
			Node* right = minHeap.Top();
			minHeap.Pop();

			Node* parent = new Node(left->_weight + right->_weight);
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
	~HuffmanTree()
	{
		_DestoryTree(_root);
		_root = NULL;
	}
protected:
	void _DestoryTree(Node* root)
	{
		if(root == NULL)
			return;
		_DestoryTree(root->_left);
		_DestoryTree(root->_right);

		delete root;
		root = NULL;
	}
protected:
	Node* _root;
};
		