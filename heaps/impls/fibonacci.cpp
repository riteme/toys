#include "framework.h"

#include <cstdio>
#include <cstring>

#include <algorithm>

using namespace std;

namespace HeapInterface {

struct Node {
    int prev, next;
    int head, parent;
    short rank;
    bool mark;
};

constexpr int S = 32;

vector<Node> m;
vector<int> aux, buf;  // TODO: optimize for aux
int minp;

void clear() {
    minp = 0;
    w[0] = INF;

    aux.reserve(S);
    aux.clear();
    buf.reserve(n);
    buf.clear();
    m.resize(n + 1);
    for (int i = 0; i <= n; i++) {
        memset(&m[i], 0, sizeof(Node));
        m[i].prev = m[i].next = i;
    }
}

/**
 * @brief insert y into list x
 */
inline void insert(int x, int y) {
    int &v = m[x].next;
    m[y].next = v;
    m[y].prev = x;
    v = m[v].prev = y;
}

/**
 * @brief link y to x
 */
inline void link(int x, int y) {
    m[y].parent = x;
    int &u = m[x].head;
    if (u) insert(u, y);
    else u = y;
}

inline void cut(int x) {
    int p = m[x].parent;
    if (m[p].rank == 1)
        m[p].head = 0;
    else {
        m[p].head = m[x].next;  // choose whatever to be head
        m[m[x].prev].next = m[x].next;
        m[m[x].next].prev = m[x].prev;
    }
    m[x].next = m[x].prev = x;
    m[x].parent = 0;
}

inline int meld(int x, int y) {
    if (w[x] > w[y])
        swap(x, y);
    link(x, y);
    m[x].rank++;
    return x;
}

void push(int x) {
    buf.push_back(x);
    if (!minp || w[x] < w[minp])
        minp = x;
}

void decrease(int x) {
    int p = m[x].parent;
    if (!p) {
        if (w[x] < w[minp])
            minp = x;
    } else if (w[x] < w[p]) {
        do {
            cut(x);
            m[x].mark = false;
            m[p].rank--;
            push(x);

            x = p;
            p = m[x].parent;
        } while (m[x].mark);
        if (p) m[x].mark = true;
    }
}

auto pop() -> int {
    int u = minp, v = m[u].head;
    if (v) {
        do {
            buf.push_back(v);
            int t = m[v].next;
            m[v].parent = 0;
            m[v].mark = false;
            m[v].prev = m[v].next = v;
            v = t;
        } while (v != m[u].head);
    }

    aux.clear();
    for (int x : buf) if (x != u) {
        int r = m[x].rank;
        for (; r < aux.size() && aux[r]; r++) {
            x = meld(x, aux[r]);
            aux[r] = 0;
        }
        if (r >= aux.size())
            aux.resize(r + 1);
        aux[r] = x;
    }

    minp = 0;
    buf.clear();
    for (int x : aux)
        if (x) push(x);

    return u;
}

auto empty() -> bool {
    return !minp;
}

}