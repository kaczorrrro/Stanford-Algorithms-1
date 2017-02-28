#include <iostream>
#include <vector>
#include<ctime>
#include <fstream>
#include <algorithm>

using namespace std;

int medianIndex(vector<int> & input, int indA, int indB, int indC) {
	vector<int> nums = { indA, indB, indC };
	sort(nums.begin(), nums.end(), [&input](int a, int b) { return input[a] > input[b]; });
	return nums[1];
}

int putPivotOnZero(vector<int> & input, int start, int end) {
	//version 1:
	//do nothing

	//version 2:
	//swap(input[start], input[end]);

	//version 3:
	swap(input[start], input[medianIndex(input, start, end, (end + start) / 2)]);
	
	return input[start];
}

long long sum = 0;

//both incluse
void quicksort(vector<int>& input, int begin, int end) {
	if (end <= begin)
		return;

	int pivot = putPivotOnZero(input, begin, end);
	
	int j = begin + 1;
	for (int i = begin+1; i <= end; i++) {
		if (input[i] < pivot) {
			swap(input[i], input[j]);
			++j;
		}
	}
	--j;

	swap(input[begin], input[j]);

	sum += end - begin;

	quicksort(input, begin, j-1);
	quicksort(input, j + 1, end);

}



int main() {

	vector<int> input;// = { 8, 2, 4, 5, 7, 1 };

	ifstream file;
	file.open("input.txt");

	int temp;
	while (file >> temp)
		input.push_back(temp);

	quicksort(input, 0, input.size()-1);

	cout << "Sum " << endl << sum << endl;

}