//
// Created by Mixtape Armand on 10/22/21.
//

#ifndef TREES_SPLAY_TREE_H
#define TREES_SPLAY_TREE_H

#include "BSTree.h"

template <class T1, class T2>
class SplayTree : public BSTree<T1, T2> {
protected:
	// Template Method Pattern
	virtual void 	hook1(std::stack<typename BSTree<T1, T2>::BSTNode **> &nodes,
						   typename BSTree<T1, T2>::BSTNode ** node) const override {
		nodes.push(node);
	} // push to stack
	virtual typename BSTree<T1, T2>::BSTNode *hook2(const T1& key, const T2& value) const override {
		return new typename BSTree<T1, T2>::BSTNode(key, value);
	} // return Node need type
	virtual int	hook3(std::stack<typename BSTree<T1, T2>::BSTNode **> &nodes);

	virtual void doBalanceAfterFind(std::stack<typename BSTree<T1, T2>::BSTNode **> &nodes) override ; // splay operation
	virtual void doBalanceAfterInsert(std::stack<typename BSTree<T1, T2>::BSTNode **> &nodes) override {
		doBalanceAfterFind(nodes);
	}

public:
	explicit SplayTree(const Comparator <T1>* _comparator) : BSTree<T1, T2>(_comparator) {}
	SplayTree(const SplayTree & other) {
		_prefix_bypass_copy(other.root);
	}
	SplayTree<T1, T2>& operator=(const SplayTree& other) {
		if (&other != this) {
			clear(BSTree<T1, T2>::root);
			BSTree<T1, T2>::root = nullptr;
			BSTree<T1, T2>::comp = other.comp;
			_prefix_bypass_for_copy(other.root);
		}
		return (*this);
	}
};

template <class T1, class T2>
void SplayTree<T1, T2>::doBalanceAfterFind(std::stack<typename BSTree<T1, T2>::BSTNode **> &nodes)
{
	if (nodes.empty())
		return ;
	typename BSTree<T1, T2>::BSTNode ** current = nullptr;
	current = nodes.top();
	nodes.pop();

	typename BSTree<T1, T2>::BSTNode ** parent = nullptr;
	typename BSTree<T1, T2>::BSTNode ** parparent = nullptr;
	while (!nodes.empty()) {
		parent = nodes.top();
		nodes.pop();
		if (nodes.empty()) {
			if ((*parent)->left == (*current)) {
				BSTree<T1, T2>::rotateRight(*parent);
			} else {
				BSTree<T1, T2>::rotateLeft(*parent);
			}
			break;
		}
		parparent = nodes.top();
		nodes.pop();

		if ((*parparent)->left == (*parent)) {
			if ((*parent)->left == (*current)) {
				BSTree<T1, T2>::rotateRight(*parparent);
				BSTree<T1, T2>::rotateRight(*parparent);
			} else {
				BSTree<T1, T2>::rotateLeft((*parparent)->left);
				BSTree<T1, T2>::rotateRight((*parparent));
			}
		} else {
			if ((*parent)->right == (*current)) {
				BSTree<T1, T2>::rotateLeft(*parparent);
				BSTree<T1, T2>::rotateLeft(*parparent);
			} else {
				BSTree<T1, T2>::rotateRight((*parparent)->right);
				BSTree<T1, T2>::rotateLeft((*parparent));
			}
		}
		current = parparent;
	}
}

#include <iostream>
template <class T1, class T2>
int	SplayTree<T1, T2>::hook3(std::stack<typename BSTree<T1, T2>::BSTNode **> &nodes)
{
	doBalanceAfterFind(nodes);
	if (static_cast<typename BSTree<T1, T2>::BSTNode *>(BSTree<T1, T2>::root)->left == nullptr) {
		auto *tmp = static_cast<typename BSTree<T1, T2>::BSTNode *>(BSTree<T1, T2>::root);
		BSTree<T1, T2>::root = static_cast<typename BSTree<T1, T2>::BSTNode*>(BSTree<T1, T2>::root)->right;
		delete tmp;
		return (1);
	}
	typename BSTree<T1, T2>::BSTNode **left_tree =
			&static_cast<typename BSTree<T1, T2>::BSTNode *>(BSTree<T1, T2>::root)->left;
	std::stack<typename BSTree<T1, T2>::BSTNode **> pathToMin;
	//pathToMin.push(reinterpret_cast<typename BSTree<T1, T2>::BSTNode**>(& this->root));
	pathToMin.push(left_tree);
	while ((*pathToMin.top())->right) {
		pathToMin.push(&(*pathToMin.top())->right);
	}
	doBalanceAfterFind(pathToMin);
	static_cast<typename BSTree<T1, T2>::BSTNode*>(BSTree<T1, T2>::root)->left->right =
			static_cast<typename BSTree<T1, T2>::BSTNode*>(BSTree<T1, T2>::root)->right;
	left_tree = &static_cast<typename BSTree<T1, T2>::BSTNode*>(BSTree<T1, T2>::root)->left;
	auto to_del = this->root;
	BSTree<T1, T2>::root = *left_tree;
	delete to_del;
	return (1);
}

#endif //TREES_SPLAY_TREE_H
