#include <cstdio>
#include <cstring>

#include <algorithm>

using namespace std;

typedef long long i64;

constexpr int MMAX = 1000000;

inline i64 qpow(i64 x, i64 p, i64 m) {
    i64 ret = 1;
    for (; p; p >>= 1) {
        if (p & 1) ret = ret * x % m;
        x = x * x % m;
    }
    return ret;
}

static int pcnt, primes[MMAX];
static bool mark[MMAX];
static i64 pw[MMAX];

int main() {
    // x^p mod m = y
    i64 p, m, y;
    scanf("%lld%lld%lld", &p, &m, &y);

    pw[1] = 1;
    for (int i = 2; i < m; i++) {
        if (!mark[i]) {
            primes[pcnt++] = i;
            pw[i] = qpow(i, p, m);
        }

        for (int j = 0; i * primes[j] < m; j++) {
            int t = i * primes[j];
            mark[t] = true;
            pw[t] = pw[i] * pw[primes[j]] % m;
            if (i % primes[j] == 0) break;
        }
    }

    for (int i = 0; i < m; i++) {
        if (pw[i] == y)
            printf("%d ", i);
    }
    puts("");

    return 0;
}
