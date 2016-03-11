// graf_mgr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <sstream>

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
			for(int i = 0; i <= vertexes; ++i) {
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

		void showNeighbor() {
			std::cout<<"\n\nNeighbor (" << data.size() << ")\n";
			for(dataIt iterator = ++(data.begin()); iterator != data.end(); ++iterator) {
				std::cout<<iterator->first<<" { ";
				std::set<int>::iterator it;
				for (it = iterator->second.begin(); it != iterator->second.end(); ++it) {
					std::cout<< *it << " ";
				}
				std::cout<<"}\n";
			}
		}

		void  showIncident() {
			std::cout<<"\n\nInciden:\n";
		}
};

class Mgr {
	private:
		Graph graph;

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
			if(s[0]=='-')
			{
				  i++;
				  m = true;
			}
			while(i<s.size())
			{
			  tmp = 10*tmp+s[i]-48;
			  i++;
			}
			return m ? -tmp : tmp;   
		}
	public:
		Mgr() { }
		std::string readFromFile() {

			return "Read from file finished successfully.\n";
		}

		void showNeighbor() {
			graph.showNeighbor();
		}

		void  showIncident() {
			graph.showIncident();
		}

		std::string readFromConsole() {
			std::string userTmpStr;
			int userTmpInt;
			int pairs;
			
			uConsoleMgr::echo("Graph is oriented or not [Y]/[N]\n");
			userTmpStr = uConsoleMgr::ask<std::string>();
			Graph graph((userTmpStr == "Y")?true:false);
			this->graph = graph;
			
			uConsoleMgr::echo("Number of vertexes\n");
			userTmpInt = uConsoleMgr::ask<int>();
			graph.setNumbOfVertexes(userTmpInt);

			uConsoleMgr::echo("Number of pairs\n");
			pairs = uConsoleMgr::ask<int>();
			for(int i = 1; i <= pairs; ++i) {
				uConsoleMgr::echo("Pair in format [a,b]\n");
				userTmpStr = uConsoleMgr::ask<std::string>();
				std::vector<std::string> ab = split(userTmpStr, ',');
				userTmpStr = graph.setPair(strToInt(ab[0]), strToInt(ab[1]));
				if(userTmpStr.length() > 0) {
					uConsoleMgr::echo(userTmpStr, uConsoleMgr::WARNING);
					--i;
				}
			}
			graph.showNeighbor();
			graph.showIncident();
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

	//mgr.readFromConsole();
	//mgr.showNeighbor();
	//mgr.showIncident();
	//system("pause");
	//return 0;

	while(true) {
		uConsoleMgr::echo("[F]: for read from file, [C]: for read from console\n");
		option = uConsoleMgr::ask<std::string>();
		uConsoleMgr::echo("You selected ");
		uConsoleMgr::echo((option == "F") ? "read from file" : "read from console");
		uConsoleMgr::echo("\n");
		if (option == "F") uConsoleMgr::echo(mgr.readFromFile(), uConsoleMgr::SUCCESS);
		else uConsoleMgr::echo(mgr.readFromConsole(), uConsoleMgr::SUCCESS);
	}
	return 0;
}

