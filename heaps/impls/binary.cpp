#include "framework.h"

#include <algorithm>

using namespace std;

static int m;
static i64 *w;
static vector<int> h;
static vector<int> pos;

inline constexpr int L(int i) {
    return i << 1;
}

inline constexpr int R(int i) {
    return (i << 1) | 1;
}

inline constexpr int F(int i) {
    return i >> 1;
}

inline void _swap(int i, int j) {
    swap(h[i], h[j]);
    pos[h[i]] = i;
    pos[h[j]] = j;
}

inline void sink(int i) {
    while (true) {
        if (R(i) <= m) {
            i64 mi = min(w[h[i]], min(w[h[L(i)]], w[h[R(i)]]));
            if (mi == w[h[L(i)]]) {
                _swap(i, L(i));
                i = L(i);
            } else if (mi == w[h[R(i)]]) {
                _swap(i, R(i));
                i = R(i);
            } else break;
        } else if (L(i) <= m && w[h[L(i)]] < w[h[i]]) {
            _swap(i, L(i));
            i = L(i);
        } else break;
    }
}

inline void swim(int i) {
    for ( ; i > 1 && w[h[i]] < w[h[F(i)]]; i = F(i))
        _swap(i, F(i));
}

void HeapInterface::clear() {
    h.resize(n + 1, 0);
    pos.resize(n + 1, 0);
    m = 0;
    ::w = this->w;
}

void HeapInterface::push(int x) {
    h[++m] = x;
    pos[x] = m;
    swim(m);
}

void HeapInterface::decrease(int x) {
    swim(pos[x]);
}

auto HeapInterface::pop() -> int {
    int u = h[1];
    h[1] = h[m--];
    pos[h[1]] = 1;
    sink(1);
    return u;
}

auto HeapInterface::size() -> int {
    return m;
}

auto HeapInterface::empty() -> bool {
    return m == 0;
}