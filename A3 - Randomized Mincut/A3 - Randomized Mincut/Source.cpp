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

using namespace std;

class Edge;


class Vertex{
public:
	int id;
	vector<Edge*> edges;
};

class Edge {
public:
	Vertex *v1, *v2;
};

bool edgeConnectsVertex(Edge * e, Vertex * v) {
	return e->v1 == v || e->v2 == v;
}


class Graph {
public:
	void addVertex(int i) {
		Vertex* v = new Vertex();
		v->id = i;
		vertices.push_back(v);
		++numVertices;
	}

	void addEdge(int id1, int id2) {
		if (alreadyConnected(id1,id2))
			return;
		Vertex * v1 = vertices[id1 - 1];
		Vertex * v2 = vertices[id2 - 1];
		Edge * e = new Edge{ v1, v2 };
		edges.insert(e);
		v1->edges.push_back(e);
		v2->edges.push_back(e);
	}

	int dropOneEdge() {
		//one edge means two vertices
		if (numVertices <= 2)
			return numVertices;

		unordered_set<Edge*>::iterator edgesStart = edges.begin();
		advance(edgesStart, rand() % edges.size());
		Edge * eToRemove = *edgesStart;
		Vertex * vertex1 = eToRemove->v1;
		Vertex * vertex2 = eToRemove->v2;
	
		/*unordered_set<Edge*> newEdges;
		newEdges.insert(vertex1->edges.begin(), vertex1->edges.end());
		newEdges.insert(vertex2->edges.begin(), vertex2->edges.end());*/

		vector<Edge*> & newEdges = vertex1->edges;
		newEdges.insert(newEdges.end(), vertex2->edges.begin(), vertex2->edges.end());
		

		//to chyba nie dziala
		for (vector<Edge*>::iterator itr = newEdges.begin(); itr != newEdges.end();){
			Edge* e = *itr;

			if (e->v1 == vertex2) e->v1 = vertex1;
			if (e->v2 == vertex2) e->v2 = vertex1;

			if (e->v1 == vertex1 && e->v2 == vertex1) {
				itr = newEdges.erase(itr);
				edges.erase(e);
				uselessEdges.insert(e);
			}
			else{
				++itr;
			}
		}


		return --numVertices;
	}

	void printMe() {
		set<Vertex*> seen;
		for (Edge * e : edges) {
			Vertex* verts[2] = {e->v1, e->v2};
			for (Vertex* v : verts) {
				if (seen.count(v) == 1) continue; else seen.insert(v);
				cout << v->id << endl;
				for (Edge * e : v->edges)
					cout << "(" << e->v1->id << " " << e->v2->id << ") ";
				cout << endl << endl;
			}
		}
	}

	Graph() {};

	Graph(const Graph& gSource) {
		//numVertices = gSource.vertices.size();
		//int numEdges = gSource.edges.size();

		//Vertex * verts = new Vertex [numVertices];
		//Edge * localEdges = new Edge [numEdges];
		//
		//for (int i=0; i<numVertices;i++){
		//	verts[i].id = gSource.vertices[i] -> id;
		//	vertices.push_back(&verts[i]);
		//	
		//}

		//for (int i = 0; i < numEdges; i++) {
		//	Edge source = *gSource.edges[i];
		//	Edge* dest = &localEdges[i];

		//	dest->v1 = vertices[source.v1->id-1];
		//	dest->v2 = vertices[source.v2->id-1];
		//	edges.push_back(dest);
		//	dest->v1->edges.push_back(dest);
		//	dest->v2->edges.push_back(dest);
		//}

		for (Vertex* v : gSource.vertices)
			addVertex(v->id);

		for (Edge* e : gSource.edges) {
			addEdge(e->v1->id, e->v2->id);
		}

	}
	~Graph() {
		for (Edge* e : edges) delete e;
		for (Edge* e : uselessEdges) delete e;
		for (Vertex* v : vertices) delete v;

	}

	int edgesLeft() {
		return edges.size();
	}

private:
	vector<Vertex*> vertices;
	unordered_set<Edge*> edges;
	unordered_set<Edge*> uselessEdges;
	int numVertices = 0;

	bool alreadyConnected(int id1, int id2) {
		Vertex * v1 = vertices[id1 - 1];
		for (Edge * e : v1->edges)
			if (e->v1->id == id2 || e->v2->id == id2)
				return true;
		return false;
	}
};

int main() {	
	srand(time(NULL));
	Graph g;
	freopen("..\\Debug\\inputP.txt", "r", stdin);
	
	int vertCount = 200;
	for (int i = 0; i < vertCount; i++) {
		g.addVertex(i+1);
	}
	string s;
	while (getline(cin, s)) {
		stringstream ss;
		ss.str(s);
		
		int vertexId;
		ss >> vertexId;
	
		int adjacent;
		while (ss >> adjacent) {
			g.addEdge(vertexId, adjacent);
			
		}
	}
	

	int minEdges = (int)1e9;


	for (int i = 0, j = vertCount *vertCount *log(vertCount); i < j; i++) {
		Graph gCopy = g;
		if (i%100 == 0)
			cout << i << " " << i / double(j) * 100 << endl;
		while (gCopy.dropOneEdge() > 2);
		//empty
		if (gCopy.edgesLeft() < minEdges)
			minEdges = gCopy.edgesLeft();
	}
	cout << minEdges << endl;

	

}
////to chyba nie dziala
//vertex1->edges.erase(remove_if(vertex1->edges.begin(), vertex1->edges.end(), [vertex1, this](Edge * e) {
//	if (e->v1 == vertex1 && e->v2 == vertex1) {
//		//this is an edge from a to a, so we delete it from vertex(returning true) and from global list of edges
//		this->edges.erase(remove(this->edges.begin(), this->edges.end(), e), this->edges.end());
//		this->uselessEdges.push_back(e);
//		return true;
//	}
//	return false;
//}), vertex1->edges.end());