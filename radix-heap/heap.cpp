#include <cctype>
#include <cstdio>
#include <cstring>

#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

#define BUFFERSIZE 65536
static size_t _pos = BUFFERSIZE;
static char _buf[BUFFERSIZE];

inline void getc(char &c) {
    if (_pos == BUFFERSIZE) {
        _pos = 0;
        fread(_buf, 1, BUFFERSIZE, stdin);
    }
    c = _buf[_pos++];
}

template <typename T>
inline void read(T &x) {
    x = 0;
    char c;
    do getc(c); while (!isdigit(c));
    do {
        x = x * 10 + (c - '0');
        getc(c);
    } while (isdigit(c));
}

#define NMAX 500000
#define INF 0x3f3f3f3f

struct Edge {
    Edge(int _v, int _w) : v(_v), w(_w) {}
    int v, w;
};

static int n, m, s;
static vector<Edge *> G[NMAX + 10];
static int dist[NMAX + 10];

void dijkstra() {
    struct State {
        State(int _u, int _t) : u(_u), t(_t) {}
        int u, t;

        bool operator<(const State &b) const {
            return t > b.t;
        }
    };

    priority_queue<State> q;
    memset(dist, 0x3f, sizeof(dist));
    dist[s] = 0;
    q.push(State(s, 0));
    while (!q.empty()) {
        State s = q.top();
        q.pop();
        if (s.t > dist[s.u]) continue;
        for (auto e : G[s.u]) {
            if (dist[e->v] > s.t + e->w) {
                dist[e->v] = s.t + e->w;
                q.push(State(e->v, dist[e->v]));
            }
        }
    }
}

int main() {
    read(n); read(m); read(s);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        read(u); read(v); read(w);
        G[u].push_back(new Edge(v, w));
    }

    dijkstra();
    for (int i = 1; i <= n; i++) printf("%d ", dist[i]);

    return 0;
}
