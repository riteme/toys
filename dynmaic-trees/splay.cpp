#include <cassert>

#include <algorithm>

#include "lct.h"

using std::swap;

static struct Node {
    int w, sum;
    int fa, lch, rch;
    bool rev;
} m[NMAX + 1];

inline void push(int x) {
    if (m[x].rev) {
        swap(m[x].lch, m[x].rch);
        m[m[x].lch].rev ^= 1;
        m[m[x].rch].rev ^= 1;
        m[x].rev = 0;
    }
}

inline void update(int x) {
    m[x].sum = m[x].w + m[m[x].lch].sum + m[m[x].rch].sum;
}

inline void lrot(int x) {
    assert(!m[x].rev);
    int y = m[x].lch;
    assert(y);
    assert(!m[y].rev);
    m[m[y].rch].fa = x;
    m[x].lch = m[y].rch;
    m[y].rch = x;
    if (m[x].fa > 0) {
        int p = m[x].fa;
        if (m[p].lch == x) m[p].lch = y;
        else m[p].rch = y;
    }
    m[y].fa = m[x].fa;
    m[x].fa = y;
    m[y].sum = m[x].sum;
    update(x);
    // update(y);
}

inline void rrot(int x) {
    assert(!m[x].rev);
    int y = m[x].rch;
    assert(y);
    assert(!m[y].rev);
    m[m[y].lch].fa = x;
    m[x].rch = m[y].lch;
    m[y].lch = x;
    if (m[x].fa > 0) {
        int p = m[x].fa;
        if (m[p].lch == x) m[p].lch = y;
        else m[p].rch = y;
    }
    m[y].fa = m[x].fa;
    m[x].fa = y;
    m[y].sum = m[x].sum;
    update(x);
    // update(y);
}

inline void access(int x) {
    if (m[x].fa > 0) access(m[x].fa);
    push(x);
}

inline void splay(int x, bool accessed = false) {
    if (!accessed) access(x);
    while (m[x].fa > 0) {
        int p = m[x].fa, p2 = m[p].fa;
        if (p2 > 0) {
            if (m[p].lch == x && m[p2].lch == p) lrot(p2);
            else if (m[p].rch == x && m[p2].rch == p) rrot(p2);
        }
        if (m[p].lch == x) lrot(p);
        else rrot(p);
    }
}

auto LCT::splice(int x) -> int {
    assert(m[x].fa < 0);
    int p = -m[x].fa;
    splay(p);
    m[m[p].rch].fa = -p;
    m[p].rch = x;
    m[x].fa = p;
    update(p);
    return p;
}

void LCT::expose(int x) {
    splay(x);
    m[m[x].rch].fa = -x;
    m[x].rch = 0;
    update(x);
    while (m[x].fa) x = splice(x);
}

void LCT::link(int x, int y) {
    splay(y);
    assert(!m[y].fa);
    m[y].fa = -x;
    // expose(y);
}

void LCT::fastcut(int x) {
    // assume the father of x on the tree has been exposed.
    splay(x);
    m[x].fa = 0;
}

void LCT::cut(int x) {
    expose(x);
    splay(x);
    int y = m[x].lch;
    if (!y) return;
    push(y);
    while (m[y].rch) {
        y = m[y].rch;
        push(y);
    }
    splay(y, true);
    m[m[y].rch].fa = 0;
    m[y].rch = 0;
    update(y);
}

void LCT::evert(int x) {
    expose(x);
    splay(x);
    m[x].rev ^= 1;
}

void LCT::set(int x, int v) {
    splay(x);
    m[x].sum += v - m[x].w;
    m[x].w = v;
}

void LCT::toggle(int x) {
    splay(x);
    m[x].sum += m[x].w ? 1 : -1;
    m[x].w ^= 1;
}

auto LCT::count(int x, int y) -> int {
    if (x == y) return 0;
    if (x < y) swap(x, y);
    expose(x);
    expose(y);
    splay(x);
    int r = m[x].sum;
    if (m[x].fa != -y) {
        expose(-m[x].fa);
        splay(y);
        r += m[y].sum;
    }
    return r;
}