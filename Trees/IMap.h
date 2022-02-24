//
// Created by Mixtape Armand on 10/21/21.
//

#ifndef TREES_MAP_H
#define TREES_MAP_H

#include <memory>
#include <exception>
#include <string>
#include <iterator>

template <class T>
class Comparator {
public:
	virtual int compare(const T& left, const T& right) const = 0;
	virtual ~Comparator() = default;
};

template <class T1, class T2>
class Map {
protected:
	struct Node {
		T1 *key;
		T2 *value;
		Node(const T1 & key, const T2 &value)
		{
			this->key = new T1(key);
			this->value = new T2(value);
		}
		Node() = default;
		const T1 & getKey() const
		{
			return (*key);
		}
		const T2 & getValue() const
		{
			return (*value);
		}
		virtual ~Node() {
			delete key;
			delete value;
		}
	};
	const Comparator<T1>	*comp;
	mutable Node 	*root;

	virtual Node* find(const T1 &key) = 0;
	virtual int insert(const T1& key, const T2& value) = 0;
	virtual int remove(const T1& key) = 0;

public:
	class Exception : public std::exception {
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

	virtual ~Map() = default;
	void operator+=(const std::pair<T1, T2>& key_value) {
		if (insert(key_value.first, key_value.second))
			throw Exception("INSERTION ERROR: Such an element already exists");
	}
	void operator-=(const T1& key) {
		if (remove(key))
			throw Exception("REMOVE ERROR: Element doesn't exists\n");
	}
	T2& operator[](const T1& key) {
		Node *node = find(key);
		if (!node)
			throw Exception("FIND ERROR: Element doesn't exists\n");
		return *(node->value);
	}
	const T2& operator[](const T1& key) const {
		Node *node = find(key);
		if (!node)
			throw Exception("FIND ERROR: Element doesn't exists\n");
		return *(node->value);
	}
};

#endif //TREES_MAP_H
