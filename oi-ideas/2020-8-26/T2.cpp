#include <cstdio>
#include <cstring>
#include <cassert>

#include <algorithm>

using namespace std;

#define NMAX 128
#define MMAX 200
#define MOD 1000000007

using i64 = long long;

void add(i64 &a, i64 b) {
    assert(a >= 0);
    assert(b >= 0);
    a += b;
    if (a >= MOD)
        a -= MOD;
    assert(a < MOD);
}

i64 inv(i64 x) {
    assert(0 < x);
    assert(x < MOD);
    return x == 1 ? 1 : MOD - MOD / x * inv(MOD % x) % MOD;
}

int n;

static i64 _c[NMAX][NMAX];

void _swap(int i, int j) {
    for (int k = 0; k < n; k++) {
        swap(_c[i][k], _c[j][k]);
    }
}

void _add(int i, i64 v, int j) {
    if (v == MOD)
        return;
    for (int k = 0; k < n; k++) {
        add(_c[j][k], v * _c[i][k] % MOD);
    }
}

void _mul(int i, i64 v) {
    for (int k = 0; k < n; k++) {
        _c[i][k] = _c[i][k] * v % MOD;
    }
}

struct mat {
    mat() {
        memset(a, 0, sizeof(a));
    }
    mat(const mat &z) {
        memcpy(a, z.a, sizeof(a));
    }

    i64 a[NMAX][NMAX];

    void to_inv() {
        memset(_c, 0, sizeof(_c));
        for (int i = 0; i < n; i++) {
            _c[i][i] = 1;
        }

        for (int i = 0; i < n; i++) {
            int p = i;
            for ( ; a[p][i] == 0 && p < n; p++) ;
            assert(p < n);
            if (p != i) {
                _swap(i, p);
                for (int j = i; j < n; j++) {
                    swap(a[i][j], a[p][j]);
                }
            }

            i64 v = inv(a[i][i]);
            _mul(i, v);
            for (int j = i; j < n; j++) {
                a[i][j] = a[i][j] * v % MOD;
            }

            for (int j = i + 1; j < n; j++) {
                _add(i, MOD - a[j][i], j);
                i64 x = a[j][i];
                for (int k = i; k < n; k++) {
                    add(a[j][k], MOD - x * a[i][k] % MOD);
                }
            }
        }

        for (int i = n - 1; i; i--) {
            assert(a[i][i] == 1);
            for (int j = i - 1; j >= 0; j--) {
                _add(i, MOD - a[j][i], j);
            }
        }

        memcpy(a, _c, sizeof(a));
    }

    auto operator[](int i) -> i64* {
        return a[i] + 0;
    }

    auto operator[](int i) const -> const i64* {
        return a[i] + 0;
    }

    auto operator=(const mat &z) -> mat& {
        memcpy(a, z.a, sizeof(a));
        return *this;
    }

    auto operator+=(const mat &b) -> mat& {
        for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            add(a[i][j], b[i][j]);
        }
        return *this;
    }

    auto operator-=(const mat &b) -> mat& {
        for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            add(a[i][j], MOD - b[i][j]);
        }
        return *this;
    }

    auto operator*=(i64 v) -> mat& {
        for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            a[i][j] = a[i][j] * v % MOD;
        }
        return *this;
    }

    auto operator*=(const mat &b) -> mat& {
        memset(_c, 0, sizeof(_c));
        for (int i = 0; i < n; i++)
        for (int k = 0; k < n; k++)
        for (int j = 0; j < n; j++) {
            add(_c[i][j], a[i][k] * b[k][j] % MOD);
        }
        memcpy(a, _c, sizeof(a));
        return *this;
    }

    void print() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                printf("%lld ", a[i][j]);
            }
            puts("");
        }
    }
};

template <typename T>
T identity();

template <>
mat identity() {
    mat c;
    for (int i = 0; i < n; i++) {
        c[i][i] = 1;
    }
    return c;
}

template <typename T>
auto qpow(T a, i64 k) -> T {
    T r = identity<T>();
    for ( ; k; k >>= 1) {
        if (k & 1)
            r *= a;
        a *= a;
    }
    return r;
}

i64 d;
i64 s[MMAX + 10][MMAX + 10];
i64 fall[MMAX + 10];
int m;

int main() {
    scanf("%d%lld%d", &n, &d, &m);

    fall[0] = 1;
    for (int i = 1; i <= m; i++) {
        i64 v = (d - i + 2) % MOD;
        if (v < 0)
            v += MOD;
        fall[i] = fall[i - 1] * v % MOD;
    }

    for (int i = 0; i <= m; i++) {
        s[i][0] = 0;
        s[i][i] = 1;
    }

    for (int i = 1; i <= m; i++)
    for (int j = 1; j < i; j++) {
        s[i][j] = (j * s[i - 1][j] + s[i - 1][j - 1]) % MOD;
    }

    mat A;
    for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) {
        scanf("%lld", &A[i][j]);
    }

    mat I = identity<mat>();
    mat U = qpow(A, d + 1);
    mat V = A;
    V -= I;
    V.to_inv();
    mat G0 = U;
    G0 -= I;
    mat G = V;
    G *= G0;

    mat Z;
    for (int i = 0; i <= m; i++) {
        if (i) {
            mat T = A;
            T *= i;
            T *= G;

            mat S = U;
            S *= fall[i];
            S -= T;

            G = V;
            G *= S;
        }

        mat S = G;
        S *= s[m][i];
        Z += S;
    }

    Z.print();

    return 0;
}
