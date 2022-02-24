#include <iostream>

#include "BinaryHeap.h"
#include "BinomialHeap.h"
#include "FibonacciHeap.h"

class ComparatorInt : public Comparator<int>
{
	int compare(const int& left, const int& right) const
	{
		return left - right;
	}
};

int main()
{
	Comparator<int> *comp = new ComparatorInt();
	HeapCreator<int, int> *creator = new BinaryHeapCreator<int, int>;
	Heap<int, int> *heap = creator->getHeap(comp);
	for (int i=0; i < 255; i++)
	{
		heap->insert(std::pair<int, int>(i, i));
		std::cout << heap->getMax() << "\n";
	}
	Heap<int, int> *heap2 = creator->getHeap(comp);
	for (int i=255; i < 256; i++)
	{
		heap2->insert(std::pair<int, int>(i, i));
	}
	heap->merge(*heap2);

	for (int i=0; i < 255; i++)
	{
		std::cout << heap->getMax() << "\n";
		heap->removeMax();
	}
	delete comp;
	delete heap;
	delete heap2;
	return (0);
}
