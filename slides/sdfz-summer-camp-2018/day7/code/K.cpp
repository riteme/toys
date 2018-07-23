#include <cstdio>
#include <cstring>
#include <climits>

#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#define NMAX 200000
#define RMAX 100
#define VER 0
#define HOR 1
#define INF 0x3f3f3f3f

struct Edge {
    Edge(int _v, int _w) : v(_v), w(_w) {}
    int v, w;
};

static vector<Edge *> G[NMAX];
static int R, C, s1, t1, s2, t2, s, t;
static int lt[RMAX + 10][RMAX + 10], rt[RMAX + 10][RMAX + 10];
static int up[RMAX + 10][RMAX + 10], dw[RMAX + 10][RMAX + 10];

inline int id(int x, int y, int d) {
    if (x < 1 || x > R || y < 1 || y > C) return 0;
    x--; y--;
    return R * C * d + C * x + y + 1;
}

inline void link(int u, int v, int w) {
    if (!u || !v || !w) return;
    printf("%d - %d: %d\n", u, v, w);
    G[u].push_back(new Edge(v, w));
    G[v].push_back(new Edge(u, w));
}

bool initialize() {
    scanf("%d%d%d%d%d%d", &R, &C, &s1, &t1, &s2, &t2);
    if (R == 0) return false;
    s = id(R, C, 1) + 1; t = s + 1;

    int w;
    for (int i = 1; i < R; i++) {
        for (int j = 1; j < C; j++) {
            scanf("%d", &w);
            rt[i][j] = lt[i][j + 1] = w;
        }

        for (int j = 1; j <= C; j++) {
            scanf("%d", &w);
            dw[i][j] = up[i + 1][j] = w;
        }
    }

    for (int j = 1; j < C; j++) {
        scanf("%d", &w);
        rt[R][j] = lt[R][j + 1] = w;
    }

    return true;
}

void finalize() {
    for (int i = 1; i <= t; i++)
        G[i].clear();
}

void setup() {
    for (int x = 1; x <= R; x++) for (int y = 1; y <= C; y++) {
        if (y < C) link(id(x, y, HOR), id(x, y + 1, HOR), rt[x][y]);
        if (x < R) link(id(x, y, VER), id(x + 1, y, VER), dw[x][y]);
        link(id(x, y, VER), id(x + 1, y + 1, HOR), 2 * (dw[x][y] + rt[x + 1][y]));
        link(id(x, y, VER), id(x + 1, y - 1, HOR), 2 * (dw[x][y] + lt[x + 1][y]));
        link(id(x, y, VER), id(x - 1, y + 1, HOR), 2 * (up[x][y] + rt[x - 1][y]));
        link(id(x, y, VER), id(x - 1, y - 1, HOR), 2 * (up[x][y] + lt[x - 1][y]));
    }

    link(s, id(s1 + 1, t1, VER), 2 * dw[s1][t1]);
    link(s, id(s1 - 1, t1, VER), 2 * up[s1][t1]);
    link(s, id(s1, t1 + 1, HOR), 2 * rt[s1][t1]);
    link(s, id(s1, t1 - 1, HOR), 2 * lt[s1][t1]);

    link(t, id(s2 + 1, t2, VER), 2 * dw[s2][t2]);
    link(t, id(s2 - 1, t2, VER), 2 * up[s2][t2]);
    link(t, id(s2, t2 + 1, HOR), 2 * rt[s2][t2]);
    link(t, id(s2, t2 - 1, HOR), 2 * lt[s2][t2]);

    if (s1 + 1 == s2 && t1 == t2) link(s, t, 2 * dw[s1][t1]);
    if (s1 - 1 == s2 && t1 == t2) link(s, t, 2 * up[s1][t1]);
    if (t1 + 1 == t2 && s1 == s2) link(s, t, 2 * rt[s1][t1]);
    if (t1 - 1 == t2 && s1 == s2) link(s, t, 2 * lt[s1][t1]);
}

static int dist[NMAX + 10];

void dijkstra() {
    struct State {
        State(int _u, int _t) : u(_u), t(_t) {}

        int u, t;

        bool operator<(const State &b) const {
            return t > b.t;
        }
    };

    memset(dist, 0x3f, sizeof(dist));
    dist[s] = 0;
    priority_queue<State> q;
    q.push(State(s, 0));
    while (!q.empty()) {
        State s = q.top();
        q.pop();

        int u = s.u;
        if (s.t > dist[u]) continue;

        for (int i = 0; i < G[u].size(); i++) {
            Edge *e = G[u][i];
            int v = e->v;
            if (dist[v] > dist[u] + e->w) {
                dist[v] = dist[u] + e->w;
                q.push(State(v, dist[v]));
            }
        }
    }
}

int main() {
    int t = 1;
    while (initialize()) {
        setup();

        dijkstra();
        printf("Case %d: ", t++);
        if (dist[t] >= INF) puts("Impossible");
        else printf("%d\n", dist[t]);

        finalize();
    }

    return 0;
}
