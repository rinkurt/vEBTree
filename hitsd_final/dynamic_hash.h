#ifndef _DYNAMIC_HASH_H_
#define _DYNAMIC_HASH_H_

#include <cstring>
#include <algorithm>
using namespace std;
#define ELEMNUM 4

template <typename Tk, typename Tv>
struct Entry {
	Tk key;
	Tv value;
};

template <typename Tk, typename Tv>
struct Bucket {
	Bucket() : prefix(1), n(0) {}
	Bucket(int p) : prefix(p), n(0) {}
	int prefix;					// 桶深度
	Entry<Tk,Tv> data[ELEMNUM];
	int n;
};

template <typename Tk, typename Tv>
class DHashTable {
public:
	DHashTable(int k);
	int hashfunc(Tk t) { return t % m; }
	Tv operator[](Tk t) { return search(t); }
	void insert(Tk t, Tv v);
	void remove(Tk t);
	Tv search(Tk t);
private:
	int m;
	int prefix;					// 表深度
	int n;
	Bucket<Tk, Tv>** table;
};

template<typename Tk, typename Tv>
DHashTable<Tk,Tv>::DHashTable(int k) :
	prefix(1), m(k), n(2) {
	table = new Bucket<Tk, Tv>*[n];
	for (int i = 0; i < n; i++) {
		table[i] = new Bucket<Tk, Tv>();
	}
}

template<typename Tk, typename Tv>
void DHashTable<Tk,Tv>::insert(Tk t, Tv v) {
	int index = hashfunc(t) & ((1 << prefix) - 1);
	Bucket<Tk, Tv>* b = table[index];
	if (b->n < ELEMNUM) {
		b->data[b->n].key = t;
		b->data[b->n].value = v;
		b->n++;
	}
	else {
		// 表扩张
		if (prefix == b->prefix) {
			prefix++;
			Bucket<Tk, Tv>** newtable = new Bucket<Tk, Tv>*[n * 2];
			memcpy(newtable, table, n * sizeof(Bucket<Tk, Tv>*));
			delete[] table;
			table = newtable;
			for (int i = n; i < 2 * n; i++) {
				table[i] = table[i - n];
			}
			n *= 2;
		}
		// 重分配桶
		int a1 = index & ((1 << b->prefix) - 1);
		int a2 = a1 + (1 << b->prefix);
		int l = 1 << (b->prefix + 1);
		Bucket<Tk, Tv>* buc1 = new Bucket<Tk, Tv>(b->prefix + 1);
		Bucket<Tk, Tv>* buc2 = new Bucket<Tk, Tv>(b->prefix + 1);
		for (int i = a1; i < n; i += l) {
			table[i] = buc1;
		}
		for (int i = a2; i < n; i += l) {
			table[i] = buc2;
		}
		for (int i = 0; i < b->n; i++) {
			insert(b->data[i].key, b->data[i].value);
		}
		insert(t, v);
	}
}

template<typename Tk, typename Tv>
void DHashTable<Tk,Tv>::remove(Tk t) {
	int index = hashfunc(t) & ((1 << prefix) - 1);
	Bucket<Tk, Tv>* b = table[index];
	for (int i = 0; i < b->n; i++) {
		if (b->data[i].key == t) {
			swap(b->data[i], b->data[b->n - 1]);
			b->n--;
		}
	}
}

template<typename Tk, typename Tv>
Tv DHashTable<Tk, Tv>::search(Tk t) {
	int index = hashfunc(t) & ((1 << prefix) - 1);
	Bucket<Tk, Tv>* b = table[index];
	for (int i = 0; i < b->n; i++) {
		Entry<Tk, Tv>* e = &(b->data[i]);
		if (e->key == t) {
			return e->value;
		}
	}
	return NULL;
}



#endif 
