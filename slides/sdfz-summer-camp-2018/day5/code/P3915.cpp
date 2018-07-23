#include <cstdio>
#include <cstring>
#include <climits>

#include <vector>
#include <algorithm>

using namespace std;

#define NMAX 100000

static int n, k;
static vector<int> T[NMAX + 10];
static int size[NMAX + 10];

void initialize() {
    scanf("%d%d", &n, &k);
    for (int i = 1; i < n; i++) {
        int u, v;
        scanf("%d%d", &u, &v);
        T[u].push_back(v);
        T[v].push_back(u);
    }
}

void finalize() {
    for (int u = 1; u <= n; u++) {
        T[u].clear();
    }
}

void dfs(int x, int f) {
    size[x] = 1;
    for (int v : T[x]) {
        if (v == f) continue;
        dfs(v, x);
        size[x] += size[v];
    }
}

int cnt() {
    int ret = 0;
    for (int u = 1; u <= n; u++) {
        if (size[u] < n && size[u] % k == 0) ret++;
    }
    return ret;
}

int main() {
    int t;
    scanf("%d", &t);
    while (t--) {
        initialize();
        dfs(1, 0);
        if (n % k == 0 && cnt() == n / k - 1) puts("YES");
        else puts("NO");
        finalize();
    }
    return 0;
}

