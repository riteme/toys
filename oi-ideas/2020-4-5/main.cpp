// http://yloj.yali.edu.cn/v3/contest_show.php?cid=567#problem/B

#pragma GCC optimize(2)

#include <cstdio>
#include <cstring>

#include <algorithm>

using namespace std;

typedef long long i64;

#define NMAX 15
#define MMAX (NMAX * (NMAX - 1) / 2)
#define SMAX (1 << NMAX)
#define MOD 1000000007

int n, m, _;
bool G[NMAX][NMAX];
i64 pw[MMAX + 1];
int e0[SMAX], e1[SMAX];

void initialize() {
    pw[0] = 1;
    for (int i = 1; i <= MMAX; i++)
        pw[i] = pw[i - 1] * 2 % MOD;

    scanf("%d%d%d", &n, &m, &_);
    for (int i = 0; i < m; i++) {
        int u, v;
        scanf("%d%d", &u, &v);
        u--; v--;
        G[u][v] = G[v][u] = 1;
    }

    for (int S = 1; S < (1 << n); S++) {
        for (int u = 0; u < n; u++)
        for (int v = u + 1; v < n; v++)
        if (G[u][v] && ((S >> u) & 1) && ((S >> v) & 1))
            e0[S]++;

        for (int u = 0; u < n; u++)
        for (int v = u + 1; v < n; v++)
        if (G[u][v] && (((S >> u) & 1) || ((S >> v) & 1)))
            e1[S]++;
    }
}

void dp(int x, i64 *f) {
    for (int S = 1; S < (1 << n); S++)
    if ((S >> x) & 1) {
        f[S] = pw[e0[S]];
        for (int T = (S - 1) & S; T; T = (T - 1) & S)
            f[S] = (f[S] - pw[e0[T]] * f[S ^ T]) % MOD;
    }
}

i64 f1[SMAX], f2[SMAX];

int main() {
    initialize();
    dp(0, f1);
    dp(1, f2);

    i64 ans = pw[m];
    int A = (1 << n) - 1;
    for (int S = 1; S <= A; S++)
    if (S & 1) {
        int sp = S;
        for (int u = 0; u < n; u++)
        if ((S >> u) & 1) {
            for (int v = 0; v < n; v++)
            if (G[u][v])
                sp |= 1 << v;
        }

        int Sc = A ^ sp;
        for (int T = Sc; T; T = (T - 1) & Sc)
        if (T & 2) {
            int r = m - e1[T] - e1[S];
            ans = (ans - pw[r] * f2[T] % MOD * f1[S]) % MOD;
        }
    }

    if (ans < 0)
        ans += MOD;
    printf("%lld\n", ans);

    return 0;
}