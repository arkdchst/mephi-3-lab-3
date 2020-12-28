#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <limits>
#include <exception>


int readInt(int from=std::numeric_limits<int>::min(), int to=std::numeric_limits<int>::max()){
	while(true){
		int read;
		std::cin >> read; std::cin.ignore();//ignore remaining newline
		if(read < from || read > to || std::cin.fail()){
			std::cout << "please, enter a number from " + std::to_string(from) + " to " + std::to_string(to) + ": ";
		}
		else return read;
	}
}


class MenuItem{
public:
	virtual void enter() = 0;//enter into item's context
	virtual std::string toString() const = 0;
};

template <typename T>
class InputItem : virtual public MenuItem{
protected:
	T value;
public:

	InputItem(const T &defaultVal = T()) : value(defaultVal){}

	virtual T getValue() const {
		return this->value;
	}

	virtual void setValue(const T &val) {
		this->value = val;
	}
};

class NamedItem : virtual public MenuItem{
private:
	std::string name;
public:

	NamedItem(const std::string &name) : name(name){}

	virtual std::string toString() const {
		return this->name;
	}
};

namespace std{
	std::string to_string(const std::string &str){
		return str;
	}
};


template <typename T>
class NamedInputItem : public NamedItem, public InputItem<T> {
public:
	NamedInputItem(const std::string &name, const T &defaultVal)
		: NamedItem(name), InputItem<T>(defaultVal){}

	virtual std::string toString() const override {
		return NamedItem::toString() + " (" + std::to_string(this->getValue()) + ")";
	}
};

class ChoiceInput : public NamedInputItem<int>{
public:
	std::vector<std::string> values;

	ChoiceInput(const std::string &name, const std::vector<std::string> &values, int defaultVal = 0) :
		NamedInputItem(name, defaultVal), values(values)
	{
		if(values.empty())
			throw std::runtime_error("values is empty");
		if(defaultVal < 0 || defaultVal >= values.size())
			throw std::runtime_error("default value out of bounds");
	}

	virtual std::string toString() const override {
		if(values.empty() || this->getValue() == -1)
			return NamedItem::toString();
		else
			return NamedItem::toString() + " (" + values[this->getValue()] + ")";
	}

	virtual void enter() override {
		std::string out;
		for(auto i = 0; i < values.size(); i++){
			out += std::to_string(i + 1) + ". " + values[i] + "\n";
		}
		std::cout << out << std::endl;

		int value = readInt(1, values.size());
		this->value = value - 1;
	}
};

class IntInput : public NamedInputItem<int>{
private:
	int from, to;

	void print(){
		std::cout << "enter integer from " + std::to_string(from) + " to " + std::to_string(to) + ": ";
	}
public:

	IntInput(const std::string &name, int defaultVal = 0, int from=std::numeric_limits<int>::min(), int to=std::numeric_limits<int>::max()) :
		NamedInputItem<int>(name, defaultVal), from(from), to(to)
	{
		if(from > to)
			throw std::runtime_error("from is greater than to");
		if(defaultVal < from || defaultVal > to)
			throw std::runtime_error("default value out of bounds");
	}

	virtual void enter() override {
		print();
		value = readInt(from, to);
	}	
};

class Menu : public NamedItem {
private:
	std::vector<MenuItem*> items;
	bool isRoot;

	void print(){
		std::string out;
		for(std::size_t i = 0; i < items.size(); i++){
			out += std::to_string(i + 1) + ". " + items[i]->toString() + "\n";
		}
		out += std::to_string(items.size() + 1) + ". " + (isRoot ? "exit" : "back") + "\n";
		std::cout << out << std::endl;
	}
public:
	Menu(const std::string &name, const std::vector<MenuItem*> &items, bool isRoot=false) :
		NamedItem(name), items(items), isRoot(isRoot)
	{
		if(items.empty())
			throw std::runtime_error("items is empty");
	
	}

	virtual void enter() override {
		while(true){
			this->print();
			std::size_t choose = readInt(1, items.size() + 1);
			if(choose == items.size() + 1)
				return;
			else
				items[choose - 1]->enter();
		}
	}
};

class TextInput : public NamedInputItem<std::string> {
public:
	TextInput(const std::string &name, const std::string &defaultVal)
		: NamedInputItem<std::string>(name, defaultVal){}

	virtual void enter(){
		std::cout << "enter text: ";
		std::getline(std::cin, this->value);
	}

};

template <typename Function>
class Action : public NamedItem {
private:
	Function actionFun;
public:
	Action(std::string name, Function actionFun) :
		NamedItem(name), actionFun(actionFun){}

	virtual void enter() override{
		actionFun();
	}
};