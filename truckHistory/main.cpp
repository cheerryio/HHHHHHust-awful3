#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

#define TRUCK_LENGTH 7
#pragma warning (disable: 4996)

/**
* ����һ��truckNumber��С�����飬Prime�㷨ÿһ�μ�¼�������е㵽��ǰ��������̵ľ���
* ����õ��Ѿ�����С�������У���ֵ����Ϊ8�����Ҳ��ٳ��Ը��¸�ֵ
*/

struct Trip {
	int destination;
	int distance;
};

int truckDistance(string a, string b) {
	int length = 0;
	for (int i = 0; i < TRUCK_LENGTH; i++) {
		if (a[i] != b[i])
			length++;
	}
	return length;
}

bool compare(Trip a, Trip b) {
	return a.distance > b.distance;
}

int Prime(vector<string> trucks) {
	int totalDistance = 0;
	int truckNumbers = trucks.size();
	int minTruck=0;
	vector<int> distances(truckNumbers, 9);

	for (int round = 0; round < truckNumbers-1; round++) {
		distances[minTruck] = 8;
		for (int i = 1; i < truckNumbers; i++) {
			if (distances[i] == 8) continue;
			int distance = truckDistance(trucks[minTruck], trucks[i]);
			if (distance < distances[i])
				distances[i] = distance;
		}
		int distance, minDistance = 8;
		for (int i = 0; i < truckNumbers; i++) {
			distance = distances[i];
			if (distance == 8) continue;
			if (distance < minDistance) {
				minTruck = i;
				minDistance = distance;
			}
		}
		totalDistance += minDistance;
	}

	return totalDistance;
}

int main() {
	int truckNumber;
	while (cin >> truckNumber, truckNumber) {
		vector<string> trucks(truckNumber);
		for (int i = 0; i < truckNumber; i++) {
			char truck[8];
			scanf("%s", truck);
			trucks[i] = string(truck);
		}
		int distance = Prime(trucks);
		printf("The highest possible quality is 1/%d.\n", distance);
	}

	return 0;
}