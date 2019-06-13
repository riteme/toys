/**
 * @file diameter-gen.cpp
 * @author riteme (riteme@qq.com)
 * @brief
 * @version 0.1
 * @date 2019-06-08
 *
 * @copyright Copyright (c) 2019
 *
 * TODO:
 *   1. LCT
 *   2. shuffle
 */

#include <cstring>

#include <random>
#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

#define NMAX 1000000

struct Edge {
    int v, w;
};

static vector<Edge> G[NMAX + 1];
static int seq[NMAX];

inline void chkmax(int &a, int b) {
    if (b > a) a = b;
}

static int dlen(int x, int fa = 0, int d = 1) {
    int r = d;
    for (auto &e : G[x]) if (e.v != fa)
        chkmax(r, dlen(e.v, x, d + 1));
    return r;
}

static int dmax(int x, int t, int fa = 0) {
    if (x == t) return -1;
    for (auto &e : G[x]) if (e.v != fa) {
        int r = dmax(e.v, t, x);
        if (r) return max(r, e.w);
    }
    return 0;
}

inline void link(int u, int v, int w) {
    printf("%d %d %d\n", u, v, w);
    G[u].push_back({v, w});
    G[v].push_back({u, w});
}

int main(int argc, char *argv[]) {
    if (argc < 4) return fprintf(stderr, "Usage: %s [n] [m] [K] [d]\n", argv[0]), -1;
    int n = atoi(argv[1]), m = atoi(argv[2]), K = atoi(argv[3]), d = atoi(argv[4]);

    random_device rd;
    srand(rd());
    for (int i = 0; i < n; i++) seq[i] = i + 1;
    // random_shuffle(seq, seq + n);

    int t = 1;
    printf("%d %d\n", n, m);

    while (t < n) {
        int p = rand() % t;
        int l = dlen(seq[p]);
        if (d == l) continue;
        link(seq[p], seq[t], rand() % (K - 3) + 1);
        int c = min(n - t, d - l);
        for (int i = 0; i < c - 1; i++)
            link(seq[t + i], seq[t + i + 1], rand() % (K - 3) + 1);
        t += c;
    }

    for (int c = 0; c < m - n + 1; c++) {
        if (c % 10000 == 0) srand(rd());
        int u = rand() % n + 1;
        int v;
        do {
            v = rand() % n + 1;
        } while (u == v);
        // int mx = dmax(u, v);
        printf("%d %d %d\n", u, v, rand() % K + 1);
    }

    return 0;
}