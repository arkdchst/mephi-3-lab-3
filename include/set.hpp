#pragma once

#include <cstddef>
#include <string>
#include <stdexcept>

#include "list.hpp"

template <typename T>
class Set{
private:
	List<T> data;

public:
	class Iterator;
	class ConstIterator;

	static const std::string ELEMENT_NOT_FOUND;
	static const std::string ELEMENT_EXISTS;

	Set() = default;

	T& add(const T &item); //returns reference to added value
	void remove(const T &item);
	bool contains(const T &item) const;

	std::size_t getSize() const;

	Iterator iterator();
	ConstIterator iterator() const;
};
template <typename T>
const std::string Set<T>::ELEMENT_NOT_FOUND = "element not found";
template <typename T>
const std::string Set<T>::ELEMENT_EXISTS = "element exists";


template <typename T>
T& Set<T>::add(const T &item){
	if(this->contains(item))
		throw std::runtime_error(this->ELEMENT_EXISTS);

	this->data.prepend(item);
	return this->data.getFirst();
}

template <typename T>
void Set<T>::remove(const T &item){
	auto it = this->data.begin();
	while(it.hasNext()){
		it.go();
		if(it.current() == item){
			it.remove();
			return;
		}
	}

	throw std::runtime_error(this->ELEMENT_NOT_FOUND);
}

template <typename T>
bool Set<T>::contains(const T &item) const {
	auto it = this->data.begin();
	while(it.hasNext()){
		it.go();
		if(it.current() == item)
			return true;
	}

	return false;
}



template <typename T>
std::size_t Set<T>::getSize() const {
	return this->data.getSize();
}


template <typename T>
typename Set<T>::ConstIterator Set<T>::iterator() const {
	return ConstIterator(this);
}

template <typename T>
typename Set<T>::Iterator Set<T>::iterator(){
	return Iterator(this);
}


template <typename T>
class Set<T>::ConstIterator {
private:
	typename List<T>::ConstIterator it;
public:
	ConstIterator(const Set<T>*);

	virtual ~ConstIterator() = default;

	const T& next();
	void go();
	const T& current();
	bool hasNext();
};

template <typename T>
class Set<T>::Iterator {
private:
	typename List<T>::Iterator it;
public:
	Iterator(Set<T>*);

	virtual ~Iterator() = default;

	T& next();
	void go();
	T& current();
	bool hasNext();
};





template <typename T>
Set<T>::ConstIterator::ConstIterator(const Set<T> *set)
	: it(set->data.begin()){}

template <typename T>
const T& Set<T>::ConstIterator::next(){
	return this->it.next();
}

template <typename T>
void Set<T>::ConstIterator::go(){
	this->it.go();
}

template <typename T>
const T& Set<T>::ConstIterator::current(){
	return this->it.current();
}

template <typename T>
bool Set<T>::ConstIterator::hasNext(){
	return this->it.hasNext();
}


template <typename T>
Set<T>::Iterator::Iterator(Set<T> *set)
	: it(set->data.begin()){}


template <typename T>
T& Set<T>::Iterator::next(){
	return this->it.next();
}

template <typename T>
void Set<T>::Iterator::go(){
	this->it.go();
}

template <typename T>
T& Set<T>::Iterator::current(){
	return this->it.current();
}

template <typename T>
bool Set<T>::Iterator::hasNext(){
	return this->it.hasNext();
}

