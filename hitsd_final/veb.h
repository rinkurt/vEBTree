#ifndef _VEB_H_
#define _VEB_H_

#include <cstring>
#include <cmath>
#include <algorithm>
#include "dynamic_hash.h"
using namespace std;

inline int mypow(int x, int y) {
	int s = 1;
	for (int i = 0; i < y; i++) {
		s *= x;
	}
	return s;
}

inline int upsqrt(int u) {
	return mypow(2, ceil(log2(u) / 2.0));
}

inline int downsqrt(int u) {
	return mypow(2, floor(log2(u) / 2.0));
}

struct vEBNode {
	vEBNode(int _u) :
		u(_u), summary(NULL), min(-1), max(-1), cluster(upsqrt(_u)) {}
	int high(int x) {
		return x / downsqrt(u);
	}
	int low(int x) {
		return x % downsqrt(u);
	}
	int index(int x, int y) {
		return x * downsqrt(u) + y;
	}
	vEBNode* clus(int x) {
		vEBNode* c = cluster.search(x);
		if (c) return c;
		cluster.insert(x, new vEBNode(downsqrt(u)));
		return cluster.search(x);
	}
	vEBNode* summ() {
		if (summary == NULL) {
			summary = new vEBNode(upsqrt(u));
		}
		return summary;
	}

	int u;
	int min;
	int max;
	vEBNode* summary;
	DHashTable<int, vEBNode*> cluster;
};

class vEBTree {
public:
	vEBTree(int u) { root = new vEBNode(u); }
	int min() { return root->min; }
	int max() { return root->max; }
	bool member(int x) { return _member(root, x); }
	int successor(int x) { return _successor(root, x); }
	int predecessor(int x) { return _predecessor(root, x); }
	void insert(int x) { _insert(root, x); }
	void remove(int x) { _remove(root, x); }

private:
	vEBNode* root;
	int _min(vEBNode* v) { return v ? v->min : -1; }
	int _max(vEBNode* v) { return v ? v->max : -1; }
	bool _member(vEBNode* v, int x);
	int _successor(vEBNode* v, int x);
	int _predecessor(vEBNode* v, int x);
	void _insert_empty(vEBNode* v, int x);
	void _insert(vEBNode* v, int x);
	void _remove(vEBNode* v, int x);
};

bool vEBTree::_member(vEBNode * v, int x) {
	if (x == v->min || x == v->max) {
		return true;
	}
	else if (v->u == 2) {
		return false;
	}
	else {
		vEBNode* a = v->cluster[v->high(x)];
		if (a == NULL) return false;
		return _member(a, v->low(x));
	}
}

int vEBTree::_successor(vEBNode * v, int x) {
	if (!v) return -1;
	if (v->u == 2) {
		if (x == 0 && v->max == 1) {
			return 1;
		}
		else return -1;
	}
	else if (v->min != -1 && x < v->min) {
		return v->min;
	}
	else {
		int max_low = v->clus(v->high(x))->max;
		if (max_low != -1 && v->low(x) < max_low) {
			int offset = _successor(v->clus(v->high(x)), v->low(x));
			return v->index(v->high(x), offset);
		}
		else {
			int suc = _successor(v->summ(), v->high(x));
			if (suc == -1) return -1;
			else {
				int offset = v->cluster[suc]->min;
				return v->index(suc, offset);
			}
		}
	}
}

int vEBTree::_predecessor(vEBNode * v, int x) {
	if (!v) return -1;
	if (v->u == 2) {
		if (x == 1 && v->min == 0) {
			return 0;
		}
		else return -1;
	}
	else if (v->max != -1 && x > v->max) {
		return v->max;
	}
	else {
		int min = _min(v->cluster[v->high(x)]);
		if (min != -1 && v->low(x) > min) {
			int offset = _predecessor(v->cluster[v->high(x)], v->low(x));
			return v->index(v->high(x), offset);
		}
		else {
			int pre = _predecessor(v->summary, v->high(x));
			if (pre == -1) {
				if (_min(v) != -1 && x > _min(v)) {
					return _min(v);
				}
				else return -1;
			}
			else {
				int offset = _max(v->cluster[pre]);
				return v->index(pre, offset);
			}
		}
	}
}

inline void vEBTree::_insert_empty(vEBNode * v, int x) {
	v->min = x;
	v->max = x;
}

void vEBTree::_insert(vEBNode * v, int x) {
	if (v->min == -1) {
		_insert_empty(v, x);
	}
	else {
		if (x == v->min) return;
		if (x < v->min) {
			swap(x, v->min);
		}
		if (v->u > 2) {
			if (v->clus(v->high(x))->min == -1) {
				_insert(v->summ(), v->high(x));
				_insert_empty(v->clus(v->high(x)), v->low(x));
			}
			else _insert(v->clus(v->high(x)), v->low(x));
		}
		if (x > v->max) {
			v->max = x;
		}
	}
}

void vEBTree::_remove(vEBNode * v, int x) {
	if (!v || x < 0) return;
	if (x == v->min && x == v->max) {
		v->min = -1;
		v->max = -1;
	}
	else if (v->u == 2) {
		if (x != v->min && x != v->max) return;
		if (x == 0) v->min = 1;
		else v->min = 0;
		v->max = v->min;
	}
	else {
		if (x == v->min) {
			int first = v->summary->min;
			x = v->index(first, v->cluster[first]->min);
			v->min = x;
		}
		_remove(v->cluster[v->high(x)], v->low(x));
		if (_min(v->cluster[v->high(x)]) == -1) {
			_remove(v->summary, v->high(x));
			if (x == v->max) {
				int sum = v->summary->max;
				if (sum == -1) {
					v->max = v->min;
				}
				else v->max = v->index(sum, _max(v->cluster[sum]));
			}
		}
		else if (x == v->max) {
			v->max = v->index(v->high(x), v->cluster[v->high(x)]->max);
		}
	}
}

#endif // !_VEB_H_
