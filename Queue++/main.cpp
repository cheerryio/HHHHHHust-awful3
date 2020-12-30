#include <iostream>

using namespace std;

extern const char* TestQUEUE(int& s);

int main() {
	int score = 0;
	const char *info=TestQUEUE(score);
	cout << "------------------Queue++----------------" << endl;
	cout << info << " score:" << score << endl;
	return 0;
}