#include <cstdio>
#include <cstring>
#include <climits>

#include <algorithm>

using namespace std;

#define NMAX 200

static int n, m;
static bool marked[NMAX + 10];
static int W[NMAX + 10][NMAX + 10], C[NMAX + 10][NMAX + 10];

void initialize() {
    scanf("%d%d", &n, &m);

    memset(W, 0x3f, sizeof(W));
    for (int i = 0; i < m; i++) {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        W[u][v] = W[v][u] = w;
        C[u][v] = C[v][u] = 1;
    }
}

int main() {
    initialize();

    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) for (int j = 1; j <= n; j++) {
            if (i == j || i == k || j == k) continue;
            if (W[i][k] + W[k][j] > W[i][j]) continue;
            if (W[i][k] + W[k][j] < W[i][j]) C[i][j] = 0;
            W[i][j] = W[i][k] + W[k][j];
            C[i][j] += C[i][k] * C[k][j];
        }
    }

    for (int k = 1; k <= n; k++) {
        for (int i = 1; i <= n; i++) for (int j = 1; j <= n; j++) {
            if (i == j || i == k || j == k) continue;
            if (W[i][k] + W[k][j] == W[i][j] && C[i][j] == 1)
                marked[k] = true;
        }
    }

    int cnt = 0;
    for (int i = 1; i <= n; i++)
        if (marked[i]) {
            printf("%d ", i);
            cnt++;
        }
    if (!cnt) puts("No important cities.");

    return 0;
}
