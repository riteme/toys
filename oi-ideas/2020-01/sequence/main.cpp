#include <cstdio>
#include <cstring>

#include <algorithm>

using namespace std;

#define NMAX 10000000
#define KMAX 64
#define MOD 998244353

typedef long long i64;

static i64 n, m;
static i64 inv[KMAX + 10];
static i64 pcnt, pr[NMAX + 10];
static bool vis[NMAX + 10];
static i64 k[NMAX + 10], f[NMAX + 10];

int main() {
    inv[1] = 1;
    for (i64 i = 2; i <= KMAX; i++)
        inv[i] = MOD - MOD / i * inv[MOD % i] % MOD;

    scanf("%lld%lld", &n, &m);
    // f[1] = 1;
    for (i64 i = 2; i <= n; i++) {
        if (!vis[i]) {
            k[i] = 1;
            f[i] = m;
            pr[pcnt++] = i;
        }
        for (int j = 0; i * pr[j] <= n; j++) {
            i64 p = pr[j], t = i * p;
            vis[t] = true;
            if (i % p) {
                k[t] = 1;
                f[t] = f[i] * m % MOD;
            } else {
                k[t] = k[i] + 1;
                f[t] = f[i] * (k[i] + m) % MOD * inv[k[t]] % MOD;
                break;
            }
        }
    }

    i64 ans = 1;
    for (i64 i = 2; i <= n; i++)
        ans = (ans + f[i]) % MOD;
    if (ans < 0) ans += MOD;
    printf("%lld\n", ans);

    return 0;
}