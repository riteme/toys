/**
 * time complexity:
 *   O(n + T(ρ + n log A + ω2^ω))
 *
 * where
 *   ρ: the complexity to factor P. typically polylog O(P^(1/4)).
 *   A: max{P, max{a_i: i = 1..n}}
 *   ω: number of prime divisors of P
 */

#include <cstdio>
#include <cstring>

#include <vector>
#include <algorithm>
#include <functional>

using namespace std;

using i64 = long long;
using i8 = __int128_t;
using vi = vector<i64>;

constexpr int NMAX = 300000;
constexpr int WMAX = 15;
constexpr int SMAX = 1 << WMAX;
constexpr i64 MOD  = 1000000007;

i64 qmul(i64 a, i64 b, i64 m) {
    return i8(a) * b % m;
}

i64 qpow(i64 a, i64 k, i64 m) {
    i64 r = 1;
    for ( ; k; k >>= 1) {
        if (k & 1)
            r = qmul(r, a, m);
        a = qmul(a, a, m);
    }
    return r;
}

bool miller_rabin(i64 n, i64 base) {
    i64 n2 = n - 1, s = 0;
    while (n2 % 2 == 0) {
        n2 /= 2;
        s++;
    }

    i64 t = qpow(base, n2, n);
    if (t == 1 || t == n - 1)
        return true;

    for (s--; s >= 0; s--) {
        if ((t = qmul(t, t, n)) == n - 1)
            return true;
    }

    return false;
}

bool is_prime(i64 n) {
    constexpr i64 base[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };

    if (n < 2)
        return false;

    for (i64 b : base) {
        if (b < n) {
            if (!miller_rabin(n, b))
                return false;
        } else
            break;
    }

    return true;
}

i64 rho(i64 n) {
    auto f = [n](i64 x) {
        i64 r = qmul(x, x, n) + 1;
        if (r >= n)
            r -= n;
        return r;
    };

    //if (is_prime(n))
    //    return n;
    if (n % 2 == 0)
        return 2;

    for (int i = 1; ; i++) {
        i64 x = i, y = f(x), p = __gcd(y - x, n);
        while (p == 1) {
            x = f(x);
            y = f(f(y));
            i64 z = y - x;
            if (z < 0)
                z += n;
            p = __gcd(z, n);
        }

        if (p != 0 && p != n)
            return p;
    }
}

vi factor(i64 n) {
    if (n == 1)
        return {};

    vi rax;
    function<void(i64)> _factor = [&rax, &_factor](i64 n) {
        if (is_prime(n))
            rax.push_back(n);
        else {
            i64 d = rho(n);
            _factor(d);
            _factor(n / d);
        }
    };

    _factor(n);

    sort(rax.begin(), rax.end());
    int cnt = unique(rax.begin(), rax.end()) - rax.begin();
    rax.resize(cnt);
    return rax;
}

int n, m;
i64 a[NMAX + 10];
i64 pw0[NMAX + 10], pw1[NMAX + 10];
i64 f[SMAX], sgn[SMAX];

void fwt() {
    for (int l = 1; l < m; l *= 2)
    for (int i = 0; i < m; i += l * 2)
    for (int j = 0; j < l; j++) {
        i64 x = f[i + j], y = f[i + j + l];
        f[i + j] = x + y;
        f[i + j + l] = y;
    }
}

void _main() {
    i64 Q, K, P;
    scanf("%lld%lld%lld", &Q, &K, &P);

    pw0[0] = pw1[0] = 1;
    for (int i = 1; i <= n; i++) {
        pw0[i] = pw0[i - 1] * K % MOD;
        pw1[i] = pw1[i - 1] * (K + 1) % MOD;
    }

    vi D = factor(P);
    m = 1 << D.size();
    memset(f, 0, sizeof(i64) * m);

    for (int i = 1; i <= n; i++) {
        int S = 0;
        i64 na = __gcd(a[i], P);
        for (int j = 0; j < D.size(); j++) {
            i64 x = na, y = 1;
            while (x % D[j] == 0) {
                x /= D[j];
                y *= D[j];
            }

            if (Q % y != 0)
                S |= 1 << j;
        }

        f[S]++;
    }

    fwt();

    i64 ans = 0;
    for (int S = 0; S < m; S++) {
        ans = (ans + sgn[S] * pw0[n - f[S]] * pw1[f[S]]) % MOD;
    }

    if (P == 1) {
        // if P = 1, it is natural to define f_0(\varnothing) = 1.
        // however empty set has been explicitly excluded.
        // TODO: remove contribution from the empty set.
    }

    if (ans < 0)
        ans += MOD;
    printf("%lld\n", ans);
}

int main() {
    sgn[0] = 1;
    for (int S = 1; S < SMAX; S++) {
        sgn[S] = sgn[S >> 1] * ((S & 1) ? -1 : 1);
    }

    int T;
    scanf("%d%d", &n, &T);
    for (int i = 1; i <= n; i++) {
        scanf("%lld", a + i);
    }

    while (T--) {
        _main();
    }

    return 0;
}
