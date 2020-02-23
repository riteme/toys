#include <cstdio>
#include <cstring>

#include <list>
#include <deque>
#include <algorithm>

using namespace std;

#define NMAX 5000

static int n, K;
static char G[NMAX + 10][NMAX + 10];
static list<int> seq[NMAX + 10];
static int d[NMAX + 10];
static bool in[NMAX + 10];

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

    deque<int> q;
    for (int i = 1; i <= n; i++) {
        in[i] = true;
        q.push_back(i);
    }

    int new_count = 0;
    long step_count = 0;
    while (!q.empty()) {
        int u = q.front();
        q.pop_front();
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
                    if (!q.empty() && d[q.front()] <= d[u])
                        q.push_front(u);
                    else
                        q.push_back(u);
                }
                if (!in[v]) {
                    in[v] = true;
                    if (!q.empty() && d[q.front()] <= d[v])
                        q.push_front(v);
                    else
                        q.push_back(v);
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