#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <Windows.h>

using namespace std;

ostream& operator<<(ostream& o, vector<int>& v) {
	for (int i : v)
		cout << i << " ";
	return o;
}


vector<int> findMedians(vector<vector<int>> inputGroups) {
	vector<int> output;
	for (vector<int> & oneGroup : inputGroups) {
		//cout << "Group" << endl;
		//cout << oneGroup << endl;
		sort(oneGroup.begin(), oneGroup.end());
		output.push_back( oneGroup[oneGroup.size()/2] );
	}
	return output;
}

int nThStatistic(const vector<int>::iterator begin, const vector<int>::iterator end, int order) {
	//for (auto i = begin; (i - end) != 0; i++)
	//	cout << *i << " ";
	//cout << endl;
	int size = end - begin;
	if (size == 1)
		return *begin;
	int numGroups = size / 5;
	vector<vector<int>> groups;
	for (int i = 0; i < numGroups; i++) {
		groups.push_back(vector<int>(begin + i*5, begin + (i+1)*5));
	}
	if (size % 5 != 0) {
		groups.push_back(vector<int>(end - size % 5, end));
	}

	vector<int> medians = findMedians(groups);
	int pivot = nThStatistic(medians.begin(), medians.end(), medians.size() / 2);	

	int j = 0;
	bool pivotFound = false;
	for (int i = 0; i < size; i++) {
		int val = *(begin + i);
		if (val < pivot) {
			swap(*(begin + i), *(begin + j));
			j++;
		}

		if (val == pivot && !pivotFound) {
			swap(*begin, *(begin + i));
			i--;
			pivotFound = true;
		}
			
	}
	j--;
	swap(*(begin), *(begin + j));
	//cout << "After pivoting " << pivot << " j " << j << endl;
	//for (auto i = begin; (i - end) != 0; i++)
	//	cout << *i << " ";
	//cout << endl << endl;

	if (j == size - order)//if pivot was a statistic
		return *(begin + j);
	if (j < size - order)//look in right side
		return nThStatistic(begin + j + 1, end, order);
	if (j > size - order)//look in left side
			return nThStatistic(begin, begin + j, order-(size-j));
	return -1;
}

int main() {
	vector<int> input;
	for (int i = 0; i < 1000000; i++)
		input.push_back(i);
	random_shuffle(input.begin(), input.end());
	
	long long t1 = GetTickCount();
	cout << "Search started" << endl;
	cout << nThStatistic(input.begin(), input.end(), 10)<< endl;
	long long t2 = GetTickCount();
	cout << "Time " << t2 - t1 << endl;

	return 0;
}