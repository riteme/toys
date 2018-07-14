#include <cstdio>
#include <cstring>

#include <algorithm>

using namespace std;

#define NMAX 100000

typedef long long i64;

static int n;
static i64 z, d[NMAX + 10], a[NMAX + 10];
static int S1[NMAX + 10], S2[NMAX + 10], t1, t2;

bool cmp1(const int x, const int y) {
    return d[x] < d[y];
}

bool cmp2(const int x, const int y) {
    return a[x] > a[y];
}

void initialize() {
    scanf("%d%lld", &n, &z);
    for (int i = 1; i <= n; i++) {
        scanf("%lld%lld", d + i, a + i);
        if (a[i] > d[i]) S1[t1++] = i;
        else S2[t2++] = i;
    }
}

static int seq[NMAX + 10], tail;

bool fuck() {
#define FUCK(x) \
    {if (z <= d[x]) \
        return false; \
    z += a[x] - d[x]; \
    seq[tail++] = x;}

    for (int i = 0; i < t1; i++)
        FUCK(S1[i]);
    for (int i = 0; i < t2; i++)
        FUCK(S2[i]);
    return true;
}

int main() {
    initialize();
    sort(S1, S1 + t1, cmp1);
    sort(S2, S2 + t2, cmp2);
    if (fuck()) {
        puts("TAK");
        for (int i = 0; i < tail; i++)
            printf("%d ", seq[i]);
    } else puts("NIE");
    return 0;
}
