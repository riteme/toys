#include "framework.h"

#include <cmath>
#include <cstring>

#include <algorithm>

using namespace std;

namespace HeapInterface {

// index i as nodes, w[i] as keys
// m[0] as null node

struct Node {
    int lch, rch, par;
};

static int THRESHOLD = 16;
static vector<Node> m;
static vector<int> buf, aux;

inline void reset(int x) {
    memset(&m[x], 0, sizeof(Node));
}

void clear() {
    THRESHOLD = int(log2(n + 1));
    _aux = THRESHOLD;
    m.resize(n + 1);
    buf.reserve(n);
    aux.reserve(THRESHOLD);

    buf.clear();
    aux.resize(1);
    aux[0] = 0;
    for (int i = 0; i <= n; i++)
        reset(i);
}

// link y to x
inline void link(int x, int y) {
    m[y].rch = m[x].lch;
    m[m[x].lch].par = y;
    m[x].lch = y;
    m[y].par = x;
}

inline void cut(int x) {
    int p = m[x].par, l = m[x].lch, r = m[x].rch;
    m[x].par = 0;

    if (l) {
        m[x].lch = m[l].rch;
        m[m[l].rch].par = x;
        m[l].rch = r;
        m[r].par = l;
        r = l;
    }

    if (x == m[p].lch) m[p].lch = r;
    else m[p].rch = r;
    m[r].par = p;
}

inline int meld(int x, int y) {
    if (!x) return y;
    // if (!y) return x;
    if (w[x] > w[y])
        swap(x, y);
    link(x, y);
    return x;
}

inline int build_insertion() {
    for (int r = buf.size(); r > 1; r = (r + 1) >> 1)
    for (int i = 0; i < r; i += 2)
        buf[i >> 1] = i + 1 < r ? meld(buf[i], buf[i + 1]) : buf[i];
    int u = buf[0];
    buf.clear();
    return u;
}

inline void build_auxiliary() {
    sort(aux.begin(), aux.end(), [](int x, int y) {
        return w[x] < w[y];
    });
    for (int i = 1; i < aux.size(); i++)
        link(aux[i - 1], aux[i]);
    aux.resize(1);
}

void push(int x) {
    buf.push_back(x);
}

void decrease(int x) {
    if (!m[x].par)
        return;

    cut(x);
    aux.push_back(x);

    if (aux.size() >= THRESHOLD)
        build_auxiliary();
}

auto combine(int x) -> int {
    if (!x) return 0;
    int y = m[x].rch, z = m[y].rch;
    return meld(combine(z), meld(y, x));
}

auto pop() -> int {
    if (!buf.empty())
        aux.push_back(build_insertion());
    build_auxiliary();

    int u = aux[0];
    aux[0] = combine(m[aux[0]].lch);
    m[aux[0]].par = 0;
    return u;
}

auto empty() -> bool {
    return !aux[0] && buf.empty();
}

}