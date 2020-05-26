#include <cstdio>
#include <cstring>

#include <vector>
#include <algorithm>

using namespace std;

using i64 = long long;

constexpr int NMAX = 200000;
constexpr int KMAX = 20;
constexpr i64 MOD = 1000000007;

struct Matrix {
    i64 a[KMAX][KMAX];

    Matrix() {
        memset(a, 0, sizeof(a));
    }

    auto operator[](int i) -> i64 * {
        return a[i];
    }

    auto operator*=(Matrix &b) -> Matrix & {
        i64 c[KMAX][KMAX];
        memset(c, 0, sizeof(c));
        for (int i = 0; i < KMAX; i++)
        for (int k = 0; k < KMAX; k++)
        for (int j = 0; j < KMAX; j++) {
            c[i][j] = (c[i][j] + a[i][k] * b[k][j]) % MOD;
        }
        memcpy(a, c, sizeof(a));
        return *this;
    }

    void apply(i64 *b) {
        i64 c[KMAX];
        memset(c, 0, sizeof(c));
        for (int i = 0; i < KMAX; i++)
        for (int j = 0; j < KMAX; j++) {
            c[i] = (c[i] + a[i][j] * b[j]) % MOD;
        }
        memcpy(b, c, sizeof(c));
    }
};

int n, q;
i64 M, a[NMAX + 10];
int cnt[KMAX][NMAX + 10];
vector<Matrix> pw[KMAX];

int main() {
    scanf("%d%lld", &n, &M);
    for (int i = 1; i <= n; i++) {
        scanf("%lld", a + i);
        a[i] %= M;

        cnt[a[i]][i]++;
        for (int k = 0; k < M; k++) {
            cnt[k][i] += cnt[k][i - 1];
        }
    }

    for (int k = 0; k < M; k++) {
        pw[k].resize(cnt[k][n] + 1);

        Matrix T;
        for (int i = 0; i < M; i++) {
            T[i][i]++;
        }

        pw[k][0] = T;

        for (int i = 0; i < M; i++) {
            T[(i + k) % M][i]++;
        }

        for (int i = 1; i <= cnt[k][n]; i++) {
            pw[k][i] = pw[k][i - 1];
            pw[k][i] *= T;
        }
    }

    scanf("%d", &q);
    while (q--) {
        int l, r;
        scanf("%d%d", &l, &r);

        i64 b[KMAX];
        memset(b, 0, sizeof(b));
        b[0] = 1;
        for (int k = 0; k < M; k++) {
            int c = cnt[k][r] - cnt[k][l - 1];
            // printf("k=%d, c=%d\n", k, c);
            pw[k][c].apply(b);
        }

        i64 ans = b[0] % MOD;
        if (ans < 0)
            ans += MOD;
        printf("%lld\n", ans);
    }

    return 0;
}