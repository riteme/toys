/**
 * Locally biased 2-3 tree
 */

#include <cstdio>
#include <cassert>
#include <cstring>

#include <algorithm>

using std::swap;
using std::max;

#include "lct.h"

#define STACKSIZE 256

#define LOG(x) (31 - __builtin_clz(x))

struct Triple {
    int l, r, p;
};

static struct Node {
    int sum, wt;
    int fa, ch[3];
    char rk, rev;
} m[NMAX * 2 + 1];

static struct Allocator {
    Allocator() {
        for (int i = 1; i <= NMAX; i++) seq[i - 1] = NMAX + i;
    }

    int seq[NMAX];

    int head = 0;
    void release(int x) {
        assert(head);
        seq[--head] = x;
    }

    auto alloc() -> int {
        assert(head < NMAX);
        int r = seq[head++];
        memset(m + r, 0, sizeof(Node));
        return r;
    }
} mem;

inline auto max3(int a, int b, int c) -> int {
    return a >= max(b, c) ? a : max(b, c);
}

inline auto leaf(int x) -> bool {
    return !m[x].ch[0];
}

inline auto first(int x) -> int& {
    assert(m[x].ch[0]);
    return m[x].ch[0];
}

inline auto last(int x) -> int& {
    assert(m[x].ch[0]);
    assert(m[x].ch[1]);
    return m[x].ch[2] ? m[x].ch[2] : m[x].ch[1];
}

inline void push(int x) {
    if (m[x].rev) {
        if (m[x].ch[2]) swap(m[x].ch[0], m[x].ch[2]);
        else swap(m[x].ch[0], m[x].ch[1]);
        m[m[x].ch[0]].rev ^= 1;
        m[m[x].ch[1]].rev ^= 1;
        m[m[x].ch[2]].rev ^= 1;
        m[x].rev = 0;
    }
}

inline void update(int x) {
    m[x].sum = m[m[x].ch[0]].sum + m[m[x].ch[1]].sum + m[m[x].ch[2]].sum;
    m[x].wt = m[m[x].ch[0]].wt + m[m[x].ch[1]].wt + m[m[x].ch[2]].wt;
    m[x].rk = 1 + max3(m[m[x].ch[0]].rk, m[m[x].ch[1]].rk, m[m[x].ch[2]].rk);
}

inline auto concat(int x, int y) -> int{
    int z = mem.alloc();
    m[z].ch[0] = x;
    m[z].ch[1] = y;
    m[x].fa = z;
    m[y].fa = z;
    update(z);
    return z;
}

#define LSHFT(x) \
    m[x].ch[0] = m[x].ch[1]; \
    m[x].ch[1] = m[x].ch[2]; \
    m[x].ch[2] = 0; \
    update(x);

static auto join(int x, int y) -> int{
    if (!x) return y;
    if (!y) return x;
    int rx = m[x].rk, ry = m[y].rk;
    if (rx == ry || (rx > ry && leaf(x)) || (rx < ry && leaf(y)))
        return concat(x, y);
    if (rx > ry) {
        push(x);
        int &u = last(x), v = join(u, y), rv = m[v].rk;
        if (rv < rx) {
            u = v;
            m[v].fa = x;
            update(x);
        } else if (m[x].ch[2]) {
            u = 0;
            update(x);
            return concat(x, v);
        } else {
            assert(m[v].ch[0]);
            assert(m[v].ch[1]);
            assert(!m[v].ch[2]);
            u = m[v].ch[0];
            m[x].ch[2] = m[v].ch[1];
            m[m[x].ch[2]].fa = m[u].fa = x;
            update(x);
            mem.release(v);
        }
        return x;
    }
    // if (rx < ry)
    push(y);
    int &u = first(y), v = join(x, u), rv = m[v].rk;
    if (rv < ry) {
        u = v;
        m[v].fa = y;
        update(y);
    } else if (m[y].ch[2]) {
        LSHFT(y);
        return concat(v, y);
    } else {
        assert(m[v].ch[0]);
        assert(m[v].ch[1]);
        assert(!m[v].ch[2]);
        m[y].ch[2] = m[y].ch[1];
        m[y].ch[1] = m[v].ch[1];
        m[y].ch[0] = m[v].ch[0];
        m[m[y].ch[0]].fa = m[m[y].ch[1]].fa = y;
        update(y);
        mem.release(v);
    }
    return y;
}

static int stk[STACKSIZE], tail;
inline void access(int x) {
    tail = 0;
    do {
        stk[tail++] = x;
        x = m[x].fa;
    } while (x > 0);
    for (int i = tail - 1; i >= 0; i--) push(stk[i]);
}

static auto split(int x) -> Triple {
    assert(leaf(x));
    access(x);
    int l = 0, r = 0, t = m[stk[tail - 1]].fa;
    for (int i = 0; i < tail - 1; i++) {
        int u = stk[i], p = stk[i + 1];
        if (m[p].ch[0] == u) {
            if (m[p].ch[2]) {
                LSHFT(p);
                r = join(r, p);
            } else {
                r = join(r, m[p].ch[1]);
                mem.release(p);
            }
        } else if (m[p].ch[1] == u) {
            l = join(m[p].ch[0], l);
            if (m[p].ch[2]) r = join(r, m[p].ch[2]);
            mem.release(p);
        } else {
            m[p].ch[2] = 0;
            update(p);
            l = join(p, l);
        }
    }
    m[l].fa = m[r].fa = m[x].fa = 0;
    return {l, r, t};
}

inline void reweight(int x, int d) {
    auto t = split(x);
    m[x].wt += d;
    m[x].rk =  LOG(m[x].wt);
    x = join(x, t.r);
    x = join(t.l, x);
    m[x].fa = t.p;
}

#ifndef NDEBUG
static int n_node;
#endif

void LCT::init(int n, int q, int c1, int c2, int c3) {
    for (int i = 1; i <= NMAX; i++) m[i].wt = 1;
#ifndef NDEBUG
    n_node = c1 + c2;
#endif
}

auto LCT::splice(int x) -> int {
    assert(m[x].fa < 0);
    int p = -m[x].fa;
    assert(p <= NMAX);
    auto t = split(p);
    m[t.r].fa = -p;
    m[p].wt += m[t.r].wt - m[x].wt;
    m[p].rk = LOG(m[p].wt);
    x = join(p, x);
    x = join(t.l, x);
    m[x].fa = t.p;
    return x;
}

void LCT::expose(int x) {
    auto t = split(x);
    m[t.r].fa = -x;
    m[x].wt += m[t.r].wt;
    m[x].rk = LOG(m[x].wt);
    x = join(t.l, x);
    m[x].fa = t.p;
    while (m[x].fa) x = splice(x);
}

void LCT::link(int x, int y) {
    while (m[y].fa) y = m[y].fa;
    expose(x);
    m[y].fa = -x;
    reweight(x, m[y].wt);
    // expose(y);
}

void LCT::fastcut(int x) {
    while (m[x].fa > 0) x = m[x].fa;
    if (m[x].fa) reweight(-m[x].fa, -m[x].wt);
    m[x].fa = 0;
}

void LCT::cut(int x) {
    expose(x);
    split(x);
}

void LCT::evert(int x) {
    expose(x);
    while (m[x].fa) x = m[x].fa;
    m[x].rev ^= 1;
}

void LCT::set(int x, int v) {
    assert(x <= NMAX);
    for (int y = m[x].fa; y > 0; y = m[y].fa) m[y].sum += v - m[x].sum;
    m[x].sum = v;
}

void LCT::toggle(int x) {
    assert(x <= NMAX);
    for (int y = m[x].fa; y > 0; y = m[y].fa) m[y].sum += m[x].sum ? -1 : 1;
    m[x].sum ^= 1;
}

#ifndef NDEBUG
static bool mark[NMAX * 2 + 100];
static int sum[NMAX + 10];
void show(int x) {
    // printf("%d {fa = %d, ch = [%d, %d, %d], wt = %d, rk = %d, sum = %d, rev = %d}\n",
    //        x, m[x].fa, m[x].ch[0], m[x].ch[1], m[x].ch[2], m[x].wt, m[x].rk, m[x].sum, (int)m[x].rev);
    if (x > NMAX) {
        if (m[m[x].ch[0]].rk < m[x].rk - 1)
            assert(m[m[x].ch[1]].rk == m[x].rk - 1 && leaf(m[x].ch[1]));
        if (m[m[x].ch[1]].rk < m[x].rk - 1)
            assert((m[m[x].ch[0]].rk == m[x].rk - 1 && leaf(m[x].ch[0])) &&
                   (!m[x].ch[2] || (m[m[x].ch[2]].rk == m[x].rk - 1 && leaf(m[x].ch[2]))));
        if (m[x].ch[2] && m[m[x].ch[2]].rk < m[x].rk - 1)
            assert(m[m[x].ch[1]].rk == m[x].rk - 1 && leaf(m[x].ch[1]));
    }
    if (m[x].fa > 0 && !mark[m[x].fa]) {
        mark[m[x].fa] = true;
        show(m[x].fa);
    } else if (m[x].fa < 0) sum[-m[x].fa] += m[x].wt;
}

void LCT::debug() {
    memset(mark, 0, sizeof(mark));
    memset(sum, 0, sizeof(sum));
    for (int i = 1; i <= NMAX; i++) show(i);
    // puts("");
    for (int i = 1; i <= NMAX; i++) assert(m[i].wt == sum[i] + 1);
}
#endif

auto LCT::count(int x, int y) -> int {
#ifndef NDEBUG
    int t1 = 1;
    while (m[t1].fa) t1 = m[t1].fa;
    assert(m[t1].wt == n_node);
#endif
    if (x == y) return 0;
    if (x < y) swap(x, y);
    expose(x);
    expose(y);
    while (m[x].fa > 0) x = m[x].fa;
    int r = m[x].sum;
    if (m[x].fa != -y) {
        expose(-m[x].fa);
        while (m[y].fa > 0) y = m[y].fa;
        r += m[y].sum;
    }
    return r;
}