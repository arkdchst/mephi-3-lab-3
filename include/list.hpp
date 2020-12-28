#pragma once

#include <stdexcept>
#include <string>
#include <cstddef>
#include <memory>


template <typename T>
class List {
private:
	struct Record{
		T *item = nullptr;
		Record *next = nullptr;

		Record(const T &item, Record *next = nullptr)
			: next(next), item(new T(item)){}

		Record(Record *next = nullptr)
			: next(next) {}

		~Record(){
			if(item){
				delete item;
				item = nullptr;
			}
		}
	};
	Record head;
	std::size_t size = 0;
public:
	static const std::string INDEX_OUT_OF_RANGE;
	static const std::string ZERO_SIZE;

	class ConstIterator;
	class Iterator;

	List();
	List(const T *items, std::size_t size);
	List(std::size_t size);
	List(const List<T> &list);
	virtual ~List();

	List<T>& operator=(const List<T> &other);

	T& getFirst();
	const T& getFirst() const;
	T& getLast();
	const T& getLast() const;
	T& get(std::size_t index);
	const T& get(std::size_t index) const;

	void set(const T &item, std::size_t index);
	std::unique_ptr<List<T>> getSublist(std::size_t start, std::size_t end) const;
	std::size_t getSize() const;
	void append(const T &item);
	void prepend(const T &item);
	void removeAt(std::size_t index);
	void insertAt(const T &item, std::size_t index);
	std::unique_ptr<List<T>> concat(const List<T> &list) const;

	bool operator==(const List<T> &other) const;

	List<T>::Iterator begin();
	List<T>::ConstIterator begin() const;
};
template <typename T>
const std::string List<T>::INDEX_OUT_OF_RANGE = "index out of range";
template <typename T>
const std::string List<T>::ZERO_SIZE = "size is 0";


template <typename T>
class List<T>::ConstIterator {
private:
	const Record *currentRec = nullptr;
	const Record *prevRec = nullptr;

	const List<T> *list;
public:
	static const std::string NO_NEXT_ELEM;
	static const std::string NO_CURRENT_ELEM;

	ConstIterator(const Record *currentRec, const List<T> *list);
	ConstIterator(const ConstIterator &it);
	virtual ~ConstIterator() = default;

	const T& next();
	virtual void go();
	const T& current();
	virtual bool hasNext();
};
template <typename T>
const std::string List<T>::ConstIterator::NO_NEXT_ELEM = "there is no next element";
template <typename T>
const std::string List<T>::ConstIterator::NO_CURRENT_ELEM = "there is no current element";

template <typename T>
class List<T>::Iterator {
private:
	Record *currentRec = nullptr;
	Record *prevRec = nullptr;

	List<T> *list;
public:
	static const std::string NO_NEXT_ELEM;
	static const std::string NO_CURRENT_ELEM;

	Iterator(Record *currentRec, List<T> *list);
	Iterator(const Iterator &it);
	virtual ~Iterator() = default;

	void go();
	T& next();
	T& current();
	void remove();
	void insert(const T &item);
	bool hasNext();
};
template <typename T>
const std::string List<T>::Iterator::NO_NEXT_ELEM = "there is no next element";
template <typename T>
const std::string List<T>::Iterator::NO_CURRENT_ELEM = "there is no current element";


#include "list.tpp"

#include "list_iterator.tpp"