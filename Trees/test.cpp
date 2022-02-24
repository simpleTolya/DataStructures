//
// Created by Mixtape Armand on 10/21/21.
//
#include "Relation.h"

class ComparatorInt : public Comparator <int> {
public:
	int compare(const int & left, const int & right) const {
		if (left > right) return 1;
		if (left == right) return 0;
		return -1;
	}
	~ComparatorInt() {

	}
};


#include <algorithm>
#include <vector>
#include <map>
#include <chrono>
#include <iostream>

int max_depth = 0;
void printNode(int key, int value, int depth) {
	static int a = 0;
	std::cout << a++ << " ";
	std::cout << "key " << key << " value: " << value << " depth: " << depth << '\n';
}

void updateMaxDepth(int key, int value, int depth)
{
	(void) key;
	(void) value;
	if (depth > max_depth)
		max_depth = depth;
}

#define TREE_TYPE2 B23Tree
#define SIZE 100000
int main(void)
{
	std::srand(std::time(0));
//	auto * comp = new ComparatorInt;
//	Map<int, int> * tree = new TREE_TYPE2 <int, int>(comp);

//	for (int i=0; i<20; i++)
//		(*tree) += std::pair<int, int>(i, 5);
//	for (auto elem : *static_cast<TREE_TYPE2<int, int> *>(tree))
//	{
//		std::cout << elem.first << " " << elem.second << "\n";
//	}

/*
	std::vector <int> array;

	for (int i=0; i<SIZE; i++)
		array.push_back(i);
	std::random_shuffle(array.begin(), array.end());

	std::cout << "My tree\n";
	auto start_time = std::chrono::steady_clock::now();
		// BEGIN
		for (int i=0; i<SIZE; i++)
			*tree += std::pair<int, int>(array[i], i);
		for (int i=0; i<SIZE; i++)
			*tree -= i;
		// END
	auto end_time = std::chrono::steady_clock::now();
	auto elapsed_ns = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cout << elapsed_ns.count() << " ms\n";

	std::cout << "std::map\n";
	std::map<int, int> STLMap;
	start_time = std::chrono::steady_clock::now();
		// BEGIN
		for (int i=0; i<SIZE; i++)
			STLMap.insert(std::pair<int, int>(array[i], i));
		for (int i=0; i<SIZE; i++)
			STLMap.erase(i);
		// END
		end_time = std::chrono::steady_clock::now();
		elapsed_ns = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
	std::cout << elapsed_ns.count() << " ms\n";

	delete tree;
	delete comp;
*/
	// RELATION PART

	auto comp = new ComparatorInt;
	//auto tree = new AVLTree <int, int>(comp);
	Relation<int, int> relation;
	
	relation.insertIndex(Relation<int, int>::IndexBuilder().newIdentifier("salary").newIdentifier("name").getResult()
	    , Relation<int, int>::TREE_TYPE::SPLAYTREE, comp).insertData(std::pair<int, int>(1, 42));
	std::cout << relation.findData(1, "salary,name");
	//delete tree;
    delete comp;

}














