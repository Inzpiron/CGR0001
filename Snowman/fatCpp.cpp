#include <iostream>

int main(int n, char** argv) {
	if(n == 1)
		return 1;
	else {
		return n * main(n-1, argv);
	}

	return 0;
}
