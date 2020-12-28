#include <cstddef>

#include "array.hpp"

template <typename T>
class IMatrix{
public:
	virtual T& get(std::size_t x, std::size_t y) = 0;
	virtual const T& get(std::size_t x, std::size_t y) const = 0;

	virtual void set(const T&, std::size_t x, std::size_t y) = 0;

	virtual std::size_t getWidth() const = 0;
	virtual std::size_t getHeight() const = 0;
};

template <typename T>
class Matrix : IMatrix<T> {
private:
	DynamicArray<DynamicArray<T>*> *data;

public:
	Matrix(T const * const * data, std::size_t widht, std::size_t height);
	Matrix(std::size_t widht, std::size_t height);
	Matrix(const Matrix<T>&);
	virtual ~Matrix();

	virtual T& get(std::size_t x, std::size_t y);
	virtual const T& get(std::size_t x, std::size_t y) const;

	virtual void set(const T&, std::size_t x, std::size_t y);

	virtual std::size_t getWidth() const;
	virtual std::size_t getHeight() const;
};

template <typename T>
Matrix<T>::Matrix(T const * const * data, std::size_t widht, std::size_t height){
	this->data = new DynamicArray<DynamicArray<T>*>(widht);
	for(std::size_t i = 0; i < widht; i++){
		this->data->get(i) = new DynamicArray<T>(height);

		for(std::size_t j = 0; j < height; j++)
			this->data->get(i)->set(data[i][j], j);

	}
}

template <typename T>
Matrix<T>::Matrix(std::size_t widht, std::size_t height){
	this->data = new DynamicArray<DynamicArray<T>*>(widht);

	for(std::size_t i = 0; i < widht; i++){
		this->data->get(i) = new DynamicArray<T>(height);
	}
}

template <typename T>
Matrix<T>::Matrix(const Matrix<T> &other){
	this->data = new DynamicArray<DynamicArray<T>*>(*other.data);

	for(std::size_t i = 0; i < other.getWidth(); i++){
		this->data->get(i) = new DynamicArray<T>(*other.data->get(i));
	}
}

template <typename T>
Matrix<T>::~Matrix(){
	for(std::size_t i = 0; i < this->getWidth(); i++){
		delete this->data->get(i);
	}

	delete this->data;
}

template <typename T>
T& Matrix<T>:: get(std::size_t x, std::size_t y){
	return const_cast<T&>(const_cast<const Matrix<T>*>(this)->get(x, y));
}

template <typename T>
const T& Matrix<T>:: get(std::size_t x, std::size_t y) const {
	return this->data->get(x)->get(y);
}

template <typename T>
void Matrix<T>:: set(const T& item, std::size_t x, std::size_t y){
	this->data->get(x)->set(item, y);
}


template <typename T>
std::size_t Matrix<T>::getWidth() const{
	return this->data->getSize();
}

template <typename T>
std::size_t Matrix<T>::getHeight() const {
	if(this->data->getSize() == 0) return 0;

	return this->data->get(0)->getSize();
}
