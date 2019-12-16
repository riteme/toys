#include <cstdio>
#include <cstring>

#include <list>
#include <queue>
#include <algorithm>

using namespace std;

#define NMAX 5000

typedef list<int>::iterator Index;

static int n, K;
static char G[NMAX + 10][NMAX + 10];
static int d[NMAX + 10], d0[NMAX + 10];
static list<int> B[NMAX + 10];
static Index p[NMAX + 10];

static queue<int> q;

static int new_count;
void add(int u) {
    B[d[u]].erase(p[u]);
    B[++d[u]].push_front(u);
    p[u] = B[d[u]].begin();

    if (d[u] > K || K - d[u] > n - 1)
        return;
    for (int v : B[K - d[u]])
    if (u != v && G[u][v] == '0') {
        // fprintf(stderr, "new: %d - %d\n", u, v);
        new_count++;
        G[u][v] = G[v][u] = '1';
        q.push(u);
        q.push(v);
    }
}

void solve() {
    for (int i = 1; i <= n; i++)
    for (int j = 0; j < d0[i]; j++)
        q.push(i);
    for ( ; !q.empty(); q.pop())
        add(q.front());
}

int main() {
    scanf("%d%d", &n, &K);
    for (int i = 1; i <= n; i++) {
        scanf("%s", G[i] + 1);
        d0[i] = count(G[i] + 1, G[i] + n + 1, '1');
        B[0].push_front(i);
        p[i] = B[0].begin();
    }

    if (K == 0) {
        for (int i = 1; i <= n; i++)
        for (int j = 1; j <= n; j++)
            if (i != j) G[i][j] = '1';
    } else solve();

    //fprintf(stderr, "new: %d\n", new_count);
    for (int i = 1; i <= n; i++)
        puts(G[i] + 1);

    return 0;
}
