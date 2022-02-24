//
// Created by Mixtape Armand on 10/25/21.
//

#ifndef TREES_B23TREE_H
#define TREES_B23TREE_H

#include <stack>
#include "IMap.h"

template <class T1, class T2>
class B23Tree final : public Map<T1, T2>
{
protected:
	struct B23Node : public Map<T1, T2>::Node
	{
		B23Node *left;
		B23Node *center;
		B23Node *right;
		typename Map<T1, T2>::Node *second;
		B23Node(const T1 & key, const T2 &value, T1 *_secondKey = nullptr,
				T2 *_secondValue = nullptr, B23Node * _left = nullptr,
				B23Node *_center = nullptr, B23Node * _right = nullptr) :
				Map<T1, T2>::Node(key, value)
		{
			second = nullptr;
			if (_secondKey || _secondValue) {
				second = new typename Map<T1, T2>::Node;
				second->key = _secondKey;
				second->value = _secondValue;
			}
			left = _left;
			center = _center;
			right = _right;
		}
		B23Node(T1 * key = nullptr, T2 *value = nullptr, T1 *_secondKey = nullptr,
				T2 *_secondValue = nullptr, B23Node * _left = nullptr,
				B23Node *_center = nullptr, B23Node * _right = nullptr)
		{
			this->key = key;
			this->value = value;
			second = nullptr;
			if (_secondKey || _secondValue) {
				second = new typename Map<T1, T2>::Node;
				second->key = _secondKey;
				second->value = _secondValue;
			}
			left = _left;
			center = _center;
			right = _right;
		}
		inline bool hasRight() const
		{
			return (second != nullptr);
		}
		inline bool isLeaf() const
		{
			return (not left);
		}
		const T1 & getKey(int i) const
		{
			if (i == 0)
				return *(this->key);
			if (i == 1)
				return *(second->key);
			return *(this->key);
		}
		const T2 & getValue(int i) const
		{
			if (i == 0)
				return *(this->value);
			if (i == 1)
				return *(second->value);
			return *(this->value);
		}
		virtual ~B23Node()
		{
			delete second;
		}
	};

	static void sortNode(B23Node *& node);

	virtual typename Map<T1, T2>::Node* find(const T1 &key);
	virtual int insert(const T1& key, const T2& value);
	virtual int remove(const T1& key);

private:
	void _prefix_bypass(void (*function) (T1, T2, int), B23Node* pp_node, int depth);
	void _infix_bypass(void (*function) (T1, T2, int), B23Node* pp_node, int depth);
	void _postfix_bypass(void (*function) (T1, T2, int), B23Node* pp_node, int depth);
	void _prefix_bypass_copy(const B23Node* p_node);
	void clear(B23Node *node);
public:
	class const_iterator : public std::iterator<std::bidirectional_iterator_tag,
								const std::pair<const T1 &, const T2 &>>
	{
		friend class B23Tree<T1, T2>;
	private:
		B23Node		**root;
		bool		is_second;
		std::stack <B23Node *> path;
		const_iterator() = default;
	public:
		const_iterator(const const_iterator & other) = default;
		~const_iterator() = default;
		const_iterator& operator++ () {
			if (path.empty())
			{
				if (*root)
				{
					path.push(*root);
					while (path.top()->left)
						path.push(path.top()->left);
					is_second = false;
				}
				return *this;
			}
			if (is_second)
			{
				if (path.top()->right)
				{
					path.push(path.top()->right);
					while (path.top()->left)
						path.push(path.top()->left);
					is_second = false;
					return *this;
				}
				B23Node *prev = path.top();
				path.pop();
				while (not path.empty())
				{
					if (path.top()->left == prev)
						break ;
					if (path.top()->center == prev and path.top()->hasRight())
						break ;
					prev = path.top();
					path.pop();
				}
				if (path.empty())
					return *this;
				if (path.top()->left == prev)
					is_second = false;
				return *this;
			}
			else
			{
				if (path.top()->center)
				{
					path.push(path.top()->center);
					while (path.top()->left)
						path.push(path.top()->left);
					return *this;
				}
				if (path.top()->second)
				{
					is_second = true;
					return *this;
				}
				B23Node *prev = path.top();
				path.pop();
				while (not path.empty())
				{
					if (path.top()->left == prev)
						break ;
					if (path.top()->center == prev and path.top()->hasRight())
						break ;
					prev = path.top();
					path.pop();
				}
				if (path.empty())
					return *this;
				if (path.top()->left == prev)
					return *this;
				is_second = true;
				return *this;
			}
			return *this;
		}
		const_iterator operator++ (int) {
			const_iterator tmp = *this;
			++(*this);
			return tmp;
		}
		const_iterator& operator--() {
			if (path.empty())
			{
				if (*root)
				{
					path.push(*root);
					while (path.top()->right or path.top()->center)
					{
						if (path.top()->right)
							path.push(path.top()->right);
						else
							path.push(path.top()->center);
					}
					is_second = false;
					if (path.top()->hasRight())
						is_second = true;
				}
				return *this;
			}
			if (is_second)
			{
				if (path.top()->center)
				{
					path.push(path.top()->center);
					while (path.top()->right or path.top()->center)
					{
						if (path.top()->right)
							path.push(path.top()->right);
						else
							path.push(path.top()->center);
					}
					return *this;
				}
				is_second = false;
				return *this;
			}
			else
			{
				if (path.top()->left)
				{
					path.push(path.top()->left);
					while (path.top()->right or path.top()->center)
					{
						if (path.top()->right)
							path.push(path.top()->right);
						else
							path.push(path.top()->center);
					}
					return *this;
				}
				B23Node *prev = path.top();
				path.pop();
				while (not path.empty() and (path.top()->right != prev or
						(path.top()->center != prev)))
				{
					prev = path.top();
					path.pop();
				}
				if (path.empty())
					return *this;
				if (path.top()->right == prev)
					is_second = true;
			}
			return *this;
		}
		const_iterator operator-- (int) {
			const_iterator tmp = *this;
			--(*this);
			return tmp;
		}
		bool operator!=(const const_iterator& it) const {
			return (not this->operator==(it));
		}
		bool operator==(const const_iterator& it) const {
			if (path.empty())
			{
				if (it.path.empty())
					return (true);
				return (false);
			}
			if (it.path.empty())
				return (false);
			if (path.top() == it.path.top())
				return is_second == it.is_second;
			return false;
		}
		std::pair<const T1 &, const T2 &> operator*() const {
			return std::pair<const T1 &, const T2 &>(path.top()->getKey(is_second),
													 path.top()->getValue(is_second));
		}
	};
	const_iterator begin() const
	{
		const_iterator	iterator;
		iterator.root = reinterpret_cast<B23Node **>(&this->root);
		++iterator;
		return iterator;
	}
	const_iterator end() const
	{
		const_iterator	iterator;
		iterator.root = reinterpret_cast<B23Node **>(&this->root);
		return iterator;
	}
	explicit B23Tree(const Comparator <T1>* _comparator) {
		Map<T1, T2>::comp = _comparator;
		Map<T1, T2>::root = nullptr;
	}
	B23Tree& operator=(const B23Tree &other) {
		if (&other != this) {
			clear(B23Tree<T1, T2>::root);
			B23Tree<T1, T2>::comp = other.comp;
			B23Tree<T1, T2>::root = nullptr;
			_prefix_bypass_copy(other.root);
		}
		return (*this);
	}
	B23Tree(const B23Tree & other) : B23Tree<T1, T2>(other.comp) {
		B23Tree<T1, T2>::root = nullptr;
		_prefix_bypass_copy(reinterpret_cast<B23Tree *>(other.root));
	}
	virtual ~B23Tree() {
		clear(static_cast<B23Node *>(Map<T1, T2>::root));
	}
	void prefixBypass(void (*function) (T1, T2, int))
	{
		_prefix_bypass(function, static_cast<B23Node *>(Map<T1, T2>::root), 0);
	}
	void postfixBypass(void (*function) (T1, T2, int))
	{
		_postfix_bypass(function, static_cast<B23Node *>(Map<T1, T2>::root), 0);
	}
	void infixBypass(void (*function) (T1, T2, int))
	{
		_infix_bypass(function, static_cast<B23Node *>(Map<T1, T2>::root), 0);
	}
};

template <class T1, class T2>
void B23Tree<T1, T2>::_prefix_bypass_copy(const B23Node* p_node)
{
	if (p_node == nullptr)
		return ;
	insert(*(p_node->key), *(p_node->value));
	if (p_node->hasRight())
		insert(*(p_node->key), *(p_node->value));
	_prefix_bypass_copy(p_node->left);
	_prefix_bypass_copy(p_node->center);
	_prefix_bypass_copy(p_node->right);
}

template <class T1, class T2>
void B23Tree<T1, T2>::clear(B23Node *node)
{
	if (node == nullptr)
		return ;
	clear(node->right);
	clear(node->center);
	clear(node->left);
	delete node;
}

template <class T1, class T2>
void B23Tree<T1, T2>::_prefix_bypass(void (*function) (T1, T2, int), B23Node* pp_node, int depth)
{
	if (pp_node == nullptr)
		return ;
	function(pp_node->getKey(0), pp_node->getValue(0), depth);
	if (pp_node->hasRight())
		function(pp_node->getKey(1), pp_node->getValue(1), depth);
	_prefix_bypass(function, pp_node->left, depth + 1);
	_prefix_bypass(function, pp_node->center, depth + 1);
	_prefix_bypass(function, pp_node->right, depth + 1);
}

template <class T1, class T2>
void B23Tree<T1, T2>::_infix_bypass(void (*function) (T1, T2, int), B23Node* pp_node, int depth)
{
	if (pp_node == nullptr)
		return ;
	_infix_bypass(function, pp_node->left, depth + 1);
	function(pp_node->getKey(0), pp_node->getValue(0), depth);
	_infix_bypass(function, pp_node->center, depth + 1);
	if (pp_node->hasRight())
	{
		function(pp_node->getKey(1), pp_node->getValue(1), depth);
		_infix_bypass(function, pp_node->right, depth + 1);
	}
}

template <class T1, class T2>
void B23Tree<T1, T2>::_postfix_bypass(void (*function) (T1, T2, int), B23Node* pp_node, int depth)
{
	if (pp_node == nullptr)
		return ;
	_prefix_bypass(function, pp_node->left, depth + 1);
	_prefix_bypass(function, pp_node->center, depth + 1);
	_prefix_bypass(function, pp_node->right, depth + 1);
	function(pp_node->getKey(0), pp_node->getValue(0), depth);
	if (pp_node->hasRight())
		function(pp_node->getKey(1), pp_node->getValue(1), depth);
}

template <class T1, class T2>
void B23Tree<T1, T2>::sortNode(B23Node *& node)
{
	if (Map<T1, T2>::comp->compare(node->getKey(0), node->getKey(1)))
	{
		std::swap(node->key, node->second.key);
		std::swap(node->value, node->second.value);
	}
}


template <class T1, class T2>
typename Map<T1, T2>::Node* B23Tree<T1, T2>::find(const T1 &key)
{
	auto **current = reinterpret_cast<B23Node **>(&(Map<T1, T2>::root));
	int compare_value;

	while (*current != nullptr)
	{
		compare_value = Map<T1, T2>::comp->compare((*current)->getKey(0), key);
		if (compare_value > 0) // left
		{
			current = &((*current)->left);
		}
		else if (compare_value < 0) // center or right
		{
			if ((*current)->hasRight())
			{
				compare_value = Map<T1, T2>::comp->compare((*current)->getKey(1), key);
				if (compare_value > 0) // center
					current = &((*current)->center);
				else if (compare_value < 0) // right
					current = &((*current)->right);
				else
					return (*current);
			}
			else
				current = &((*current)->center);
		}
		else
			return (*current);
	}
	return (nullptr);
}

template <class T1, class T2>
int	B23Tree<T1, T2>::insert(const T1& key, const T2& value)
{
	auto **current = reinterpret_cast<B23Node **>(&(Map<T1, T2>::root));
	std::stack <B23Node **> nodes;
	int compare_value;

	while (*current != nullptr)
	{
		compare_value = Map<T1, T2>::comp->compare((*current)->getKey(0), key);
		if (compare_value > 0) // left
		{
			nodes.push(current);
			current = &((*current)->left);
		}
		else if (compare_value < 0) // center or right
		{
			if ((*current)->hasRight())
			{
				compare_value = Map<T1, T2>::comp->compare((*current)->getKey(1), key);
				if (compare_value > 0) // center
				{
					nodes.push(current);
					current = &((*current)->center);
				}
				else if (compare_value < 0) // right
				{
					nodes.push(current);
					current = &((*current)->right);
				}
				else
					return (1);
			}
			else {
				nodes.push(current);
				current = &((*current)->center);
			}
		}
		else
			return (1);
	}
	// do Balance
	B23Node *tmp = new B23Node(key, value);
	while (not nodes.empty())
	{
		// if has only one key
		if (not (*nodes.top())->hasRight())
		{
			// left of top will be center
			// center of top will be right
			(*nodes.top())->second = new typename Map<T1, T2>::Node;
			if (&(*nodes.top())->left == current) // come from left
			{
				// [KEY SWAP] tmp_key node_key
				(*nodes.top())->second->key = (*nodes.top())->key;
				(*nodes.top())->second->value = (*nodes.top())->value;
				(*nodes.top())->key = tmp->key;
				(*nodes.top())->value = tmp->value;

				// [TREES SWAP] tmp left center
				(*nodes.top())->right = (*nodes.top())->center;
				(*nodes.top())->center = tmp->center;
				(*nodes.top())->left = tmp->left;
			}
			else if (&(*nodes.top())->center == current) // come from center
			{
				// [KEY SWAP] node_key tmp_key
				(*nodes.top())->second->key = tmp->key;
				(*nodes.top())->second->value = tmp->value;

				// [TREES SWAP] left tmp->left tmp->center
				(*nodes.top())->right = tmp->center;
				(*nodes.top())->center = tmp->left;
			}
			tmp->key = nullptr;
			tmp->value = nullptr;
			delete tmp;
			return (0);
		}
		// if has two keys
		if (&(*nodes.top())->left == current) // come from left
		{
			// [KEY SWAP] // tmp_key key_left key_right -> tmp_key key_right      key_left
			std::swap(tmp->key, (*nodes.top())->key);
			std::swap(tmp->value, (*nodes.top())->value);

			B23Node *left_tmp = tmp->left;
			B23Node *center_tmp = tmp->center;
			tmp->left = new B23Node((*nodes.top())->key, (*nodes.top())->value);
			tmp->left->left = left_tmp;
			tmp->left->center = center_tmp;
			tmp->center = new B23Node((*nodes.top())->second->key, (*nodes.top())->second->value);
			tmp->center->left = (*nodes.top())->center;
			tmp->center->center = (*nodes.top())->right;
		}
		else if (&(*nodes.top())->center == current) // come from center
		{
			// [KEY SWAP] left_key tmp_key right_key

			B23Node *left_tmp = tmp->left;
			B23Node *center_tmp = tmp->center;
			tmp->left = new B23Node((*nodes.top())->key, (*nodes.top())->value);
			tmp->left->left = (*nodes.top())->left;
			tmp->left->center = left_tmp;
			tmp->center = new B23Node((*nodes.top())->second->key, (*nodes.top())->second->value);
			tmp->center->left = center_tmp;
			tmp->center->center = (*nodes.top())->right;
		}
		else // come from right
		{
			// [KEY SWAP] left_key right_key tmp_key -> left_key tmp_key      right_key
			std::swap(tmp->key, (*nodes.top())->second->key);
			std::swap(tmp->value, (*nodes.top())->second->value);

			B23Node *left_tmp = tmp->left;
			B23Node *center_tmp = tmp->center;
			tmp->left = new B23Node((*nodes.top())->key, (*nodes.top())->value);
			tmp->left->left = (*nodes.top())->left;
			tmp->left->center = (*nodes.top())->center;
			tmp->center = new B23Node((*nodes.top())->second->key, (*nodes.top())->second->value);
			tmp->center->left = left_tmp;
			tmp->center->center = center_tmp;
		}
		(*nodes.top())->key = nullptr;
		(*nodes.top())->value = nullptr;
		(*nodes.top())->second->key = nullptr;
		(*nodes.top())->second->value = nullptr;
		current = nodes.top();
		delete *nodes.top();
		*nodes.top() = nullptr;
		nodes.pop();
	}
	Map<T1, T2>::root = tmp;
	return (0);
}

template <class T1, class T2>
int	B23Tree<T1, T2>::remove(const T1& key)
{
	auto **current = reinterpret_cast<B23Node **>(&(Map<T1, T2>::root));
	std::stack <B23Node **> nodes;
	int compare_value;

	while (*current != nullptr)
	{
		compare_value = Map<T1, T2>::comp->compare((*current)->getKey(0), key);
		if (compare_value > 0) // left
		{
			nodes.push(current);
			current = &((*current)->left);
		}
		else if (compare_value < 0) // center or right
		{
			if ((*current)->hasRight())
			{
				compare_value = Map<T1, T2>::comp->compare((*current)->getKey(1), key);
				if (compare_value > 0) // center
				{
					nodes.push(current);
					current = &((*current)->center);
				}
				else if (compare_value < 0) // right
				{
					nodes.push(current);
					current = &((*current)->right);
				}
				else
				{
					nodes.push(current);
					// find max of center subtree
					B23Node **max = current;
					nodes.push(&(*current)->center);
					while (*nodes.top())
					{
						max = nodes.top();
						if ((*nodes.top())->hasRight())
							nodes.push(&(*nodes.top())->right);
						else
							nodes.push(&(*nodes.top())->center);
					}
					if (nodes.top() == &(*max)->right)
					{
						std::swap((*current)->second->key, (*max)->second->key);
						std::swap((*current)->second->value, (*max)->second->value);
					}
					else
					{
						std::swap((*current)->second->key, (*max)->key);
						std::swap((*current)->second->value, (*max)->value);
					}
					current = max;
					break ;
				}
			}
			else {
				nodes.push(current);
				current = &((*current)->center);
			}
		}
		else
		{
			nodes.push(current);
			// find max of left subtree
			B23Node **max = current;
			nodes.push(&(*current)->left);
			while (*nodes.top())
			{
				max = nodes.top();
				if ((*nodes.top())->hasRight())
					nodes.push(&(*nodes.top())->right);
				else
					nodes.push(&(*nodes.top())->center);
			}
			if (nodes.top() == &(*max)->right)
			{
				std::swap((*current)->key, (*max)->second->key);
				std::swap((*current)->value, (*max)->second->value);
			}
			else
			{
				std::swap((*current)->key, (*max)->key);
				std::swap((*current)->value, (*max)->value);
			}
			current = max;
			break ;
		}
	}
	if (*current == nullptr)
	{
		return (1);
	}

	current = nodes.top();
	nodes.pop();
	if ((*nodes.top())->hasRight())
	{
		if (&(*nodes.top())->right == current)
		{
			delete (*nodes.top())->second;
			(*nodes.top())->second = nullptr;
		}
		else {
			std::swap((*nodes.top())->second->key, (*nodes.top())->key);
			std::swap((*nodes.top())->second->value, (*nodes.top())->value);
			delete (*nodes.top())->second;
			(*nodes.top())->second = nullptr;
		}
		return (0);
	}
	delete (*nodes.top());
	(*nodes.top()) = nullptr;
	B23Node *tmpSubTree = nullptr;
	current = nodes.top();
	nodes.pop();

	while (not nodes.empty())
	{
		if (&(*nodes.top())->left == current) // left child deleted
		{
			if (not (*nodes.top())->hasRight()) // parent is 2 node
			{
				if ((*nodes.top())->center->hasRight()) // brother is 3 node
				{
					(*nodes.top())->left = new B23Node((*nodes.top())->key,
														(*nodes.top())->value);
					(*nodes.top())->left->left = tmpSubTree;
					(*nodes.top())->left->center = (*nodes.top())->center->left;
					B23Node *center = new B23Node(
								(*nodes.top())->center->second->key,
								(*nodes.top())->center->second->value
								);
					(*nodes.top())->key = (*nodes.top())->center->key;
					(*nodes.top())->value = (*nodes.top())->center->value;
					center->left = (*nodes.top())->center->center;
					center->center = (*nodes.top())->center->right;
					(*nodes.top())->center->key = nullptr;
					(*nodes.top())->center->second->key = nullptr;
					(*nodes.top())->center->value = nullptr;
					(*nodes.top())->center->second->value = nullptr;
					delete (*nodes.top())->center;
					(*nodes.top())->center = center;
					return (0);
				}
				// brother is 2 node
				tmpSubTree = new B23Node((*nodes.top())->key,
										 (*nodes.top())->value,
										 (*nodes.top())->center->key,
										 (*nodes.top())->center->value,
										 tmpSubTree,
										 (*nodes.top())->center->left,
										 (*nodes.top())->center->center);
				(*nodes.top())->key = nullptr;
				(*nodes.top())->value = nullptr;
				(*nodes.top())->center->key = nullptr;
				(*nodes.top())->center->value = nullptr;
				delete (*nodes.top())->center;
				delete (*nodes.top());
				(*nodes.top()) = nullptr;
			}
			else // parent is 3 node
			{
				if ((*nodes.top())->right->hasRight() and
						not (*nodes.top())->center->hasRight())
				{
					(*nodes.top())->center->second = new typename Map<T1, T2>::Node;
					(*nodes.top())->center->second->key = (*nodes.top())->second->key;
					(*nodes.top())->center->second->value = (*nodes.top())->second->value;
					(*nodes.top())->second->key   = (*nodes.top())->right->key;
					(*nodes.top())->second->value = (*nodes.top())->right->value;
					(*nodes.top())->right->key    = (*nodes.top())->right->second->key;
					(*nodes.top())->right->value  = (*nodes.top())->right->second->value;
					(*nodes.top())->right->second->key = nullptr;
					(*nodes.top())->right->second->value = nullptr;
					delete (*nodes.top())->right->second;
					(*nodes.top())->right->second = nullptr;
					(*nodes.top())->center->right = (*nodes.top())->right->left;
					(*nodes.top())->right->left   = (*nodes.top())->right->center;
					(*nodes.top())->right->center = (*nodes.top())->right->right;
					(*nodes.top())->right->right = nullptr;
				}
				if ((*nodes.top())->center->hasRight())
				{
					(*nodes.top())->left = new B23Node((*nodes.top())->key,
														(*nodes.top())->value);
					(*nodes.top())->key = (*nodes.top())->center->key;
					(*nodes.top())->value = (*nodes.top())->center->value;
					(*nodes.top())->left->left = tmpSubTree;
					(*nodes.top())->left->center = (*nodes.top())->center->left;
					B23Node *center = new B23Node(
								(*nodes.top())->center->second->key,
								(*nodes.top())->center->second->value
								);
					center->left = (*nodes.top())->center->center;
					center->center = (*nodes.top())->center->right;
					(*nodes.top())->center->key = nullptr;
					(*nodes.top())->center->second->key = nullptr;
					(*nodes.top())->center->value = nullptr;
					(*nodes.top())->center->second->value = nullptr;
					delete (*nodes.top())->center;
					(*nodes.top())->center = center;
					return (0);
				}
				(*nodes.top())->left = new B23Node(
							(*nodes.top())->key,
							(*nodes.top())->value,
							(*nodes.top())->center->key,
							(*nodes.top())->center->value,
							tmpSubTree,
							(*nodes.top())->center->left,
							(*nodes.top())->center->center
							);
				(*nodes.top())->key   = (*nodes.top())->second->key;
				(*nodes.top())->value = (*nodes.top())->second->value;
				(*nodes.top())->second->key = nullptr;
				(*nodes.top())->second->value = nullptr;
				delete (*nodes.top())->second;
				(*nodes.top())->second = nullptr;
				(*nodes.top())->center->key = nullptr;
				(*nodes.top())->center->value = nullptr;
				delete (*nodes.top())->center;
				(*nodes.top())->center = (*nodes.top())->right;
				(*nodes.top())->right = nullptr;
				return (0);
			}
		}
		else if (&(*nodes.top())->center == current) // center child deleted
		{
			if (not (*nodes.top())->hasRight()) // parent is 2 node
			{
				if ((*nodes.top())->left->hasRight()) // left brother is 3 node
				{
					(*nodes.top())->center = new B23Node(
								(*nodes.top())->key,
								(*nodes.top())->value);
					(*nodes.top())->center->left   = (*nodes.top())->left->right;
					(*nodes.top())->center->center = tmpSubTree;
					(*nodes.top())->key   = (*nodes.top())->left->second->key;
					(*nodes.top())->value = (*nodes.top())->left->second->value;
					(*nodes.top())->left->second->key   = nullptr;
					(*nodes.top())->left->second->value = nullptr;
					delete ((*nodes.top())->left->second);
					(*nodes.top())->left->second = nullptr;
					(*nodes.top())->left->right = nullptr;
					return (0);
				}
				// brother is 2 node
				tmpSubTree = new B23Node((*nodes.top())->left->key,
										 (*nodes.top())->left->value,
										 (*nodes.top())->key,
										 (*nodes.top())->value,
										 (*nodes.top())->left->left,
										 (*nodes.top())->left->center,
										 tmpSubTree);
				(*nodes.top())->left->key = nullptr;
				(*nodes.top())->left->value = nullptr;
				(*nodes.top())->key = nullptr;
				(*nodes.top())->value = nullptr;
				delete (*nodes.top())->left;
				delete (*nodes.top());
				(*nodes.top()) = nullptr;
			}
			else // parent is 3 node
			{
				if ((*nodes.top())->left->hasRight()) // left brother is 3 node
				{
					(*nodes.top())->center = new B23Node(
								(*nodes.top())->key,
								(*nodes.top())->value
								);
					(*nodes.top())->center->left  = (*nodes.top())->left->right;
					(*nodes.top())->left->right   = nullptr;
					(*nodes.top())->center->center = tmpSubTree;
					(*nodes.top())->key   = (*nodes.top())->left->second->key;
					(*nodes.top())->value = (*nodes.top())->left->second->value;
					(*nodes.top())->left->second->key   = nullptr;
					(*nodes.top())->left->second->value = nullptr;
					delete ((*nodes.top())->left->second);
					(*nodes.top())->left->second = nullptr;
					return (0);
				}
				else if ((*nodes.top())->right->hasRight()) // right brother is 3 node
				{
					(*nodes.top())->center = new B23Node(
								(*nodes.top())->second->key,
								(*nodes.top())->second->value
								);
					(*nodes.top())->center->left  = tmpSubTree;
					(*nodes.top())->center->center = (*nodes.top())->right->left;
					(*nodes.top())->second->key   = (*nodes.top())->right->key;
					(*nodes.top())->second->value = (*nodes.top())->right->value;
					(*nodes.top())->right->key    = (*nodes.top())->right->second->key;
					(*nodes.top())->right->value  = (*nodes.top())->right->second->value;
					(*nodes.top())->right->second->key   = nullptr;
					(*nodes.top())->right->second->value = nullptr;
					(*nodes.top())->right->left   = (*nodes.top())->right->center;
					(*nodes.top())->right->center = (*nodes.top())->right->right;
					(*nodes.top())->right->right  = nullptr;
					delete ((*nodes.top())->right->second);
					(*nodes.top())->right->second = nullptr;
					return (0);
				}
				(*nodes.top())->center = new B23Node(
							(*nodes.top())->second->key,
							(*nodes.top())->second->value,
							(*nodes.top())->right->key,
							(*nodes.top())->right->value,
							tmpSubTree,
							(*nodes.top())->right->left,
							(*nodes.top())->right->center
							);
				(*nodes.top())->right->key   = nullptr;
				(*nodes.top())->right->value = nullptr;
				delete (*nodes.top())->right;
				(*nodes.top())->right = nullptr;
				(*nodes.top())->second->key = nullptr;
				(*nodes.top())->second->value = nullptr;
				delete (*nodes.top())->second;
				(*nodes.top())->second = nullptr;
				return (0);
			}
		}
		else // right child deleted
		{
			// parent is 3 node
			if ((*nodes.top())->left->hasRight() and
					not (*nodes.top())->center->hasRight())
			{
				(*nodes.top())->center->second = new typename Map<T1, T2>::Node;
				(*nodes.top())->center->second->key   = (*nodes.top())->center->key;
				(*nodes.top())->center->second->value = (*nodes.top())->center->value;
				(*nodes.top())->center->key   = (*nodes.top())->key;
				(*nodes.top())->center->value = (*nodes.top())->value;
				(*nodes.top())->key   = (*nodes.top())->left->second->key;
				(*nodes.top())->value = (*nodes.top())->left->second->value;
				(*nodes.top())->left->second->key   = nullptr;
				(*nodes.top())->left->second->value = nullptr;
				(*nodes.top())->center->right  = (*nodes.top())->center->center;
				(*nodes.top())->center->center = (*nodes.top())->center->left;
				(*nodes.top())->center->left   = (*nodes.top())->left->right;
				(*nodes.top())->left->right = nullptr;
				delete ((*nodes.top())->left->second);
				(*nodes.top())->left->second = nullptr;
			}
			if ((*nodes.top())->center->hasRight())
			{
				(*nodes.top())->right = new B23Node(
							(*nodes.top())->second->key,
							(*nodes.top())->second->value
							);
				(*nodes.top())->second->key = (*nodes.top())->center->second->key;
				(*nodes.top())->second->value = (*nodes.top())->center->second->value;
				(*nodes.top())->center->second->key   = nullptr;
				(*nodes.top())->center->second->value = nullptr;
				(*nodes.top())->right->left   = (*nodes.top())->center->right;
				(*nodes.top())->right->center = tmpSubTree;
				(*nodes.top())->center->right = nullptr;
				delete (*nodes.top())->center->second;
				(*nodes.top())->center->second = nullptr;
				return (0);
			}
			(*nodes.top())->center->second = new typename Map<T1, T2>::Node;
			(*nodes.top())->center->second->key   = (*nodes.top())->second->key;
			(*nodes.top())->center->second->value = (*nodes.top())->second->value;
			(*nodes.top())->second->key = nullptr;
			(*nodes.top())->second->value = nullptr;
			delete (*nodes.top())->second;
			(*nodes.top())->second = nullptr;
			(*nodes.top())->center->right = tmpSubTree;
			return (0);
		}
		current = nodes.top();
		nodes.pop();
	}
	Map<T1, T2>::root = tmpSubTree;
	return (0);
}

#endif //TREES_B23TREE_H
