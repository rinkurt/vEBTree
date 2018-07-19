#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "veb.h"
using namespace std;

// 2 ^ 30
#define MAX 1048576
#define N 1000000

unsigned long ulrand() {
	return (
		(((unsigned long)rand() << 24) & 0xFF000000ul)
		| (((unsigned long)rand() << 12) & 0x00FFF000ul)
		| (((unsigned long)rand()) & 0x00000FFFul));
}

int main() {
	srand(time(0));
	int *a = new int[N];
	vEBTree t(MAX);
	RBTree r;
	for (int i = 0; i < N; i++) {
		a[i] = ulrand() % MAX;
	}
	printf("²åÈë\t");
	clock_t begin, end;
	begin = clock();
	for (int i = 0; i < N; i++) {
		t.insert(a[i]);
	}
	end = clock();
	printf("%d\t", end - begin);
	begin = clock();
	for (int i = 0; i < N; i++) {
		r.insert(a[i]);
	}
	end = clock();
	printf("%d\nËÑË÷\t", end - begin);
	for (int i = 0; i < N / 2; i++) {
		a[i] = 1;
	}
	begin = clock();
	for (int i = 0; i < N; i++) {
		t.member(a[i]);
	}
	end = clock();
	printf("%d\t", end - begin);
	begin = clock();
	for (int i = 0; i < N; i++) {
		r.search(a[i]);
	}
	end = clock();
	printf("%d\n", end - begin);
	delete[] a;
	return 0;
}