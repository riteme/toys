#include <cstdio>
#include <cassert>
#include <cstring>

#include <random>
#include <algorithm>
#include <unordered_map>

using namespace std;

using i64 = long long;
using u64 = unsigned long long;

constexpr int AMAX = 64;
constexpr int CMAX = 15;
constexpr int PMAX = 200000;
constexpr i64 NMAX = 1000000000000000000;

constexpr i64 prime[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};

u64 rwt[AMAX];
u64 H(int n, int a[]) {
    u64 h0 = 0, h1 = 1;
    for (int i = 0; i < n; i++) {
        if (i && a[i] != a[i - 1]) {
            h0 ^= h1;
            h1 = 1;
        }
        h1 *= rwt[a[i]];
    }
    return h0 ^ h1;
}

i64 MOD;
i64 C[100][100];
i64 pw[100][100];
i64 pw2[PMAX];
int m, d[100];

i64 enumerate(int i, i64 cnt) {
    i64 rax = 0;

    if (i >= m) {
        return pw2[cnt];
    } else {
        int j = i;
        for ( ; j < m && d[j] == d[i]; j++) {}
        int len = j - i;

        for (int c0 = 0; c0 <= len; c0++)
        for (int c1 = 0; c1 <= len - c0; c1++) {
            int c2 = len - c0 - c1;
            rax = (rax +
                (c1 & 1 ? -1 : 1) *
                C[len][c0] *
                C[len - c0][c1] % MOD *
                pw2[c1] % MOD *
                enumerate(j, cnt * pw[d[i] + 1][c0] * pw[d[i]][c1] * pw[d[i] - 1][c2])) % MOD;
        }
    }

    return rax;
}

i64 eval() {
    i64 v = enumerate(0, 1);
    return v;
    // i64 c = 1;
    // for (int j = 0, k; j < m; j = k) {
    //     for (k = j; k < m && d[k] == d[j]; k++) {}
    //     c *= k - j + 1;
    // }

    // return (pw2[c] - v) % MOD;
}

unordered_map<u64, i64> ans;

// int now;
void dfs(int i, i64 n, int maxa) {
    if (n < 1)
        return;

    // now++;
    u64 h = H(i, d);
    m = i;
    i64 v = eval();
    if (v < 0)
        v += MOD;
    assert(!ans.count(h));
    ans[h] = v;
    // printf("[%d] H=%016llx, v=%lld\n", now, h, v);

    if (i >= CMAX)
        return;

    for (int &a = d[i] = 1; a <= maxa; a++) {
        n /= prime[i];
        dfs(i + 1, n, a);
    }
}

int main() {
    int T;
    scanf("%d%lld", &T, &MOD);

    mt19937_64 gen(0xdeadbeef19260817);
    for (int i = 0; i < AMAX; i++) {
        rwt[i] = gen();
    }

    for (int i = 0; i < 100; i++) {
        C[i][0] = 1;
    }
    for (int i = 1; i < 100; i++)
    for (int j = 1; j <= i; j++) {
        C[i][j] = (C[i - 1][j] + C[i - 1][j - 1]) % MOD;
    };

    for (int i = 0; i < 100; i++) {
        pw[i][0] = 1;
        for (int j = 1; j < 100; j++) {
            pw[i][j] = pw[i][j - 1] * i % MOD;
        }
    }

    pw2[0] = 1;
    for (int i = 1; i < PMAX; i++) {
        pw2[i] = pw2[i - 1] * 2 % MOD;
    }

    dfs(0, NMAX, AMAX);
    // puts("done");

    while (T--) {
        i64 n;
        scanf("%lld", &n);

        for (int i = 0; i < 25; i++) {
            for (int &j = d[i] = 0; n % prime[i] == 0; j++) {
                n /= prime[i];
            }
        }

        sort(d, d + 25, greater<int>());
        m = find(d, d + 25, 0) - d;
        u64 h = H(m, d);
        i64 v = ans[h];
        printf("%lld\n", v);
    }

    return 0;
}