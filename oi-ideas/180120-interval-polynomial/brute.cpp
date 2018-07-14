#include <cstdio>
#include <cctype>
#include <cstring>

#include <algorithm>

using namespace std;

#define MOD 998244353

typedef long long i64;

static int n, q, kmax;
static i64 poly[200];
static i64 seq[10000];

inline i64 eval(i64 x) {
    i64 r = 0;
    for (int i = 100; i >= 0; i--) {
        r = (r * x + poly[i]) % MOD;
    }
    return r;
}

inline char getch() {
    char c;
    do {
        c = getchar();
    } while (!isalpha(c));
    return c;
}

int main() {
    scanf("%d%d%d", &n, &q, &kmax);

    for (int i = 0; i < q; i++) {
        char op = getch();
        int l, r;
        scanf("%d%d", &l, &r);

        if (op == 'M') {
            int k;
            scanf("%d", &k);
            memset(poly, 0, sizeof(poly));
            for (int i = 0; i < k; i++) {
                int x, y;
                scanf("%d%d", &x, &y);
                poly[y] = x;
            }

            for (int i = 1; i <= r - l + 1; i++) {
                seq[l + i - 1] += eval(i);
                seq[l + i - 1] %= MOD;
            }
        } else {
            i64 ans = 0;
            for (int i = l; i <= r; i++) {
                ans = (ans + seq[i]) % MOD;
            }

            if (ans < 0)
                ans += MOD;

            printf("%lld\n", ans);
        }
    }

    return 0;
}
