#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include<ctime>
using namespace std;



vector<int> countInversions(vector<int> v, long long &invSum) {
	if (v.size() == 1)
		return v;

	int totalElements = v.size();
	int middle = totalElements / 2;

	vector<int> left = countInversions(vector<int>(v.begin(), v.begin() + middle), invSum);
	vector<int> right = countInversions(vector<int>(v.begin() + middle, v.end()), invSum);

	int lElemLeft = left.size();
	int rElemLeft = right.size();

	for (int i = 0, lPos = 0, rPos = 0; i < totalElements; i++) {
		//no elems left in one array
		if (rElemLeft == 0) {
			v[i] = left[lPos++];
		}
		else if (lElemLeft == 0) {
			v[i] = right[rPos++];
		}
		//some elements in both
		else if (left[lPos] < right[rPos]) {
			v[i] = left[lPos++];
			lElemLeft--;
		}
		else {
			v[i] = right[rPos++];
			rElemLeft--;
			invSum += (long long)lElemLeft;
		}
	}

	return v;
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "Usage: Counting Inversions Filename" << endl;
		return 1;
	}
	srand(time(NULL));
	vector<int> input;


	ifstream file;
	file.open(argv[1]);
	int temp;
	while (file >> temp) {
		input.push_back(temp);
	}
	cout << input.size() << endl;

	//for (int i : input)
	//	cout << i << endl;
	//for (int i = 0; i < 10000; i++)
	//	input.push_back(10000-i);



	//int sumTemp = 0;
	//int size = input.size();
	//for (int i = 0; i < size; i++) {
	//	if (i % 1000 == 0)
	//		cout << i << endl;
	//	for (int j = i+1; j < size; j++) {
	//		if (input[i] > input[j])
	//			sumTemp++;
	//	}
	//}
	//cout << sumTemp << endl;

	long long sum = 0;
	cout << "start" << endl;
	vector<int> output = countInversions(input,sum);
	//for (int i = 1; i < 10000; i++)
	//	if (output[i] < output[i - 1])
	//		cout << "zjebae" << endl;

	cout << sum << endl;
	
}



