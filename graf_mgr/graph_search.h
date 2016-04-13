

/*
Kolorowy wierzcho³ek u¿ywany do wyszukiwania wg³¹b i wszerz
*/
class ColorVertex {
	public:
		enum color {
			WHITE = 1,
			GREY = 2,
			BLACK = 3
		};

	private:
		color c;
		int distance; // -1 is oo
		int previous;
		int processTime;

	public:
		ColorVertex(int previous = NULL, color c = WHITE) {
			this->c = c;
			this->distance = -1;
			this->previous = previous;
			this->processTime = 0;
		}

		void setColor(color c) {
			this->c = c;
		}

		color getColor() {
			return this->c;
		}

		int getProcessTime() {
			return this->processTime;
		}

		void setDistance(int distance) {
			this->distance = distance;
		}

		void setPrevious(int previous) {
			this->previous = previous;
		}

		void setProcessTime(int processTime) {
			this->processTime = processTime;
		}
};

/*
Przeszukianie wszerz
*/
class BFS {
	private:
		typedef std::map<int, std::set<int>>::iterator graphIt;
		std::map<int, std::set<int>> vertexesNeighbor;
		std::map<int, ColorVertex> vertexes;
		int initVertex;

		void colourVertexes() {
			for (graphIt iterator = vertexesNeighbor.begin(); iterator != vertexesNeighbor.end(); ++iterator) {
				vertexes.insert(std::pair<int, ColorVertex>(iterator->first, ColorVertex()));
			}
		}

		void process() {
			uConsoleMgr::echo("\nProcessing search(BFS)\n", uConsoleMgr::CUTE);
			uConsoleMgr::echo(initVertex, uConsoleMgr::NORMAL);
			uConsoleMgr::echo(" ", uConsoleMgr::INFO);
			//std::vector<int> addOrder;
			std::queue<int> greyVertexes;
			greyVertexes.push(initVertex);
			int previous, distance = 0;
			while (greyVertexes.size() > 0) {
				uConsoleMgr::echo("| ", uConsoleMgr::INFO);
				for (int j = 0, maxJ = greyVertexes.size(); j < maxJ; ++j) { // pêtla for ¿eby dystans poprawnie siê generowa³...
					int i = greyVertexes.front();
					greyVertexes.pop();
					previous = i;
					vertexes[i].setColor(ColorVertex::BLACK);
					for (std::set<int>::iterator it = vertexesNeighbor[i].begin(); it != vertexesNeighbor[i].end(); ++it) {
						if ( (vertexes[*it].getColor() == ColorVertex::BLACK) || (vertexes[*it].getColor() == ColorVertex::GREY) ) continue;
						uConsoleMgr::echo(*it, uConsoleMgr::NORMAL);
						uConsoleMgr::echo(" ", uConsoleMgr::INFO);
						//addOrder.push_back(*it);
						vertexes[*it].setColor(ColorVertex::GREY);
						vertexes[*it].setDistance(distance);
						vertexes[*it].setPrevious(previous);
						greyVertexes.push(*it);
					}
				}
				++distance;
			}
			uConsoleMgr::echo("\nProcessed successfully\n\n", uConsoleMgr::SUCCESS);
		}

	public:
		BFS(std::map<int, std::set<int>> graph, int initVertex) {
			this->vertexesNeighbor = graph;
			this->colourVertexes();
			this->initVertex = initVertex;
			vertexes[this->initVertex].setColor(ColorVertex::GREY);
			vertexes[this->initVertex].setDistance(0);


			this->process();
		}
};

/*
Przeszukianie wg³¹b
*/
class DFS {
	private:
		typedef std::map<int, std::set<int>>::iterator graphIt;
		typedef std::map<int, ColorVertex>::iterator vertxIt;
		typedef std::list<int>::iterator topSortIt;
		std::map<int, std::set<int>> vertexesNeighbor;
		std::map<int, ColorVertex> vertexes;
		std::map<int, ColorVertex> vertexesForSCCProcess;
		std::list<int> topologicalSorted;
		std::set<int> SCC;
		int currentProcessTime;

		void colourVertexes(std::map<int, ColorVertex> &v) {
			for (graphIt iterator = vertexesNeighbor.begin(); iterator != vertexesNeighbor.end(); ++iterator) {
				v.insert(std::pair<int, ColorVertex>(iterator->first, ColorVertex()));
			}
		}

		void visit(int &x) {
			vertexes[x].setColor(ColorVertex::GREY);
			uConsoleMgr::echo(x, uConsoleMgr::NORMAL);
			uConsoleMgr::echo(" ");
			for (std::set<int>::iterator it = vertexesNeighbor[x].begin(); it != vertexesNeighbor[x].end(); ++it) {
				if ((vertexes[*it].getColor() == ColorVertex::BLACK) || (vertexes[*it].getColor() == ColorVertex::GREY)) continue;
				visit((int)*it);
			}
			topologicalSorted.push_front(x);
			vertexes[x].setColor(ColorVertex::BLACK);
			vertexes[x].setProcessTime(currentProcessTime);
			++this->currentProcessTime;
		}

		void reverseGraphVisit(int x) {
			//std::cout << x << "\n $ ";
			for (graphIt iterator = vertexesNeighbor.begin(); iterator != vertexesNeighbor.end(); ++iterator) {
				for (std::set<int>::iterator it = iterator->second.begin(); it != iterator->second.end(); ++it) {
					if ((*it == x) && (vertexesForSCCProcess[iterator->first].getColor() == ColorVertex::WHITE)) {
						//std::cout << iterator->first << ", ";
						SCC.insert(iterator->first);
						vertexesForSCCProcess[iterator->first].setColor(ColorVertex::GREY);
						reverseGraphVisit(iterator->first);
					}
				}
			}
		}

		void process() {
			uConsoleMgr::echo("\nProcessing search(DFS)\n", uConsoleMgr::CUTE);
			uConsoleMgr::echo("DFS way: ", uConsoleMgr::INFO);
			for (vertxIt it = vertexes.begin(); it != vertexes.end(); ++it) {
				if (it->second.getColor() == ColorVertex::WHITE) visit((int)it->first);
			}
			uConsoleMgr::echo("\nProcessed successfully", uConsoleMgr::SUCCESS);
			uConsoleMgr::echo("\nTopological sort: ", uConsoleMgr::INFO);
			for (topSortIt it = topologicalSorted.begin(); it != topologicalSorted.end(); ++it) {
				uConsoleMgr::echo(*it, uConsoleMgr::NORMAL);
				uConsoleMgr::echo(" ");
			}
			uConsoleMgr::echo("\nProcess Time: ", uConsoleMgr::INFO);
			for (vertxIt it = vertexes.begin(); it != vertexes.end(); ++it) {
				uConsoleMgr::echo(it->first, uConsoleMgr::NORMAL);
				uConsoleMgr::echo(" (");
				uConsoleMgr::echo(it->second.getProcessTime(), uConsoleMgr::NORMAL);
				uConsoleMgr::echo(") ");
			}
			uConsoleMgr::echo("\n\n");
		}

	public:
		DFS(std::map<int, std::set<int>> graph) {
			this->vertexesNeighbor = graph;
			this->colourVertexes(this->vertexes);

			this->currentProcessTime = 1;
			this->process();
		}

		/*
			WYgeneruj silnie spojne sk³adowe
			http://www.algorytm.org/algorytmy-grafowe/silnie-spojne-skladowe.html
		*/
		void getSCC() {
			uConsoleMgr::echo("\nProcessing Strongly-Connected-Components\n", uConsoleMgr::CUTE);
			int actProcessTime = vertexes.size();
			this->colourVertexes(this->vertexesForSCCProcess);
			for (vertxIt it = vertexes.begin(); it != vertexes.end(); ++it) {
				for (vertxIt itD = vertexes.begin(); itD != vertexes.end(); ++itD) {
					if (itD->second.getProcessTime() == actProcessTime) {
						--actProcessTime;
						if (vertexesForSCCProcess[itD->first].getColor() == ColorVertex::WHITE) {
							uConsoleMgr::echo("\n - - -\n");
							SCC.clear();
							SCC.insert(itD->first);
							vertexesForSCCProcess[itD->first].setColor(ColorVertex::GREY);
							this->reverseGraphVisit(itD->first);
							for (std::set<int>::iterator i = SCC.begin(); i != SCC.end(); i++) {
								uConsoleMgr::echo(*i, uConsoleMgr::SUCCESS);
								uConsoleMgr::echo(", ", uConsoleMgr::SUCCESS);
							}
						}
					}
				}
			}
			uConsoleMgr::echo("\n\n");

		}
};

