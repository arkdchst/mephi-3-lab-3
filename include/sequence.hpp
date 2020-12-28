#pragma once

#include <stdexcept>
#include <string>
#include <memory>

#include "array.hpp"
#include "list.hpp"

template <typename T> class Sequence{
public:
	virtual ~Sequence() = default;

	static const std::string INDEX_OUT_OF_RANGE_MESSAGE;

	virtual T getFirst() const = 0;
	virtual T getLast() const = 0;
	virtual T get(int index) const = 0;
	virtual int getSize() const = 0;

	virtual std::unique_ptr<Sequence<T>> clone() const;

	virtual std::unique_ptr<Sequence<T>> getSubsequence(int start, int end) const = 0; //end excluding
	virtual void set(const T &item, int index) = 0;
	virtual void append(const T &item) = 0;
	virtual void prepend(const T &item) = 0;
	virtual void insertAt(const T &item, int index) = 0;
	virtual std::unique_ptr<Sequence<T>> concat(const Sequence<T>& seq) const = 0;

	virtual bool operator==(const Sequence<T> &seq) const;
};
template <typename T>
const std::string Sequence<T>::INDEX_OUT_OF_RANGE_MESSAGE = "index is out of range";

template <typename T> class ArraySequence : public Sequence<T>{
protected:
	DynamicArray<T> *array;

	ArraySequence(DynamicArray<T> *array);
public:
	ArraySequence();
	ArraySequence(const ArraySequence<T> &seq);
	ArraySequence(const T *items, int size);
	ArraySequence(int size);
	virtual ~ArraySequence();
	virtual T getFirst() const override;
	virtual T getLast() const override;
	virtual T get(int index) const override;
	virtual int getSize() const override;
	virtual void set(const T &item, int index) override;
	virtual std::unique_ptr<Sequence<T>> getSubsequence(int start, int end) const override;
	virtual void append(const T &item) override;
	virtual void prepend(const T &item) override;
	virtual void insertAt(const T &item, int index) override;
	virtual std::unique_ptr<Sequence<T>> concat(const Sequence<T>& seq) const override;
};

template <typename T> class ListSequence : public Sequence<T>{
protected:
	List<T> *list;

	ListSequence(List<T> *list);
public:
	ListSequence();
	ListSequence(const ListSequence<T> &seq);
	ListSequence(const T *items, int size);
	ListSequence(int size);
	virtual ~ListSequence();
	virtual T getFirst() const override;
	virtual T getLast() const override;
	virtual T get(int index) const override;
	virtual int getSize() const override;
	virtual std::unique_ptr<Sequence<T>> getSubsequence(int start, int end) const override;
	virtual void set(const T &item, int index) override;
	virtual void append(const T &item) override;
	virtual void prepend(const T &item) override;
	virtual void insertAt(const T &item, int index) override;
	virtual std::unique_ptr<Sequence<T>> concat(const Sequence<T>& seq) const override;
};

#include "sequence.tpp"