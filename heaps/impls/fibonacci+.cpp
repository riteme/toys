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
vector<int> aux;
int tr;

void clear() {
    tr = 0;
    aux.resize(S);
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
    else u = m[y].prev = m[y].next = y;
}

inline void cut(int x) {
    int p = m[x].parent;
    if (m[x].next == x)
        m[p].head = 0;
    else {
        m[p].head = m[x].next;  // choose whatever to be head
        m[m[x].prev].next = m[x].next;
        m[m[x].next].prev = m[x].prev;
    }
}

inline int meld(int x, int y) {
    if (!x) return y;
    if (w[x] > w[y])
        swap(x, y);
    link(x, y);
    m[x].rank += m[x].rank == m[y].rank;
    return x;
}

void push(int x) {
    tr = meld(tr, x);
}

void decrease(int x) {
    if (x == tr || w[x] >= w[m[x].parent])
        return;

    m[tr].mark = false;
    int u = x;
    do {
        u = m[u].parent;
        if (m[u].rank)
            m[u].rank--;
        m[u].mark ^= 1;
    } while (m[u].mark);

    cut(x);
    tr = meld(tr, x);
}

auto pop() -> int {
    int u = tr;

    tr = 0;
    if (m[u].head) {
        int mx = 0, v = m[u].head;
        do {
            int r = m[v].rank, t = m[v].next;

            for (; aux[r]; r++) {
                v = meld(v, aux[r]);
                aux[r] = 0;
            }
            mx = max(mx, r);
            aux[r] = v;

            v = t;
        } while (v != m[u].head);

        for (int i = 0; i <= mx; i++) if (aux[i]) {
            tr = meld(tr, aux[i]);
            aux[i] = 0;
        }
    }

    return u;
}

auto empty() -> bool {
    return !tr;
}

}