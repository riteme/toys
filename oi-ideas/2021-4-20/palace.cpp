#include <cstdio>
#include <cstring>

#include <vector>
#include <algorithm>

using namespace std;

using i64 = long long;

constexpr int NMAX = 200000;
constexpr i64 INF = 0x3f3f3f3f3f3f3f3f;
constexpr int KMAX = 100;

int n, K;
i64 w[NMAX + 10];
vector<int> G[NMAX + 10];
int sz[NMAX + 10];
i64 f[NMAX + 10][KMAX + 1];
int h[NMAX + 10][KMAX + 1];
i64 g[KMAX + 1];
struct {
    int u, v;
} gs[KMAX + 1];
double ans[KMAX + 10];

void dfs(int x, int p) {
    for (int i = 1; i <= KMAX; i++) {
        f[x][i] = -INF;
    }

    f[x][1] = w[x];
    h[x][1] = x;
    sz[x] = 1;

    for (int v : G[x]) if (v != p) {
        dfs(v, x);

        for (int i = 1; i <= sz[x] && i <= KMAX; i++)
        for (int j = 1; j <= sz[v] && j <= KMAX; j++) {
            if (i + j > KMAX)
                break;

            // g[i + j] = max(g[i + j], f[x][i] + f[v][j]);
            if (g[i + j] < f[x][i] + f[v][j]) {
                g[i + j] = f[x][i] + f[v][j];
                gs[i + j] = {h[x][i], h[v][j]};
            }
        };

        for (int j = 1; j <= sz[v] && j < KMAX; j++) {
            // f[x][j + 1] = max(f[x][j + 1], w[x] + f[v][j]);
            if (f[x][j + 1] < w[x] + f[v][j]) {
                f[x][j + 1] = w[x] + f[v][j];
                h[x][j + 1] = h[v][j];
            }
        }

        sz[x] += sz[v];
    }

    for (int i = 1; i <= sz[x] && i <= KMAX; i++) {
        // g[i] = max(g[i], f[x][i]);
        if (g[i] < f[x][i]) {
            g[i] = f[x][i];
            gs[i] = {x, h[x][i]};
        }
    }
}

int main() {
    int T;
    scanf("%d", &T);
    for (int i = 0; i < T; i++) {
        scanf("%d%d", &n, &K);
        for (int i = 1; i <= n; i++) {
            scanf("%lld", w + i);
            G[i].clear();
        }

        for (int i = 1; i < n; i++) {
            int u, v;
            scanf("%d%d", &u, &v);
            G[u].push_back(v);
            G[v].push_back(u);
        }

        memset(g, 0, sizeof(g));
        memset(gs, 0, sizeof(gs));
        dfs(1, 0);

        for (int i = KMAX; i >= 1; i--) {
            ans[i] = g[i] / double(i);
            ans[i] = max(ans[i], ans[i + 1]);

            fprintf(stderr, "[k=%d] u=%d, v=%d\n", i, gs[i].u, gs[i].v);
        }

        for (int i = 1; i <= K; i++) {
            printf("%.5lf ", ans[i]);
        }
        puts("");
    }

    return 0;
}