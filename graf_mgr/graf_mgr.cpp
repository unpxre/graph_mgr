// graf_mgr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <sstream>
#include <fstream>

#define CMD_PROMPT "graph mgr:>"

#include "console_mgr.h"

#define VERSION 1.1

class Graph {
	private:
		bool isOriented;
		unsigned int vertexes;
		std::map<int, std::set<int>> data;
		typedef std::map<int, std::set<int>>::iterator dataIt;

	public:
		Graph(bool isOriented = false) {
			this->isOriented = isOriented;
		}

		bool setNumbOfVertexes(int vertexes) {
			this->vertexes = vertexes;
			for(int i = 1; i <= vertexes; ++i) {
				data.insert( std::pair<int, std::set<int>>(i, std::set<int>()) );
			}
			return true;
		}

		std::string setPair(int a, int b) {
			if ( (a > vertexes) || (b > vertexes) ) return "Bad pair data!\n";
			if (isOriented) {
				this->data[a].insert( b );
			} else {
				this->data[a].insert( b );
				this->data[b].insert( a );
			}
			return "";
		}

		/*
			Wyœwietl macierz s¹siedztwa
			https://pl.wikipedia.org/wiki/Macierz_s%C4%85siedztwa

		*/
		void showAdjacencyMatrix() {
			uConsoleMgr::echo("\n\nAdjacency Matrix\n", uConsoleMgr::CUTE);
			for(dataIt iterator = data.begin(); iterator != data.end(); ++iterator) {
				uConsoleMgr::echo(iterator->first, uConsoleMgr::SUCCESS);
				uConsoleMgr::echo(" [ ", uConsoleMgr::INFO);
				for (int i = 1; i <= data.size(); ++i) {
					if (iterator->second.find(i) != iterator->second.end()) uConsoleMgr::echo("1 ", uConsoleMgr::NORMAL);
					else uConsoleMgr::echo("0 ", uConsoleMgr::NORMAL);
				}
				std::set<int>::iterator it;
				uConsoleMgr::echo("]\n", uConsoleMgr::INFO);
			}
		}

		/*
		Wyœwietl macierz s¹siedztwa
		https://pl.wikipedia.org/wiki/Macierz_incydencji

		*/
		void  showIncidenceMatrix() {
			if (isOriented) {
				uConsoleMgr::echo("\n\nIncidence Matrix\n", uConsoleMgr::CUTE);
				for (dataIt iterator = data.begin(); iterator != data.end(); ++iterator) {
					uConsoleMgr::echo(iterator->first, uConsoleMgr::SUCCESS);
					uConsoleMgr::echo(" [ ", uConsoleMgr::INFO);
					for (int i = 1; i <= data.size(); ++i) {
						if (iterator->second.find(i) != iterator->second.end()) uConsoleMgr::echo("1 ", uConsoleMgr::NORMAL);
						else uConsoleMgr::echo("0 ", uConsoleMgr::NORMAL);
					}
					std::set<int>::iterator it;
					uConsoleMgr::echo("]\n", uConsoleMgr::INFO);
				}
			} else uConsoleMgr::echo("Can not show Incidence Matrix for not oriented Graph.\n", uConsoleMgr::WARNING);
		}
};

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

		int strToInt(std::string s) {
			bool m=false;
			int tmp=0;
			int i=0;
			if(s[0]=='-') {
				  i++;
				  m = true;
			}
			while(i<s.size()) {
			  tmp = 10*tmp+s[i]-48;
			  i++;
			}
			return m ? -tmp : tmp;   
		}

		std::string trim(std::string &str) {
			if (str.empty()) return str;
			std::size_t firstScan = str.find_first_not_of(' ');
			std::size_t first = firstScan == std::string::npos ? str.length() : firstScan;
			std::size_t last = str.find_last_not_of(' ');
			return str.substr(first, last - first + 1);
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
		std::string readFromFile(std::string filePath) {
			std::string line, tmp;
			std::fstream fileHandle;
			bool isOriented;
			int vertexes;
			fileHandle.open(filePath.c_str(), std::ios::in);
			if (fileHandle.is_open()) {
				// Odczytanie podstawowych informacji o grafie
				if (fileHandle.good()) {
					getline(fileHandle, line);
					isOriented = (confLineToInt(line) == 1) ? true : false;
				} else return ("First line in file: " + filePath + " must contain 1/0 (graph is oriented or not)\n");
				if (graphExist) delete this->graph;
				this->graph = new Graph(isOriented);
				graphExist = true;
				
				if (fileHandle.good()) {
					getline(fileHandle, line);
					vertexes = confLineToInt(line);
				}
				else return ("Second line in file: " + filePath + " must the number of vertexes\n");
				graph->setNumbOfVertexes(vertexes);
				
				// Odczytanie par
				while (fileHandle.good()) {
					getline(fileHandle, line);
					std::vector <std::string> ab = confLineToVector(line);
					if (ab.size() == 2) {
						tmp = graph->setPair(strToInt(ab[0]), strToInt(ab[1]));
						if (tmp.length() > 0) {
							uConsoleMgr::echo(tmp, uConsoleMgr::WARNING);
						}
					}
				}
			} else return ("Can not open " + filePath + "\n");
			

			return "Read from file finished successfully.\n";
		}

		void showAdjacencyMatrix() {
			if (!graphExist) return;
			graph->showAdjacencyMatrix();
		}

		void  showIncidenceMatrix() {
			if (!graphExist) return;
			graph->showIncidenceMatrix();
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

	while(true) {
		uConsoleMgr::echo("[F]: for read from file, [C]: for read from console, [X]: exit\n");
		option = uConsoleMgr::ask<std::string>();
		if (option == "X") break;
		uConsoleMgr::echo("You selected ");
		uConsoleMgr::echo((option == "F") ? "read from file" : "read from console");
		uConsoleMgr::echo("\n");
		if (option == "F") uConsoleMgr::echo(mgr.readFromFile("graf.txt"), uConsoleMgr::SUCCESS);
		else uConsoleMgr::echo(mgr.readFromConsole(), uConsoleMgr::SUCCESS);

		mgr.showAdjacencyMatrix();
		mgr.showIncidenceMatrix();
	}
	return 0;
}

