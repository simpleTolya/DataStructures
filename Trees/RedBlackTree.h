//
// Created by Mixtape Armand on 10/24/21.
//

#ifndef TREES_RED_BLACK_TREE_H
#define TREES_RED_BLACK_TREE_H

#include "BSTree.h"

template <class T1, class T2>
class RBTree : public BSTree<T1, T2>
{
protected:
	struct RBNode : public BSTree <T1, T2>::BSTNode
	{
		bool is_red;
		RBNode(const T1 & key, const T2 &value,
				typename BSTree<T1, T2>::BSTNode * _left = nullptr,
				typename BSTree<T1, T2>::BSTNode * _right = nullptr) :
				BSTree<T1, T2>::BSTNode(key, value, _left, _right)
		{
			is_red = true;
		}
		virtual ~RBNode() = default;
	};

	virtual void 	hook1(std::stack<typename BSTree <T1, T2>::BSTNode **> &nodes,
						   typename BSTree <T1, T2>::BSTNode ** node) const override {
		nodes.push(node);
	}
	virtual typename BSTree <T1, T2>::BSTNode *hook2(const T1& key, const T2& value) const override {
		return new RBNode(key, value);
	} // return Node need type

	static inline bool	isRed(RBNode *node);

	virtual void doBalanceAfterFind(std::stack<typename BSTree <T1, T2>::BSTNode **> &nodes) override {}
	virtual void doBalanceAfterInsert(std::stack<typename BSTree <T1, T2>::BSTNode **> &nodes) override;
	virtual void doBalanceAfterRemove(std::stack<typename BSTree <T1, T2>::BSTNode **> &nodes,
									  typename BSTree <T1, T2>::BSTNode *to_del) override;
public:
	explicit RBTree(const Comparator <T1>* _comparator) : BSTree<T1, T2>(_comparator) {}
	RBTree(const RBTree & other) {
		_prefix_bypass_copy(other.root);
	}
	RBTree<T1, T2>& operator=(const RBTree& other) {
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
bool	RBTree<T1, T2>::isRed(RBNode *node)
{
	return node ? node->is_red : false;
}

template <class T1, class T2>
void RBTree<T1, T2>::doBalanceAfterInsert(std::stack<typename BSTree<T1, T2>::BSTNode **> &nodes)
{
	// top is inserted node
	while (not nodes.empty())
	{
		auto child = nodes.top();
		nodes.pop();
		if (child == reinterpret_cast<typename BSTree<T1, T2>::BSTNode **>(& this->BSTree<T1, T2>::root)) {
			static_cast<RBNode *>(*child)->is_red = 0;
			return ;
		}
		if (isRed(reinterpret_cast<RBNode *>(*nodes.top())))
		{
			typename BSTree<T1, T2>::BSTNode *&parent = (*nodes.top());
			nodes.pop();
			if (&parent == &(*nodes.top())->left) // see right uncle
			{
				if (isRed(static_cast<RBNode *>((*nodes.top())->right))) // red uncle
				{
					static_cast<RBNode *>((*nodes.top()))->is_red = 1;
					static_cast<RBNode *>((*nodes.top())->left)->is_red = 0;
					static_cast<RBNode *>((*nodes.top())->right)->is_red = 0;
				}
				else // black uncle
				{
					if (&parent->right == child)
						BSTree<T1, T2>::rotateLeft(parent);
					BSTree<T1, T2>::rotateRight(*nodes.top());
					static_cast<RBNode *>((*nodes.top()))->is_red = 0;
					static_cast<RBNode *>((*nodes.top())->right)->is_red = 1;
					return ;
				}
			}
			else // see left uncle
			{
				if (isRed(static_cast<RBNode *>((*nodes.top())->left))) // red uncle
				{
					static_cast<RBNode *>((*nodes.top()))->is_red = 1;
					static_cast<RBNode *>((*nodes.top())->left)->is_red = 0;
					static_cast<RBNode *>((*nodes.top())->right)->is_red = 0;
				}
				else // black uncle
				{
					if (&parent->left == child)
						BSTree<T1, T2>::rotateRight(parent);
					BSTree<T1, T2>::rotateLeft(*nodes.top());
					static_cast<RBNode *>((*nodes.top()))->is_red = 0;
					static_cast<RBNode *>((*nodes.top())->left)->is_red = 1;
					return ;
				}
			}
		}
		else
			break;
	}
}

template <class T1, class T2>
void RBTree<T1, T2>::doBalanceAfterRemove(std::stack<typename BSTree <T1, T2>::BSTNode **> &nodes,
										  typename BSTree <T1, T2>::BSTNode *to_del)
{
	// top elem is ptr to removed node
	if (static_cast<RBNode *>(to_del)->is_red)
		return ;
	if (to_del->left or to_del->right) // case then only one child
	{
		(*reinterpret_cast<RBNode **>(nodes.top()))->is_red = 0;
		return ;
	}
	while (not nodes.empty())
	{
		auto child = nodes.top();
		nodes.pop();
		if (nodes.empty())
			return ;
		if (nodes.top() == reinterpret_cast<typename BSTree<T1, T2>::BSTNode **>(&this->root))
		{
			if (*nodes.top())
				static_cast<RBNode *>(*nodes.top())->is_red = 0;
			return ;
		}
		if (child == &(*nodes.top())->right) // see left
		{
			if (isRed(static_cast<RBNode *>(*nodes.top()))) // dad red
			{
				if (isRed(static_cast<RBNode *>((*nodes.top())->left->right)))
				{
					static_cast<RBNode *>(*nodes.top())->is_red = 0;
					BSTree<T1, T2>::rotateLeft((*nodes.top())->left);
				}
				BSTree<T1, T2>::rotateRight(*nodes.top());
				return ;
			}
			else // black dad
			{
				if (isRed(static_cast<RBNode *>((*nodes.top())->left))) // brother red
				{
					if (not isRed(static_cast<RBNode *>((*nodes.top())->left->right->left))
						and isRed(static_cast<RBNode *>((*nodes.top())->left->right->right)))
					{
						static_cast<RBNode *>((*nodes.top())->left->right->right)->is_red = 0;
						static_cast<RBNode *>((*nodes.top())->left->right)->is_red = 1;
						BSTree<T1, T2>::rotateLeft((*nodes.top())->left->right);
					}
					if (isRed(static_cast<RBNode *>((*nodes.top())->left->right->left)))
					{
						static_cast<RBNode *>((*nodes.top())->left->right->left)->is_red = 0;
						BSTree<T1, T2>::rotateLeft((*nodes.top())->left);
						BSTree<T1, T2>::rotateRight(*nodes.top());
						return ;
					}
					static_cast<RBNode *>((*nodes.top())->left)->is_red = 0;
					static_cast<RBNode *>((*nodes.top())->left->right)->is_red = 1;
					BSTree<T1, T2>::rotateRight(*nodes.top());
					return ;
				}
				else // brother black
				{
					if (isRed(static_cast<RBNode *>((*nodes.top())->left->right)))
					{
						BSTree<T1, T2>::rotateLeft((*nodes.top())->left);
						BSTree<T1, T2>::rotateRight(*nodes.top());
						static_cast<RBNode *>(*nodes.top())->is_red = 0;
						return ;
					}
					if (isRed(static_cast<RBNode *>((*nodes.top())->left->left)))
					{
						BSTree<T1, T2>::rotateRight(*nodes.top()); // SEG
						static_cast<RBNode *>((*nodes.top())->left)->is_red = 0;
						return ;
					}
					static_cast<RBNode *>((*nodes.top())->left)->is_red = 1;
				}
			}
		}
		else // see right
		{
			if (isRed(static_cast<RBNode *>(*nodes.top()))) // red dad
			{
				if (isRed(static_cast<RBNode *>((*nodes.top())->right->left)))
				{
					static_cast<RBNode *>(*nodes.top())->is_red = 0;
					BSTree<T1, T2>::rotateRight((*nodes.top())->right);
				}
				BSTree<T1, T2>::rotateLeft(*nodes.top());
				return ;
			}
			else // black dad
			{
				if (isRed(static_cast<RBNode *>((*nodes.top())->right))) // brother red
				{ //
					if (not isRed(static_cast<RBNode *>((*nodes.top())->right->left->right)) //
						and isRed(static_cast<RBNode *>((*nodes.top())->right->left->left))) //
					{
						static_cast<RBNode *>((*nodes.top())->right->left->left)->is_red = 0;
						static_cast<RBNode *>((*nodes.top())->right->left)->is_red = 1;
						BSTree<T1, T2>::rotateRight((*nodes.top())->right->left);
					}
					if (isRed(static_cast<RBNode *>((*nodes.top())->right->left->right)))
					{
						static_cast<RBNode *>((*nodes.top())->right->left->right)->is_red = 0;
						BSTree<T1, T2>::rotateRight((*nodes.top())->right);
						BSTree<T1, T2>::rotateLeft(*nodes.top());
						return ;
					}
					static_cast<RBNode *>((*nodes.top())->right)->is_red = 0;
					static_cast<RBNode *>((*nodes.top())->right->left)->is_red = 1;
					BSTree<T1, T2>::rotateLeft(*nodes.top());
					return ;
				}
				else // brother black
				{
					if (isRed(static_cast<RBNode *>((*nodes.top())->right->left)))
					{
						BSTree<T1, T2>::rotateRight((*nodes.top())->right);
						BSTree<T1, T2>::rotateLeft(*nodes.top());
						static_cast<RBNode *>(*nodes.top())->is_red = 0;
						return ;
					}
					if (isRed(static_cast<RBNode *>((*nodes.top())->right->right)))
					{
						BSTree<T1, T2>::rotateLeft(*nodes.top());
						static_cast<RBNode *>((*nodes.top())->right)->is_red = 0;
						return ;
					}
					static_cast<RBNode *>((*nodes.top())->right)->is_red = 1;
				}
			}
		}
	}
}

#endif //TREES_RED_BLACK_TREE_H
