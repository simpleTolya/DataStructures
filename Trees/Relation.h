#ifndef RELATION_H
#define RELATION_H

#include <algorithm>
#include <map>
#include <vector>
#include "B23tree.h"
#include "BSTree.h"
#include "RedBlackTree.h"
#include "AVLTree.h"
#include "SplayTree.h"

template <class TKey, class TValue>
class Relation
{
private:
	std::map    <std::string, Map<TKey, TValue> *> map;
	static std::string    doCorrectIndex(const std::string & index);
public:
	static bool	isCorrectIdentifier(const std::string & index);
    enum TREE_TYPE {SPLAYTREE, AVLTREE, RBTREE, BSTREE, B23TREE};
	class Exception final : public std::exception
	{
	private:
		std::string		error_text;
	public:
		Exception(const std::string & _error_text) {
			error_text = _error_text;
		}
		Exception(std::string && _error_text) {
			error_text = _error_text;
		}
		const char * what() const noexcept override {
			return error_text.c_str();
		}
	};
	struct IndexBuilder
	{
	private:
		std::vector<std::string> identifiers;
	public:
		IndexBuilder() = default;
		IndexBuilder &	newIdentifier(std::string identifier)
		{
			if (not Relation<TKey, TValue>::isCorrectIdentifier(identifier))
				throw Exception("Index has not correct identifier\n");
			identifiers.push_back(identifier);
			return *this;
		}
		std::string		getResult()
		{
			std::string	res;
			std::sort(identifiers.begin(), identifiers.end());
			for (auto elem : identifiers)
				res += elem;
			return res;
		}
	};
	Relation<TKey, TValue> &	insertIndex(const std::string & index, TREE_TYPE type, 
											const Comparator<TKey> * comp);
	Relation<TKey, TValue> &	removeIndex(const std::string & index);
	Relation<TKey, TValue> &	insertData(const std::pair<TKey, TValue> & data);
	Relation<TKey, TValue> &	removeData(const TKey & data);
	TValue	 &	findData(const TKey & key, const std::string & index);
};

template <class TKey, class TValue>
std::string  Relation<TKey, TValue>::doCorrectIndex(const std::string & index)
{
    std::string res = index;
    
    std::vector <std::string> vec;
    auto i = res.find(",");
    while (i != std::string::npos)
    {
        vec.push_back(res.substr(0, i));
        if (not isCorrectIdentifier(vec[vec.size() - 1]))
        	throw Exception("Index has not correct identifier\n");
        res = res.substr(i + 1);
        i = res.find(",");
    }
    if (not isCorrectIdentifier(res))
    	throw Exception("Index has not correct identifier\n");
	vec.push_back(res);
	res = "";
    std::sort(vec.begin(), vec.end());
    for (auto str : vec)
        res += str;
    return (res);
}

template <class TKey, class TValue>
bool	Relation<TKey, TValue>::isCorrectIdentifier(const std::string & identifier)
{
	if (identifier.length() > 32)
		return (false);
	for (long unsigned int i=0; i < identifier.length(); i++)
	{
		if (not (std::isalnum(identifier[i])))
			return (false);
	}
	return (true);
}

template <class TKey, class TValue>
Relation<TKey, TValue> &	Relation<TKey, TValue>::insertIndex(const std::string & index, 
    typename Relation<TKey, TValue>::TREE_TYPE type, const Comparator<TKey> * comp)
{
	std::string correctIndex = doCorrectIndex(index);
	if (this->map.find(correctIndex) != this->map.end())
		throw Exception("Index already exists\n");
	Map <TKey, TValue> *tree;
	if (type == Relation<TKey, TValue>::SPLAYTREE)
	    tree = new SplayTree<TKey, TValue>(comp);
	else if (type == Relation<TKey, TValue>::TREE_TYPE::AVLTREE)
	    tree = new AVLTree<TKey, TValue>(comp);
	else if (type == Relation<TKey, TValue>::B23TREE)
	    tree = new B23Tree<TKey, TValue>(comp);
	else if (type == Relation<TKey, TValue>::RBTREE)
	    tree = new RBTree<TKey, TValue>(comp);
	else if (type == Relation<TKey, TValue>::BSTREE)
	    tree = new BSTree<TKey, TValue>(comp);
	if (not map.empty())
	{
		auto first = map.begin()->second;
		if ((type == Relation<TKey, TValue>::B23TREE))
		{
			for (auto elem : *static_cast<B23Tree<TKey, TValue> *>(first))
				*tree += elem;
		}
		else
		{
			for (auto elem : *static_cast<BSTree<TKey, TValue> *>(first))
				*tree += elem;
		}
	}
	this->map.insert(std::pair<std::string, Map<TKey, TValue> *>(correctIndex, tree));
	return *this;
}

template <class TKey, class TValue>
Relation<TKey, TValue> &	Relation<TKey, TValue>::removeIndex(const std::string & index)
{
    std::string    correctIndex = doCorrectIndex(index);
	if (not this->map.contains(correctIndex))
		throw Exception("Index not found\n");
	this->map.erase(correctIndex);
	return *this;
}

template <class TKey, class TValue>
Relation<TKey, TValue> &	Relation<TKey, TValue>::insertData(const std::pair<TKey, TValue> & data)
{
    if (this->map.empty())
        throw Exception("Insert index before inserting data\n");
	for (auto it = this->map.begin(); it != this->map.end(); it++)
	{
		*(it->second) += data;
	}
	return *this;
}

template <class TKey, class TValue>
Relation<TKey, TValue> &	Relation<TKey, TValue>::removeData(const TKey & data)
{
	for (auto it = this->map.begin(); it != this->map.end(); it++)
	{
		*(it->second) -= data;
	}
	return *this;
}

template <class TKey, class TValue>
TValue & Relation<TKey, TValue>::findData(const TKey & key, const std::string & index)
{
	return (*this->map[doCorrectIndex(index)])[key];
}



#endif // RELATION_H

