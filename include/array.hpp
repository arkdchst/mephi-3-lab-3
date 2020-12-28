#pragma once

#include <stdexcept>
#include <string>
#include <cstddef>

template <typename T> class DynamicArray{
private:
	T *data = nullptr;
	std::size_t size = 0;
public:
	static const std::string INDEX_OUT_OF_RANGE;

	DynamicArray();
	DynamicArray(std::size_t size);
	DynamicArray(const T *data, std::size_t size);
	DynamicArray(const DynamicArray<T> &array, std::size_t size);
	DynamicArray(const DynamicArray<T> &array);
	virtual ~DynamicArray();
	DynamicArray<T>& operator=(const DynamicArray &array);
	
	T& get(std::size_t index);
	const T& get(std::size_t index) const;
	std::size_t getSize() const;
	void set(const T &value, std::size_t index);
	void resize(std::size_t size);
	bool operator==(const DynamicArray<T> &arr) const;
};
template <typename T>
const std::string DynamicArray<T>::INDEX_OUT_OF_RANGE = "index out of range";


#include "array.tpp"