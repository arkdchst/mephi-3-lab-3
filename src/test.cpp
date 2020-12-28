#include <functional>
#include <utility>
#include <tuple>
#include <vector>
#include <map>
#include <set>

#include <gtest/gtest.h>

#include "task.hpp"


using DijkstraParam =
std::pair<
	//graph
	std::tuple<
		std::vector<int>, //verices
		std::vector<std::tuple<int,int,int>>, //edges
		int
	>,
	//expected
	std::map<int,int>
>;

class DijkstraTest : public testing::TestWithParam<DijkstraParam>{
public:

};

TEST_P(DijkstraTest,Test){
	Graph<int,int> graph;

	for(auto vertex : std::get<0>(GetParam().first)){
		graph.addVertex(vertex);
	}

	for(auto tuple : std::get<1>(GetParam().first)){
		graph.addEdge(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));
	}

	auto dijkstraOut = dijkstra(graph, std::get<2>(GetParam().first));

	auto it = dijkstraOut->iterator();
	while(it->hasNext()){
		it->go();
		ASSERT_EQ(dijkstraOut->get(it->current()).hasValue() ? dijkstraOut->get(it->current()).getValue() : -1, GetParam().second.at(it->current()));
	}

}

const DijkstraParam dijkstraData[] = {
	{{{1,2,3,4,5,6,7},{{7,1,2},{15,2,4},{6,4,5},{9,5,6},{14,6,1},{9,1,3},{10,2,3},{11,3,4},{2,3,6}},1}, {{3, 9}, {6, 11}, {4, 20}, {1, 0}, {2, 7}, {5, 20}, {7, -1}}},
	{ {{1,2,3,4,5,6},{{7,1,3},{5,1,5},{1,2,5},{2,2,6},{8,3,4},{1,3,5},{3,5,6}},1}, {{1,0},{2,6},{3,6},{4,14},{5,5},{6,8}} }
};
INSTANTIATE_TEST_SUITE_P(, DijkstraTest, testing::ValuesIn(dijkstraData));



using SpanningParam =
//graph
std::tuple<
	std::vector<int>, //verices
	std::vector<std::tuple<int,int>> //edges
>;

class SpanningTest : public testing::TestWithParam<SpanningParam>{
public:

};

TEST_P(SpanningTest,Test){
	Graph<int,int> graph;

	for(auto vertex : std::get<0>(GetParam())){
		graph.addVertex(vertex);
	}

	for(auto tuple : std::get<1>(GetParam())){
		graph.addEdge(0, std::get<0>(tuple), std::get<1>(tuple));
	}

	auto tree = spanning(graph);

	ASSERT_EQ(tree.getVertices().getSize(), graph.getVertices().getSize()); //check count of vertices
	//check that "tree" is a tree
	ASSERT_EQ(tree.getVertices().getSize(), tree.getEdges().getSize() + 1);
	ASSERT_TRUE(tree.isConnected());
}

const SpanningParam spanningData[] = {
	{{1,2,3,4,5,6},{{1,2},{2,4},{4,5},{5,6},{6,1},{1,3},{2,3},{3,4},{3,6}}},
	{{1,2,3,4,5,6},{{1,3},{1,5},{2,5},{2,6},{3,4},{3,5},{5,6}}}
};
INSTANTIATE_TEST_SUITE_P(, SpanningTest, testing::ValuesIn(spanningData));



class GraphTest : public testing::Test {
public:
	Graph<int,int> graph;

	void SetUp() {
		this->graph.addVertex(1);
		this->graph.addVertex(2);
		this->graph.addVertex(3);
		this->graph.addVertex(4);
		this->graph.addVertex(5);
		this->graph.addVertex(6);

		this->graph.addEdge(7,1,2);
		this->graph.addEdge(15,2,4);
		this->graph.addEdge(6,4,5);
		this->graph.addEdge(9,5,6);
		this->graph.addEdge(14,6,1);
		this->graph.addEdge(9,1,3);
		this->graph.addEdge(10,2,3);
		this->graph.addEdge(11,3,4);
		this->graph.addEdge(2,3,6);
	}

};



TEST_F(GraphTest, addVertex){
	int count = this->graph.getVertices().getSize();

	ASSERT_FALSE(this->graph.getVertices().contains({.id=7}));

	this->graph.addVertex(7);

	ASSERT_EQ(this->graph.getVertices().getSize(), count + 1);
	ASSERT_TRUE(this->graph.getVertices().contains({.id=7}));
}

TEST_F(GraphTest, removeVertex){
	int vertCount = this->graph.getVertices().getSize();
	int edgeCount = this->graph.getEdges().getSize();

	this->graph.removeVertex(3);

	ASSERT_EQ(this->graph.getVertices().getSize(), vertCount - 1);
	ASSERT_EQ(this->graph.getEdges().getSize(), edgeCount - 4);
}

TEST_F(GraphTest, addEdge){
	int count = this->graph.getEdges().getSize();

	this->graph.addEdge(0,1,5);

	ASSERT_EQ(this->graph.getEdges().getSize(), count + 1);

	bool found = false;
	auto it = this->graph.getEdges().iterator();
	while(it.hasNext()){
		it.go();
		auto edge = it.current();
		if(edge.weight == 0) if(edge.vertices.first->id == 5 && edge.vertices.second->id == 1 || edge.vertices.first->id == 1 && edge.vertices.second->id == 5){
			found = true;
			break;
		}
	}
	ASSERT_TRUE(found);
}

TEST_F(GraphTest, removeEdge){
	int count = this->graph.getEdges().getSize();

	this->graph.removeEdge(1,3);

	ASSERT_EQ(this->graph.getEdges().getSize(), count - 1);

	bool found = false;
	auto it = this->graph.getEdges().iterator();
	while(it.hasNext()){
		it.go();
		auto edge = it.current();
		if(edge.weight == 0) if(edge.vertices.first->id == 3 && edge.vertices.second->id == 1 || edge.vertices.first->id == 1 && edge.vertices.second->id == 3){
			found = true;
			break;
		}
	}
	ASSERT_FALSE(found);
}
