#include <cstdio>
#include <cstring>

#include <vector>
#include <algorithm>

using namespace std;

#define NMAX 300000
#define MOD 998244353

typedef long long i64;

static int n, m;
static vector<int> G[NMAX + 10];
static int dfn[NMAX + 10], low[NMAX + 10], now;
static int ed[NMAX + 10], dep[NMAX + 10];
static i64 pw[NMAX + 10], ans;

void dfs(int x, int fa) {
    dfn[x] = low[x] = ++now;
    for (int v : G[x]) if (v != fa) {
        if (!dfn[v]) {
            dep[v] = dep[x] + 1;
            dfs(v, x);
            low[x] = min(low[x], low[v]);
            if (low[v] > dfn[x])
                ans = ans * 2 % MOD;
            else if (low[v] == dfn[x])
                ans = ans * (pw[dep[ed[x]] - dep[x] + 1] - 1) % MOD;
        } else {
            ed[v] = x;
            low[x] = min(low[x], dfn[v]);
        }
    }
}

int main() {
    pw[0] = 1;
    for (int i = 1; i <= NMAX; i++) pw[i] = pw[i - 1] * 2 % MOD;
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d", &n, &m);
        for (int i = 1; i <= n; i++) G[i].clear();
        now = 0;
        memset(dfn + 1, 0, sizeof(int) * n);
        //memset(dep + 1, 0, sizeof(int) * n);
        for (int i = 0; i < m; i++) {
            int u, v;
            scanf("%d%d", &u, &v);
            G[u].push_back(v);
            G[v].push_back(u);
        }
        ans = 1;
        for (int i = 1; i <= n; i++)
            if (!dfn[i]) dfs(i, 0);
        if (ans < 0) ans += MOD;
        printf("%lld\n", ans);
    }

    return 0;
}
