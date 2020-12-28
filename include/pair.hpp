#pragma once

template <typename F, typename S>
struct Pair{
	F first;
	S second;

	bool operator==(const Pair &other){
		return this->first == other.first && this->second == other.second;
	}

	bool operator!=(const Pair &other){
		return ! operator==(other);
	}
};
