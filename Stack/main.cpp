#include "Stack.h"
#include "queue++.h"
#include <iostream>

extern const char* TestSTACK(int& s);

using namespace std;

int main() {
	int s;
	const char *info=TestSTACK(s);
	cout << "--------Stack--------" << endl;
	cout << info << "  " << s << endl;
	return 0;
}