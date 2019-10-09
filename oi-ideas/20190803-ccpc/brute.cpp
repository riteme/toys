#include <cstdio>
#include <cstring>

#include <algorithm>

#define NMAX 25

static struct Edge {
    int v, id, nxt;
} e[2 * NMAX + 10];
static int n, m, K;
static int head[NMAX + 10], size[NMAX + 10];
static bool mark[NMAX + 10];

bool dfs(int x, int fa, int s) {
    mark[x] = true;
    size[x] = 1;
    for (int i = head[x]; i; i = e[i].nxt) if ((s >> e[i].id) & 1) {
        int v = e[i].v;
        if (v == fa) continue;
        if (mark[v] || !dfs(v, x, s)) return false;
        size[x] += size[v];
    }
    return true;
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d%d%d", &n, &m, &K);
        memset(head, 0, sizeof(head));
        for (int i = 0; i < m; i++) {
            int u, v;
            scanf("%d%d", &u, &v);
            e[2 * i + 2] = {v, i, head[u]};
            e[2 * i + 3] = {u, i, head[v]};
            head[u] = 2 * i + 2;
            head[v] = 2 * i + 3;
        }
        int ans = 0;
        for (int s = 0; s < (1 << m); s++) {
            memset(mark, 0, sizeof(mark));
            bool ok = true;
            for (int i = 1; ok && i <= n; i++) if (!mark[i])
                ok &= dfs(i, 0, s) && size[i] <= K;
            ans += ok;
        }
        printf("%d\n", ans);
    }
    return 0;
}
