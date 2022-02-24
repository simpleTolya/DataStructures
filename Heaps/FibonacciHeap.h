//
// Created by Mixtape Armand on 11/8/21.
//

#ifndef INC_3LABA_FIB_HEAP_H
#define INC_3LABA_FIB_HEAP_H

#include "Heap.h"
#include "BinomialHeap.h"

template <class T1, class T2>
class FibonacciHeap;

template <class T1, class T2>
class FibonacciHeapCreator : public HeapCreator<T1, T2>
{
public:
	virtual Heap<T1, T2> * createHeap(const Comparator<T1> *comp)
	{
		return new FibonacciHeap<T1, T2>(comp);
	}
};

template <class T1, class T2>
class FibonacciHeap : public BinomialHeap<T1, T2>
{
	friend FibonacciHeapCreator<T1, T2>;
private:
	using BinomialTree = typename BinomialHeap<T1, T2>::BinomialTree;
	using Node		=	 typename BinomialHeap<T1, T2>::Node;
	using ListIterator = typename std::list<BinomialTree *>::iterator;

	virtual	int _insert(const std::pair<T1, T2>	&key_value);
	virtual int _merge(const Heap<T1, T2> &heap);
	virtual int _removeMax();

	FibonacciHeap(const Comparator<T1> * comp) :
			BinomialHeap<T1, T2>(comp)
	{}
public:
	FibonacciHeap(const FibonacciHeap & other) :
			BinomialHeap<T1, T2>(other)
	{}
	~FibonacciHeap()
	{}
	FibonacciHeap &	operator=(const FibonacciHeap & other)
	{
		BinomialHeap<T1, T2>::operator=(other);
	}
};

template <class T1, class T2>
int FibonacciHeap<T1, T2>::_insert(const std::pair<T1, T2>	&key_value)
{
	auto *tmp = new BinomialTree(
			key_value.first, key_value.second
	);
	this->bheaps.push_back(tmp);
	if (this->maxTree == this->bheaps.end() or
			this->comp->compare(key_value.first, (*this->maxTree)->root.key))
		this->maxTree = --ListIterator(this->bheaps.end());
	return (0);
}

template <class T1, class T2>
int	FibonacciHeap<T1, T2>::_merge(const Heap<T1, T2> &heap)
{
	auto _heap = dynamic_cast<const FibonacciHeap<T1, T2> &>(heap);

	std::list<BinomialTree *> treeCopies;
	ListIterator	maxTreeCopy;
	for (auto tree : _heap.bheaps)
	{
		auto *copy = new BinomialTree(*tree);
		treeCopies.push_back(copy);
		if (tree == *_heap.maxTree)
			maxTreeCopy = --ListIterator(treeCopies.end());
	}
	if (this->comp->compare((*_heap.maxTree)->root.key, (*this->maxTree)->root.key))
	{
		this->maxTree = maxTreeCopy;
	}
	this->bheaps.splice(this->bheaps.end(), treeCopies);
	return (0);
}

template <class T1, class T2>
int	FibonacciHeap<T1, T2>::_removeMax()
{
	std::vector<ListIterator>	array(5, this->bheaps.end());

	if (this->maxTree == this->bheaps.end())
		return (1);
	for (auto tree : (*this->maxTree)->trees)
	{
		this->bheaps.push_back(tree);
	}
	this->bheaps.erase(this->maxTree);
	for (auto it = this->bheaps.begin(); it != this->bheaps.end();)
	{
		if ((*it)->degree >= array.size()) {
			array.resize((*it)->degree + 1, this->bheaps.end());
		}
		if (array[(*it)->degree] == this->bheaps.end())
		{
			array[(*it)->degree] = it;
			++it;
		}
		else
		{
			auto newTree = BinomialHeap<T1, T2>::_merge(*it, *array[(*it)->degree]);
			this->bheaps.insert(++ListIterator(it), newTree);
			this->bheaps.erase(array[newTree->degree - 1]);
			array[newTree->degree - 1] = this->bheaps.end();
			++it;
			this->bheaps.erase(--ListIterator(it));
		}
	}
	this->maxTree = this->bheaps.begin();
	for (auto it = ++this->bheaps.begin(); it != this->bheaps.end(); ++it)
	{
		if (this->comp->compare((*it)->root.key, (*this->maxTree)->root.key) > 0)
			this->maxTree = it;
	}
	return (0);
}

#endif //INC_3LABA_FIB_HEAP_H
