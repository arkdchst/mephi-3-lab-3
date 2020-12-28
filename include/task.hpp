#include <memory>

#include "set.hpp"
#include "graph.hpp"
#include "dict.hpp"
#include "optional.hpp"


template <typename I, typename W>
std::unique_ptr<IDictionary<I,Optional<W>>> dijkstra(const Graph<I,W> &graph, const I &start){
	IDictionary<I,Optional<W>> *distances = new HashTable<I,Optional<W>>;
	IDictionary<I,bool> *visited = new HashTable<I,bool>;
	distances->set(start, W());

	auto vertices = graph.getVertices();
	auto edges = graph.getEdges();

	auto vertit = vertices.iterator();
	while(vertit.hasNext()){
		vertit.go();
		if(vertit.current().id != start){
			distances->set(vertit.current().id, Optional<W>());
		}
		visited->set(vertit.current().id, false);
	}


	while(true){
		vertit = vertices.iterator();
		Optional<I> nextVert;
		Optional<W> minDist;
		while(vertit.hasNext()){ //find new vertex
			vertit.go();
			if(distances->get(vertit.current().id).hasValue() && !visited->get(vertit.current().id)){
				W dist = distances->get(vertit.current().id).getValue();
				if(!minDist.hasValue() || minDist.getValue() > dist){
					minDist = dist;
					nextVert = vertit.current().id;
				}
			}
		}

		if(!nextVert.hasValue()) break; //check that all vertices are visited

		auto edgit = graph.getVertex(nextVert.getValue()).edges.iterator(); //update distances
		while(edgit.hasNext()){
			edgit.go();
			I neighboorId = edgit.current()->neighboor(nextVert.getValue())->id;
			if(!visited->get(neighboorId)){
				
				if(!distances->get(neighboorId).hasValue()
					|| distances->get(graph.getVertex(nextVert.getValue()).id).getValue() + edgit.current()->weight
						< distances->get(neighboorId).getValue()
				){

					distances->get(neighboorId) =
						distances->get(graph.getVertex(nextVert.getValue()).id).getValue() + edgit.current()->weight;
				}
			}
		}
		visited->get(nextVert.getValue()) = true;
	}

	delete visited;

	return std::unique_ptr<IDictionary<I,Optional<W>>>(distances);
}

template <typename I, typename W>
Graph<I,W> spanning(const Graph<I,W> &graph){
	Graph<I,W> tree;

	auto edges = graph.getEdges();	

	auto it = edges.iterator();
	while(it.hasNext()){
		it.go();

		auto edge = it.current();
		if(tree.getVertices().contains(*edge.vertices.first) && tree.getVertices().contains(*edge.vertices.second)) continue;

		if(! tree.getVertices().contains(*edge.vertices.first)) tree.addVertex(edge.vertices.first->id);
		if(! tree.getVertices().contains(*edge.vertices.second)) tree.addVertex(edge.vertices.second->id);

		tree.addEdge(edge.weight, edge.vertices.first->id, edge.vertices.second->id);
	}

	return tree;
}
