#include <cstdio>
#include <cstring>

#include <vector>
#include <algorithm>

using namespace std;

#define NMAX 100000
#define MOD 998244353
#define PRIV 3

using i64 = long long;
using vi = vector<i64>;

i64 add(i64 a, i64 b) {
    a += b;
    if (a >= MOD)
        a -= MOD;
    return a;
}

i64 qpow(i64 a, i64 k) {
    i64 r = 1;
    for ( ; k; k >>= 1) {
        if (k & 1)
            r = r * a % MOD;
        a = a * a % MOD;
    }
    return r;
}

i64 inv(i64 x) {
    return x == 1 ? 1 : MOD - MOD / x * inv(MOD % x) % MOD;
}

void ntt(vi &a, int op = 1) {
    int n = a.size();
    for (int i = 1, j = 0; i < n - 1; i++) {
        for (int s = n; j ^= s >>= 1, ~j & s; ) ;
        if (i < j)
            swap(a[i], a[j]);
    }

    for (int i = 1; i < n; i *= 2) {
        i64 u = qpow(PRIV, (MOD - 1) / (i * 2));
        if (op == -1)
            u = inv(u);
        for (int j = 0; j < n; j += i * 2) {
            i64 w = 1;
            for (int k = 0; k < i; k++, w = w * u % MOD) {
                i64 x = a[j + k], y = w * a[j + k + i] % MOD;
                a[j + k] = add(x, y);
                a[j + k + i] = add(x, MOD - y);
            }
        }
    }

    if (op == -1) {
        i64 iv = inv(n);
        for (int i = 0; i < n; i++) {
            a[i] = a[i] * iv % MOD;
        }
    }
}

int nxtp(int n) {
    int r = 1;
    while (r < n) {
        r <<= 1;
    }
    return r;
}

vi conv(vi a, vi b) {
    int n = nxtp(a.size() + b.size());
    a.resize(n);
    b.resize(n);
    ntt(a);
    ntt(b);
    for (int i = 0; i < n; i++) {
        a[i] = a[i] * b[i] % MOD;
    }
    ntt(a, -1);
    return a;
}

int n, m;
i64 cnt[NMAX + 10];
int sz[NMAX + 10];
bool mark[NMAX + 10];
vector<int> G[NMAX + 10];

void dfs(int x, int p) {
    sz[x] = 1;
    for (int v : G[x])
    if (!mark[v] && v != p) {
        dfs(v, x);
        sz[x] += sz[v];
    }
}

int center(int x, int p, int maxs) {
    for (int v : G[x])
    if (!mark[v] && v != p) {
        if (sz[v] * 2 > maxs)
            return center(v, x, maxs);
    }
    return x;
}

void scan(int x, int p, int d, vi &b) {
    if (d + 1 > b.size())
        b.resize(d + 1);
    b[d]++;
    for (int v : G[x])
    if (!mark[v] && v != p) {
        scan(v, x, d + 1, b);
    }
}

void solve(int x) {
    dfs(x, 0);
    int p = center(x, 0, sz[x]);
    mark[p] = true;

    vector<vi*> s;
    s.reserve(G[p].size());
    for (int v : G[p]) if (!mark[v]) {
        vi *b = new vi;
        scan(v, p, 1, *b);
        s.push_back(b);
    }

    sort(s.begin(), s.end(), [](vi *a, vi *b) {
        return a->size() < b->size();
    });

    vi f = {1};
    for (vi *b : s) {
        vi &g = *b;
        vi r = conv(f, g);
        for (int i = 1; i < min(n, (int) r.size()); i++) {
            cnt[i] = add(cnt[i], r[i]);
        }

        f.resize(g.size());
        for (int i = 0; i < g.size(); i++) {
            f[i] += g[i];
        }

        delete b;
    }

    for (int v : G[p]) if (!mark[v]) {
        solve(v);
    }
}

i64 h[NMAX + 10][2];

int main() {
    int _;
    scanf("%d%d%d", &_, &n, &m);
    for (int i = 1; i < n; i++) {
        int u, v;
        scanf("%d%d", &u, &v);
        G[u].push_back(v);
        G[v].push_back(u);
    }

    solve(1);
    for (int i = 1; i < n; i++) {
        cnt[i] = cnt[i] * 2 % MOD;
    }

    i64 ans = 0;

    h[0][0] = 1;
    for (int i = 0; i < n; i++) {
        h[i + 1][0] = (h[i][0] + h[i][1]) % MOD;
        h[i + 1][1] = h[i][0];
    }

    i64 inv2 = inv(2);
    for (int i = 1; i < n; i++) {
        i64 q = (h[i + 1][0] + h[i + 1][1]) * qpow(inv2, i + 1) % MOD;
        i64 p = qpow(q, m);
        ans = (ans + cnt[i] * p) % MOD;
    }
    ans = (ans + n) % MOD;
    if (ans < 0)
        ans += MOD;
    printf("%lld\n", ans);

    return 0;
}
