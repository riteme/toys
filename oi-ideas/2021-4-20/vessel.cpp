#pragma GCC option("unroll-loops")

#include <cmath>
#include <cstdio>
#include <cstring>

#include <algorithm>

using namespace std;

using i64 = long long;

constexpr i64 SCALE_FACTOR[] = {0, 1, 2, 5, 13, 28, 58, 120, 246, 498, 1006};

i64 icbrt(i64 n) {
    i64 r = i64(cbrtl(n) + 0.5L) + 2;
    while (r * r * r > n) {
        r--;
    }
    return r;
}

i64 csqrt(i64 n) {
    i64 r = max(i64(sqrtl(n) - 0.5L) - 2, 0LL);
    while (r * r < n) {
        r++;
    }
    return r;
}

i64 fsqrt(i64 n) {
    i64 r = i64(sqrtl(n) + 0.5L) + 2;
    while (r * r > n) {
        r--;
    }
    return r;
}

i64 eval3(i64 n) {
    i64 v = 0, p = icbrt(n);

    for (i64 k = 2; ; k++) {
        i64 q = n / (k * k);
        if (q < p)
            break;
        v += q;
    }

    for (i64 q = 1; q < p; q++) {
        i64 l = max(4LL, n / (q + 1) + 1);
        i64 r = max(l, n / q);
        i64 c = fsqrt(r) - csqrt(l) + 1;
        // printf("[%lld, %lld] q=%lld, c=%lld\n", l, r, q, c);
        v += c * q;
    }

    return v;
}

i64 eval(i64 n, i64 K) {
    if (K == 3)
        return eval3(n);

    i64 v = 0;
    for (int i = 2; ; i++) {
        i64 pw = 1;
        for (int j = 1; j < K; j++) {
            pw *= i;
        }

        i64 c = n / pw;
        if (c == 0)
            break;

        v += c;
    }

    return v;
}

i64 solve(i64 m, i64 K) {
    i64 l = 0, r = SCALE_FACTOR[K] * m;
    i64 v = -1;
    while (l < r) {
        i64 n = (l + r) / 2;

        v = eval(n, K);
        if (v < m)
            l = n + 1;
        else
            r = n;
    }

    v = eval(l, K);
    return v == m ? l : -1;
}

int main() {
    int T;
    scanf("%d", &T);
    while (T--) {
        i64 n, m, K;
        scanf("%lld%lld", &K, &m);
        n = solve(m, K);
        printf("%lld\n", n);
    }

    return 0;
}