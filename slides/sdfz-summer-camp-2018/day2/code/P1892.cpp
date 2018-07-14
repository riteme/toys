#include <cstdio>
#include <cstring>

#include <vector>
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

static bool mark[NMAX + 10];
static vector<int> G[NMAX + 10];

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    while (m--) {
        char buf[10];
        int u, v;
        scanf("%s%d%d", buf, &u, &v);
        if (buf[0] == 'F')
            meld(u, v);
        else {
            G[u].push_back(v);
            G[v].push_back(u);
        }
    }

    for (int u = 1; u <= n; u++) {
        for (int i = 1; i < G[u].size(); i++) {
            meld(G[u][i - 1], G[u][i]);
        }
    }

    int ans = 0;
    for (int i = 1; i <= n; i++) {
        int x = find(i);
        if (!mark[x]) {
            ans++;
            mark[x] = true;
        }
    }
    printf("%d\n", ans);

    return 0;
}
