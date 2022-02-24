#ifndef BINOM_HEAP_H
#define BINOM_HEAP_H


#include <list>
#include <optional>
#include "Heap.h"

template <class T1, class T2>
class BinomialHeap;

template <class T1, class T2>
class BinomialHeapCreator : public HeapCreator<T1, T2>
{
public:
	virtual Heap<T1, T2> * createHeap(const Comparator<T1> *comp)
	{
		return new BinomialHeap<T1, T2>(comp);
	}
};

template <class T1, class T2>
class BinomialHeap : public Heap<T1, T2>
{
	friend BinomialHeapCreator<T1, T2>;
protected:
	using Node = typename Heap<T1, T2>::Node;
	struct BinomialTree
	{
		std::list<BinomialTree*>	trees;
		Node	root;
		unsigned char	degree;

		BinomialTree(const T1 &key, const T2 &value) :
			root(key, value), degree(0)
		{}
		BinomialTree(const BinomialTree & tree) :
			degree(tree.degree), root(tree.root)
		{
			for (auto tree : tree.trees)
			{
				auto *copy = new BinomialTree(*tree);
				trees.push_back(copy);
			}
		}
	};
	using ListIterator = typename std::list<BinomialTree *>::iterator;

	static void	deepClear(BinomialTree &to_del);

	std::list<BinomialTree *> bheaps;
	typename std::list<BinomialTree *>::iterator maxTree;

	BinomialTree * _merge(BinomialTree *left, BinomialTree *right);
	std::optional<ListIterator>	 _insertFrom(ListIterator from,
					BinomialHeap<T1, T2>::BinomialTree	*tree);
	virtual	int _insert(const std::pair<T1, T2>	&key_value) override;
	virtual int _merge(const Heap<T1, T2> &heap) override;
	virtual int _getMax(Node *& node) const override;
	virtual int _removeMax() override;

	BinomialHeap(const Comparator<T1> * comp)
	{
		maxTree = bheaps.end();
		this->comp = comp;
	}

public:
	BinomialHeap(const BinomialHeap & other)
	{
		this->comp = other.comp;
		for (auto it = other.bheaps.begin(); it != other.bheaps.end(); ++it)
		{
			auto *copy = new BinomialTree(**it);
			bheaps.push_back(copy);
			if (it == other.maxTree)
			{
				this->maxTree = --bheaps.end();
			}
		}
	}
	~BinomialHeap()
	{
		for (auto tree : bheaps)
		{
			deepClear(*tree);
		}
	}
	BinomialHeap &	operator=(const BinomialHeap & other)
	{
		this->comp = other.comp;
		for (auto tree : bheaps)
		{
			deepClear(*tree);
		}
		for (auto it = other.bheaps.begin(); it != other.bheaps.end(); ++it)
		{
			auto *copy = new BinomialTree(**it);
			bheaps.push_back(copy);
			if (it == other.maxTree)
			{
				this->maxTree = --bheaps.end();
			}
		}
	}
};

template <class T1, class T2>
typename BinomialHeap<T1, T2>::BinomialTree	*BinomialHeap<T1, T2>::_merge(
									BinomialHeap<T1, T2>::BinomialTree *left,
									BinomialHeap<T1, T2>::BinomialTree *right)
{
//	if (left->degree != right->degree)
//		return nullptr;
	int compare_value = this->comp->compare(left->root.key,
											right->root.key);
	if (compare_value > 0)
	{
		left->trees.push_front(right);
		left->degree++;
		return	left;
	}
	if (compare_value < 0)
	{
		right->trees.push_front(left);
		right->degree++;
		return	right;
	}
	return nullptr;
}

template <class T1, class T2>
void BinomialHeap<T1, T2>::deepClear(BinomialTree &to_del)
{
	if (to_del.degree == 0)
		return ;
	for (auto tree : to_del.trees)
	{
		deepClear(*tree);
	}
	delete &to_del;
}

template <class T1, class T2>
int BinomialHeap<T1, T2>::_insert(const std::pair<T1, T2>	&key_value)
{
	auto *tmp = new BinomialHeap<T1, T2>::BinomialTree(
					key_value.first, key_value.second
				);
	if (_insertFrom(bheaps.begin(), tmp) == std::nullopt) {
		delete tmp;
		return (1);
	}
	return (0);
}

template <class T1, class T2>
std::optional<typename BinomialHeap<T1, T2>::ListIterator>
        BinomialHeap<T1, T2>::_insertFrom(ListIterator from,
									  		BinomialHeap<T1, T2>::BinomialTree	*tree)
{
	auto it = from;
	for (; it != bheaps.end();)
	{
		if ((*it)->degree == tree->degree)
		{
			if (it == maxTree)
				maxTree = bheaps.end();
			tree = _merge(tree, *it);
			if (not tree)
				return (std::nullopt);
			++it;
			bheaps.pop_front();
		}
		else if ((*it)->degree < tree->degree) {
			++it;
			continue;
		}
		else
			break ;
	}
	it = bheaps.insert(it, tree);
	if (maxTree != bheaps.end() and
			this->comp->compare((*it)->root.key, (*maxTree)->root.key) > 0)
		maxTree = it;
	if (maxTree == bheaps.end())
		maxTree = it;
	return (it);
}

template <class T1, class T2>
int	BinomialHeap<T1, T2>::_getMax(typename Heap<T1, T2>::Node *& node) const
{
	if (bheaps.empty())
		return (1);
	node = new typename Heap<T1, T2>::Node((*maxTree)->root);
	return (0);
}

template <class T1, class T2>
int	BinomialHeap<T1, T2>::_merge(const Heap<T1, T2> &heap)
{
	auto &_heap = dynamic_cast<const BinomialHeap<T1, T2> &>(heap);
	std::optional<typename std::list<BinomialTree *>::iterator> it;
	it = bheaps.begin();
	for (BinomialHeap<T1, T2>::BinomialTree * tree : _heap.bheaps)
	{
		auto treeCopy = new BinomialHeap<T1, T2>::BinomialTree(*tree);
		it = _insertFrom(*it, treeCopy);
		if (it == std::nullopt)
			return (1);
	}
	return (0);
}

template <class T1, class T2>
int	BinomialHeap<T1, T2>::_removeMax()
{
	if (this->bheaps.empty())
		return (1);
	auto heap = (*maxTree);
	bheaps.erase(maxTree);

	maxTree = bheaps.end();
	std::optional<typename std::list<BinomialTree *>::iterator> it;
	it = bheaps.begin();
	for (auto tree : heap->trees)
	{
		it = _insertFrom(*it, tree);
		if (it == std::nullopt)
			return (1);
	}
	delete heap;
	maxTree = bheaps.begin();
	for (auto it = ++bheaps.begin(); it != bheaps.end(); ++it)
	{
		if (this->comp->compare((*it)->root.key, (*maxTree)->root.key) > 0)
			maxTree = it;
	}
	return (0);
}


#endif // BINOM_HEAP_H
