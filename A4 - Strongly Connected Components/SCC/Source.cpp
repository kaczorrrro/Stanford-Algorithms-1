#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <random>
#include <set>
#include <unordered_set>
#include <algorithm>
#include <ctime>
#include <fstream>
#include <stack>
#include <Windows.h>

using namespace std;

class Edge;


class Vertex {
public:
	int id;
	int masterId = -1;
	int timeFinished = -1;
	bool visited = false;
	vector<Edge*> inputs, outputs;
};

class Edge {
public:
	Vertex *from, *to;
};

//bool edgeConnectsVertex(Edge * e, Vertex * v) {
//	return e->v1 == v || e->v2 == v;
//}


class Graph {
public:
	void addVertex(int i) {
		batchAddVertex(vector<int> { i });
	}

	void batchAddVertex(const vector<int> & ids) {
		int size = ids.size();
		if (size <= 0)
			return;
		Vertex* verts = new Vertex[size];
		memoryPointersToVerts.push_back(verts);

		for (int i = 0; i < size; i++) {
			Vertex* v = verts + i;
			v->id = ids[i];
			vertices.push_back(v);
		}
		numVertices += size;
	}

	void addEdge(int fromId, int toId) {
		Vertex * from = vertices[fromId - 1];
		Vertex * to = vertices[toId - 1];
		Edge * e = new Edge{ from, to };
		memoryPointersToEdges.insert(e);
		from->outputs.push_back(e);
		to->inputs.push_back(e);
	}

	void batchAddEdge(vector<pair<int, int>> fromToPairs) {
		int size = fromToPairs.size();
		if (size == 0)
			return;

		Edge * edgeObjects = new Edge[size];
		for (int i = 0; i < size; i++) {
			Vertex * from = vertices[fromToPairs[i].first - 1];
			Vertex * to = vertices[fromToPairs[i].second - 1];
			Edge* e = edgeObjects + i;
			e->from = from;
			e->to = to;
			from->outputs.push_back(e);
			to->inputs.push_back(e);
		}
		memoryPointersToEdges.insert(edgeObjects);
	}

	void printMe() {
		for (Vertex* v : vertices) {
			cout << "Vertex " << v->id << "\tFinished time:" << v->timeFinished <<  endl;
			cout << "Inputs: ";
			for (Edge* e : v->inputs)
				cout << e->from->id << " ";
			cout << endl;

			cout << "Outputs: ";
			for (Edge* e : v->outputs)
				cout << e->to->id << " ";
			cout << endl << endl;
		}
	}

	Graph() {};

	~Graph() {
		for (Edge* e : memoryPointersToEdges) delete e;
		for (Vertex* v : memoryPointersToVerts) delete [] v;
	}

	void DFS_loop(bool reversed) {
		if (!reversed && !sortedByTime) {
			cout << "First sort by time!" << endl;
			return;
		}
		for (Vertex* v: vertices){
			if (v->visited)continue;
			int groupSize = 0;
			DFS(v, reversed, groupSize);
			if (!reversed) { //put in memory only group sizes from second (non reversed) run 
				groupSizes.push_back(groupSize);
			}
		}
	}

	void unvisitAll() {
		for (Vertex *v : vertices) {
			v->visited = false;
		}
	}

	void sortVerticesByFinishingTime() {
		sort(vertices.begin(), vertices.end(), [](Vertex* v1, Vertex* v2) {return v1->timeFinished > v2->timeFinished; });
		sortedByTime = true;
		//this should also block possibility of adding new edges/vertices
	}

	vector<int> getGroupSizes() {
		vector<int> sizes = groupSizes;
		sort(sizes.begin(), sizes.end(), [](int size1, int size2) {return size1 > size2; });
		return sizes;
	}


private:
	vector<Vertex*> vertices;
	vector<Vertex*> memoryPointersToVerts;
	unordered_set<Edge*> memoryPointersToEdges;
	vector<int> groupSizes;
	int numVertices = 0;
	int currentTimeFinished = 0;
	bool sortedByTime = false;

	void DFS(Vertex* start, bool reversed, int & groupSize) {
		stack<Vertex*> stackedVerts;
		stackedVerts.push(start);

		while (!stackedVerts.empty()) {
			Vertex* vert = stackedVerts.top();
			stackedVerts.pop();
			//check here is only necessary in verion with stack, and not recursive calls
			if (vert->visited) {
				vert->timeFinished = currentTimeFinished++;
				continue;
			}

			//put on stack for postProcessing after all nodes visible from here will be processed
			stackedVerts.push(vert);

			vert->visited = true;
			

			vector<Edge*> & outputs = reversed ? vert->inputs : vert->outputs;

			for (Edge *e : outputs) {
				Vertex *v = reversed ? e->from : e->to;
				if (!v->visited)
					stackedVerts.push(v);
			}
			
			++groupSize;
		}
	}
};


int main(int argc, char* argv[]) {
	int vertCount = 875714;
	if (argc < 2) {
		cout << "Assuming input size " << vertCount << endl;
	}
	else{
		vertCount = atoi(argv[1]);
	}


	Graph g;

	//std::ios_base::sync_with_stdio(false);

	if (!freopen("trueInput.txt", "r", stdin)) {
		cout << "Problem z plikiem" << endl;
		return -1;
	}

	
	vector<int> inputVerts;
	for (int i = 1; i <= vertCount; i++) {
		inputVerts.push_back(i);
	}
	g.batchAddVertex(inputVerts);
	cout << "Verts added" << endl;
	

	long t1 = GetTickCount();
	vector<pair<int, int>> inputEdges;
	int from, to;
	while (cin >> from && cin >> to) {
		inputEdges.push_back(make_pair(from, to));
		//g.addEdge(from, to); 3 seconds slower (33%)
	}
	cout << "Edges read" << endl;
	g.batchAddEdge(inputEdges);
	cout << "Edges added" << endl;
	long t2 = GetTickCount();
	cout << "Time " << t2 - t1 << endl;


	g.DFS_loop(true);
	cout << "First loop finished" << endl;
	g.unvisitAll();
	//g.printMe();
	g.sortVerticesByFinishingTime();
	cout << "Sorted" << endl;
	//g.printMe();
	g.DFS_loop(false);

	auto sizes = g.getGroupSizes();
	cout << endl;
	for (int i = 0; i < 5; i++)
		cout << sizes[i] << endl;



}
