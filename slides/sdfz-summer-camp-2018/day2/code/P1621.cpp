#include <cstdio>
#include <cstring>

#include <algorithm>

using namespace std;

#define NMAX 100000

static int fa[NMAX + 10];

inline void link(int x, int y) {
    if (x == y) return;
    if (fa[x] > fa[y]) swap(x, y);
    if (fa[x] == fa[y]) fa[x]--;
    fa[y] = x;
}

inline int find(int x) {
    return fa[x] > 0 ? fa[x] = find(fa[x]) : x;
}

inline void meld(int x, int y) {
    link(find(x), find(y));
}

inline bool isp(int n) {
    for (int k = 2; k * k <= n; k++) {
        if (n % k == 0) return false;
    }
    return true;
}

static bool mark[NMAX + 10];

int main() {
    int A, B, P;
    scanf("%d%d%d", &A, &B, &P);
    for (int p = P; p <= B; p++) {
        if (!isp(p)) continue;
        int last = 0;
        for (int k = 1; p * k <= B; k++) {
            int t = p * k;
            if (t < A) continue;
            if (last) meld(last, t);
            last = t;
        }
    }

    int ans = 0;
    for (int i = A; i <= B; i++) {
        int x = find(i);
        if (!mark[x]) {
            mark[x] = true;
            ans++;
        }
    }

    printf("%d\n", ans);

    return 0;
}
