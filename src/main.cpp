#include <iostream>
#include <memory>

#include "set.hpp"
#include "graph.hpp"
#include "dict.hpp"
#include "optional.hpp"
#include "task.hpp"


int main(){
	Graph<int,float> g;


	// auto s = spanning(g);
	// auto it = s.getEdges().iterator();

	// while(it.hasNext()){
	// 	it.go();
	// 	std::cout << it.current().vertices.first->id << " " << it.current().vertices.second->id << std::endl;
	// }

	// auto a = dijkstra(g, 1);
	// auto it = a->iterator();
	// while(it->hasNext()){
	// 	it->go();
	// 	std::cout << it->current() << " " << (a->get(it->current()).hasValue() ? a->get(it->current()).getValue() : -1) << std::endl;
	// }


	return 0;
}