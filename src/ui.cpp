#include <iostream>
#include <limits>
#include <string>
#include <utility>
#include <vector>

#include "graph.hpp"
#include "optional.hpp"
#include "task.hpp"


const std::string WENT_WRONG = "something went wrong";
const std::string help =
"usage: addv <id>             : add vertex with specified id\n"
"       adde <w> <id1> <id2>  : add edge with weight w between id1 and id2\n"
"       remv <id>             : remove vertex with specified id\n"
"       reme <id1> <id2>      : remove vertex with specified id\n"
"       dij  <start>          : start dijkstra on vertex start\n"
"       span                  : create spanning tree\n"
"       exit                  : \n";



void printUsage(bool went_wrong = false, const std::string &exc = ""){
	if(went_wrong){
		std::cout << WENT_WRONG;
		if(exc != "") std::cout << ": " << exc << std::endl;
	}

	std::cout << help << std::endl;	
}


std::pair<std::string, std::vector<int>> parseCommand(std::string line){
	std::string command;
	std::vector<int> args;

	command = line.substr(0, line.find(" "));
	line.erase(0, command.size() + 1);

	while(line.size()){
		if(line[0] == ' '){
			line.erase(0, 1);
			continue;
		}

		int spacePos = line.find(" ");
		std::string strArg;
		if(spacePos == std::string::npos){
			strArg = line;
			line.clear();
		}else{
			strArg = line.substr(0, line.find(" "));
			line.erase(0, strArg.size() + 1);
		}

		int arg = std::stoi(strArg);
		args.push_back(arg);
	}
	return {command, args};
}

void handleCommand(Graph<int,int> &graph){
	std::string line;
	std::getline(std::cin, line);

	if(std::cin.fail()){
		printUsage(true);
		return;
	}

	std::string command;
	std::vector<int> args;
	try{
		auto out = parseCommand(line);
		command = out.first;
		args = out.second;
	}catch(const std::exception &e){
		printUsage(true, e.what());
		return;
	}

	const std::string BAD_ARG_COUNT = "bad argument count";
	if(command == "addv"){
		if(args.size() != 1){
			printUsage(true, BAD_ARG_COUNT);
			return;
		}
		graph.addVertex(args[0]);
	}else
	if(command == "adde"){
		if(args.size() != 3){
			printUsage(true, BAD_ARG_COUNT);
			return;
		}
		graph.addEdge(args[0], args[1], args[2]);
	}else
	if(command == "remv"){
		if(args.size() != 1){
			printUsage(true, BAD_ARG_COUNT);
			return;
		}
		graph.removeVertex(args[0]);
	}else
	if(command == "reme"){
		if(args.size() != 2){
			printUsage(true, BAD_ARG_COUNT);
			return;
		}
		graph.removeEdge(args[0], args[1]);
	}else
	if(command == "dij"){
		if(args.size() != 1){
			printUsage(true, BAD_ARG_COUNT);
			return;
		}
		auto out = dijkstra(graph, args[0]);
		auto it = out->iterator();
		std::cout << "distances to other vertices:" << std::endl;
		while(it->hasNext()){
			it->go();
			std::cout << it->current() << ": " << (out->get(it->current()).hasValue() ? out->get(it->current()).getValue() : -1) << std::endl;
		}
	}else
	if(command == "span"){
		if(args.size() != 0){
			printUsage(true, BAD_ARG_COUNT);
			return;
		}
		auto out = spanning(graph);
		auto it = out.getEdges().iterator();
		std::cout << "selected edges:" << std::endl;
		while(it.hasNext()){
			it.go();
			std::cout << it.current().vertices.first->id << " - " << it.current().vertices.second->id << std::endl;
		}
	}else
	if(command == "exit"){
		exit(0);
		return;
	}else
	if(command == ""){
		return;
	}else{
		printUsage();
		return;
	}

	std::cout << "OK!" << std::endl;
}




int main(int argc, const char *argv[]){
	Graph<int,int> graph;

	printUsage();

	while(true){
		try{
			handleCommand(graph);
		}catch(const std::exception &e){
			printUsage(true, e.what());
		}
	}

	return 0;
}