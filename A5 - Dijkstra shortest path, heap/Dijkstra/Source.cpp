#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <Windows.h>
#include "Graph.h"


using std::vector;
using std::stringstream;
using std::string;
using std::cin;
using std::cout;



int main(int argc, char* argv[]) {
	if (!freopen("dijkstraInput.txt", "r", stdin)) {
		cout << "Problem z plikiem" << endl;
		return -1;
	}

	int vertCount;
	cin >> vertCount;

	Graph g;

	vector<int> inputVerts;
	for (int i = 1; i <= vertCount; i++) {
		inputVerts.push_back(i);
	}
	g.batchAddVertex(inputVerts);
	cout << vertCount << " verts added" << endl;
	
	int from, to, len;
	string s;
	while (getline(cin, s)) {
		std::replace(s.begin(), s.end(), ',', ' ');
		stringstream ss;
		ss.str(s);
		ss >> from;
		while (ss >> to && ss >> len)
			g.addEdge(from, to, len);
	}
	
	//g.printMe();

	g.calculateDistancesUsingDijkstra(1);
	g.printDistances();



}