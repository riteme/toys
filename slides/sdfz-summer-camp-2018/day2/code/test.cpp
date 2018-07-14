#include <cassert>
#include <cstdio>

struct Node {
    Node(int _v) : data(_v), pre(NULL), nxt(NULL) {}
    int data;
	Node *pre, *nxt;
};

void insert(Node *x, int v) {
    assert(x);  // 保证 x != NULL
    Node *y = new Node(v);
    y->pre = x; y->nxt = x->nxt;
    if (x->nxt) x->nxt->pre = y;
    x->nxt = y;
}

void remove(Node *x) {
    if (x->pre) x->pre->nxt = x->nxt;
    if (x->nxt) x->nxt->pre = x->pre;
    // x->pre = x->nxt = NULL;
}

int main() {
    Node *x = new Node(1);
    return 0;
}
