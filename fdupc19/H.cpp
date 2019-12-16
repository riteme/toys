// #define NDEBUG
#define FAST_IO
// #define TIMER

#include <cstdio>
#include <cctype>
#include <cstring>

#include <chrono>
#include <vector>
#include <algorithm>

using namespace std;

#define ASSERT(expr) if (!(expr)) exit(233);

#define BUFFERSIZE 65536
static size_t _pos = BUFFERSIZE;
static char _buf[BUFFERSIZE];

void _getc(char &c) {
    if (_pos == BUFFERSIZE) {
        _pos = 0;
        fread(_buf, 1, BUFFERSIZE, stdin);
    }
    c = _buf[_pos++];
}

template <typename T>
void read(T &x) {
    x = 0;
    char c;
    do _getc(c); while (!isdigit(c));
    do {
        x = x * 10 + (c - '0');
        _getc(c);
    } while (isdigit(c));
}

#define NMAX 5000000
#define WMAX 1000000
#define INF 0x3f3f3f3f
#define MASK 0xfffffu

typedef unsigned int u32;
u32 magic(u32 x) {
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

static int n, q;
static int seq[NMAX + 10], buc[WMAX + 10];
static int dx[WMAX + 10], ux[WMAX + 10], by[WMAX + 10];

int locate(int x, int k) {
    if (k < buc[x]) return 0;
    if (k >= buc[x + 1]) return n + 1;
    return seq[k];
}

static struct Node {
    int x, y, pos, lch, rch;

    int go(int d) const {
        return locate(x, pos + (d > 0 ? 1 : -1));
    }
} e[NMAX + 10];

// layout:
// key: [x1] [x2] [x3] ... [xn] [yroot]
// val: [y1] [y2] [y3] ... [yn] [yn + 1]
static int key[(NMAX + WMAX) * 2 + 10], val[(NMAX + WMAX) * 2 + 10], cur;

int cmp(const Node &u, const Node &v) {
    return u.y < v.y ? -1 : 1;
}

void linkage() {
    for (int i = 1; i <= n; i++) {
        int y = e[i].y;
        e[i].pos = by[y];
        buc[e[i].x]++;
        by[y] = i;
    }

    e[0].y = -INF;
    e[n + 1].y = INF;
    for (int i = 1; i <= WMAX; i++)
        buc[i] += buc[i - 1];
    for (int y = WMAX; y >= 1; y--)
    for (int i = by[y], j = e[i].pos; i; j = e[i = j].pos) {
        int x = e[i].x;
        seq[--buc[x]] = i;
        e[i].pos = buc[x];
        dx[x] = i;
        if (!ux[x]) ux[x] = i;
    }
    buc[WMAX + 1] = n;
}

static u32 A, B;

void initialize() {
#ifdef FAST_IO
    read(n); read(q); read(A); read(B);
#else
    scanf("%d%d%u%u", &n, &q, &A, &B);
#endif

    for (int i = 1; i <= n; i++)
#ifdef FAST_IO
        read(e[i].x), read(e[i].y);
#else
        scanf("%d%d", &e[i].x, &e[i].y);
#endif

    linkage();
}

static int stk[WMAX + 10], t;
static int in[WMAX + 10], ex[WMAX + 10], m, m0;
int build(int d) {
    if (m == 0) return 0;
    t = 0;
    for (int i = 1; i <= m; i++) {
        int u = in[i];
        e[u].lch = ex[i];
        for ( ; t && cmp(e[stk[t - 1]], e[u]) == d; t--) {
            int v = stk[t - 1];
            e[v].rch = e[u].lch;
            e[u].lch = v;
        }
        stk[t++] = u;
    }
    stk[t] = ex[m + 1];
    for (int i = 0; i < t; i++)
        e[stk[i]].rch = stk[i + 1];
    return stk[0];
}

void dfs(int u0, int y) {
    if (!u0) {
        m = 0;
        m0 = 1;
        ex[1] = u0;
        return;
    }

    int *ps = stk + 1, *pi = in, *pe = ex;
    *ps = u0;
    while (ps != stk) {
        int u = *ps;
        if (u > 0) {
            *ps = -u;
            int lc = e[u].lch;
            if (lc && e[lc].y == y)
                *(++ps) = lc;
            else
                *(++pe) = lc;
        } else {
            u = -u;
            *(++pi) = u;

            int rc = e[u].rch;
            if (rc && e[rc].y == y)
                *ps = rc;
            else {
                *(++pe) = rc;
                ps--;
            }
        }
    }

    m = pi - in;
    m0 = pe - ex;
}

void scan(int *bx, int sy, int d) {
    m = 0;
    for (int x = 1; x <= WMAX; x++)
        if (bx[x]) in[++m] = bx[x];
    m0 = m + 1;
    memset(ex + 1, 0, sizeof(int) * m0);
    int tr = build(d);
    int ed = d > 0 ? WMAX : 1;

    for (int y = sy; y != ed + d; y += d)
    if (e[tr].y == y) {
        m = m0 = 0;
        dfs(tr, e[tr].y);
        for (int i = 1; i <= m; i++) {
            key[cur] = e[in[i]].x;
            val[cur++] = e[ex[i]].y;
        }
        key[cur] = e[tr].y;
        val[cur++] = e[ex[m0]].y;
        bx[y] = cur;  // reuse bx

        if (y == ed) break;

        int j = 1, k = 1;
        for (int i = 1; i <= m; i++) {
            int v = e[in[i]].go(d);
            if (1 <= v && v <= n) {
                in[j++] = v;
                ex[k++] = ex[i];
            }
        }
        ex[k] = ex[m + 1];
        m = j - 1;
        tr = build(d);
    } else {
        key[cur] = e[tr].y;
        val[cur++] = -INF;  // e[0].y
        bx[y] = cur;
    }
}

int search(int l, int r, int x, int y) {
    if (key[r] != y) return key[r];
    while (l < r) {
        int mi = (l + r) / 2;
        if (key[mi] < x) l = mi + 1;
        else r = mi;
    }
    return val[l];
}

void print(int u, int space = 0) {
    if (!u) return;
    print(e[u].lch, space + 1);
    for (int i = 0; i < space; i++)
        fputc(' ', stderr);
    fprintf(stderr, "[%d] (%d, %d)\n", u, e[u].x, e[u].y);
    print(e[u].rch, space + 1);
}

int main() {
    initialize();
    scan(dx, 1, 1);
    ux[WMAX + 1] = cur;
    scan(ux, WMAX, -1);

#ifndef NDEBUG
    for (int i = 1; i <= WMAX; i++)
    if (i == 1 || dx[i - 1] + 1 < dx[i]) {
        fprintf(stderr, "suf[%d]: ", i);
        for (int j = dx[i - 1]; j < dx[i] - 1; j++)
            fprintf(stderr, "%d ", key[j]);
        fprintf(stderr, "[%d] / ", key[dx[i] - 1]);
        for (int j = dx[i - 1]; j < dx[i]; j++)  // *
            fprintf(stderr, "%d ", val[j] < 0 ? -1 : val[j]);
        fprintf(stderr, "\n");
    }

    for (int i = WMAX; i >= 1; i--)
    if (i == WMAX || ux[i + 1] + 1 < ux[i]) {
        fprintf(stderr, "pre[%d]: ", i);
        for (int j = ux[i + 1]; j < ux[i] - 1; j++)
            fprintf(stderr, "%d ", key[j]);
        fprintf(stderr, "[%d] / ", key[ux[i] - 1]);
        for (int j = ux[i + 1]; j < ux[i]; j++)  // *
            fprintf(stderr, "%d ", val[j] < 0 ? -1 : val[j]);
        fprintf(stderr, "\n");
    }
#endif

#ifdef TIMER
    auto _t1 = chrono::high_resolution_clock::now();
#endif

    bool ok;
    for (int i = 1; i <= q; i++) {
        int x, y;

#ifdef FAST_IO
        read(x); read(y);
#else
        scanf("%d%d", &x, &y);
#endif

        if (i > 1) {
            if (ok) {
                x ^= A & MASK;
                y ^= A & MASK;
            } else {
                x ^= B & MASK;
                y ^= B & MASK;
            }
        }
        A = magic(A);
        B = magic(B);

        e[n + 2].y = y;
        int k = upper_bound(seq + buc[x], seq + buc[x + 1], n + 2, [](int u, int v) {
            return e[u].y < e[v].y;
        }) - seq;

        int t1 = locate(x, k - 1);
        int y1 = search(ux[y + 1], ux[y] - 1, x, y);
        ok = y1 < 0 || y1 <= e[t1].y;
#ifndef NDEBUG
        fprintf(stderr, "y1 = %d, e[t1: %d].y = %d\n", y1, t1, e[t1].y);
#endif

        if (ok) {
            int t2 = locate(x, k);
            int y2 = search(dx[y - 1], dx[y] - 1, x, y);
            ok = y2 < 0 || y2 >= e[t2].y;
#ifndef NDEBUG
            fprintf(stderr, "y2 = %d, e[t2: %d].y = %d\n", y2, t2, e[t2].y);
#endif
        }

        puts(ok ? "YES" : "NO");
    }

#ifdef TIMER
    auto _t2 = chrono::high_resolution_clock::now();
    auto _t = chrono::duration<double>(_t2 - _t1).count();
    fprintf(stderr, "time = %.3lf\n", _t);
#endif

    /*
    while (true)
        system("sleep 1");
    */

    return 0;
}
