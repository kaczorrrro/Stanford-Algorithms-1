#include<iostream>
#include"Heap.h"
#include<vector>
#include<stdio.h>

class MedianMaintener {
public:
	void addNumber(int num) {
		totalElems++;

		if (totalElems == 1) {
			maxHeap.insert(Heap<int>::Entry{ -num, num });
			return;
		}

		int currentMedian = maxHeap.extractMin().obj;
		if (num > currentMedian) {
			minHeap.insert(Heap<int>::Entry{ num, num });
		}
		else {
			maxHeap.insert(Heap<int>::Entry{ -num, num });
		}
		balanceHeaps();
		

	}
	int getMedian() { return maxHeap.extractMin().obj; }
private:
	Heap<int> minHeap;
	Heap<int> maxHeap;
	int totalElems = 0;

	void balanceHeaps() {
		if (maxHeap.size() > minHeap.size() + 1) {
			auto element = maxHeap.extractMin();
			maxHeap.popMin();

			element.key *= -1;
			minHeap.insert(element);
		}
		else if (minHeap.size() > maxHeap.size()) {
			auto element = minHeap.extractMin();
			minHeap.popMin();

			element.key *= -1;
			maxHeap.insert(element);
		}
	}

};

int main() {
	freopen("medianInput.txt", "r", stdin);
	MedianMaintener m;
	long long sumOfMedians = 0;
	
	int i;
	while (cin >> i) {
		m.addNumber(i);
		sumOfMedians += m.getMedian();
	}

	cout << sumOfMedians % 10000 << endl;


}

