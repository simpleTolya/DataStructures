#ifndef HEAP_H
#define HEAP_H

#include <utility>
#include <exception>
#include <string>

template <class T>
class Comparator {
public:
	virtual int compare(const T& left, const T& right) const = 0;
	virtual ~Comparator() = default;
};

template <class T1, class T2>
class Heap;

template <class T1, class T2>
class HeapCreator
{
protected:
	virtual Heap<T1, T2> * createHeap(const Comparator<T1> *comp) = 0;
public:
	Heap<T1, T2> * getHeap(const Comparator<T1> *comp)
	{
		Heap <T1, T2> *heap = createHeap(comp);
		return heap;
	}
};

template <class T1, class T2>
class Heap
{
	friend HeapCreator<T1, T2>;
protected:
	struct Node
	{
		T1	key;
		T2	value;

		Node(const T1 &key, const T2 &value)
		{
			this->key = key;
			this->value = value;
		}
		Node(const Node &other) = default;
	};
	const Comparator<T1> *comp;

	virtual	int _insert(const std::pair<T1, T2>	&key_value) = 0;
	virtual int _merge(const Heap<T1, T2> &heap) = 0;
	virtual int _getMax(Node *& node) const = 0;
	virtual int _removeMax() = 0;
	Heap() = default;
public:
	virtual ~Heap() = default;
	void	insert(const std::pair<T1, T2>	&key_value);
	void	merge(const Heap<T1, T2> &heap);
	void	removeMax();
	T2		getMax() const;

	struct Exception : std::exception {
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
	struct RemovingMinException : Exception {
	public:
		RemovingMinException() : Exception("The heap is empty\n") {}
	};
	struct GettingMinException : Exception {
	public:
		GettingMinException() : Exception("The heap is empty\n") {}
	};
};

template <class T1, class T2>
void	Heap<T1, T2>::insert(const std::pair<T1, T2>	&key_value)
{
	_insert(key_value);
}

template <class T1, class T2>
void	Heap<T1, T2>::merge(const Heap<T1, T2> &heap)
{
	_merge(heap);
}

template <class T1, class T2>
void	Heap<T1, T2>::removeMax()
{
	if (_removeMax())
		throw RemovingMinException();
}

template <class T1, class T2>
T2	Heap<T1, T2>::getMax() const
{
	Node	*tmp = nullptr;
	if (_getMax(tmp))
		throw GettingMinException();
	Node	res(*tmp);
	delete tmp;
	return (res.value);
}

#endif // HEAP_H
