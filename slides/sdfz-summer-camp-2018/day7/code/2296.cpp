#include <cstdio>
#include <cstring>
#include <climits>

#include <queue>
#include <algorithm>

using namespace std;

#define NMAX 10000
#define INF 0x3f3f3f3f

static int n, m, s, t;
static int G[NMAX + 10][NMAX + 10];
static bool chk1[NMAX + 10], chk2[NMAX + 10], vis[NMAX + 10];
static int dist[NMAX + 10];

void initialize() {
    scanf("%d%d", &n, &m);
    for (int i = 0; i < m; i++) {
        int u, v;
        scanf("%d%d", &u, &v);
        G[u][v] = 1;
    }
    scanf("%d%d", &s, &t);
}

void dfs(int x) {
    chk1[x] = true;
    for (int v = 1; v <= n; v++) {
        if (G[v][x] && !chk1[v])
            dfs(v);
    }
}

void bfs() {
    memset(dist, 0x3f, sizeof(dist));
    vis[s] = true;
    dist[s] = 0;
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v = 1; v <= n; v++) {
            if (G[u][v] && chk2[v] && !vis[v]) {
                vis[v] = true;
                dist[v] = dist[u] + 1;
                q.push(v);
            }
        }
    }
}

int main() {
    initialize();

    dfs(t);

    memcpy(chk2, chk1, sizeof(chk2));
    for (int i = 1; i <= n; i++) {
        if (!chk1[i]) continue;
        for (int v = 1; v <= n; v++) {
            if (G[i][v] && !chk1[v]) {
                chk2[i] = false;
                break;
            }
        }
    }

    bfs();
    if (dist[t] < INF) printf("%d\n", dist[t]);
    else puts("-1");

    return 0;
}
