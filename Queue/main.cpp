#include <iostream>

using namespace std;

extern const char* TestQueue(int& s);

int main() {
	int i = 10;
	const char *e = TestQueue(i);
	cout << "------------------Queue---------------" << endl;
	cout << e << "   " << "score is:" << i;
	return 0;
}