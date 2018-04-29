// #define NDEBUG
#define USE_FILE_IO

#include <cassert>
#include <cstdio>
#include <cstring>
#include <climits>

#include <stack>
#include <vector>
#include <algorithm>

using namespace std;

#ifdef NDEBUG
#define DEBUG(...)
#else

template <typename ... TArgs>
void DEBUG(const char *msg, const TArgs& ...args) {
    fprintf(stderr, msg, args...);
}

template <>
void DEBUG(const char *msg) {
    fputs(msg, stderr);
}

#endif

#define NMAX 600000
#define MMAX 600000

typedef long long i64;

struct Pair {
    Pair(int _u, int _v) : u(_u), v(_v) {}

    int u, v;
};

struct Edge {
    Edge() : p(0) {}

    int u, v, k, p;
};

static int n, m, cnt;  // cnt: number of blocks
static vector<int> G[NMAX + 10], T[NMAX + 10];
static int father[NMAX + 10], in[NMAX + 10];
static bool marked[NMAX + 10];
static i64 num[NMAX + 10];  // num: numbers of nodes in one component
static i64 size[NMAX + 10]/*, g[NMAX + 10]*/;

#define IS_BLOCK(x) (x > n)

// Biconnected components
void bcc(int u, int f = 0) {
    static stack<Pair> stk;
    static int low[NMAX + 10], cur;
    in[u] = low[u] = ++cur;

    for (int v : G[u]) {
        if (v == f) {
            f = 0;  // Cancel on the first edge to father
            continue;
        }

        if (in[v]) 
            low[u] = min(low[u], in[v]);
        else {
            stk.push(Pair(u, v));
            bcc(v, u);
            low[u] = min(low[u], low[v]);

            if (low[v] > in[u]) {
                DEBUG("bcc: cut edge: (%d, %d)\n", u, v);
                T[u].push_back(v);
                T[v].push_back(u);
                stk.pop();
            } else if (low[v] >= in[u]) {
                cnt++;
                DEBUG("bcc: %d: ", cnt);
                int linked = 0, p = n + cnt;
                auto add = [p, &linked](int x) {
                    if (!marked[x]) {
                        marked[x] = true;
                        T[p].push_back(x);
                        T[x].push_back(p);
                        linked++;

                        DEBUG("%d ", x);
                    }
                };

                while (!stk.empty()) {
                    Pair x = stk.top();
                    stk.pop();
                    add(x.u);
                    add(x.v);

                    if (x.u == u && x.v == v)
                        break;
                }

                for (int v : T[p])
                    marked[v] = false;

                num[p] += linked;
                if (linked == 0) {
                    cnt--;
                    DEBUG("(empty)");
                }

                DEBUG("\n");
            }
        }
    }
}

void dfs(int x, int f) {
    i64 sum = 0;
    for (int v : T[x]) {
        if (v == f)
            continue;

        father[v] = x;
        dfs(v, x);
        // g[x] += sum * size[v];
        sum += size[v];
    }

    size[x] = sum + (IS_BLOCK(x) ? 0 : 1);
}

void initialize() {
    scanf("%d%d", &n, &m);

    int u, v;
    for (int i = 0; i < m; i++) {
        scanf("%d%d", &u, &v);

        if (u == v)
            continue;

        G[u].push_back(v);
        G[v].push_back(u);
    }

    for (int i = 1; i <= n; i++) {
        if (!in[i])
            bcc(i);
    }

    for (int i = 1; i <= n; i++) {
        if (!size[i])
            dfs(i, 0);
    }
}

inline i64 c(i64 m) {  // m choose 2
    return m * (m - 1) / 2;
}

int main() {
#ifdef USE_FILE_IO
    freopen("data.in", "r", stdin);
    // freopen("data.out", "w", stdout);
#endif
    initialize();

    // TODO: BUG
    i64 ans = n * (n - 1) * (n - 2) / 2;
    memset(marked, 0, sizeof(marked));
    for (int u = n + 1; u <= n + cnt; u++) {
        assert(IS_BLOCK(u));
        i64 m = num[u];
        for (int v : T[u]) {
            assert(!IS_BLOCK(v));
            marked[v] = true;

            if (father[u] == v) {
                i64 a = size[v], b = size[u];
                ans -= m * (n - a) * (a - b - 1) +
                       (m - 1) * (n - b - 1) +
                       m * (c(n - a) + c(a - b - 1));
            } else
                ans -= m * c(size[v] - 1) + (m - 1) * (size[v] - 1);
        }
    }

    for (int u = 1; u <= n; u++) {
        if (!marked[u])  // Cut nodes
            ans -= c(size[u] - 1) + c(n - size[u]);
    }

    printf("%lld\n", ans);
    return 0;
}
