#include <cstdio>
#include <cstring>

#include <algorithm>

using namespace std;

#define NMAX 5000

static int n, K;
static char G[NMAX + 10][NMAX + 10];
static int d[NMAX + 10];

int main() {
    scanf("%d%d", &n, &K);
    for (int i = 1; i <= n; i++)
        scanf("%s", G[i] + 1);
    for (int i = 1; i <= n; i++)
        d[i] = count(G[i] + 1, G[i] + n + 1, '1');

    int new_count = 0;
    int stage_count = 0;
    while (true) {
        stage_count++;
        bool neo = false;
        for (int u = 1; u <= n; u++)
        for (int v = u + 1; v <= n; v++)
        if (G[u][v] == '0' && d[u] + d[v] >= K) {
            neo = true;
            new_count++;
            G[u][v] = G[v][u] = '1';
            fprintf(stderr, "%d - %d\n", u, v);
            d[u]++;
            d[v]++;
        }

        if (!neo) break;
    }

    fprintf(stderr, "new:   %d\n", new_count);
    fprintf(stderr, "stage: %d\n", stage_count);
    for (int i = 1; i <= n; i++)
        puts(G[i] + 1);
    return 0;
}
