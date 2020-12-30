#include <iostream>
#include <vector>

using namespace std;

/**
* 01背包，动态规划
* 方法1：给定背包最大容量（为一个定值），通过迭代让价值最大
* 方法2：给定总价值（将所有价值数相加），通过迭代让容量最小
*/

int main() {
	int itemsLen, weight, valueSum;
	cin >> itemsLen;
	cin >> weight;
	vector<pair<int, int>> items(itemsLen);
	for (int i = 0; i < itemsLen; i++) {
		cin >> items[i].first;
		cin >> items[i].second;
		valueSum += items[i].second;
	}
	vector<vector<int>> board(itemsLen+1,vector<int>(valueSum+1));
	for (int i = 0; i < valueSum + 1; i++)
		board[0][i] = 0;
	for (int i = 0; i < itemsLen; i++)
		board[i][0] = valueSum;
	for (int m = 1; m < itemsLen + 1; m++) {
		int w = items[m - 1].first;
		int v = items[m - 1].second;
		for (int n = 1; n < valueSum+1; n++) {
			board[m][n] = min(board[m-1][n],board[m-1][n-v]);
		}
	}

	for (int i = 1; i < valueSum + 1; i++) {
		board[itemsLen][
	}

	cout << board[itemsLen][weight];

	return 0;
}