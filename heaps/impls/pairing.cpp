#include "framework.h"

#include <cstring>

#include <algorithm>

using namespace std;

namespace HeapInterface {

// index i as nodes, w[i] as keys
// m[0] as null node

struct Node {
    int lch, rch, par;
};

static vector<Node> m;
static vector<int> tmp;
static int root;

inline void reset(int x) {
    memset(&m[x], 0, sizeof(Node));
}

void clear() {
    m.resize(n + 1);
    tmp.reserve(n);
    for (int i = 0; i <= n; i++)
        reset(i);
    root = 0;
}

inline int meld(int x, int y) {
    if (!x) return y;
    // if (!y) return x;
    if (w[x] > w[y])
        swap(x, y);

    m[y].rch = m[x].lch;
    m[m[x].lch].par = y;
    m[x].lch = y;
    m[y].par = x;

    return x;
}

void push(int x) {
    root = meld(root, x);
}

void decrease(int x) {
    if (x == root)
        return;

    int p = m[x].par, r = m[x].rch;
    if (x == m[p].lch) m[p].lch = r;
    else m[p].rch = r;
    m[r].par = p;
    root = meld(root, x);
}

auto combine(int x) -> int {
    if (!x) return 0;
    int y = m[x].rch, z = m[y].rch;
    return meld(combine(z), meld(y, x));
}

auto icombine(int x) -> int {
    if (!x) return 0;
    tmp.clear();
    for (int y, z; x; x = z) {
        y = m[x].rch;
        z = m[y].rch;
        tmp.push_back(meld(y, x));
    }
    x = tmp.back();
    for (int i = tmp.size() - 2; i >= 0; i--)
        x = meld(x, tmp[i]);
    m[x].par = 0;
    return x;
}

auto pop() -> int {
    int u = root, x = m[u].lch;
    root = combine(x);
    return u;
}

auto empty() -> bool {
    return !root;
}

}