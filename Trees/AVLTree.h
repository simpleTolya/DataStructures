//
// Created by Mixtape Armand on 10/22/21.
//

#ifndef TREES_AVL_TREE_H
#define TREES_AVL_TREE_H

#include "BSTree.h"

template <class T1, class T2>
class AVLTree : public BSTree <T1, T2>
{
protected:
	struct AVLNode : public BSTree <T1, T2>::BSTNode
	{
		signed char depth;
		AVLNode(const T1 & key, const T2 &value,
				typename BSTree<T1, T2>::BSTNode * _left = nullptr,
				typename BSTree<T1, T2>::BSTNode * _right = nullptr) :
				BSTree<T1, T2>::BSTNode(key, value, _left, _right)
		{
			depth = 1;
		}
		virtual ~AVLNode() = default;
	};
	static int	balanceFactor(AVLNode *& node);
	static void updateDepth(AVLNode *& node);
	static void doBalanceNode(AVLNode *& node);

	// Template Method Pattern
	virtual void	doBalanceAfterFind(std::stack<typename BSTree<T1, T2>::BSTNode **> &nodes) override {}
	virtual void 	hook1(std::stack<typename BSTree<T1, T2>::BSTNode **> &nodes,
						   typename BSTree<T1, T2>::BSTNode ** node) const override {
		nodes.push(node);
	} // push to stack
	virtual typename BSTree<T1, T2>::BSTNode *hook2(const T1& key, const T2& value) const override {
		return new AVLNode(key, value);
	} // return Node need type
	virtual void	doBalanceAfterInsert(std::stack<typename BSTree<T1, T2>::BSTNode **> &nodes) override ;
	virtual void	doBalanceAfterRemove(std::stack<typename BSTree<T1, T2>::BSTNode **> &nodes,
										 typename BSTree<T1, T2>::BSTNode *to_del) override {
		doBalanceAfterInsert(nodes);
	}

public:
	explicit AVLTree(const Comparator <T1>* comparator) : BSTree<T1, T2>(comparator) {}
	AVLTree& operator=(const AVLTree &other) {
		if (&other != this) {
			clear(BSTree<T1, T2>::root);
			BSTree<T1, T2>::comp = other.comp;
			BSTree<T1, T2>::root = nullptr;
			_prefix_bypass_copy(other.root);
		}
		return (*this);
	}
	AVLTree(const AVLTree & other) : BSTree<T1, T2>(other.comp) {
		BSTree<T1, T2>::root = nullptr;
		_prefix_bypass_copy(reinterpret_cast<AVLNode *>(other.root));
	}
};

template <class T1, class T2>
int	AVLTree<T1, T2>::balanceFactor(AVLNode *&node) {
	if (node == nullptr) {
		return (0);
	}
	int right = 0;
	if (node->right) {
		right = reinterpret_cast<AVLNode *&>(node->right)->depth;
	}
	int left = 0;
	if (node->left) {
		left = reinterpret_cast<AVLNode *&>(node->left)->depth;
	}
	return (right - left);
}

template <class T1, class T2>
void	AVLTree<T1, T2>::updateDepth(AVLNode *& node)
{
	signed char right = 0;
	if (node->right) {
		right = reinterpret_cast<AVLNode *&>(node->right)->depth;
	}
	signed char left = 0;
	if (node->left) {
		left = reinterpret_cast<AVLNode *&>(node->left)->depth;
	}
	node->depth = (right > left ? right : left) + 1;
}

template <class T1, class T2>
void	AVLTree<T1, T2>::doBalanceNode(AVLNode *& node)
{
	updateDepth(node);
	int bf = balanceFactor(node);
	if (bf == 2) { // right > left
		if (balanceFactor(reinterpret_cast<AVLNode *&>(node->right)) < 0) {
			BSTree<T1, T2>::rotateRight(node->right);
			updateDepth(reinterpret_cast<AVLNode *&>(node->right->right));
			updateDepth(reinterpret_cast<AVLNode *&>(node->right));
		}
		BSTree<T1, T2>::rotateLeft(reinterpret_cast<typename BSTree<T1, T2>::BSTNode *&>(node));
		updateDepth(reinterpret_cast<AVLNode *&>(node->left));
		updateDepth(reinterpret_cast<AVLNode *&>(node));
		return;
	}
	if (bf == -2) { // left > right
		if (balanceFactor(reinterpret_cast<AVLNode *&>(node->left)) > 0) {
			BSTree<T1, T2>::rotateLeft(node->left);
			updateDepth(reinterpret_cast<AVLNode *&>(node->left->left));
			updateDepth(reinterpret_cast<AVLNode *&>(node->left));
		}
		BSTree<T1, T2>::rotateRight(reinterpret_cast<typename BSTree<T1, T2>::BSTNode *&>(node));
		updateDepth(reinterpret_cast<AVLNode *&>(node->right));
		updateDepth(reinterpret_cast<AVLNode *&>(node));
	}
}

template <class T1, class T2>
void	AVLTree<T1, T2>::doBalanceAfterInsert(std::stack<typename BSTree<T1, T2>::BSTNode **> &nodes)
{
	nodes.pop();
	while (not nodes.empty())
	{
		if (balanceFactor(reinterpret_cast<AVLNode *&>(*nodes.top())) == 0)
			return ;
		doBalanceNode(reinterpret_cast<AVLNode *&>(*nodes.top()));
		nodes.pop();
	}
}

#endif //TREES_AVL_TREE_H
