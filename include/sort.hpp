#pragma once

#include <stack>
#include <utility>
#include <iostream>
#include <string>
#include <memory>

#include "sequence.hpp"


template <typename T>
std::unique_ptr<Sequence<T>> sort(Sequence<T> *seq, int (*cmp)(T,T) ){
	auto newSeq = std::unique_ptr<Sequence<T>>(seq->clone());

	auto swap = [&newSeq](int i, int j) {
		T tmp = newSeq->get(i);
		newSeq->set(newSeq->get(j), i);
		newSeq->set(tmp, j);
	};

	if(seq->getSize() == 0) return newSeq;
	if(seq->getSize() == 1) return newSeq;

	std::stack<std::pair<int,int>> edges;
	edges.push({0, newSeq->getSize() - 1});

	while(!edges.empty()){
		int start = edges.top().first;
		int end = edges.top().second;
		edges.pop();

		int p = start;
		int i = p + 1;
		int j = end;

		while(i <= j && i <= end && j > p){
			if(cmp(newSeq->get(i), newSeq->get(p)) <= 0) i++;
			else if(cmp(newSeq->get(j), newSeq->get(p)) > 0) j--;
			else swap(i, j);
		}
		swap(p, j);
		if(j > start + 1)
			edges.push({p, j - 1});
		if(j < end - 1)
			edges.push({j + 1, end});
	}
	return newSeq;
}

