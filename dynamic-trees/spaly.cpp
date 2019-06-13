#include <cassert>
#include <cstring>

#include <algorithm>

#include "lct.h"

using std::swap;

static struct Node {
    int val, mx, pos;
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
    m[x].mx = m[x].val;
    m[x].pos = x;
    chkmax(m[x], m[m[x].lch]);
    chkmax(m[x], m[m[x].rch]);
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
    m[y].mx = m[x].mx;
    m[y].pos = m[x].pos;
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
    m[y].mx = m[x].mx;
    m[y].pos = m[x].pos;
    update(x);
    // update(y);
}

inline void access(int x) {
    if (m[x].fa > 0) access(m[x].fa);
    push(x);
}

inline void spaly(int x, bool accessed = false) {
    if (!accessed) access(x);
    while (m[x].fa > 0) {
        int p = m[x].fa;
        if (m[p].lch == x) lrot(p);
        else rrot(p);
    }
}

void LCT::init(int n) {
    memset(m + 1, 0, sizeof(Node) * n);
    for (int i = 1; i <= n; i++) m[i].pos = i;
}

auto LCT::splice(int x) -> int {
    assert(m[x].fa < 0);
    int p = -m[x].fa;
    spaly(p);
    m[m[p].rch].fa = -p;
    m[p].rch = x;
    m[x].fa = p;
    update(p);
    return p;
}

void LCT::expose(int x) {
    spaly(x);
    m[m[x].rch].fa = -x;
    m[x].rch = 0;
    update(x);
    while (m[x].fa) x = splice(x);
}

void LCT::link(int x, int y) {
    spaly(y);
    assert(!m[y].fa);
    m[y].fa = -x;
    // expose(y);
}

void LCT::fastcut(int x) {
    // assume the father of x on the tree has been exposed.
    spaly(x);
    m[x].fa = 0;
}

void LCT::cut(int x) {
    expose(x);
    spaly(x);
    int y = m[x].lch;
    if (!y) return;
    push(y);
    while (m[y].rch) {
        y = m[y].rch;
        push(y);
    }
    spaly(y, true);
    m[m[y].rch].fa = 0;
    m[y].rch = 0;
    update(y);
}

void LCT::evert(int x) {
    expose(x);
    spaly(x);
    m[x].rev ^= 1;
}

void LCT::set(int x, int v) {
    spaly(x);
    m[x].val = v;
    if (v > m[x].mx) {
        m[x].mx = v;
        m[x].pos = x;
    }
}

auto LCT::query(int x, int y) -> int {
    evert(x);
    expose(y);
    spaly(y);
    while (m[x].fa) x = m[x].fa;
    return x != y ? 0 : m[y].pos;
}

auto LCT::get(int x) -> int {
    return m[x].val;
}