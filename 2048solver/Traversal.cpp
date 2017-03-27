#include "Traversal.h"


Traversal::Traversal(Vector vec) {
	for (int pos = 0; pos < N; pos++) {
		x[pos] = pos;
		y[pos] = pos;
	}

	if (vec.x == 1) reverse(x);
	if (vec.y == 1) reverse(y);
}

void Traversal::reverse(int arr[N]) {
	int temp;
	for (int i = 0; i < N / 2; i++) {
		temp = arr[i];
		arr[i] = arr[N-1 - i];
		arr[N-1 - i] = temp;
	}
}