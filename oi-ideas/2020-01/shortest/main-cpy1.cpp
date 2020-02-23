// #define NDEBUG

#include <cassert>
#include <cstdio>
#include <cctype>
#include <cstring>

#include <stack>
#include <queue>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std;
using namespace std::chrono;

#ifndef NDEBUG
#define DEBUG(message, ...) fprintf(stderr, "⇒ " message "\n", __VA_ARGS__);
#define DPUTS(message) fputs(message "\n", stderr);
#else
#define DEBUG(...)
#define DPUTS(...)
#endif

typedef long long i64;

constexpr i64 INF = 0x3f3f3f3f3f3f3f3f;

struct Reader {
    static constexpr int BUFFERSIZE = 65536;

    size_t pos;
    char *buffer;

    Reader() : pos(0), buffer(new char[BUFFERSIZE]) {}
    ~Reader() { delete[] buffer; }

    char getc() {
        if (pos == BUFFERSIZE) {
            pos = 0;
            auto cnt = fread(buffer, 1, BUFFERSIZE, stdin);
            if (cnt < BUFFERSIZE)
                buffer[cnt] = 0;
        }
        return buffer[pos++];
    }

    template <typename T>
    void read(T &x) {
        char c;
        x = 0;
        do c = getc(); while (!isdigit(c));
        do {
            x = x * 10 + c - '0';
            c = getc();
        } while (isdigit(c));
    }

    template <typename T>
    void operator()(T &x) {
        read(x);
    }

    template <typename T, typename ... TArgs>
    void operator()(T &x, TArgs& ...args) {
        read(x);
        (*this)(args...);
    }
};

struct Edge {
    int v, w;
};

struct Graph {
    typedef vector<Edge> AdjacencyList;

    int n;
    AdjacencyList *adj;

    Graph() : n(0), adj(nullptr) {}
    Graph(int _n) : n(_n), adj(new AdjacencyList[n]) {}
    Graph(const Graph &G) : n(G.n), adj(new AdjacencyList[n]) {
        for (int i = 0; i < n; i++)
            adj[i] = G.adj[i];
    }
    Graph(Graph &&G) : n(G.n), adj(G.adj) {
        G.n = 0;
        G.adj = nullptr;
    }
    ~Graph() { if (adj) delete[] adj; }

    auto operator=(const Graph &G) -> Graph& {
        if (adj) delete[] adj;
        n = G.n;
        adj = new AdjacencyList[n];
        for (int i = 0; i < n; i++)
            adj[i] = G.adj[i];
        return *this;
    }

    auto operator=(Graph &&G) -> Graph& {
        if (adj) delete[] adj;
        n = G.n;
        adj = G.adj;
        G.n = 0;
        G.adj = nullptr;
        return *this;
    }

    void link(int u, int v, int w) {
        adj[u - 1].push_back({v, w});
        adj[v - 1].push_back({u, w});
    }

    void clear() {
        for (int i = 0; i < n; i++)
            adj[i].clear();
    }

    auto operator[](int u) -> AdjacencyList& {
        return adj[u - 1];
    }
};

struct SPTree {
    typedef Edge *EdgePtr;

    EdgePtr *e;

    SPTree() : e(nullptr) {}
    SPTree(int n) : e(new EdgePtr[n + 1]) {}
    SPTree(const SPTree &) = delete;
    SPTree(SPTree &&rhs) : e(rhs.e) {
        rhs.e = nullptr;
    }

    auto operator=(const SPTree &) -> SPTree& = delete;
    auto operator=(SPTree &&rhs) -> SPTree& {
        e = rhs.e;
        rhs.e = nullptr;
        return *this;
    }

    ~SPTree() {
        if (e)
            delete[] e;
    }
};

struct SPFAAlgorithm {
    int n;
    bool *mark;
    i64 *dist;

    SPFAAlgorithm(int _n) : n(_n),
        mark(new bool[n + 1]),
        dist(new i64[n + 1]) {
        memset(mark + 1, 0, n);
    }
    ~SPFAAlgorithm() {
        delete[] mark;
        delete[] dist;
    }

    auto operator()(Graph &G, int s) -> SPTree {
        memset(dist + 1, 0x3f, sizeof(i64) * n);
        dist[s] = 0;

        deque<int> q;
        SPTree tr(n);
        q.push_back(s);
        tr.e[s] = nullptr;
        mark[s] = true;
        while (!q.empty()) {
            int u = q.front();
            q.pop_front();
            mark[u] = false;

            for (auto &e : G[u]) {
                int v = e.v;
                if (dist[v] > dist[u] + e.w) {
                    dist[v] = dist[u] + e.w;
                    tr.e[v] = &e;
                    if (!mark[v]) {
                        if (!q.empty() && dist[v] < dist[q[0]])
                            q.push_front(v);
                        else
                            q.push_back(v);
                        mark[v] = true;
                    }
                }
            }
        }

        return tr;
    }
};

template <typename TCallback>
struct TarjanLCA {
    struct UnionFind {
        int n;
        int *fa;

        UnionFind(int _n) : n(_n), fa(new int[n + 1]) {}
        ~UnionFind() { delete[] fa; }

        void clear() {
            memset(fa, 0, sizeof(int) * (n + 1));
        }

        int find(int u) {
            return fa[u] ? fa[u] = find(fa[u]) : u;
        }

        auto operator[](int u) -> int& {
            return fa[u];
        }
    };

    const TCallback &callback;
    UnionFind dsu;
    i64 *dist;

    TarjanLCA(int n, const TCallback &_callback)
        : callback(_callback), dsu(n), dist(new i64[n + 1]) {}
    ~TarjanLCA() { delete[] dist; }

    struct Impl {
        Graph &G;
        SPTree &tr;
        TarjanLCA *self;

        Impl(Graph &_G, SPTree &_tr, TarjanLCA *_self)
            : G(_G), tr(_tr), self(_self) {}

        void dfs(int u) {
            for (auto &e : G[u])
            if (tr.e[e.v] == &e) {
                int v = e.v;
                self->dist[v] = self->dist[u] + e.w;
                dfs(v);
                self->dsu[v] = u;
                self->callback(u, e, e.w);
            }

            for (auto &e : G[u])
            if (tr.e[e.v] != &e && tr.e[u] != &e &&
                self->dsu[e.v]) {
                int v = e.v, p = self->dsu.find(v);
                self->callback(
                    u, e,
                    self->dist[u] + self->dist[v] - 2 * self->dist[p]
                );
            }
        }
    };

    void operator()(Graph &G, SPTree &tr, int s) {
        dsu.clear();
        dist[s] = 0;
        Impl(G, tr, this).dfs(s);
    }
};

struct Heap {
    static constexpr int MMAX = 50000;

    int cnt, rt;
    int nxt[MMAX], ch[MMAX];
    i64 key[MMAX];
    int val[MMAX];

    Heap() {
        nxt[0] = ch[0] = 0;
    }

    void clear() {
        cnt = rt = 0;
    }

    i64 topkey() {
        return key[rt];
    }

    bool empty() {
        return !rt;
    }

    int meld(int x, int y) {
        if (key[x] > key[y])
            swap(x, y);
        nxt[y] = ch[x];
        ch[x] = y;
        return x;
    }

    void push(i64 k, int v) {
        key[++cnt] = k;
        val[cnt] = v;
        nxt[cnt] = ch[cnt] = 0;
        if (!rt) rt = cnt;
        else rt = meld(rt, cnt);
    }

    int pop() {
        int rax = val[rt];
        rt = ch[rt];
        if (nxt[rt]) {
            int nr = nxt[rt], x = nxt[nr];
            nxt[rt] = nxt[nr] = 0;
            rt = meld(rt, nr);
            for (int y = nxt[x], z = nxt[y]; x; x = z, y = nxt[z], z = nxt[y]) {
                nxt[x] = nxt[y] = 0;
                if (y) rt = meld(rt, meld(x, y));
                else rt = meld(rt, x);
            }
        }
        return rax;
    }
};

static i64 esum = 0;
struct Solver {
    static constexpr int TMAX = 40;
    static constexpr int CMAX = 2;

    typedef SPFAAlgorithm Algorithm;

    int n;
    Graph G;
    Algorithm shortest;

    Solver(int _n, int m, Graph &_G)
        : n(_n), G(_G), shortest(G.n),
          len(new i64*[n + 1]),
          tm(new int[n + 1]) {
        dist[0] = new i64[n + 1];
        dist[1] = new i64[n + 1];
        memset(tm, 0, sizeof(int) * (n + 1));

        // TODO: random
        // static random_device rd;
        // static mt19937_64 randi(rd());
        static mt19937_64 randi(19260817);

        Graph G1(n);
        auto check = [&G1](int u, Edge &e, i64 d) {
            if (e.w <= d)
                G1.link(u, e.v, e.w);
        };
        TarjanLCA<decltype(check)> travesal(n, check);

        for (int i = 1; i <= n; i++)
            len[i] = new i64[TMAX];
        for (int i = 0; i < TMAX; i++)
            _tmp[i] = i;
        for (int i = 0, s = 1; i < TMAX;
             i++, s = randi() % n + 1) {
            // DEBUG("s = %d", s);
            auto tr = shortest(G, s);  // TODO: reuse "tr"

            G1.clear();
            travesal(G, tr, s);
            G = G1;

            for (int u = 1; u <= n; u++)
                len[u][i] = travesal.dist[u];
        }
    }

    ~Solver() {
        delete[] tm;
        delete[] dist[0];
        delete[] dist[1];
        for (int i = 1; i <= n; i++)
            delete[] len[i];
        delete[] len;
    }

    i64 h(int u, int v) {
        i64 mx = 0;
        for (int i = 0; i < CMAX; i++) {
            int j = _tmp[i];
            mx = max(mx, abs(len[u][j] - len[v][j]));
        }
        return mx;
    }

    i64 query(int s, int t) {
        if (s == t)
            return 0;

        nth_element(_tmp, _tmp + CMAX - 1, _tmp + TMAX, [this, s, t](int i, int j) {
            return abs(len[s][i] - len[t][i]) > abs(len[s][j] - len[t][j]);
        });

        return search(s, t);
    }

  private:
    i64 **len;
    int _tmp[TMAX];

    int *tm;
    i64 *dist[2];

    i64 search(int s, int t) {
        static int cur;
        cur += 2;

        struct State {
            int u;
            i64 esi;

            bool operator<(const State &z) const {
                return esi > z.esi;
            }
        };

        i64 esi = h(s, t);
        i64 ans = INF, mi[] = {esi, esi};
        int ed[] = {t, s};
        tm[s] = tm[t] = cur;
        dist[0][s] = dist[1][t] = 0;
        dist[1][s] = dist[0][t] = INF;

        int ecnt = 0;
        static Heap q[2];
        q[0].clear();
        q[1].clear();
        q[0].push(esi, s);
        q[1].push(esi, t);
        while (!q[0].empty() || !q[1].empty()) {
            int i;
            if (!q[0].empty() && !q[1].empty())
                i = q[0].cnt < q[1].cnt ? 0 : 1;
            else
                i = q[1].empty() ? 0 : 1;

            int u = q[i].pop();
            if (tm[u] & 1)
                continue;
            tm[u] |= 1;

            if (dist[i][u] + h(u, ed[i]) >= ans ||
                dist[i][u] + mi[i ^ 1] - h(u, ed[i ^ 1]) >= ans)
                continue;

            for (auto &e : G[u]) {
                int v = e.v;
                ecnt++;
                if (tm[v] >= cur + 1)
                    continue;
                if (tm[v] < cur) {
                    tm[v] = cur;
                    dist[0][v] = dist[1][v] = INF;
                }

                if (dist[i][v] > dist[i][u] + e.w) {
                    dist[i][v] = dist[i][u] + e.w;
                    q[i].push(dist[i][v] + h(v, ed[i]), v);
                    ans = min(ans, dist[0][v] + dist[1][v]);
                }
            }

            mi[i] = q[i].topkey();
        }

        // fprintf(stderr, "cnt = %d\n", ecnt);
        esum += ecnt;
        return ans;
    }
};

#define NMAX 20000

static int cnt, cur, _m;
static Graph G(NMAX), G0(NMAX);
static int id[NMAX + 10], com[NMAX + 10];
static Solver *instance[NMAX + 10];

void dfs(int x) {
    if (id[x]) return;
    id[x] = ++cur;
    com[x] = cnt;
    for (auto &e : G[x]) {
        dfs(e.v);
        if (e.v < x) {
            _m++;
            G0.link(id[x], id[e.v], e.w);
        }
    }
}

int main() {
    Reader read;
    int n, m;
    read(n, m);

    auto t1 = high_resolution_clock::now();

    for (int i = 1; i <= m; i++) {
        int u, v, w;
        read(u, v, w);
        G.link(u, v, w);
    }

    for (int u = 1; u <= n; u++) if (!id[u]) {
        cnt++;
        cur = _m = 0;
        G0.clear();
        dfs(u);
        G0.n = cur;
        if (cur > 1)
            instance[cnt] = new Solver(cur, _m, G0);
    }

    auto t2 = high_resolution_clock::now();

    auto span = duration_cast<milliseconds>(t2 - t1).count();
    DEBUG("init: %ldms", span);

    int q;
    read(q);

    t1 = high_resolution_clock::now();
    for (int _ = 0; _ < q; _++) {
        int s, t;
        read(s, t);
        i64 ans;
        if (s == t) ans = 0;
        else if (com[s] != com[t]) ans = -1;
        else ans = instance[com[s]]->query(id[s], id[t]);
        printf("%lld\n", ans);
    }
    t2 = high_resolution_clock::now();

    span = duration_cast<microseconds>(t2 - t1).count();
    auto aver = static_cast<double>(span) / q / 1000;
    DEBUG("aver. = %.3lfms", aver);
    DEBUG("aver. cnt = %.3lf", static_cast<double>(esum) / q);

    return 0;
}
