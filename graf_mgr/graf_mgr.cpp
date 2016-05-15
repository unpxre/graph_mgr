// graf_mgr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <list>
#include <queue>
#include <sstream>
#include <fstream>

#define CMD_PROMPT "graph mgr:>"

#include "utils.h"
#include "console_mgr.h"
#include "graph_search.h"

#include "graph.h"

#define VERSION 1.2

// wczytywanie wag zrob...
// https://pl.wikipedia.org/wiki/Algorytm_Prima
//drzewo dowolnie rozpinajace 
//dijska


class Mgr {
	private:
		Graph *graph;
		bool graphExist;

		std::vector<std::string> split(const std::string &text, char sep) {
		  std::vector<std::string> tokens;
		  std::size_t start = 0, end = 0;
		  while ((end = text.find(sep, start)) != std::string::npos) {
			tokens.push_back(text.substr(start, end - start));
			start = end + 1;
		  }
		  tokens.push_back(text.substr(start));
		  return tokens;
		}

		int confLineToInt(std::string line) {
			size_t found = line.find("#");
			if (found != std::string::npos) {
				line = line.substr(0, found);
			}
			return strToInt(trim(line));
		}

		std::vector<std::string> confLineToVector(std::string line) {
			size_t found = line.find("#");
			if (found != std::string::npos) {
				line = line.substr(0, found);
			}
			line = trim(line);
			return split(line, ',');
		}

	public:
		Mgr() {
			graphExist = false;
		}
		~Mgr() {
			if (graphExist) delete this->graph;
		}
		std::string readFromFile(std::string filePath, bool withWeight = false) {
			std::string line, tmp;
			std::fstream fileHandle;
			bool isOriented;
			int vertexes;
			fileHandle.open(filePath.c_str(), std::ios::in);
			if (fileHandle.is_open()) {
				// Odczytanie podstawowych informacji o grafie
				if (fileHandle.good()) {
					line = "";
					while (fileHandle.good() && line.length() < 2) getline(fileHandle, line);
					isOriented = (confLineToInt(line) == 1) ? true : false;
					if (isOriented) uConsoleMgr::echo("Graph reading from file is oriented.\n", uConsoleMgr::INFO);
					else uConsoleMgr::echo("Graph reading from file is NOT oriented.\n", uConsoleMgr::INFO);
				} else return ("First line in file: " + filePath + " must contain 1/0 (graph is oriented or not)\n");
				if (graphExist) delete this->graph;
				this->graph = new Graph(isOriented);
				graphExist = true;
				
				if (fileHandle.good()) {
					line = "";
					while (fileHandle.good() && line.length() < 2) getline(fileHandle, line);
					vertexes = confLineToInt(line);
					uConsoleMgr::echo("Graph reading from file have " + intToStr(vertexes) + " vertexes.\n", uConsoleMgr::INFO);
				}
				else return ("Second line in file: " + filePath + " must the number of vertexes\n");
				graph->setNumbOfVertexes(vertexes);
				
				// Odczytanie par
				while (fileHandle.good()) {
					getline(fileHandle, line);
					if (line.length() < 2) continue;
					std::vector <std::string> ab = confLineToVector(line);
					if (ab.size() >= 2 && !withWeight) {
						tmp = graph->setPair(strToInt(ab[0]), strToInt(ab[1]));
						if (tmp.length() > 0) {
							uConsoleMgr::echo(tmp, uConsoleMgr::WARNING);
						}
					} else if (ab.size() >= 3 && withWeight) {
						tmp = graph->setPair(strToInt(ab[0]), strToInt(ab[1]), strToInt(ab[2]));
						if (tmp.length() > 0) {
							uConsoleMgr::echo(tmp, uConsoleMgr::WARNING);
						}
					}
				}
			} else return ("Can not open " + filePath + "\n");
			

			return "Read from file finished successfully.\n";
		}
		
		std::string readWeightsFromFile(std::string filePath) {


			return "";
		}

		void showAdjacencyMatrix() {
			if (!graphExist) return;
			graph->showAdjacencyMatrix();
		}

		void  showIncidenceMatrix() {
			if (!graphExist) return;
			graph->showIncidenceMatrix();
		}

		void getBFSWay() {
			if (!graphExist) return;
			graph->getBFSWay(1);
		}

		void getDFSWay() {
			if (!graphExist) return;
			graph->getDFSWay();
		}

		void getSCC() {
			if (!graphExist) return;
			//graph->getDFSWay();
			graph->getSCC();
		}

		void getMinTree() {
			if (!graphExist) return;
			graph->getMinTree();
		}

		void findWay() {
			if (!graphExist) return;
			unsigned int a, b;
			uConsoleMgr::echo("Set initial vertex:\n");
			a = uConsoleMgr::ask<int>();

			uConsoleMgr::echo("Set target vertex:\n");
			b = uConsoleMgr::ask<int>();

			graph->findWay(a,b);
		}

		void minColors() {
			if (!graphExist) return;
			graph->minColors();
		}

		void maxFlow() {
			if (!graphExist) return;
			unsigned int a, b;
			uConsoleMgr::echo("Set initial vertex:\n");
			a = uConsoleMgr::ask<int>();

			uConsoleMgr::echo("Set target vertex:\n");
			b = uConsoleMgr::ask<int>();

			graph->maxFlow(a, b);
		}

		std::string readFromConsole() {
			std::string userTmpStr;
			int userTmpInt;
			int pairs;
			
			uConsoleMgr::echo("Graph is oriented or not [Y]/[N]\n");
			userTmpStr = uConsoleMgr::ask<std::string>();
			
			if (graphExist) delete this->graph;
			this->graph = new Graph((userTmpStr == "Y") ? true : false);
			graphExist = true;
			
			uConsoleMgr::echo("Number of vertexes\n");
			userTmpInt = uConsoleMgr::ask<int>();
			graph->setNumbOfVertexes(userTmpInt);

			uConsoleMgr::echo("Number of pairs\n");
			pairs = uConsoleMgr::ask<int>();
			for(int i = 1; i <= pairs; ++i) {
				uConsoleMgr::echo("Pair in format [a,b]\n");
				userTmpStr = uConsoleMgr::ask<std::string>();
				std::vector<std::string> ab = split(userTmpStr, ',');
				if (ab.size() == 2) {
					userTmpStr = graph->setPair(strToInt(ab[0]), strToInt(ab[1]));
					if (userTmpStr.length() > 0) {
						uConsoleMgr::echo(userTmpStr, uConsoleMgr::WARNING);
						--i;
					}
				} else {
					uConsoleMgr::echo("Wrong pair format", uConsoleMgr::WARNING);
					--i;
				}
			}
			return "Read from console finished successfully.\n";
		}
};

int _tmain(int argc, _TCHAR* argv[])
{
	uConsoleMgr::echo("Graph mgr v ", uConsoleMgr::CUTE);
	uConsoleMgr::echo(VERSION, uConsoleMgr::SUCCESS);
	uConsoleMgr::echo("\n");
	std::string option;
	Mgr mgr;

	enum mainOption {
		READ_GRAPH_FROM_FILE					= 'F',
		READ_GRAPH_AND_WEIGHTS_FROM_FILE		= 'W',
		READ_GRAPH_FROM_CONSOLE					= 'C',
		SHOW_ADJENCY_MATRIX						= 'A',
		SHOW_INCIDENCE_MATRIX					= 'I',
		SHOW_BFS								= 'B',
		SHOW_DFS								= 'D',
		SHOW_SCC								= 'S',
		SHOW_PRIM								= 'P',
		SHOW_DIJSKRA							= 'J',
		SHOW_GRAPH_MIN_COLORS					= 'K',
		SHOW_MAXIMUM_FLOW						= 'M',

		EXIT									= 'X'
	};

	std::string usageInfo =
		"[F]: for read graph from file,\n"
		"[W]: for read graph and weights from file,\n"
		"[C]: for read graph from console,\n" 
		"[A]: show adjency matrix for graph,\n"
		"[I]: show incidence matrix for graph,\n"
		"[B]: show BFS,\n"
		"[D]: show DFS,\n"
		"[S]: show Strongly-Connected-Components,\n"
		"[P]: minimum spanning tree (prim algorithm),\n"
		"[K]: graph coloring,\n"
		"[M]: maximum flow,\n"
		
		"[X]: exit\n";

	while(true) {
		uConsoleMgr::echo(usageInfo);
		option = uConsoleMgr::ask<std::string>();
		
		if (option.length() < 1) continue;

		switch (option[0]) {
			case READ_GRAPH_FROM_FILE:
				uConsoleMgr::echo("You selected read graph from file.\n");
				uConsoleMgr::echo(mgr.readFromFile("g.txt"), uConsoleMgr::SUCCESS);
				break;
			case READ_GRAPH_AND_WEIGHTS_FROM_FILE:
				uConsoleMgr::echo("You selected read graph and weights from file.\n");
				uConsoleMgr::echo(mgr.readFromFile("g.txt", true), uConsoleMgr::SUCCESS);
				break;
			case READ_GRAPH_FROM_CONSOLE:
				uConsoleMgr::echo("You selected read graph from console.\n");
				uConsoleMgr::echo(mgr.readFromConsole(), uConsoleMgr::SUCCESS);
				break;
			case SHOW_ADJENCY_MATRIX:
				mgr.showAdjacencyMatrix();
				break;
			case SHOW_INCIDENCE_MATRIX:
				mgr.showIncidenceMatrix();
				break;
			case SHOW_BFS:
				mgr.getBFSWay();
				break;
			case SHOW_DFS:
				mgr.getDFSWay();
				break;
			case SHOW_SCC:
				mgr.getSCC();
				break;
			case SHOW_PRIM:
				mgr.getMinTree();
				break;
			case SHOW_DIJSKRA:
				mgr.findWay();
				break;
			case SHOW_GRAPH_MIN_COLORS:
				mgr.minColors();
				break;
			case SHOW_MAXIMUM_FLOW:
				mgr.maxFlow();
				break;

			case EXIT:
				return 0;
		}
	}
	return 0;
}

