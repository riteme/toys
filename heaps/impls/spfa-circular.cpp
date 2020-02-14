#include "framework.h"

#include <vector>

using namespace std;

namespace HeapInterface {

static int l, r;
static vector<int> q;
static vector<bool> in;

inline void inc(int &x) {
    x = x == n ? 0 : x + 1;
}

void clear() {
    l = r = 0;
    q.resize(n + 1, 0);
    in.resize(n + 1, 0);
}

void push(int x) {
    if (!in[x]) {
        q[r] = x;
        inc(r);
        in[x] = true;
    }
}

void decrease(int x) {
    push(x);
}

auto pop() -> int {
    int u = q[l];
    inc(l);
    in[u] = false;
    return u;
}

auto empty() -> bool {
    return l == r;
}

}