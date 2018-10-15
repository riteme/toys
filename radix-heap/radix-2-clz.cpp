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
#define BUCSIZE 32

struct Edge {
    Edge(int _v, int _w) : v(_v), w(_w) {}
    int v, w;
};

static int n, m, s;
static vector<Edge *> G[NMAX + 10];
static int dist[NMAX + 10];
static int last, buc[32], nxt[NMAX + 10], pre[NMAX + 10];

#define ID(x) fhs(last ^ dist[x])
inline int fhs(unsigned x) {
    return x ? sizeof(unsigned) * 8 - __builtin_clz(x) : 0;
}

inline void push(int x) {
    int &h = buc[ID(x)];
    nxt[x] = h;
    pre[x] = 0;
    pre[h] = x;
    h = x;
}

inline int pop(int x) {
    if (nxt[x]) pre[nxt[x]] = pre[x];
    if (pre[x]) nxt[pre[x]] = nxt[x];
    else buc[ID(x)] = nxt[x];
    return x;
}

inline void decrease(int x, int v) {
    pop(x);
    dist[x] = v;
    push(x);
}

inline int extract() {
    static int tmp[NMAX + 10];
    if (buc[0]) return pop(buc[0]);
    else {
        int i, p = 0, r = 0;  // dist[0] = INF initially
        for (i = 1; i < BUCSIZE && !buc[i]; i++) ;
        for (int x = buc[i]; x; x = nxt[x]) {
            tmp[p++] = x;
            pop(x);
            if (dist[x] <= dist[r]) r = x;
        }
        last = dist[r];
        for (int i = 0; i < p; i++) if (tmp[i] != r) push(tmp[i]);
        return r;
    }
}

void dijkstra() {
    last = 0;
    memset(buc, 0, sizeof(buc));
    memset(dist, 0x3f, sizeof(dist));
    for (int i = 1; i <= n; i++) push(i);
    decrease(s, 0);
    for (int i = 0; i < n; i++) {
        int u = extract();
        for (auto e : G[u])
            if (dist[e->v] > dist[u] + e->w) decrease(e->v, dist[u] + e->w);
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
