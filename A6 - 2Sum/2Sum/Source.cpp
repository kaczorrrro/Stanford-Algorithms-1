#include <iostream>
#include <string>
#include <vector>
#include <unordered_set>
#include <algorithm>

template<class T>
class HashSet {
private:
	struct Entry {
		Entry* next;
		T element;
	};
	static const int initialSize = 97;

public:
	HashSet() {
		currentSize = initialSize;
		totalElements = 0;
		initializeStorage(currentSize);
	};

	void add(T i) {
		totalElements++;

		if (totalElements / (double)currentSize >= 0.8)
			resize();

		int bucketNo = hash(i);
		if (storage[bucketNo] == nullptr) {
			storage[bucketNo] = new Entry{ nullptr, i };
		}
		else {
			Entry* newEntry = new Entry{storage[bucketNo],i};
			storage[bucketNo] = newEntry;
		}
	}

	bool contains(T i) {
		int bucketNo = hash(i);
		Entry* e = storage[bucketNo];
		while(e != nullptr) {
			if (e->element == i)
				return true;
			e = e->next;
		}
		return false;
	}

	void printAll() {
		for (int i = 0; i < currentSize; i++) {
			Entry* e = storage[i];
			for (int j = 0; e != nullptr; j++) {
				std::cout << "Bucket " << i << " position " << j << " value: " << e->element << std::endl;
				e = e->next;
			}
		}
	}

private:
	Entry** storage;
	int currentSize;
	int totalElements;

	void initializeStorage(int elements) {
		storage = (Entry**)calloc(elements, sizeof(Entry*));
	}
	
	inline int hash(T i) {
		int output = (i >= 0) ? i%currentSize : (-i) % currentSize;
		if (output < 0) {
			std::cout << "Hash " << i << " is <0" << std::endl;
			return 0;
		}
		else
			return output;
	}

	void resize() {
		Entry** oldStorage = storage;
		int oldSize = currentSize;

		currentSize = nextSize();
		initializeStorage(currentSize);
		std::cout << "Resize, new size: " << currentSize << std::endl;
		totalElements = 0;
		for (int i = 0; i < oldSize; i++) {
			Entry* e = oldStorage[i];
			while (e != nullptr) {
				add(e->element);
				Entry* eCopy = e;
				e = e->next;
				delete eCopy;
			}
		}
		free(oldStorage);
	}

	int nextSize() {
		int newSize = currentSize*2;
		while (!isPrime(newSize))
			++newSize;
		return newSize;
	}

	bool isPrime(int n) {
		int sqrtN = sqrt(n);
		for (int i = 2; i <= sqrtN; i++)
			if (n%i == 0)
				return false;
		return true;
	}


};

//My implementation
int main() {
	using namespace std;
	HashSet<long long> hashSet;
	vector<long long> ints;

	if (!freopen("2sumInput.txt", "r", stdin)) {
		cout << "Problem z plikiem" << endl;
		return -1;
	}
	
	long long temp;
	while (cin >> temp) {
		hashSet.add(temp);
		ints.push_back(temp);
	}
	cout << "Finished adding" << endl;
	cout << ints.size() << endl;

	long long minTarget = -10000;
	long long maxTarget = 10000;

	int sumsFound = 0;
	for (long long target = minTarget; target <= maxTarget; target++) {
		if (target % 10 == 0)
			cout << (target-minTarget)*100.0 / (maxTarget - minTarget) << "%" << endl;

		for (long long x : ints) {
			if (x != (target - x) && hashSet.contains(target - x)) {
				sumsFound++;
				break;
			}
		}
	}

	cout << "Sums " << sumsFound << endl;
	


	
	
	return 0;
}


//unordered_set option

//int main() {
//	using namespace std;
//	unordered_set<long long> hashSet;
//	vector<long long> ints;
//
//	if (!freopen("2sumInput.txt", "r", stdin)) {
//		cout << "Problem z plikiem" << endl;
//		return -1;
//	}
//
//	long long temp;
//	while (cin >> temp) {
//		hashSet.insert(temp);
//		ints.push_back(temp);
//	}
//	cout << "Finished adding" << endl;
//	cout << ints.size() << endl;
//
//	long long minTarget = -10000;
//	long long maxTarget = 10000;
//
//	int sumsFound = 0;
//	for (long long target = minTarget; target <= maxTarget; target++) {
//		if (target % 10 == 0)
//			cout << (target - minTarget)*100.0 / (maxTarget - minTarget) << "%" << endl;
//
//		for (long long x : ints) {
//			if (x != (target - x) && hashSet.find(target - x) != hashSet.end()) {
//				sumsFound++;
//				break;
//			}
//		}
//	}
//
//	cout << "Sums " << sumsFound << endl;
//
//	return 0;
//}


//BINARY SEARCH

//int main() {
//	using namespace std;
//	vector<long long> ints;
//
//	if (!freopen("2sumInput.txt", "r", stdin)) {
//		cout << "Problem z plikiem" << endl;
//		return -1;
//	}
//
//	long long temp;
//	while (cin >> temp) {
//		ints.push_back(temp);
//	}
//	
//	cout << "Finished adding" << endl;
//	cout << ints.size() << endl;
//	vector<long long> sortedInts (ints);
//	sort(sortedInts.begin(), sortedInts.end());
//	
//
//	long long minTarget = -10000;
//	long long maxTarget = 10000;
//
//	int sumsFound = 0;
//	for (long long target = minTarget; target <= maxTarget; target++) {
//		if (target % 10 == 0)
//			cout << (target - minTarget)*100.0 / (maxTarget - minTarget) << "%" << endl;
//
//		for (long long x : ints) {
//			if (x != (target - x) && binary_search(sortedInts.begin(), sortedInts.end(), (target - x))) {
//				sumsFound++;
//				break;
//			}
//		}
//	}
//
//	cout << "Sums " << sumsFound << endl;
//
//	return 0;
//}