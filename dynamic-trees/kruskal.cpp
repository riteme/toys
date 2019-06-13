#include <cstring>

#include <chrono>
#include <iostream>
#include <algorithm>

using namespace std;

#define NMAX 1048576
#define MMAX (NMAX * 8)

typedef long long i64;

static int n, m;
static struct Edge {
    int u, v, w;

    bool operator<(const Edge &e) const {
        return w < e.w;
    }
} e[MMAX];

static i64 ans;
static int fa[NMAX + 1];

static int find(int x) {
    return fa[x] ? fa[x] = find(fa[x]) : x;
}

inline void kruskal() {
    memset(fa + 1, 0, sizeof(int) * n);
    sort(e, e + m);
    ans = 0;
    for (int i = 0; i < m; i++) {
        int u = find(e[i].u), v = find(e[i].v);
        if (u == v) continue;
        fa[u] = v;
        ans += e[i].w;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) return fprintf(stderr, "Usage: %s [T]\n", argv[0]), -1;
    int T = atoi(argv[1]);

    ios::sync_with_stdio(false);
    cin >> n >> m;
    for (int i = 0; i < m; i++) cin >> e[i].u >> e[i].v >> e[i].w;

    if (T == 0) {
        kruskal();
        cout << ans << endl;
        return 0;
    }
    kruskal();  // warm up cache
    int cnt = T;
    auto t1 = chrono::high_resolution_clock::now();
    while (cnt--) kruskal();
    auto t2 = chrono::high_resolution_clock::now();

    chrono::duration<double> t = t2 - t1;
    double aver = t.count() / (1ll * T * m);
    cerr << aver << endl;
    cout << ans  << endl;

    return 0;
}