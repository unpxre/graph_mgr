
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
			for (int i = 1; i <= vertexes; ++i) {
				data.insert(std::pair<int, std::set<int>>(i, std::set<int>()));
			}
			return true;
		}

		std::string setPair(int a, int b) {
			if ((a > vertexes) || (b > vertexes)) {
				return "Bad pair data!\n";
			}
			if (isOriented) {
				this->data[a].insert(b);
			}
			else {
				this->data[a].insert(b);
				this->data[b].insert(a);
			}
			return "";
		}

		/*
		Wyœwietl macierz s¹siedztwa
		https://pl.wikipedia.org/wiki/Macierz_s%C4%85siedztwa

		*/
		void showAdjacencyMatrix() {
			uConsoleMgr::echo("\n\nAdjacency Matrix\n", uConsoleMgr::CUTE);
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
			}
			else uConsoleMgr::echo("Can not show Incidence Matrix for not oriented Graph.\n", uConsoleMgr::WARNING);
		}

		/*
		Przeszukaj wszerz (BFS)
		*/
		std::vector<int> getBFSWay(int initVertex) {
			BFS(this->data, initVertex);
			std::vector < int > a;
			return a;
		}

		/*
		Przeszukaj w g³¹b (DFS)
		*/
		std::vector<int> getDFSWay(int initVertex) {
			DFS(this->data, initVertex);
			std::vector < int > a;
			return a;
		}
};
