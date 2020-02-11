#include <cstdio>
#include <cstring>

#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#define NMAX 100000

template <typename T>
struct Heap {
    Heap() {}

    priority_queue<T> Q;

    void pop() {
        Q.pop();
    }

    void push(const T &x) {
        Q.push(x);
    }

    T top() {
        return Q.top();
    }

    bool empty() const {
        return Q.empty();
    }
};

struct Edge {
    int v, w;
};

static int n, m, s;
static int dist[NMAX + 10];
static vector<Edge> G[NMAX + 10];

void shortest() {
    struct State {
        int u, t;

        bool operator<(const State &z) const {
            return t > z.t;
        }
    };

    Heap<State> q;
    memset(dist, 0x3f, sizeof(dist));
    dist[s] = 0;
    q.push({s, 0});
    while (!q.empty()) {
        auto s = q.top();
        q.pop();
        if (dist[s.u] < s.t) continue;
        int u = s.u;
        for (auto &e : G[u]) {
            int v = e.v;
            if (dist[v] > dist[u] + e.w) {
                dist[v] = dist[u] + e.w;
                q.push({v, dist[v]});
            }
        }
    }
}

int main() {
    scanf("%d%d%d", &n, &m, &s);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        G[u].push_back({v, w});
    }
    shortest();
    for (int i = 1; i <= n; i++)
        printf("%d ", dist[i]);
    return 0;
}
