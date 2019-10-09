#define NDEBUG

#include <cstdio>
#include <cstring>

#include <vector>
#include <algorithm>

using namespace std;

#define NMAX 100000
#define KMAX 15
#define MOD 998244353

typedef long long i64;

inline void add(i64 &a, i64 b) {
    a += b;
    if (a >= MOD) a -= MOD;
}

static int n, m, K;
static vector<int> G[NMAX + 10];
static int dfn[NMAX + 10], low[NMAX + 10], now;
static int ed[NMAX + 10], fa[NMAX + 10];
static int a[NMAX + 10], cnt;
static i64 f[NMAX + 10][KMAX + 1];
static i64 g[KMAX + 1], h[KMAX + 1];

inline void dp(i64 *a, i64 *b, bool t = false) {
    i64 sum = 0;
    if (t) for (int j = 0; j <= K; j++) add(sum, a[j]);
    for (int j = K; j >= 2; j--) {
        a[j] = 0;
        for (int k = 1; k < j; k++) add(a[j], a[j - k] * b[k] % MOD);
    }
    a[1] = 0;
    if (t) for (int j = 1; j <= K; j++) add(a[j], sum * b[j] % MOD);
}

void dfs(int x) {
    dfn[x] = low[x] = ++now;
    memset(f[x], 0, sizeof(f[x]));
    f[x][1] = 1;
    for (int v : G[x]) if (v != fa[x]) {
        if (!dfn[v]) {
            fa[v] = x;
            dfs(v);
            low[x] = min(low[x], low[v]);
            if (low[v] > dfn[x]) {
#ifndef NDEBUG
                fprintf(stderr, "%d -> %d\n", x, v);
#endif

                memcpy(g, f[x], sizeof(g));
                i64 sum = 0;
                for (int j = 1; j <= K; j++) add(sum, f[v][j]);
                for (int i = K; i >= 2; i--) {
                    f[x][i] = f[x][i] * sum % MOD;
                    for (int j = 1; j < i; j++)
                        add(f[x][i], f[x][i - j] * f[v][j] % MOD);
                }
                f[x][1] = f[x][1] * sum % MOD;

#ifndef NDEBUG
                fprintf(stderr, "f[%d]\n", x);
                for (int i = 0; i <= KMAX; i++) fprintf(stderr, "%lld ", f[x][i]);
                fprintf(stderr, "\n");
#endif
            } else if (low[v] == dfn[x]) {
                //a[1] = x;
                for (cnt = 1, v = ed[x]; v != x; v = fa[v]) a[++cnt] = v;

#ifndef NDEBUG
                for (int i = 1; i <= cnt; i++) fprintf(stderr, "%d ", a[i]);
                fprintf(stderr, "\n");
#endif
                memset(h, 0, sizeof(h));
                for (int s = 1; s <= K && s <= cnt; s++) {
                    if (s < cnt) {
                        memcpy(g, f[a[s + 1]], sizeof(g));
                        for (int i = s + 2; i <= cnt; i++) dp(g, f[a[i]], true);
                    } else {
                        memset(g, 0, sizeof(g));
                        g[0] = 1;
                    }
                    dp(g, f[x], true);
                    for (int i = 2; i <= s; i++) dp(g, f[a[i]]);
                    for (int j = 1; j <= K; j++) add(h[j], g[j]);
                }
                memcpy(f[x], h, sizeof(h));

#ifndef NDEBUG
                fprintf(stderr, "f[%d]\n", x);
                for (int i = 0; i <= KMAX; i++) fprintf(stderr, "%lld ", f[x][i]);
                fprintf(stderr, "\n");
#endif
            }
        } else {
            ed[v] = x;
            low[x] = min(low[x], dfn[v]);
        }
    }
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d%d", &n, &m, &K);
        for (int i = 1; i <= n; i++) G[i].clear();
        now = 0;
        memset(dfn + 1, 0, sizeof(int) * n);
        //memset(low + 1, 0, sizeof(int) * n);
        memset(fa + 1, 0, sizeof(int) * n);
        for (int i = 0; i < m; i++) {
            int u, v;
            scanf("%d%d", &u, &v);
            G[u].push_back(v);
            G[v].push_back(u);
        }
        i64 ans = 1;
        for (int i = 1; i <= n; i++) if (!dfn[i]) {
            dfs(i);
            i64 sum = 0;
            for (int j = 1; j <= K; j++) add(sum, f[i][j]);
            ans = ans * sum % MOD;
        }
        printf("%lld\n", ans);
    }

    return 0;
}
