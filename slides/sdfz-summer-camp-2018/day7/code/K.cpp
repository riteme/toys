#include <cstdio>
#include <cstring>
#include <climits>

#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#define NMAX 50000
#define RMAX 100
#define LEFT 0
#define RIGHT 1
#define UP 2
#define DOWN 3
#define STOP 4
#define INF 0x3f3f3f3f

struct Edge {
    Edge(int _v, int _w) : v(_v), w(_w) {}
    int v, w;
};

static vector<Edge *> G[NMAX];
static int R, C, s1, t1, s2, t2, s, t;
static int L[4][RMAX + 10][RMAX + 10];

inline int id(int x, int y, int m) {
    if (x < 1 || x > R || y < 1 || y > C) return 0;
    x--; y--;
    return R * C * m + C * x + y + 1;
}

bool initialize() {
    scanf("%d%d%d%d%d%d", &R, &C, &s1, &t1, &s2, &t2);
    if (R == 0) return false;
    s = id(R, C, STOP) + 1;
    t = s + 1;

    int w;
    memset(L, 0, sizeof(L));
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
}

void setup() {
    for (int i = 1; i <= R; i++) for (int j = 1; j <= C; j++) {
        link(id(i, j, LEFT), id(i, j - 1, LEFT), L[LEFT][i][j]);
        link(id(i, j, RIGHT), id(i, j + 1, RIGHT), L[RIGHT][i][j]);
        link(id(i, j, UP), id(i - 1, j, UP), L[UP][i][j]);
        link(id(i, j, DOWN), id(i + 1, j, DOWN), L[DOWN][i][j]);

        link(id(i, j, LEFT), id(i, j, STOP), L[LEFT][i][j + 1]);
        link(id(i, j, RIGHT), id(i, j, STOP), L[RIGHT][i][j - 1]);
        link(id(i, j, UP), id(i, j, STOP), L[UP][i + 1][j]);
        link(id(i, j, DOWN), id(i, j, STOP), L[DOWN][i - 1][j]);

        for (int m = 0; m < 4; m++)
            link(id(i, j, STOP), id(i, j, m), L[m][i][j]);
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
