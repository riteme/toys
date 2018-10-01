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
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define INF 0x3f3f3f3f

struct Edge {
    Edge(int _v, int _w) : v(_v), w(_w) {}
    int v, w;
};

static vector<Edge *> G[NMAX];
static int R, C, s1, t1, s2, t2, s, t;
static int L[4][RMAX + 10][RMAX + 10];

inline int id(int x, int y, int d) {
    if (x < 1 || x > R || y < 1 || y > C) return 0;
    x--; y--;
    return R * C * d + C * x + y + 1;
}

bool initialize() {
    memset(L, 0, sizeof(L));
    scanf("%d%d%d%d%d%d", &R, &C, &s1, &t1, &s2, &t2);
    if (R == 0) return false;
    s = id(R, C, 1) + 1; t = s + 1;

    int w;
    for (int i = 1; i < R; i++) {
        for (int j = 1; j < C; j++) {
            scanf("%d", &w);
            L[RIGHT][i][j] = L[LEFT][i][j + 1] = w;
        }

        for (int j = 1; j <= C; j++) {
            scanf("%d", &w);
            L[DOWN][i][j] = L[UP][i + 1][j] = w;
        }
    }

    for (int j = 1; j < C; j++) {
        scanf("%d", &w);
        L[RIGHT][R][j] = L[LEFT][R][j + 1] = w;
    }

    return true;
}

void finalize() {
    for (int i = 1; i <= t; i++)
        G[i].clear();
}

inline void link(int u, int v, int w) {
    if (!u || !v || !w) return;
    // printf("%d %d %d\n", u, v, w);
    G[u].push_back(new Edge(v, w));
    G[v].push_back(new Edge(u, w));
}

inline int len(int x, int y, char d) {
    switch (d) {
        case 'L': return L[LEFT][x][y];
        case 'R': return L[RIGHT][x][y];
        case 'U': return L[UP][x][y];
        case 'D': return L[DOWN][x][y];
    }
}

inline int dx(char d) {
    switch (d) {
        case 'L':
        case 'R': return 0;
        case 'U': return -1;
        case 'D': return 1;
    }
}

inline int dy(char d) {
    switch (d) {
        case 'U':
        case 'D': return 0;
        case 'L': return -1;
        case 'R': return 1;
    }
}

inline int dir(char d) {
    switch (d) {
        case 'L':
        case 'R': return HOR;
        case 'U':
        case 'D': return VER;
    }
}

void qway(int sx, int sy, const char *D,
          bool ssp = false, bool tsp = false, int factor = 2) {
    char d;
    int sum = 0;
    int x = sx, y = sy;

    for (int i = 0; D[i]; i++) {
        d = D[i];
        int w = len(x, y, d);
        if (!w) return;
        sum += w;
        x += dx(d);
        y += dy(d);
    }

    if (tsp && (x != s2 || y != t2)) return;
    int idx = ssp ? s : id(sx, sy, dir(D[0]));
    int idy = tsp ? t : id(x, y, dir(d));
    link(idx, idy, sum * factor);
}

void setup() {
    for (int x = 1; x <= R; x++) for (int y = 1; y <= C; y++) {
        if (y < C) qway(x, y, "R", false, false, 1);
        if (x < R) qway(x, y, "D", false, false, 1);
        qway(x, y, "UL");
        qway(x, y, "UR");
        qway(x, y, "DL");
        qway(x, y, "DR");
    }

    const char dirs[] = {'L', 'R', 'U', 'D'};
    char path[3] = {0};
    for (int i = 0; i < 4; i++) {
        path[0] = dirs[i];
        path[1] = 0;

        qway(s1, t1, path, true);
        qway(s1, t1, path, true, true);
        for (int x = 1; x <= R; x++) for (int y = 1; y <= C; y++)
            qway(x, y, path, false, true);

        for (int j = 0; j < 4; j++) {
            path[1] = dirs[j];
            qway(s1, t1, path, true);
            qway(s1, t1, path, true, true);
            for (int x = 1; x <= R; x++) for (int y = 1; y <= C; y++)
                qway(x, y, path, false, true);
        }
    }
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
    int T = 1;
    while (initialize()) {
        setup();

        dijkstra();
        printf("Case %d: ", T++);
        if (dist[t] >= INF) puts("Impossible");
        else printf("%d\n", dist[t]);

        finalize();
    }

    return 0;
}
