// graf_mgr.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
#include <sstream>

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
			
			std::cout<< "Graph is oriented or not [Y]/[N]\ngraph mgr:>";
			std::cin>>userTmpStr;
			Graph graph((userTmpStr == "Y")?true:false);
			this->graph = graph;
			
			std::cout<< "Number of vertexes\ngraph mgr:>";
			std::cin>>userTmpInt;
			graph.setNumbOfVertexes(userTmpInt);

			std::cout<< "Number of pairs\ngraph mgr:>";
			std::cin>>pairs;
			for(int i = 1; i <= pairs; ++i) {
				std::cout<< "Pair in format [a,b]\ngraph mgr:>";
				std::cin>>userTmpStr;
				std::vector<std::string> ab = split(userTmpStr, ',');
				userTmpStr = graph.setPair(strToInt(ab[0]), strToInt(ab[1]));
				if(userTmpStr.length() > 0) {
					std::cerr << userTmpStr;
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
	std::cout<< "Graph mgr v 1.0\n";
	std::string option;
	Mgr mgr;

	mgr.readFromConsole();
	//mgr.showNeighbor();
	//mgr.showIncident();
	system("pause");
	return 0;

	while(true) {
		std::cout<< "[0]: for read from file, [1]: for read from console\ngraph mgr:>";
		std::cin>>option;
		std::cout << "You selected " << ((option == "0")?"read from file":"read from console") << std::endl;
		if(option == "0") std::cout<< mgr.readFromFile();
		else std::cout<< mgr.readFromConsole();
	}
	return 0;
}

