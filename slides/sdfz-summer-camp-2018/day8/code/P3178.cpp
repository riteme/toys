#include <cstdio>
#include <cstring>
#include <climits>

#include <vector>
#include <algorithm>

using namespace std;

typedef long long i64;

#define NMAX 100000
#define OP_NODEADD 1
#define OP_SUBADD 2
#define OP_QUERY 3

struct FenwickArray {
    FenwickArray(int _n) : n(_n) {
        memset(arr, 0, sizeof(arr));
    }

    int n;
    i64 arr[NMAX * 2 + 10];

    void modify(int x, i64 v) {
        for (; x <= n; x += x & -x)
            arr[x] += v;
    }

    i64 query(int r) {
        i64 ret = 0;
        for (; r; r -= r & -r)
            ret += arr[r];
        return ret;
    }
};

struct Command {
    Command(int _type, int _t, i64 _v)
        : type(_type), t(_t), v(_v) {}

    int type, t;
    i64 v;
};

static int n, q;
static int depth[NMAX + 10];
static i64 ans[NMAX * 2 + 10];
static vector<int> T[NMAX + 10];
static vector<Command *> op[NMAX + 10];
static FenwickArray *sum, *tag;

void dfs(int x, int f) {
    for (auto c : op[x]) {
        if (c->type == OP_NODEADD)
            sum->modify(c->t, c->v);
        else if (c->type == OP_SUBADD) {
            sum->modify(c->t, -depth[x] * c->v);
            tag->modify(c->t, c->v);
        }
    }

    for (auto c : op[x]) {
        if (c->type == OP_QUERY)
            c->v = sum->query(c->t) + (depth[x] + 1) * tag->query(c->t);
    }

    for (int v : T[x]) {
        if (v != f) {
            depth[v] = depth[x] + 1;
            dfs(v, x);
        }
    }

    for (auto c : op[x]) {
        if (c->type == OP_NODEADD)
            sum->modify(c->t, -c->v);
        else if (c->type == OP_SUBADD) {
            sum->modify(c->t, depth[x] * c->v);
            tag->modify(c->t, -c->v);
        }
    }
}

void initialize() {
    scanf("%d%d", &n, &q);

    for (int i = 1; i <= n; i++) {
        i64 v;
        scanf("%lld", &v);
        op[i].push_back(new Command(OP_NODEADD, i, v));
    }

    for (int i = 1; i < n; i++) {
        int u, v;
        scanf("%d%d", &u, &v);
        T[u].push_back(v);
        T[v].push_back(u);
    }

    for (int i = 1; i <= q; i++) {
        int c, x;
        i64 v = 0;
        scanf("%d%d", &c, &x);
        if (c != 3)
            scanf("%lld", &v);
        op[x].push_back(new Command(c, n + i, v));
    }

    sum = new FenwickArray(n + q);
    tag = new FenwickArray(n + q);
}

int main() {
    initialize();
    dfs(1, 0);

    for (int i = 1; i <= n + q; i++)
        ans[i] = LLONG_MIN;

    for (int u = 1; u <= n; u++) {
        for (auto c : op[u]) {
            if (c->type == OP_QUERY)
                ans[c->t] = c->v;
        }
    }

    for (int i = 1; i <= n + q; i++)
        if (ans[i] > LLONG_MIN) printf("%lld\n", ans[i]);

    return 0;
}
