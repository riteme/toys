#include <cassert>
#include <cstdio>
#include <cstring>

#include <random>

static std::random_device rd;

typedef long long i64;

inline i64 qpow(i64 x, i64 p, i64 m) {
    i64 r = 1;
    for (; p; p >>= 1, x = x * x % m)
        if (p & 1) r = r * x % m;
    return r;
}

int main() {
    // x^2 mod p = y, p odd prime
    i64 p, y;
    scanf("%lld%lld", &p, &y);
    assert(qpow(y, (p - 1) / 2, p) == 1);

    i64 b;
    do {
        b = rd() % p;
    } while (!b || qpow(b, (p - 1) / 2, p) == 1);

    int t = __builtin_ctz(p - 1);
    int s = (p - 1) >> t;
    i64 x0 = qpow(y, (s + 1) / 2, p), inv = qpow(y, p - 2, p);
    for (int k = t - 1; k; k--) {
        i64 e = x0 * x0 % p * inv % p;
        if (qpow(e, 1 << (k - 1), p) != 1)
            x0 = x0 * qpow(b, (1 << (t - 1 - k)) * s, p) % p;
    }
    printf("%lld %lld\n", x0, p - x0);

    return 0;
}
