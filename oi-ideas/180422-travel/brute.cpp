#define NDEBUG
// #define USE_DECODER
// #define USE_FILE_IO

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

#define NMAX 1000000
#define MMAX 1000000
#define QMAX 500000

class Decoder {
 public:
    Decoder() : _pos(0) {
        fread(_buffer, 1, MAX_LENGTH, stdin);

        int v = 0;
        for (int i = 'a'; i <= 'z'; i++)
            _table[i] = v++;
        for (int i = 'A'; i <= 'Z'; i++)
            _table[i] = v++;
        for (int i = '0'; i <= '9'; i++)
            _table[i] = v++;
        _table['%'] = v++;
        _table['@'] = v++;
    }

    int next() {
        int r = _table[_buffer[_pos++]] << 18;
        r |= _table[_buffer[_pos++]] << 12;
        r |= _table[_buffer[_pos++]] << 6;
        r |= _table[_buffer[_pos++]];
        return r;
    }

 private:
    static const size_t MAX_LENGTH = 30000000;

    size_t _pos;
    char _buffer[MAX_LENGTH];
    int _table[256];
};

struct Pair {
    Pair(int _u, int _v) : u(_u), v(_v) {}

    int u, v;
};

struct Edge {
    Edge() : p(0) {}

    int u, v, k, p;
};

static int n, m, q, cnt;
static vector<int> G[NMAX + 10], T[NMAX + 10];
static Edge query[QMAX + 10];
static vector<Edge *> queue[NMAX + 10];
static int father[NMAX + 10];
static int in[NMAX + 10], out[NMAX + 10];
static int set[NMAX + 10];

#define IS_BLOCK(x) (x + cnt > n)

inline int find(int u) {
    return set[u] ? set[u] = find(set[u]) : u;
}

inline bool in_subtree(int u, int p) {
    return in[p] <= in[u] && in[u] <= out[p];
}

inline bool on_path(int s, int t, int p, int u) {
    return p == u || in_subtree(s, u) != in_subtree(t, u);
}

void bcc(int u, int f = 0) {
    static stack<Pair> stk;
    static bool marked[NMAX + 10];
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

                for (int v : T[p]) {
                    marked[v] = false;
                }

                if (linked == 0) {
                    cnt--;
                    DEBUG("(empty)");
                }

                DEBUG("\n");
            }
        }
    }
}

void lca(int u, int f = 0) {
    static int cur;
    in[u] = ++cur;
    father[u] = f;

    for (int v : T[u]) {
        if (v == f)
            continue;

        lca(v, u);
        set[v] = u;
    }

    for (auto p : queue[u]) {
        if (p->p)
            continue;

        int v = u == p->u ? p->v : p->u;
        if (in[v]) {
            p->p = find(v);
            DEBUG("lca: %d x %d -> %d\n", u, v, p->p);
        }
    }

    out[u] = cur;
}

void initialize() {
#ifdef USE_DECODER
    Decoder *decoder = new Decoder;
    n = decoder->next();
    m = decoder->next();
    q = decoder->next();
#else
    scanf("%d%d", &n, &m);
#endif

    int u, v;
    for (int i = 0; i < m; i++) {
#ifdef USE_DECODER
        u = decoder->next();
        v = decoder->next();
#else
        scanf("%d%d", &u, &v);
#endif

        if (u == v)
            continue;

        G[u].push_back(v);
        G[v].push_back(u);
    }

    for (int i = 1; i <= n; i++) for (int j = i + 1; j <= n; j++) {
        for (int k = 1; k <= n; k++) {
            if (k == i || k == j)
                continue;

            q++;
            Edge &p = query[q];
            p.u = i;
            p.v = j;
            p.k = k;
            queue[p.u].push_back(&p);
            queue[p.v].push_back(&p);
        }
    }

    for (int i = 1; i <= n; i++) {
        if (!in[i])
            bcc(i);
    }

    n += cnt;
    memset(in, 0, sizeof(in));
    for (int i = 1; i <= n; i++) {
        if (!father[i]) {
            lca(i);
            set[i] = n + 1;
        }
    }
}

int main() {
#ifdef USE_FILE_IO
    freopen("night.in", "r", stdin);
    freopen("night.out", "w", stdout);
#endif
    initialize();

    bool ans;
    int out = 0;
    for (int i = 1; i <= q; i++) {
        Edge &pa = query[i];
        int u = pa.u, v = pa.v, k = pa.k, p = pa.p;

        if (p > n)
            ans = false;
        else if (on_path(u, v, p, k))
            ans = true;
        else if (in_subtree(u, k) && in_subtree(v, k))
            ans = (IS_BLOCK(p) && father[p] == k);
        else
            ans = IS_BLOCK(father[k]) && on_path(u, v, p, father[k]);

        if (ans)
            out++;
        else
            printf("%d, %d, k = %d\n", u, v, k);
    }

    printf("%d\n", out);
    return 0;
}
