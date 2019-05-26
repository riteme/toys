#include <cstdio>

#include <chrono>
#include <vector>
#include <algorithm>

#include "lct.h"

using std::vector;

inline void chkmax(int &a, int b) {
    if (b > a) a = b;
}

inline void chkmin(int &a, int b) {
    if (b < a) a = b;
}

struct Query {
    int i, x, y;
};

static int n, q, sp[NMAX + 1], ans[NMAX + 1];
static int L[NMAX + 1], R[NMAX + 1], top[NMAX + 1];
static LCT lct;
static vector<Query> queries[NMAX + 1];
static vector<int> ins[NMAX + 1], del[NMAX + 1];
static vector<int> tog[NMAX + 2];

int main() {
    scanf("%d%d", &n, &q);
    int cnt[3] = {1, 1, 0}, op;
    L[1] = 1;
    R[1] = n;
    tog[1].push_back(1);
    for (int i = 0; i < q; i++) {
        scanf("%d", &op);
        switch (op) {
            case 0: {
                int l, r;
                scanf("%d%d", &l, &r);
                top[++cnt[0]] = q + cnt[1];
                tog[l].push_back(cnt[0]);
                tog[r + 1].push_back(cnt[0]);
                L[cnt[0]] = l;
                R[cnt[0]] = r;
            } break;
            case 1: {
                int l, r, x;
                scanf("%d%d%d", &l, &r, &x);
                chkmax(l, L[x]);
                chkmin(r, R[x]);
                if (l > r) continue;
                sp[++cnt[1]] = x;
                ins[l].push_back(cnt[1]);
                del[r].push_back(cnt[1]);
            } break;
            case 2: {
                int x, u, v;
                scanf("%d%d%d", &x, &u, &v);
                queries[x].push_back({++cnt[2], u, v});
            } break;
        }
    }

    // expose + fastcut is slightly slower.
    lct.init(n, q, cnt[0], cnt[1], cnt[2]);  // Initialization time is not taken into account.
    auto t1 = std::chrono::high_resolution_clock::now();

    lct.link(1, q + 1);
    for (int i = 2; i <= cnt[1]; i++) lct.link(q + i - 1, q + i);
    for (int i = 2; i <= cnt[0]; i++) lct.link(top[i], i);
    for (int t = 1; t <= n; t++) {
        for (int i : ins[t]) {
            // lct.expose(q + i - 1);
            // lct.fastcut(q + i);
            lct.cut(q + i);
            lct.link(sp[i], q + i);
        }
        for (int i : tog[t]) lct.toggle(i);
        for (auto &e : queries[t]) {
            // The following 5 lines of code are used to test evert.
            // lct.evert(e.x);
            // lct.expose((e.x + e.y) >> 1);
            // lct.evert(e.y);
            // lct.expose(996);
            // lct.evert(1);
            ans[e.i] = lct.count(e.x, e.y);
        }
        for (int i : del[t]) {
            // lct.expose(sp[i]);
            // lct.fastcut(q + i);
            lct.cut(q + i);
            lct.link(q + i - 1, q + i);
        }
    }

    auto t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> t = t2 - t1;

    fprintf(stderr, "%.6lf\n", t.count());
    for (int i = 1; i <= cnt[2]; i++) printf("%d\n", ans[i]);
    return 0;
}