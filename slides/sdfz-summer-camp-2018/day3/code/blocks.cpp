#include <cstdio>
#include <cstring>

#include <algorithm>

using namespace std;

typedef long long i64;

#define NMAX 100000

static int n, q;
static i64 A[NMAX + 10];

#define SMAX 300
#define BMAX (NMAX / SMAX + 1)
#define BID(x) ((x - 1) / SMAX)
#define L(x) (SMAX * x + 1)
#define R(x) min(SMAX * (x + 1), n)
#define NUM(x) (R(x) - L(x) + 1)

static i64 offest[BMAX + 10], sum[BMAX + 10];

void initialize() {
    for (int i = 1; i <= n; i++)
        sum[BID(i)] += A[i];
}

void modify(int l, int r, i64 v) {
    int bl = BID(l), br = BID(r);
    for (int i = bl; i <= br; i++) {
        if (l > L(i) || r < R(i)) {
            int rl = max(l, L(i)), rr = min(r, R(i));
            sum[i] += (rr - rl + 1) * v;
            for (int j = rl; j <= rr; j++)
                A[j] += v;
        } else offest[i] += v;
    }
}

i64 query(int l, int r) {
    int bl = BID(l), br = BID(r);
    i64 ret = 0;
    for (int i = bl; i <= br; i++) {
        if (l > L(i) || r < R(i)) {
            int rl = max(l, L(i)), rr = min(r, R(i));
            ret += (rr - rl + 1) * offest[i];
            for (int j = rl; j <= rr; j++)
                ret += A[j];
        } else ret += sum[i] + NUM(i) * offest[i];
    }
    return ret;
}

int main() {
    scanf("%d%d", &n, &q);
    for (int i = 1; i <= n; i++)
        scanf("%lld", A + i);

    initialize();
    while (q--) {
        int op, l, r;
        scanf("%d%d%d", &op, &l, &r);

        if (op == 1) {
            i64 v;
            scanf("%lld", &v);
            modify(l, r, v);
        } else printf("%lld\n", query(l, r));
    }

    return 0;
}
