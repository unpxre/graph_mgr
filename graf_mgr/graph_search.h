

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

	public:
		ColorVertex(int previous = NULL, color c = WHITE) {
			this->c = c;
			this->distance = -1;
			this->previous = previous;
		}

		void setColor(color c) {
			this->c = c;
		}

		color getColor() {
			return this->c;
		}

		void setDistance(int distance) {
			this->distance = distance;
		}

		void setPrevious(int previous) {
			this->previous = previous;
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
TODO: nie liczysz czasu a nie wiesz czy trzeba i wtf jest ten czas? i po co petle w liniach 4,5,6 w pdf?
*/
class DFS {
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

		void visit(int &x) {
			vertexes[x].setColor(ColorVertex::GREY);
			uConsoleMgr::echo(x, uConsoleMgr::NORMAL);
			uConsoleMgr::echo(" ");
			for (std::set<int>::iterator it = vertexesNeighbor[x].begin(); it != vertexesNeighbor[x].end(); ++it) {
				if ((vertexes[*it].getColor() == ColorVertex::BLACK) || (vertexes[*it].getColor() == ColorVertex::GREY)) continue;
				visit((int)*it);
			}
			vertexes[x].setColor(ColorVertex::BLACK);
		}

		void process() {
			uConsoleMgr::echo("\nProcessing search(DFS)\n", uConsoleMgr::CUTE);
			visit(initVertex);
			uConsoleMgr::echo("\nProcessed successfully\n\n", uConsoleMgr::SUCCESS);
		}

	public:
		DFS(std::map<int, std::set<int>> graph, int initVertex) {
			this->vertexesNeighbor = graph;
			this->colourVertexes();
			this->initVertex = initVertex;		

			this->process();
		}
};

