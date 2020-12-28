#pragma once

#include <stdexcept>

template <typename T>
std::unique_ptr<Sequence<T>> Sequence<T>::clone() const {
	return this->getSubsequence(0, this->getSize());
}

template <typename T>
bool Sequence<T>::operator==(const Sequence<T> &seq) const {
	if(this->getSize() != seq.getSize()) return false;
	for(int i = 0; i < this->getSize(); i++){
		if(this->get(i) != seq.get(i)) return false;
	}
	return true;
}



template <typename T>
ArraySequence<T>::ArraySequence(DynamicArray<T> *array){
	this->array = array;
}

template <typename T>
ArraySequence<T>::ArraySequence(){
	this->array = new DynamicArray<T>();
}

template <typename T>
ArraySequence<T>::ArraySequence(const ArraySequence<T> &seq){
	this->array = new DynamicArray<T>(*seq.array);
}



template <typename T>
ArraySequence<T>::ArraySequence(const T *items, int size){
	this->array = new DynamicArray<T>(items, size);
}

template <typename T>
ArraySequence<T>::ArraySequence(int size){
	this->array = new DynamicArray<T>(size);
}

template <typename T>
ArraySequence<T>::~ArraySequence(){
	delete this->array;
}

template <typename T>
T ArraySequence<T>::getFirst() const {
	return this->array->get(0);
}

template <typename T>
T ArraySequence<T>::getLast() const {
	return this->array->get(this->getSize() - 1);
}

template <typename T>
T ArraySequence<T>::get(int index) const {
	return this->array->get(index);
}

template <typename T>
int ArraySequence<T>::getSize() const {
	return this->array->getSize();
}

template <typename T>
void ArraySequence<T>::set(const T &item, int index) {
	if(index < 0 || index >= this->getSize()) throw std::out_of_range(this->INDEX_OUT_OF_RANGE_MESSAGE);

	this->array->set(item, index);
}


template <typename T>
std::unique_ptr<Sequence<T>> ArraySequence<T>::getSubsequence(int start, int end) const {
	if(!(0 <= start && start <= end && end <= this->getSize())){
		if(start < 0 || start >= this->getSize()) throw std::out_of_range(this->INDEX_OUT_OF_RANGE_MESSAGE);
		if(end < 0 || end > this->getSize()) throw std::out_of_range(this->INDEX_OUT_OF_RANGE_MESSAGE);
		if(start > end) throw std::logic_error("end must be not less than start");
	}

	T subArray[end - start];
	for(int i = 0; i < end - start; i++)
		subArray[i] = this->array->get(i + start);
	
	ArraySequence<T> *subSequence = new ArraySequence<T>(subArray, end - start);

	return std::unique_ptr<ArraySequence<T>>(subSequence);
}

template <typename T>
void ArraySequence<T>::append(const T &item) {
	this->array->resize(this->getSize() + 1);
	this->array->set(item, this->getSize() - 1);
}

template <typename T>
void ArraySequence<T>::prepend(const T &item) {
	this->array->resize(this->getSize() + 1);
	T t1 = this->array->get(0);
	T t2;
	for(int i = 0; i < this->getSize() - 1; i++){
		t2 = t1;
		t1 = this->array->get(i + 1);
		this->array->set(t2, i + 1);
	}
	this->array->set(item, 0);
}

template <typename T>
void ArraySequence<T>::insertAt(const T &item, int index) {
	if(index < 0 || index > this->getSize()) throw std::out_of_range(this->INDEX_OUT_OF_RANGE_MESSAGE);

	this->array->resize(this->getSize() + 1);
	T t1 = this->array->get(index);
	T t2;
	for(int i = index; i < this->getSize() - 1; i++){
		t2 = t1;
		t1 = this->array->get(i + 1);
		this->array->set(t2, i + 1);
	}
	this->array->set(item, index);
}

template <typename T>
std::unique_ptr<Sequence<T>> ArraySequence<T>::concat(const Sequence<T>& seq) const {
	DynamicArray<T> *array = new DynamicArray<T>(this->getSize() + seq.getSize());
	ArraySequence<T> *newSequence = new ArraySequence<T>(array);
	for(int i = 0; i < this->getSize(); i++)
		newSequence->set(this->get(i), i);

	for(int i = 0; i < seq.getSize(); i++)
		newSequence->set(seq.get(i), i + this->getSize());

	return std::unique_ptr<ArraySequence<T>>(newSequence);
}




template <typename T>
ListSequence<T>::ListSequence(List<T> *list){
	this->list = list;
}

template <typename T>
ListSequence<T>::ListSequence(){
	this->list = new List<T>();
}

template <typename T>
ListSequence<T>::ListSequence(const ListSequence<T> &seq){
	this->list = new List<T>(*seq.list);
}


template <typename T>
ListSequence<T>::ListSequence(const T *items, int size){
	this->list = new List<T>(items, size);
}

template <typename T>
ListSequence<T>::ListSequence(int size){
	this->list = new List<T>(size);
}

template <typename T>
ListSequence<T>::~ListSequence(){
	delete this->list;
}


template <typename T>
T ListSequence<T>::getFirst() const {
	return this->list->getFirst();
}

template <typename T>
T ListSequence<T>::getLast() const {
	return this->list->getLast();
}

template <typename T>
T ListSequence<T>::get(int index) const {
	return this->list->get(index);
}

template <typename T>
int ListSequence<T>::getSize() const {
	return this->list->getSize();
}

template <typename T>
std::unique_ptr<Sequence<T>> ListSequence<T>::getSubsequence(int start, int end) const {
	List<T> *subList = this->list->getSublist(start, end);
	ListSequence<T> *seq = new ListSequence<T>(subList);
	return std::unique_ptr<ArraySequence<T>>(seq);
}

template <typename T>
void ListSequence<T>::set(const T &item, int index) {
	this->list->set(item, index);
}

template <typename T>
void ListSequence<T>::append(const T &item) {
	this->list->append(item);
}

template <typename T>
void ListSequence<T>::prepend(const T &item) {
	this->list->prepend(item);
}

template <typename T>
void ListSequence<T>::insertAt(const T &item, int index) {
	this->list->insertAt(item, index);
}

template <typename T>
std::unique_ptr<Sequence<T>> ListSequence<T>::concat(const Sequence<T>& seq) const {
	ListSequence<T> *newSequence = new ListSequence<T>();

	for(int i = 0; i < this->getSize(); i++)
		newSequence->append(this->get(i));

	for(int i = 0; i < seq.getSize(); i++)
		newSequence->append(seq.get(i));

	return std::unique_ptr<ListSequence<T>>(newSequence);
}
