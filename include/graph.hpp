#pragma once

#include <stdexcept>

#include "set.hpp"
#include "optional.hpp"
#include "pair.hpp"
#include "dict.hpp"

template <typename I, typename W>
struct Edge;

template <typename I, typename W>
struct Vertex{
	I id;
	Set<Edge<I,W>*> edges;

	bool operator==(const Vertex &other) const {
		return this->id == other.id;
	}
};

template <typename I, typename W>
struct Edge{
	Pair<Vertex<I,W>*,Vertex<I,W>*> vertices;
	W weight;

	bool operator==(const Edge &other) const {
		return (this->vertices.first->id == other.vertices.first->id && this->vertices.second->id == other.vertices.second->id)
			|| (this->vertices.first->id == other.vertices.second->id && this->vertices.second->id == other.vertices.first->id);
	}

	Vertex<I,W> *neighboor(const I &current){
		if(current == vertices.first->id) return vertices.second;
		if(current == vertices.second->id) return vertices.first;

		throw std::runtime_error("not in vertices");
	}
};


template <typename I, typename W>
class Graph{
private:
	Set<Vertex<I,W>> vertices;
	Set<Edge<I,W>> edges;

	Vertex<I,W>& _getVertex(const I&);
public:
	Graph() = default;

	const Set<Vertex<I,W>>& getVertices() const;
	const Set<Edge<I,W>>& getEdges() const;

	const Vertex<I,W>& getVertex(const I&) const;

	void addVertex(const I&);
	void removeVertex(const I&);
	void addEdge(const W&, const I&, const I&); //add edge between two vertices
	void removeEdge(const I&, const I&);

	bool isConnected() const;
};

template <typename I, typename W>
const Set<Vertex<I,W>>& Graph<I,W>::getVertices() const {
	return this->vertices;
}

template <typename I, typename W>
const Set<Edge<I,W>>& Graph<I,W>::getEdges() const {
	return this->edges;
}


template <typename I, typename W>
const Vertex<I,W>& Graph<I,W>::getVertex(const I &id) const {
	auto it = this->vertices.iterator();
	while(it.hasNext()){
		if(it.next().id == id) return it.current();
	}

	throw std::runtime_error("vertex not found");
}

template <typename I, typename W>
Vertex<I,W>& Graph<I,W>::_getVertex(const I &id){
	return const_cast<Vertex<I,W>&>(const_cast<const Graph<I,W>*>(this)->getVertex(id));
}

template <typename I, typename W>
void Graph<I,W>::addVertex(const I &id){
	this->vertices.add({.id=id});
}

template <typename I, typename W>
void Graph<I,W>::removeVertex(const I &id){
	auto vertex = this->getVertex(id);

	auto edges = vertex.edges;
	auto it = edges.iterator();
	while(it.hasNext()){
		this->removeEdge(id, it.next()->neighboor(id)->id);
	}
	this->vertices.remove(vertex);
}

template <typename I, typename W>
void Graph<I,W>::addEdge(const W &weight, const I &from, const I &to){
	auto &fromV = this->_getVertex(from);
	auto &toV = this->_getVertex(to);
	auto &addedEdge = this->edges.add({{&fromV, &toV}, weight});
	fromV.edges.add(&addedEdge);
	toV.edges.add(&addedEdge);
}

template <typename I, typename W>
void Graph<I,W>::removeEdge(const I &from, const I &to){
	auto &fromV = this->_getVertex(from);
	auto &toV = this->_getVertex(to);

	auto it = this->edges.iterator();
	while(it.hasNext()){
		it.go();
		if(it.current() == Edge<I,W>{.vertices = {&fromV, &toV}}){
			fromV.edges.remove(&it.current());
			toV.edges.remove(&it.current());
			this->edges.remove(it.current());
			return;
		}
	}

	throw std::runtime_error("there is no such edge");
}

template <typename I, typename W>
bool Graph<I,W>::isConnected() const {
	IDictionary<I,bool> *visited = new HashTable<I,bool>;
	Set<I> toVisit;

	auto vertit = vertices.iterator();
	while(vertit.hasNext()){
		vertit.go();
		visited->set(vertit.current().id, false);
	}

	toVisit.add(this->vertices.iterator().next().id);

	while(toVisit.getSize()){
		I nextVert = toVisit.iterator().next();

		auto edgit = this->getVertex(nextVert).edges.iterator(); //update distances
		while(edgit.hasNext()){
			edgit.go();
			I neighboorId = edgit.current()->neighboor(nextVert)->id;
			if(!visited->get(neighboorId) && !toVisit.contains(neighboorId)){
				toVisit.add(neighboorId);
			}
		}
		toVisit.remove(nextVert);
		visited->get(nextVert) = true;
	}

	int count = 0;
	auto it = visited->iterator();
	while(it->hasNext()){
		if(visited->get(it->next()))
			count++;
	}

	delete visited;

	return count == this->vertices.getSize();
}
