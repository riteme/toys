#include <cstdio>
#include <cstring>

#include <algorithm>

using namespace std;

#define NMAX 5000

static int n, K;
static char G[NMAX + 10][NMAX + 10];
static int d[NMAX + 10], d0[NMAX + 10], id[NMAX + 10];

int main() {
    scanf("%d%d", &n, &K);
    for (int i = 1; i <= n; i++) {
        scanf("%s", G[i] + 1);
        d[i] = count(G[i] + 1, G[i] + n + 1, '1');
        id[i] = i;
    }

    int new_count = 0;
    int step_count = 0;
    while (true) {
        step_count++;
        bool updated = false;
        sort(id + 1, id + n + 1, [](int x, int y) {
            return d[x] > d[y];
        });
        memset(d0 + 1, 0, sizeof(int) * n);

        int j = n;
        for (int i = 1; i <= n && j; i++) {
            while (j && d[id[i]] + d[id[j]] < K) j--;
            for (int k = 1; k <= j; k++)
            if (id[i] != id[k] && G[id[i]][id[k]] == '0') {
                updated = true;
                new_count++;
                G[id[i]][id[k]] = '1';
                G[id[k]][id[i]] = '1';
                d0[id[i]]++;
                d0[id[k]]++;
            }
        }
        for (int i = 1; i <= n; i++)
            d[i] += d0[i];

        if (!updated) break;
    }

    fprintf(stderr, "new:  %d\n", new_count);
    fprintf(stderr, "step: %d\n", step_count);
    for (int i = 1; i <= n; i++)
        puts(G[i] + 1);
    return 0;
}