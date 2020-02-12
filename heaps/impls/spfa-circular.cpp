#include "framework.h"

#include <vector>

using namespace std;

static int l, r, n;
static vector<int> q;
static vector<bool> in;

inline void inc(int &x) {
    x = x + 1 == n ? 0 : x + 1;
}

void HeapInterface::clear() {
    l = r = 0;
    ::n = this->n + 1;
    q.resize(n + 1, 0);
    in.resize(n + 1, 0);
}

void HeapInterface::push(int x) {
    if (!in[x]) {
        q[r] = x;
        inc(r);
        in[x] = true;
    }
}

void HeapInterface::decrease(int x) {
    push(x);
}

auto HeapInterface::pop() -> int {
    int u = q[l];
    inc(l);
    in[u] = false;
    return u;
}

auto HeapInterface::size() -> int {
    return l <= r ? r - l : r + n - l;
}

auto HeapInterface::empty() -> bool {
    return l == r;
}