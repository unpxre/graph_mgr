
class Graph {
	private:
		bool isOriented;
		unsigned int vertexes;
		std::map<int, std::set<int>> data;
		std::map<std::pair<unsigned int, unsigned int>, int> weights;
		typedef std::map<int, std::set<int>>::iterator dataIt;
		typedef std::set<int>::iterator dataNeightIt;

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

		std::string setPair(unsigned int a, unsigned int b, unsigned int weight = 0) {
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

			return this->setWeight(a, b, weight);
		}

		std::string setWeight(unsigned int a, unsigned int b, unsigned int w) {
			if ((a > vertexes) || (b > vertexes)) {
				return "Bad weight data!\n";
			}
			if (isOriented) {
				this->weights.insert(std::pair<std::pair<unsigned int, unsigned int>, int>(std::pair<unsigned int, unsigned int>(a, b), w));
			} else {
				this->weights.insert(std::pair<std::pair<unsigned int, unsigned int>, int>(std::pair<unsigned int, unsigned int>(a, b), w));
				this->weights.insert(std::pair<std::pair<unsigned int, unsigned int>, int>(std::pair<unsigned int, unsigned int>(b, a), w));
			}

			return "";
		}

		/*
		Wyœwietl macierz s¹siedztwa
		https://pl.wikipedia.org/wiki/Macierz_s%C4%85siedztwa

		*/
		void showAdjacencyMatrix() {
			uConsoleMgr::echo("\nAdjacency Matrix\n    ", uConsoleMgr::CUTE);
			for (dataIt iterator = data.begin(); iterator != data.end(); ++iterator) {
				uConsoleMgr::echo(iterator->first, uConsoleMgr::SUCCESS);
				uConsoleMgr::echo(" ");
			}
			uConsoleMgr::echo("\n");
			for (dataIt iterator = data.begin(); iterator != data.end(); ++iterator) {
				uConsoleMgr::echo(iterator->first, uConsoleMgr::SUCCESS);
				uConsoleMgr::echo(" [ ", uConsoleMgr::INFO);
				for (unsigned int i = 1; i <= data.size(); ++i) {
					if (iterator->second.find(i) != iterator->second.end()) uConsoleMgr::echo("1 ", uConsoleMgr::NORMAL);
					else uConsoleMgr::echo("0 ", uConsoleMgr::NORMAL);
				}
				uConsoleMgr::echo("]\n", uConsoleMgr::INFO);
			}
		}

		/*
		Wyœwietl macierz incydencji
		https://pl.wikipedia.org/wiki/Macierz_incydencji

		*/
		void  showIncidenceMatrix() {
			if (isOriented) {
				uConsoleMgr::echo("\nIncidence Matrix\n", uConsoleMgr::CUTE);
				for (dataIt iterator = data.begin(); iterator != data.end(); ++iterator) {
					uConsoleMgr::echo(iterator->first, uConsoleMgr::SUCCESS);
					uConsoleMgr::echo(" [ ", uConsoleMgr::INFO);
					for (dataIt iteratorD = data.begin(); iteratorD != data.end(); ++iteratorD) {
						for (dataNeightIt it = iteratorD->second.begin(); it != iteratorD->second.end(); ++it) {
							if (iterator->first == iteratorD->first) uConsoleMgr::echo(" 1 ", uConsoleMgr::NORMAL);
							else if (iterator->first == *it) uConsoleMgr::echo("-1 ", uConsoleMgr::NORMAL);
							else uConsoleMgr::echo(" 0 ", uConsoleMgr::NORMAL);
						}
					}
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
		std::vector<int> getDFSWay() {
			DFS(this->data);
			std::vector < int > a;
			return a;
		}

		/*
		Spójne sk³adowe
		*/
		void getSCC() {
			DFS dfs = DFS(this->data);
			dfs.getSCC();
		}

		/*
		Minimlane drzewo rozpinaj¹ce
		https://pl.wikipedia.org/wiki/Minimalne_drzewo_rozpinaj%C4%85ce
		*/
		void getMinTree() {

		}

		/*
		Najkrótsza œcie¿ka
		https://pl.wikipedia.org/wiki/Algorytm_Dijkstry
		*/
		void findWay(unsigned int startVertex, unsigned int endVertex) {
			const int INFINITY_VALUE = 100000;

			std::vector<int> minimalDistance(this->vertexes+1, INFINITY_VALUE);
			minimalDistance[startVertex] = 0;
			std::set< std::pair<int, int> > activeVertices;
			activeVertices.insert({ 0, startVertex });
			std::map<int, int> prevVertex;
			unsigned int minDist = INFINITY_VALUE;

			while (!activeVertices.empty()) {
				int where = activeVertices.begin()->second;

				if (where == endVertex) {
					minDist = minimalDistance[where];
					break;
				}

				activeVertices.erase(activeVertices.begin());

				for (unsigned int x : data[where]) {
					if (minimalDistance[x] > minimalDistance[where] + weights[std::pair<int,int>(where,x)]) {
						activeVertices.erase({ minimalDistance[x], x });
						minimalDistance[x] = minimalDistance[where] + weights[std::pair<int, int>(where, x)];
						activeVertices.insert({ minimalDistance[x], x });
						if (prevVertex.find(x) == prevVertex.end()) {
							prevVertex.insert({ x, where });
						} else {
							prevVertex[x] = where;
						}
					}
				}
			}

			if (minDist != INFINITY_VALUE) {
				uConsoleMgr::echo("Minimal distance is ", uConsoleMgr::INFO);
				uConsoleMgr::echo(minDist, uConsoleMgr::SUCCESS);
				uConsoleMgr::echo(", way:\nSTART -> ", uConsoleMgr::INFO);
				int actVertex = endVertex;
				std::deque<int> way;
				way.push_front(actVertex);
				while (true) {
					if (prevVertex.find(actVertex) != prevVertex.end()) {
						way.push_front(prevVertex[actVertex]);
						actVertex = prevVertex[actVertex];
					}
					else break;
				}
				for (int q : way) {
					uConsoleMgr::echo(q, uConsoleMgr::CUTE);
					uConsoleMgr::echo(" -> ", uConsoleMgr::INFO);
				}
				uConsoleMgr::echo("END \n", uConsoleMgr::INFO);
			}

			

		}
};
