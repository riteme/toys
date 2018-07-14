#include <cstdio>
#include <cstring>

#include <algorithm>

using namespace std;

typedef long long i64;

#define NMAX 100000

static int n, q;
static i64 A[NMAX + 10];

void initialize() {}

void modify(int l, int r, i64 v) {
    for (int i = l; i <= r; i++) A[i] += v;
}

i64 query(int l, int r) {
    i64 ret = 0;
    for (int i = l; i <= r; i++) ret += A[i];
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
