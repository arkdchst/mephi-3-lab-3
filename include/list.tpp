#pragma once

template <typename T>
List<T>::List()
	: size(0){}

template <typename T>
List<T>::List(const T *items, std::size_t size) : List() {
	Record *ptr = &(this->head);
	for(std::size_t i = 0; i < size; ++i){
		ptr->next = new Record(items[i]);

		ptr = ptr->next;
	}

	this->size = size;
}

template <typename T>
List<T>::List(std::size_t size) : List() {
	Record *ptr = &(this->head);
	for(std::size_t i = 0; i < size; i++){
		ptr->next = new Record(T());
		ptr = ptr->next;
	}

	this->size = size;
}

template <typename T>
List<T>::List(const List<T> &list){
	const Record *other = &(list.head);
	Record *ptr = &(this->head);

	while(other->next != nullptr){
		ptr->next = new Record(*(other->next->item));

		other = other->next;
		ptr = ptr->next;
	}

	this->size = list.size;
}

template <typename T>
List<T>::~List(){
	Record *ptr = this->head.next;
	Record *next;
	while(ptr != nullptr){
		next = ptr->next;
		delete ptr;
		ptr = next;
	}

	this->size = 0;
}

template <typename T>
List<T>& List<T>::operator=(const List<T> &other){
	if(this == &other) return *this;

	List<T> temp(other);

	std::swap(this->head, temp.head);
	std::swap(this->size, temp.size);

	return *this;
}

template <typename T>
T& List<T>::getFirst(){
	return const_cast<T&>(const_cast<const List<T>*>(this)->getFirst());
}

template <typename T>
const T& List<T>::getFirst() const {
	if(this->size == 0) throw std::length_error(ZERO_SIZE);

	return *(this->head.next->item);
}

template <typename T>
T& List<T>::getLast(){
	return const_cast<T&>(const_cast<const List<T>*>(this)->getLast());
}

template <typename T>
const T& List<T>::getLast() const{
	if(this->size == 0) throw std::length_error(ZERO_SIZE);

	Record *ptr = &(this->head);
	while(ptr->next != nullptr) ptr = ptr->next;
	return *(ptr->item);
}

template <typename T>
T& List<T>::get(std::size_t index){
	return const_cast<T&>(const_cast<const List<T>*>(this)->get(index));
}

template <typename T>
const T& List<T>::get(std::size_t index) const {
	if(index >= this->size) throw std::out_of_range(INDEX_OUT_OF_RANGE);


	const Record *ptr = &(this->head);
	for(std::size_t i = 0; i < index + 1; i++){
		ptr = ptr->next;
	}

	return *(ptr->item);
}


template <typename T>
void List<T>::set(const T &item, std::size_t index){
	if(index >= this->size) throw std::out_of_range(INDEX_OUT_OF_RANGE);


	Record *ptr = &(this->head);
	for(std::size_t i = 0; i < index + 1; i++){
		ptr = ptr->next;
	}

	ptr->item = item;
}

template <typename T>
std::unique_ptr<List<T>> List<T>::getSublist(std::size_t start, std::size_t end) const { //end excluding
	if(!(0 <= start && start <= end && end <= this->size)){
		if(start >= this->size) throw std::out_of_range(INDEX_OUT_OF_RANGE);
		if(end > this->size) throw std::out_of_range(INDEX_OUT_OF_RANGE);
		if(start > end) throw std::logic_error("end must be not less than start");
	}

	List<T> *newList = new List<T>();


	Record *ptr = &(this->head);
	Record *newPtr = &(newList->head);

	for(std::size_t i = 0; i < start; i++) ptr = ptr->next;

	for(std::size_t i = 0; i < end - start; i++){
		newPtr->next = new Record(ptr->next->item);

		ptr = ptr->next;
		newPtr = newPtr->next;
	}

	newList->size = end - start;

	return std::unique_ptr<List<T>>(newList);
}

template <typename T>
std::size_t List<T>::getSize() const { return this->size; }

template <typename T>
void List<T>::append(const T &item){
	Record *ptr = &(this->head);
	while(ptr->next != nullptr) ptr = ptr->next;

	ptr->next = new Record(item);



	this->size++;
}

template <typename T>
void List<T>::prepend(const T &item){
	Record *newRec = new Record(item, this->head.next);
	this->head.next = newRec;

	this->size++;		
}

template <typename T>
void List<T>::removeAt(std::size_t index){
	if(index >= this->size) throw std::out_of_range(INDEX_OUT_OF_RANGE);

	Record *ptr = &(this->head);
	for(std::size_t i = 0; i < index; i++) ptr = ptr->next;

	Record *rem = ptr->next;
	ptr->next = rem->next;

	delete rem;

	this->size--;
}

template <typename T>
void List<T>::insertAt(const T &item, std::size_t index){
	if(index > this->size) throw std::out_of_range(INDEX_OUT_OF_RANGE);


	Record *ptr = &(this->head);
	for(std::size_t i = 0; i < index; i++) ptr = ptr->next;

	Record *newRec = new Record(item = item, ptr->next);


	ptr->next = newRec;

	this->size++;
}

template <typename T>
std::unique_ptr<List<T>> List<T>::concat(const List<T> &list) const {
	Record *ptr1 = &(this->head);
	const Record *ptr2 = &(list.head);

	List<T> *newList = new List<T>();
	Record *ptr = &(newList->head);
	while(ptr1->next != nullptr){
		ptr->next = new Record(ptr1->next->item);

		ptr1 = ptr1->next;
		ptr = ptr->next;
	}
	while(ptr2->next != nullptr){
		ptr->next = new Record(ptr2->next->item);

		ptr2 = ptr2->next;
		ptr = ptr->next;
	}

	newList->size = this->size + list.size;
	return std::unique_ptr<List<T>>(newList);
}

template <typename T>
bool List<T>::operator==(const List<T> &other) const{
	if(this->getSize() != other.getSize()) return false;

	for(int i = 0; i < this->getSize(); i++){
		if(this->get(i) != other.get(i)) return false;
	}
	return true;
}

template <typename T>
typename List<T>::Iterator List<T>::begin(){
	return List<T>::Iterator(&(this->head), this);
}

template <typename T>
typename List<T>::ConstIterator List<T>::begin() const{
	return List<T>::ConstIterator(&(this->head), this);
}

