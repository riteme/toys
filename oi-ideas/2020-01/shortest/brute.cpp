#define NDEBUG

#include <cassert>
#include <cstdio>
#include <cctype>
#include <cstring>

#include <stack>
#include <queue>
#include <chrono>
#include <random>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

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
    int v, w, id;
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

    void link(int u, int v, int w, int id = 0) {
        adj[u - 1].push_back({v, w, id});
        adj[v - 1].push_back({u, w, id});
    }

    void clear() {
        for (int i = 0; i < n; i++)
            adj[i].clear();
    }

    auto operator[](int u) -> AdjacencyList& {
        return adj[u - 1];
    }

    auto operator[](int u) const -> const AdjacencyList& {
        return adj[u - 1];
    }
};

struct SPTree {
    int /* *fa,  */*eid;

    SPTree() : /* fa(nullptr),  */eid(nullptr) {}
    SPTree(int n) : /* fa(new int[n + 1]),  */eid(new int[n + 1]) {}
    SPTree(const SPTree &) = delete;
    SPTree(SPTree &&rhs) : /* fa(rhs.fa),  */eid(rhs.eid) {
        /* rhs.fa = */ rhs.eid = nullptr;
    }

    auto operator=(const SPTree &) = delete;
    auto operator=(SPTree &&rhs) -> SPTree& {
        /* fa = rhs.fa; */
        eid = rhs.eid;
        /* rhs.fa = */ rhs.eid = nullptr;
        return *this;
    }

    ~SPTree() {
        /* if (fa)
            delete[] fa; */
        if (eid)
            delete[] eid;
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

    auto operator()(const Graph &G, int s) -> SPTree {
        memset(dist + 1, 0x3f, sizeof(i64) * n);
        dist[s] = 0;

        deque<int> q;
        SPTree tr(n);
        q.push_back(s);
        /* tr.fa[s] = */ tr.eid[s] = 0;
        mark[s] = true;
        while (!q.empty()) {
            int u = q.front();
            q.pop_front();
            mark[u] = false;

            for (auto &e : G[u]) {
                int v = e.v;
                if (dist[v] > dist[u] + e.w) {
                    dist[v] = dist[u] + e.w;
                    /* tr.fa[v] = u; */
                    tr.eid[v] = e.id;
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

struct DijkstraAlgorithm {
    int n;
    i64 *dist;

    DijkstraAlgorithm(int _n) : n(_n), dist(new i64[n]) {}
    ~DijkstraAlgorithm() { delete[] dist; }

    auto operator()(const Graph &G, int s) -> SPTree {
        memset(dist + 1, 0x3f, sizeof(i64) * n);
        dist[s] = 0;

        struct State {
            int u;
            i64 t;

            bool operator<(const State &z) const {
                return t > z.t;
            }
        };

        SPTree tr(n);
        priority_queue<State> q;
        /* tr.fa[s] = */ tr.eid[s] = 0;
        q.push({s, 0});
        while (!q.empty()) {
            auto _ = q.top();
            q.pop();
            int u = _.u;
            if (_.t > dist[u])
                continue;

            for (auto &e : G[u]) {
                int v = e.v;
                if (dist[v] > dist[u] + e.w) {
                    dist[v] = dist[u] + e.w;
                    /* tr.fa[v] = u; */
                    tr.eid[v] = e.id;
                    q.push({v, dist[v]});
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
        const Graph &G;
        const SPTree &tr;
        TarjanLCA *self;

        Impl(const Graph &_G, const SPTree &_tr, TarjanLCA *_self)
            : G(_G), tr(_tr), self(_self) {}

        void dfs(int u) {
            for (auto &e : G[u])
            if (tr.eid[e.v] == e.id) {
                int v = e.v;
                self->dist[v] = self->dist[u] + e.w;
                dfs(v);
                self->dsu[v] = u;
                self->callback(u, e, -1);
            }

            for (auto &e : G[u])
            if (tr.eid[e.v] != e.id && tr.eid[u] != e.id &&
                self->dsu[e.v]) {
                int v = e.v, p = self->dsu.find(v);
                self->callback(
                    u, e,
                    self->dist[u] + self->dist[v] - 2 * self->dist[p]
                );
            }
        }
    };

    void operator()(const Graph &G, const SPTree &tr, int s) {
        dsu.clear();
        dist[s] = 0;
        Impl(G, tr, this).dfs(s);
    }
};

typedef bool *EnableList;

static i64 esum = 0;
struct Solver {
    int TMAX;
    int CMAX;

    typedef SPFAAlgorithm Algorithm;

    int n;
    Graph G, G0;
    Algorithm shortest;

    Solver(int _n, int m, const Graph &_G)
        : n(_n), G(_G), G0(_G), shortest(G.n),
          len(new i64*[n + 1]),
          tm(new int[n + 1]) {
        if (n <= 5000)         TMAX = 500,  CMAX = 5;
        else if (n <= 10000)   TMAX = 1500, CMAX = 5;
        else if (n <= 1000000) TMAX = 10,   CMAX = 3;
        else  /* n >= 1e6 */   TMAX = 6,    CMAX = 3;
        _tmp = new int[TMAX];

        dist[0] = new i64[n + 1];
        dist[1] = new i64[n + 1];
        memset(tm, 0, sizeof(int) * (n + 1));

        // TODO: random
        static random_device rd;
        static mt19937_64 randi(rd());
        // static mt19937_64 randi(19260817);

        Graph G1(n);
        // Strange if I remove the "check" variable and write the function
        // directly in "TarjanLCA"'s constructor, it results in a segmentation fault
        // on g++ or clang with -O1
        // Looks like "G1" is not correctly captured.
        auto check = [&G1](int u, const Edge &e, i64 d) {
            if (d < 0 || e.w < d)
                G1.link(u, e.v, e.w, e.id);
            // else DEBUG("%d - %d (%d) removed.", u, e.v, e.w);
        };
        TarjanLCA travesal(n, check);

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

        has.reserve(m);
        for (int u = 1; u <= n; u++)
        for (auto &e : G0[u])
            has.insert(e.id);
    }

    ~Solver() {
        delete[] tm;
        delete[] dist[0];
        delete[] dist[1];
        for (int i = 1; i <= n; i++)
            delete[] len[i];
        delete[] len;
        delete[] _tmp;
    }

    i64 h(int u, int v) {
        i64 mx = 0;
        for (int i = 0; i < CMAX; i++) {
            int j = _tmp[i];
            mx = max(mx, abs(len[u][j] - len[v][j]));
        }
        return mx;
    }

    i64 query(int s, int t, EnableList en, const vector<int> &disabled) {
        if (s == t)
            return 0;

        nth_element(_tmp, _tmp + CMAX - 1, _tmp + TMAX, [this, s, t](int i, int j) {
            return abs(len[s][i] - len[t][i]) > abs(len[s][j] - len[t][j]);
        });

        bool ok = all_of(
            disabled.begin(), disabled.end(),
            [this](int i) { return !has.count(i); }
        );
        return search(ok ? G : G0, s, t, en);
    }

  private:
    i64 **len; // i64 *len[TMAX]
    int *_tmp; // int _tmp[TMAX]

    int *tm;
    i64 *dist[2];
    unordered_set<int> has;

    i64 search(const Graph &_G, int s, int t, EnableList en) {
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
        /* static */ priority_queue<State> q[2];
        // while (!q[0].empty()) q[0].pop();
        // while (!q[1].empty()) q[1].pop();
        q[0].push({s, esi});
        q[1].push({t, esi});
        while (!q[0].empty() || !q[1].empty()) {
            int i;
            if (!q[0].empty() && !q[1].empty())
                i = q[0].size() < q[1].size() ? 0 : 1;
            else
                i = q[1].empty() ? 0 : 1;

            auto _ = q[i].top();
            q[i].pop();
            int u = _.u;
            if (tm[u] & 1)
                continue;
            tm[u] |= 1;

            if (dist[i][u] + h(u, ed[i]) >= ans ||
                dist[i][u] + mi[i ^ 1] - h(u, ed[i ^ 1]) >= ans)
                continue;

            for (auto &e : _G[u]) if (en[e.id]) {
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
                    q[i].push({v, dist[i][v] + h(v, ed[i])});
                    ans = min(ans, dist[0][v] + dist[1][v]);
                }
            }

            mi[i] = q[i].top().esi;
        }

        // fprintf(stderr, "cnt = %d\n", ecnt);
        esum += ecnt;
        return ans;
    }
};

struct BCTree {
    int n, m;
    const Graph &G;
    Graph T;
    int *dfn, *low;

    BCTree(const Graph &_G)
        : n(_G.n), m(n), G(_G), T(2 * n),
          dfn(new int[n + 1]),
          low(new int[n + 1]),
          marked(new bool[n + 1]),
          cur(0) {
        memset(marked, 0, n + 1);
        memset(dfn, 0, sizeof(int) * (n + 1));
        for (int u = 1; u <= n; u++)
            if (!dfn[u])
                bcc(u);

        fa = new int[m + 1];
        dep = new int[m + 1];
        w = new EdgePtr[m + 1];
        memset(fa + 1, 0, sizeof(int) * m);
        for (int u = n + 1; u <= m; u++) if (!fa[u]) {
            dep[u] = 0;
            dfs(u);
        }

        id = new IDMap[m - n];
        instance = new SolverPtr[m - n];
        for (int i = 0; i < m - n; i++) {
            int p = n + i + 1, cnt = 0;
            Graph G0(T[p].size());
            for (auto &e : T[p]) {
                int u = e.v;
                auto it = id[i].find(u);
                if (it == id[i].end())
                    id[i][u] = ++cnt;
            }

            int _m = 0;
            for (auto &_ : T[p]) {
                int u = _.v;
                auto idu = id[i].find(u);
                for (auto &e : G[u]) {
                    int v = e.v;
                    auto idv = id[i].find(v);
                    if (v < u && idv != id[i].end()) {
                        _m++;
                        G0.link(idu->second, idv->second, e.w, e.id);
                    }
                }
            }

            instance[i] = new Solver(G0.n, _m, std::move(G0));
        }
    }
    ~BCTree() {
        delete[] dfn;
        delete[] low;
        delete[] marked;
        delete[] fa;
        delete[] dep;
        delete[] w;
        delete[] id;
        for (int i = 0; i < m - n; i++)
            delete instance[i];
        delete[] instance;
    }

    i64 query(int u, int v, EnableList en, const vector<int> &disabled) {
        if (u == v)
            return 0;

        i64 ret = 0;
        for (int p; u != v && fa[u] != fa[v]; u = p) {
            if (dep[u] < dep[v])
                swap(u, v);
            p = fa[u];
            if (p > n) {
                int i = p - n - 1;
                p = fa[p];
                if (!p) return INF;  // root block
                ret += instance[i]->query(id[i][u], id[i][p], en, disabled);
                if (ret >= INF)
                    return INF;
            } else if (en[w[u]->id])
                ret += w[u]->w;
            else return INF;
        }

        if (!fa[u])
            return INF;
        if (u != v) {
            assert(fa[u] == fa[v]);
            int p = fa[u];
            if (p > n) {
                int i = p - n - 1;
                ret += instance[i]->query(id[i][u], id[i][v], en, disabled);
            } else if (en[w[u]->id] && en[w[v]->id])
                ret += w[u]->w + w[v]->w;
            else return INF;
        }

        return ret >= INF ? INF : ret;
    }

    void print_statistics() {
        i64 sum = 0;
        for (int p = n + 1; p <= m; p++)
            sum += T[p].size();
        DEBUG("number of blocks: %d", m - n);
        DEBUG("average block size: %.3lf", static_cast<double>(sum) / (m - n));
    }

  private:
    typedef unordered_map<int, int> IDMap;
    typedef Solver *SolverPtr;
    typedef Edge *EdgePtr;

    bool *marked;
    int *fa, *dep;
    EdgePtr *w;
    IDMap *id;
    SolverPtr *instance;
    int cur;

    void dfs(int u, int f = 0) {
        for (auto &e : T[u]) if (e.v != f) {
            int v = e.v;
            fa[v] = u;
            dep[v] = dep[u] + 1;
            w[v] = &e;
            dfs(v, u);
        }
    }

    void bcc(int u, int f = 0) {
        struct Pair { int u, v; };
        static stack<Pair> stk;

        dfn[u] = low[u] = ++cur;
        for (auto &e : G[u]) {
            int v = e.v;
            if (v == f) f = 0;
            else if (dfn[v]) low[u] = min(low[u], dfn[v]);
            else {
                stk.push({u, v});
                bcc(v, u);
                low[u] = min(low[u], low[v]);

                if (low[v] > dfn[u]) {  // cut edge
                    T[u].push_back(e);
                    T[v].push_back({u, e.w, e.id});
                    stk.pop();
                } else if (low[v] >= dfn[u]) {
                    m++;
                    int cnt = 0, p = m;
                    auto add = [this, p, &cnt](int x) {
                        if (!marked[x]) {
                            marked[x] = true;
                            T[p].push_back({x, 0, 0});
                            T[x].push_back({p, 0, 0});
                            cnt++;
                        }
                    };

                    while (!stk.empty()) {
                        auto _ = stk.top();
                        stk.pop();
                        add(_.u);
                        add(_.v);
                        if (_.u == u && _.v == v) break;
                    }

                    for (auto &e0 : T[p])
                        marked[e0.v] = false;
                    if (!cnt) m--;
                }
            }
        }
    }
};

int main() {
    Reader read;
    int n, m;
    read(n, m);

    Graph G(n);
    for (int i = 1; i <= m; i++) {
        int u, v, w;
        read(u, v, w);
        if (u == v)
            continue;
        G.link(u, v, w, i);
    }

    vector<int> disabled;
    EnableList en = new bool[m + 1];
    memset(en, 1, m + 1);

    auto t1 = high_resolution_clock::now();
    BCTree core(G);
    auto t2 = high_resolution_clock::now();
    auto span = duration_cast<milliseconds>(t2 - t1).count();
    DEBUG("init: %ldms", span);
    core.print_statistics();

    int q;
    read(q);

    t1 = high_resolution_clock::now();
    for (int _ = 0; _ < q; _++) {
        int s, t, cnt = 0;
        read(s, t);
        disabled.clear();
        disabled.reserve(cnt);
        while (cnt--) {
            int u;
            read(u);
            disabled.push_back(u);
        }

        for (int i : disabled)
            en[i] = false;

        i64 ans = core.query(s, t, en, disabled);

        for (int i : disabled)
            en[i] = true;

        printf("%lld\n", ans < INF ? ans : -1);
    }
    t2 = high_resolution_clock::now();
    span = duration_cast<microseconds>(t2 - t1).count();
    auto aver = static_cast<double>(span) / q / 1000;
    DEBUG("aver. = %.3lfms", aver);
    DEBUG("aver. cnt = %.3lf", static_cast<double>(esum) / q);

    delete[] en;
    return 0;
}
