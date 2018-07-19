#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "veb.h"
#include <set>
using namespace std;

#define MAX 4096
// 2 ^ 20
#define TMAX 1048576
#define N 100000

unsigned long ulrand() {
	return (
		(((unsigned long)rand() << 24) & 0xFF000000ul)
		| (((unsigned long)rand() << 12) & 0x00FFF000ul)
		| (((unsigned long)rand()) & 0x00000FFFul));
}

void test() {
	vEBTree t(TMAX);
	set<int> s;
	srand(time(0));
	int *a = new int[N];
	for (int i = 0; i < N; i++) {
		a[i] = ulrand() % TMAX;
	}
	printf("\tvEB树\tstd::set\n");
	printf("插入\t");
	clock_t begin, end;
	begin = clock();
	for (int i = 0; i < N; i++) {
		t.insert(a[i]);
	}
	end = clock();
	printf("%d\t", end - begin);
	begin = clock();
	for (int i = 0; i < N; i++) {
		s.insert(a[i]);
	}
	end = clock();
	printf("%d\n搜索\t", end - begin);
	for (int i = 0; i < N / 2; i++) {
		a[i] = ulrand() % TMAX;
	}
	begin = clock();
	for (int i = 0; i < N; i++) {
		t.member(a[i]);
	}
	end = clock();
	printf("%d\t", end - begin);
	begin = clock();
	for (int i = 0; i < N; i++) {
		s.find(a[i]);
	}
	end = clock();
	printf("%d\n删除\t", end - begin);
	begin = clock();
	for (int i = 0; i < N; i++) {
		t.remove(a[i]);
	}
	end = clock();
	printf("%d\t", end - begin);
	begin = clock();
	for (int i = 0; i < N; i++) {
		s.erase(a[i]);
	}
	end = clock();
	printf("%d\n", end - begin);
	delete[] a;
}

int main() {
	vEBTree t(MAX);
	while (true) {
		printf(">> ");
		char ch;
		scanf(" %c", &ch);
		if (ch == 'h') {
			printf("h: 显示此帮助\n"
			    "i <num>: 插入一个数字\n"
				"d <num>: 删除一个数字\n"
				"s <num>: 搜索一个数字\n"
				"b <num>: 寻找一个数字的前驱\n"
				"f <num>: 寻找一个数字的后继\n"
				"n: 输出最小元素\n"
				"x: 输出最大元素\n"
				"t: 测试\n"
				"e: 退出程序\n"
			);
		}
		else if (ch == 'n') {
			printf("%d\n", t.min());
		}
		else if (ch == 'x') {
			printf("%d\n", t.max());
		}
		else if (ch == 'e') break;
		else if (ch == 't') {
			test();
		}
		else {
			int num;
			scanf("%d", &num);
			if (ch == 'i') t.insert(num);
			else if (ch == 'd') t.remove(num);
			else if (ch == 's') {
				printf(t.member(num) ? "存在\n" : "不存在\n");
			}
			else if (ch == 'b') {
				printf("%d\n", t.predecessor(num));
			}
			else if (ch == 'f') {
				printf("%d\n", t.successor(num));
			}
		}
	}
	return 0;
}