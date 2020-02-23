#include <cstdio>
#include <cstring>

#include <list>
#include <queue>
#include <algorithm>

using namespace std;

#define NMAX 5000

static int n, K;
static char G[NMAX + 10][NMAX + 10];
static list<int> seq[NMAX + 10];
static int d[NMAX + 10];
static bool in[NMAX + 10];
static int _sort[NMAX + 10];

int main() {
    scanf("%d%d", &n, &K);
    for (int i = 1; i <= n; i++) {
        scanf("%s", G[i] + 1);
        d[i] = count(G[i] + 1, G[i] + n + 1, '1');
    }
    for (int i = 1; i <= n; i++)
    for (int j = 1; j <= n; j++)
    if (i != j && G[i][j] == '0' && d[j] + n - 1 >= K)
        seq[i].push_back(j);

    queue<int> q;
    for (int i = 1; i <= n; i++)
        _sort[i] = i;
    sort(_sort + 1, _sort + n + 1, [](int x, int y) {
        return d[x] > d[y];
    });
    for (int i = 1; i <= n; i++) {
        in[i] = true;
        q.push(_sort[i]);
    }

    int new_count = 0;
    long step_count = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        in[u] = false;
        for (auto it = seq[u].begin(); it != seq[u].end(); it++) {
            step_count++;
            int v = *it;
            if (G[u][v] == '0' && d[u] + d[v] >= K) {
                new_count++;
                G[u][v] = G[v][u] = '1';
                d[u]++;
                d[v]++;
                if (!in[u]) {
                    in[u] = true;
                    q.push(u);
                }
                if (!in[v]) {
                    in[v] = true;
                    q.push(v);
                }
            }
        }
    }

    fprintf(stderr, "new:  %d\n", new_count);
    fprintf(stderr, "step: %ld\n", step_count);
    for (int i = 1; i <= n; i++)
        puts(G[i] + 1);
    return 0;
}
