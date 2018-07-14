/**
 * TODO:
 * [x] Treap
 * [ ] Debug "Treap"
 * [ ] Segment Tree
 * [ ] Debug "Segment Tree"
 * [ ] Main loop
 * [ ] Debug all
 */

// #define NDEBUG

#define NAME "cut"

#include <cstdio>
#include <cstring>
#include <climits>
#include <cstdlib>

#include <utility>
#include <algorithm>

using namespace std;

struct RandomDecision {
    static constexpr int Sample = 10007;
    static constexpr int LoopLength = 50000;

    int randseq[LoopLength], ptr;

    RandomDecision() : ptr(0) {
        srand(*new int);
        for (int i = 0; i < LoopLength; i++) {
            randseq[i] = rand() % Sample + 1;
        }
    }

    bool operator()(int a, int b) {
        if (ptr == LoopLength)
            ptr = 0;
        return a * Sample >= b * randseq[ptr++];
    }
} decide;

#define CP(x) new Node(*x)

extern struct Node *nil;

struct Node {
    Node()  // nil constructor
        : len(0), sum(0), size(0),
          val(0), mark(0),
          lch(nil), rch(nil) {}

    // node length, subtree lens in sum, number of nodes in subtree
    int len, sum, size;
    // data & increment
    int val, mark;
    // left son & right son
    Node *lch, *rch;

    void update() {
        size = lch->size + rch->size;
        sum = lch->sum + rch->sum + len;
    }

    void pushdown() {
        if (mark) {
            val += mark;

            if (lch != nil)
                lch = CP(lch);
            if (rch != nil)
                rch = CP(rch);

            lch->mark += mark;
            rch->mark += mark;
            mark = 0;
        }
    }
};

typedef pair<Node *, Node *> NodePair;

Node* nil = new Node;

Node *alloc(int _len, int _val) {
    Node *x = new Node;
    x->len = x->sum = _len;
    x->size = 1;
    x->val = _val;
    return x;
}

NodePair split(Node *x, int k, int l = 0) {
    if (x == nil)
        return NodePair(nil, nil);

    l += x->lch->sum;
    NodePair ret;
    if (k <= l) {
        x = CP(x);
        x->pushdown();
        ret = split(x->lch, k, l - x->lch->sum);
        x->lch = ret.second;
        ret.second = x;
        x->update();
    } else if (k > l + x->len) {
        x = CP(x);
        x->pushdown();
        ret = split(x->rch, k, l + x->len);
        x->rch = ret.first;
        ret.first = x;
        x->update();
    } else {
        // Mark here are not pushed.
        Node *a = alloc(k - l, x->val + x->mark);
        Node *b = alloc(l + x->len - k, x->val + x->mark);
        a->lch = x->lch;
        b->rch = x->rch;
        a->update();
        b->update();
        ret = NodePair(a, b);
    }

    return ret;
}

Node *merge(Node *x, Node *y) {
    if (x == nil)
        return y;
    if (y == nil)
        return x;

    if (decide(x->size, x->size + y->size)) {
        x = CP(x);
        x->pushdown();
        x->rch = merge(x->rch, y);
        x->update();
        return x;
    } else {
        y = CP(y);
        y->pushdown();
        y->lch = merge(x, y->lch);
        y->update();
        return y;
    }
}

int query(Node *x, int k, int l = 0) {
    if (x == nil)
        return 0;

    l += x->lch->sum;
    if (k <= l)
        return query(x->lch, k, l - x->lch->sum) + x->mark;
    else if (k > l + x->len)
        return query(x->rch, k, l + x->len) + x->mark;
    else
        return x->val + x->mark;
}

#define NMAX 100
#define WMAX 50

static int n;
static int H[NMAX + 10], K[NMAX + 10];
static Node *tree;

void initialize() {
    scanf("%d", &n);

    for (int i = 1; i <= n; i++) {
        scanf("%d%d", H + i, K + i);
    }

    tree = alloc(WMAX, 0);
    for (int i = n; i >= 1; i--) {
        NodePair a = split(tree, H[i] - 1);
        NodePair b = split(tree, WMAX - K[i]);
        NodePair c = split(b.first, H[i] - K[i] - 1);
        Node *d = H[i] - K[i] <= 0 ? alloc(K[i] - H[i] + 1, K[i]) : nil;
        c.second->mark += K[i];
        tree = merge(a.first, merge(d, c.second));
    }
}

int main() {
#ifdef NDEBUG
    freopen(NAME ".in", "r", stdin);
    freopen(NAME ".out", "w", stdout);
#endif
    initialize();

    int x;
    while (scanf("%d", &x) != EOF) {
        printf("-%d\n", query(tree, x));
    }

    return 0;
}
