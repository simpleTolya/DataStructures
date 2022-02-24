#ifndef BINARY_TREE_H
#define BINARY_TREE_H

#include <iterator>
#include <utility>
#include <stack>
#include "IMap.h"

template <class T1, class T2>
class BSTree : public Map<T1, T2> {
protected:
	struct BSTNode : public Map<T1, T2>::Node
	{
		BSTNode *left;
		BSTNode *right;
		BSTNode(const T1 & key, const T2 &value,
				BSTNode * _left = nullptr, BSTNode * _right = nullptr) :
				Map<T1, T2>::Node(key, value)
		{
			left = _left;
			right = _right;
		}
		virtual ~BSTNode() = default;
	};
	// Template Method Pattern
	virtual void 	hook1(std::stack<BSTNode **> &nodes, BSTNode ** node) const {} // push to stack
	virtual BSTNode *hook2(const T1& key, const T2& value) const {
		return new BSTNode(key, value);
	} // return Node need type
	virtual int	hook3(std::stack<BSTNode **> &nodes) {return (0);}
	BSTNode * templateMethodFind(const T1 &key);
	virtual void doBalanceAfterFind(std::stack<BSTNode **> &nodes) {}

	int templateMethodInsert(const T1& key, const T2& value);
	virtual void doBalanceAfterInsert(std::stack<BSTNode **> &nodes) {}

	int templateMethodRemove(const T1& key);
	virtual void doBalanceAfterRemove(std::stack<BSTNode **> &nodes, BSTNode *to_del) {}

	static void rotateRight(BSTNode *& node);
	static void rotateLeft(BSTNode *& node);
	virtual typename Map<T1, T2>::Node	*find(const T1 &key) final {
		return templateMethodFind(key);
	}
	virtual int insert(const T1& key, const T2& value) final {
		return templateMethodInsert(key, value);
	}
	virtual int remove(const T1& key) final {
		return templateMethodRemove(key);
	}
private:
	void _prefix_bypass(void (*function) (T1, T2, int), BSTNode* pp_node, int depth);
	void _infix_bypass(void (*function) (T1, T2, int), BSTNode* pp_node, int depth);
	void _postfix_bypass(void (*function) (T1, T2, int), BSTNode* pp_node, int depth);
	void _prefix_bypass_copy(const BSTNode* p_node);
	void clear(BSTNode* subTree);
public:
	// Iterator
	class const_iterator : public std::iterator<std::bidirectional_iterator_tag,
								const std::pair<const T1 &, const T2 &>>
	{
		friend class BSTree<T1, T2>;
	private:
		BSTNode		**root;
		std::stack <BSTNode *> path;
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
				}
				return *this;
			}
			if (path.top()->right)
			{
				path.push(path.top()->right);
				while (path.top()->left)
					path.push(path.top()->left);
				return *this;
			}
			BSTNode *prev = path.top();
			path.pop();
			while (not path.empty() and path.top()->left != prev)
			{
				prev = path.top();
				path.pop();
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
					while (path.top()->right)
						path.push(path.top()->right);
				}
				return *this;
			}
			if (path.top()->left)
			{
				path.push(path.top()->left);
				while (path.top()->right)
					path.push(path.top()->right);
				return *this;
			}
			BSTNode *prev = path.top();
			path.pop();
			while (not path.empty() and path.top()->right != prev)
			{
				prev = path.top();
				path.pop();
			}
			return *this;
		}
		const_iterator operator-- (int) {
			const_iterator tmp = *this;
			--(*this);
			return tmp;
		}
		bool operator!=(const const_iterator& it) const {
			if (path.empty())
			{
				if (it.path.empty())
					return (false);
				return (true);
			}
			if (it.path.empty())
				return (true);
			return (path.top() != it.path.top());
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
			return (path.top() == it.path.top());
		}
		std::pair<const T1 &, const T2 &> operator*() const {
			return std::pair<const T1 &, const T2 &>(path.top()->getKey(),
													 path.top()->getValue());
		}
	};
	const_iterator begin() const
	{
		const_iterator	iterator;
		iterator.root = reinterpret_cast<BSTNode **>(&this->root);
		++iterator;
		return iterator;
	}
	const_iterator end() const
	{
		const_iterator	iterator;
		iterator.root = reinterpret_cast<BSTNode **>(&this->root);
		return iterator;
	}
	explicit BSTree(const Comparator <T1>* _comparator) {
		Map<T1, T2>::comp = _comparator;
		Map<T1, T2>::root = nullptr;
	}
	virtual ~BSTree() {
		clear(static_cast<BSTNode *>(Map<T1, T2>::root));
	}
	BSTree(const BSTree<T1, T2> & _tree) {
		Map<T1, T2>::root = nullptr;
		Map<T1, T2>::comp = _tree.comp;
		_prefix_bypass_copy(static_cast<BSTNode *>(_tree.root));
	}
	BSTree<T1, T2>& operator=(const BSTree<T1, T2>& _tree) {
		if (&_tree != this) {
			if (Map<T1, T2>::root != nullptr) {
				clear(Map<T1, T2>::root);
				Map<T1, T2>::root = nullptr;
			}
			Map<T1, T2>::comp = _tree.comp;
			_prefix_bypass_copy(_tree.root);
		}
		return (*this);
	}
	void prefixBypass(void (*function) (T1, T2, int))
	{
		_prefix_bypass(function, static_cast<BSTNode *>(Map<T1, T2>::root), 0);
	}
	void postfixBypass(void (*function) (T1, T2, int))
	{
		_postfix_bypass(function, static_cast<BSTNode *>(Map<T1, T2>::root), 0);
	}
	void infixBypass(void (*function) (T1, T2, int))
	{
		_infix_bypass(function, static_cast<BSTNode *>(Map<T1, T2>::root), 0);
	}
};

template <typename T1, typename T2>
void BSTree<T1, T2>::clear(BSTNode* subTree) {
	if (subTree == nullptr) {
		return;
	}
	clear(subTree->left);
	clear(subTree->right);
	delete subTree;
}

template <typename T1, typename T2>
void BSTree<T1, T2>::rotateRight(BSTNode *& node) {
	if (node == nullptr) {
		return;
	}
	BSTNode * tmp = node->left;
	node->left = tmp->right;
	tmp->right = node;
	node = tmp;
}

template <typename T1, typename T2>
void BSTree<T1, T2>::rotateLeft(BSTNode *& node) {
	if (node == nullptr) {
		return;
	}
	BSTNode * tmp = node->right;
	node->right = tmp->left;
	tmp->left = node;
	node = tmp;
}

template <typename T1, typename T2>
void	BSTree<T1, T2>::_prefix_bypass_copy(const BSTNode* p_node) {
	if (p_node == nullptr) {
		return;
	}
	insert(*(p_node->key), *(p_node->value));
	_prefix_bypass_copy(p_node->left);
	_prefix_bypass_copy(p_node->right);
}

template <typename T1, typename T2>
void BSTree<T1, T2>::_prefix_bypass(void (*function) (T1, T2, int), BSTNode* pp_node, int depth) {
	if (pp_node == nullptr) {
		return ;
	}
	function(pp_node->getKey(), pp_node->getValue(), depth);
	_prefix_bypass(function, pp_node->left, depth + 1);
	_prefix_bypass(function, pp_node->right, depth + 1);
}

template <typename T1, typename T2>
void BSTree<T1, T2>::_infix_bypass(void (*function) (T1, T2, int), BSTNode* pp_node, int depth) {
	if (pp_node == nullptr) {
		return;
	}
	_infix_bypass(function, pp_node->left, depth + 1);
	function(pp_node->getKey(), pp_node->getValue(), depth);
	_infix_bypass(function, pp_node->right, depth + 1);
}

template <typename T1, typename T2>
void BSTree<T1, T2>::_postfix_bypass(void (*function) (T1, T2, int), BSTNode* pp_node, int depth) {
	if (pp_node == nullptr) {
		return;
	}
	_postfix_bypass(function, pp_node->left, depth + 1);
	_postfix_bypass(function, pp_node->right, depth + 1);
	function(pp_node->getKey(), pp_node->getValue(), depth);
}

template <typename T1, typename T2>
typename BSTree<T1, T2>::BSTNode * BSTree<T1, T2>::templateMethodFind(const T1 &key)
{
	std::stack <BSTNode **> nodes;
	auto	**current = reinterpret_cast<BSTNode **>(&(Map<T1, T2>::root));
	auto 	finded = *current;
	int 	compare_value;

	while (*current != nullptr)
	{
		compare_value = Map<T1, T2>::comp->compare((*current)->getKey(), key);
		if (compare_value > 0) {
			hook1(nodes, current);
			current = &((*current)->left);
		}
		else if (compare_value < 0) {
			hook1(nodes, current);
			current = &((*current)->right);
		}
		else {
			hook1(nodes, current);
			finded = *current;
			break ;
		}
	}
	doBalanceAfterFind(nodes);
	return (finded);
}

template <typename T1, typename T2>
int	BSTree<T1, T2>::templateMethodInsert(const T1& key, const T2& value)
{
	std::stack <BSTNode **> nodes;
	auto	**current = reinterpret_cast<BSTNode **>(&(Map<T1, T2>::root));
	int 	compare_value;

	while (*current != nullptr)
	{
		compare_value = Map<T1, T2>::comp->compare((*current)->getKey(), key);
		if (compare_value > 0) {
			hook1(nodes, current);
			current = &((*current)->left);
		}
		else if (compare_value < 0) {
			hook1(nodes, current);
			current = &((*current)->right);
		}
		else {
			hook1(nodes, current);
			break ;
		}
	}
	if (*current == nullptr)
	{
		*current = hook2(key, value);
		hook1(nodes, current);
		doBalanceAfterInsert(nodes);
		return (0);
	}
	doBalanceAfterFind(nodes);
	return (1);
}

template <typename T1, typename T2>
int	BSTree<T1, T2>::templateMethodRemove(const T1& key)
{
	std::stack <BSTNode **> nodes;
	auto	**current = reinterpret_cast<BSTNode **>(&(Map<T1, T2>::root));
	int 	compare_value;

	while (*current != nullptr)
	{
		compare_value = Map<T1, T2>::comp->compare((*current)->getKey(), key);
		if (compare_value > 0) {
			hook1(nodes, current);
			current = &((*current)->left);
		}
		else if (compare_value < 0) {
			hook1(nodes, current);
			current = &((*current)->right);
		}
		else {
			hook1(nodes, current);
			break ;
		}
	}
	if (*current == nullptr)
	{
		doBalanceAfterFind(nodes);
		return (1);
	}
	if (hook3(nodes))
		return (0);
	if ((*current)->left and (*current)->right) {
		BSTNode **minNode = &((*current)->right);
		hook1(nodes, minNode);
		while ((*minNode)->left) {
			minNode = &((*minNode)->left);
			hook1(nodes, minNode);
		}
		std::swap((*current)->key, (*minNode)->key);
		std::swap((*current)->value, (*minNode)->value);
		current = minNode;
	}
	BSTNode *to_del = *current;
	if ((*current)->left == nullptr and (*current)->right == nullptr)
		*current = nullptr;
	else if ((*current)->left == nullptr)
		*current = (*current)->right;
	else
		*current = (*current)->left;
	doBalanceAfterRemove(nodes, to_del);
	delete to_del;
	return (0);
}

#endif
