#include <cstdio>
#include <cstring>

#include <algorithm>

using namespace std;

typedef long long i64;
typedef __int128_t i128;

#define NMAX 100000

static int n, q;
static i64 A[NMAX + 10];
static i64 bit[NMAX + 10], tag[NMAX + 10];

void initialize() {
    for (int i = 2; i <= n; i++)
        A[i] += A[i - 1];
}

void _modify(int l, i64 v) {
    for (int x = l; x <= n; x += x & -x) {
        bit[x] += (x - l + 1) * v;
        tag[x] += v;
    }
}

i64 _query(int r) {
    i64 ret = 0;
    for (int x = r; x; x -= x & -x)
        ret += bit[x] + (r - x) * tag[x];
    return ret;
}

void modify(int l, int r, i64 v) {
    _modify(l, v);
    _modify(r + 1, -v);
}

i64 query(int l, int r) {
    return _query(r) - _query(l - 1) + A[r] - A[l - 1];
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
