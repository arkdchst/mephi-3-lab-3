#pragma once

#include <stdexcept>

template <typename T>
class Optional{
private:
	T *value = nullptr;

public:
	Optional() = default;

	Optional(const Optional<T> &optional){
		if(optional.hasValue())
			this->value = new T(*optional.value);
	}

	Optional(const T &value){
		this->value = new T(value);
	}

	virtual ~Optional(){
		this->delValue();
	}

	void setValue(const T &value){
		if(!this->hasValue())
			this->value = new T(value);
		else
			*this->value = value;
	}

	void delValue(){		
		if(this->hasValue()){
			delete this->value;
			this->value = nullptr;
		}
	}

	T& getValue(){
		if(value == nullptr) throw std::runtime_error("there is no value");
		return *this->value;
	}

	const T& getValue() const {
		if(value == nullptr) throw std::runtime_error("there is no value");
		return *this->value;
	}

	bool hasValue() const {
		return this->value != nullptr;
	}

	Optional& operator=(const T &value){
		this->setValue(value);
		return *this;
	}
};