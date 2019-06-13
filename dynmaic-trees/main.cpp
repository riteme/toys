#include <cstring>

#include <chrono>
#include <vector>
#include <iostream>
#include <algorithm>

#include "lct.h"

using namespace std;

#define NMAX 1048576
#define MMAX (NMAX * 8)

typedef long long i64;

static int n, m;
static struct Edge {
    int u, v, w;
} E[MMAX];
static i64 ans;
static Allocator *mem;
static LCT lct;
static Edge *ptr[NMAX + 1];

inline void mst() {
    mem->reset(1, n);
    lct.init(n * 2);
    ans = 0;
    for (int i = 0; i < m; i++) {
        auto &e = E[i];
        int p = lct.query(e.u, e.v), t;
        if (p && lct.get(p) <= e.w) continue;
        if (p) {
            t = p - n;
            lct.evert(p);
            lct.expose(p);
            lct.fastcut(ptr[t]->u);
            lct.fastcut(ptr[t]->v);
            mem->release(t);
            ans -= ptr[t]->w;
        }
        t = mem->alloc();
        ptr[t] = &e;
        p = n + t;
        lct.set(p, e.w);
        lct.link(e.u, p);
        lct.evert(p);
        lct.link(e.v, p);
        ans += e.w;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) return fprintf(stderr, "Usage: %s [T]\n", argv[0]), -1;
    int T = atoi(argv[1]);

    ios::sync_with_stdio(false);
    cin >> n >> m;
    mem = new Allocator(n);
    for (int i = 0; i < m; i++) cin >> E[i].u >> E[i].v >> E[i].w;

    mst();
    int cnt = T;
    auto t1 = chrono::high_resolution_clock::now();
    while (cnt--) mst();
    auto t2 = chrono::high_resolution_clock::now();
    chrono::duration<double> t = t2 - t1;
    auto aver = t.count() / (1ll * T * m);
    cerr << aver << endl;
    cout << ans  << endl;

    delete mem;
    return 0;
}