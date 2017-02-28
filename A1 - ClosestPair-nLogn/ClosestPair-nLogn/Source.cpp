#include<iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <assert.h>
#include <random>
#include<Windows.h>

using namespace std;

typedef struct{
	int x, y;
}point;

ostream& operator << (ostream& out, point& p) {
	out << p.x << "\t"<< p.y;
	return out;
}

ostream& operator << (ostream& out, vector<point>& v) {
	for (auto p : v)
		out << p.x << "\t" << p.y << endl;
	return out;
}

double dist(point& p1, point& p2) {
	return sqrt( pow(p1.x-p2.x, 2) + pow(p1.y-p2.y, 2) );
}

double dist(pair<point,point>& p) {
	return dist(p.first, p.second);
}

bool compX(point& p1, point& p2) {
	return p1.x < p2.x;
}

bool compY(point& p1, point& p2) {
	return p1.y < p2.y;
}

pair<point, point> closestPointInBetween(vector<point> & Sx, vector<point> & Sy, double minDist, bool & foundBetter) {
	int middleX = Sx[Sx.size() / 2].x;

	vector<point> filteredSy;
	copy_if(Sy.begin(), Sy.end(), back_inserter(filteredSy), [middleX, minDist](point p) {return (middleX - minDist) <= p.x && p.x <= (middleX + minDist); });


	//cout << "Min dist " << minDist << endl;
	//cout << "Sx" << endl << Sx << endl<<"Sy"<<endl<<Sy<<"Filtered"<<endl<< filteredSy << endl;


	pair<point, point> bestPair;

	for (unsigned int i = 0; i < filteredSy.size(); i++) {
		for (unsigned int j = 1; j < 8 && i + j < filteredSy.size(); j++) {
			pair<point, point> temp = make_pair(filteredSy[i], filteredSy[i + j]);
			if (dist(temp) < minDist){
				minDist = dist(temp);
				bestPair = temp;
				foundBetter = true;
			}

		}
	}
	return bestPair;
	
}


pair<point, point> closestPair(vector<point> & Sx, vector<point> & Sy) {
	if (Sx.size() == 2)//currently limited to sizez 2^n
		return make_pair(Sx[0],Sx[1]);

	int numElem = Sx.size();
	vector<point> SxL(Sx.begin(), Sx.begin() + numElem / 2);
	vector<point> SxR(Sx.begin() + numElem / 2, Sx.end());
	
	//this part should not be implemented as sorting but rather selection from Sy to SyL/SyR based on presence in SxL or SxR
	vector<point> SyL(SxL);
	sort(SyL.begin(), SyL.end(), compY);
	
	vector<point> SyR(SxR);
	sort(SyR.begin(), SyR.end(), compY);



	auto leftPair = closestPair(SxL, SyL);
	auto rightPair = closestPair(SxR, SyR);
	double lDist = dist(leftPair);
	double rDist = dist(rightPair);

	bool foundBetter = false;
	auto centerPair = closestPointInBetween(Sx, Sy, min(lDist, rDist), foundBetter);
	
	if (foundBetter)
		return centerPair;

	return lDist < rDist ? leftPair : rightPair;

}

pair<point, point> computeClosestPoints(vector<point> & input) {
	vector<point> inputX = input;
	sort(inputX.begin(), inputX.end(), compX);

	vector<point> inputY = input;
	sort(inputY.begin(), inputY.end(), compY);
	
	return closestPair(inputX, inputY);
}

bool equals(double val1, double val2, double tolerance) {
	return abs(val1 - val2) <= tolerance;
}

vector<point> test() {
	vector<point> points = {
		{0,0},
		{1,14},
		{2,0},
		{3, 8},
		{3, 18},
		{5,0},
		{5,7},
		{5,12},
		{5,15},
		{7,1},
		{7,9},
		{7, 14},
		{9,6},
		{9,13},
		{11,6},
		{11,13}
	};

	assert(	equals(dist(points[0], points[1]), sqrt(1+14*14), 0.01) );
	return points;
}

pair<point, point> naiveClosestPoint(vector<point> & points) {
	double bestDist = 1e300;
	pair<point, point> best;
	for (unsigned int i = 0; i < points.size(); i++){
		for (unsigned int j = 1; i + j < points.size(); j++){
			if (dist(points[i], points[i+j]) < bestDist) {
				best = make_pair(points[i], points[i+j]);
				bestDist = dist(points[i], points[i+j]);
			}
		}
	}

	return best;
}

void testSpeed() {
	std::random_device rd;     // only used once to initialise (seed) engine
	std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
	std::uniform_int_distribution<int> uni(-100000, 100000); // guaranteed unbiased
	cout << "Size\ttime" << endl;
	for (int i = 2; i < 20; i++) {
		vector<point> testInput;
		int points = pow(2, i);
		for (int i = 0; i < points; i++) {
			point p = { uni(rng), uni(rng) };
			testInput.push_back(p);
		}
		long int before = GetTickCount();
		pair<point, point> p = computeClosestPoints(testInput);
		long int after = GetTickCount();

		long int beforeSq = GetTickCount();
		p = naiveClosestPoint(testInput);
		long int afterSq = GetTickCount();

		cout << points << "\t" << after - before <<"\t" << afterSq-beforeSq<<  endl;


	}
}


int main() {
	testSpeed();


	//
	//
	//cout << "nlogn: " << endl << p.first << endl << p.second << endl << "Dist: " << dist(p) << endl;

	//p = naiveClosestPoint(testInput);
	//cout <<"n^2:" << endl << p.first << endl << p.second << endl<< "Dist: " << dist(p) << endl;
}

