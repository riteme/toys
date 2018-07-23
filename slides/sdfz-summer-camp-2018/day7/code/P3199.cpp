#include <cstdio>
#include <cstring>
#include <climits>

#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#define NMAX 3000
#define WMAX 1e7
#define EPS 1e-8

struct Edge {
    Edge(int _v, double _w) : v(_v), w(_w) {}

    int v;
    double w;
};

static int n, m;
static double ans;
static vector<Edge *> G[NMAX + 10];

void initialize() {
    scanf("%d%d", &n, &m);

    for (int i = 0; i < m; i++) {
        int u, v;
        double w;
        scanf("%d%d%lf", &u, &v, &w);
        G[u].push_back(new Edge(v, w));
    }
}

bool spfa() {
    static int cnt[NMAX + 10];
    static double dist[NMAX + 10];
    static bool exist[NMAX + 10];

    memset(cnt, 0, sizeof(cnt));
    queue<int> q;
    for (int i = 1; i <= n; i++) {
        dist[i] = 0;
        exist[i] = true;
        q.push(i);
    }

    while (!q.empty()) {
        int u = q.front();
        exist[u] = false;
        q.pop();

        for (auto e : G[u]) {
            int v = e->v;
            double w = e->w - ans;
            if (dist[v] > dist[u] + w) {
                dist[v] = dist[u] + w;
                cnt[v]++;
                if (cnt[v] == n) return true;
                if (!exist[v]) {
                    exist[v] = true;
                    q.push(v);
                }
            }
        }
    }

    return false;
}

int main() {
    initialize();

    double l = -WMAX, r = WMAX;
    while (r - l > EPS) {
        ans = (l + r) * 0.5;
        if (spfa()) r = ans;
        else l = ans;
    }

    printf("%.8lf\n", (l + r) * 0.5);

    return 0;
}
