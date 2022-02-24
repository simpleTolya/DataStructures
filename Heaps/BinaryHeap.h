#ifndef BIN_HEAP_H
#define BIN_HEAP_H

#include <vector>
#include "Heap.h"

template <class T1, class T2>
class BinaryHeap;

template <class T1, class T2>
class BinaryHeapCreator : public HeapCreator<T1, T2>
{
public:
	virtual Heap<T1, T2> * createHeap(const Comparator<T1> *comp)
	{
		return new BinaryHeap<T1, T2>(comp);
	}
};

template <class T1, class T2>
class BinaryHeap : public Heap<T1, T2>
{
	friend BinaryHeapCreator<T1, T2>;
private:
	std::vector<typename Heap<T1, T2>::Node> nodes;
	int	siftUp(unsigned int index);
	int	siftDown(unsigned int index);

	virtual	int _insert(const std::pair<T1, T2>	&key_value) override;
	virtual int _merge(const Heap<T1, T2> &heap) override;
	virtual int _getMax(typename Heap<T1, T2>::Node *& node) const override;
	virtual int _removeMax() override;

	BinaryHeap(const Comparator<T1> * comp)
	{
		this->comp = comp;
	}
public:
	BinaryHeap(const BinaryHeap &other) = default;
	BinaryHeap & operator=(const BinaryHeap &other) = default;
	unsigned int size()
	{
		return nodes.size();
	}
};

template <class T1, class T2>
int	BinaryHeap<T1, T2>::siftUp(unsigned int index)
{
	// parent(i) -> (i - 1)/2
	// left(i)  -> 2i + 1
	// right(i) -> 2i + 2
	unsigned int parent = (index - 1)/2;
	while (index)
	{
		int	compare_value = this->comp->compare(nodes[parent].key,
												nodes[index].key);
		if (compare_value > 0)
			return 0;
		else if (compare_value < 0)
			std::swap(nodes[parent], nodes[index]);
		else
			return (1);
		index = parent;
		parent = (index - 1)/2;
	}
	return (0);
}

template <class T1, class T2>
int	BinaryHeap<T1, T2>::siftDown(unsigned int index)
{
	// parent(i) -> (i - 1)/2
	// left(i)  -> 2i + 1
	// right(i) -> 2i + 2
	while (true)
	{
		if (2*index + 1 < nodes.size())
		{
			int	compare_value = this->comp->compare(nodes[index].key,
													nodes[2*index + 1].key);
			if (compare_value > 0)
			{
				// current > left son
				if (2*index + 2 < nodes.size())
				{
					int compare_value2 = this->comp->compare(nodes[index].key,
														nodes[2*index + 2].key);
					if (compare_value2 > 0)
					{
						// current > right son
						return (0);
					}
					else if (compare_value2 < 0)
					{
						// right son > current > left son
						std::swap(nodes[index], nodes[2*index + 2]);
						index = 2*index + 2;
					}
					else
						return (1);
				}
				else
					return (0);
			}
			else if (compare_value < 0)
			{
				// left son > current
				if (2*index + 2 < nodes.size())
				{
					int compare_value2 = this->comp->compare(nodes[index].key,
														nodes[2*index + 2].key);
					if (compare_value2 > 0)
					{
						// left son > current > right son
						std::swap(nodes[index], nodes[2*index + 1]);
						index = 2*index + 1;
					}
					else if (compare_value2 < 0)
					{
						// left > current
						// right > current
						int compare_value3 = this->comp->compare(nodes[2*index + 1].key,
															nodes[2*index + 2].key);
						if (compare_value3 > 0)
						{
							// left > right > current
							std::swap(nodes[index], nodes[2*index + 1]);
							index = 2*index + 1;
						}
						else if (compare_value3 < 0)
						{
							// right > left > current
							std::swap(nodes[index], nodes[2*index + 2]);
							index = 2*index + 2;
						}
						else
							return (1);
					}
					else
						return (1);
				}
				else
				{
					std::swap(nodes[index], nodes[2*index + 1]);
					index = 2*index + 1;
				}
			}
			else
				return (1);
		}
		else
			return (0);
	}
}

template <class T1, class T2>
int BinaryHeap<T1, T2>::_insert(const std::pair<T1, T2>	&key_value)
{
	nodes.push_back(typename Heap<T1, T2>::Node(
						key_value.first,
						key_value.second)
					);
	return siftUp(nodes.size() - 1);
}

template <class T1, class T2>
int BinaryHeap<T1, T2>::_removeMax()
{
	if (nodes.empty())
		return (1);
	std::swap(nodes[0], nodes[nodes.size() - 1]);
	nodes.pop_back();
	return siftDown(0);
}

template <class T1, class T2>
int BinaryHeap<T1, T2>::_getMax(typename Heap<T1, T2>::Node *& node) const
{
	if (nodes.empty())
		return (1);
	node = new typename Heap<T1, T2>::Node(nodes[0]);
	return (0);
}

template <class T1, class T2>
int BinaryHeap<T1, T2>::_merge(const Heap<T1, T2> &heap)
{
	auto &_heap = dynamic_cast<const BinaryHeap<T1, T2> &>(heap);
	nodes.insert(nodes.end(), _heap.nodes.begin(), _heap.nodes.end());
	for (unsigned int index = nodes.size() / 2; index > 0; --index)
	{
		if (siftDown(index))
			return (1);
	}
	return siftDown(0);
}

#endif // BIN_HEAP_H
