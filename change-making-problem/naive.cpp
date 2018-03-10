#include <cstdio>
#include <cstring>
#include <climits>

#include <algorithm>

using  namespace std;

#define NMAX 100
#define TESTMAX 1000000

static int n;
static int C[NMAX + 10], f[TESTMAX + 10], g[TESTMAX + 10];;

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%d", C + i);
    }
    sort(C + 1, C + n + 1, [](int x, int y) { return x > y; });

    int found = 0;
    for (int i = 1; i <= TESTMAX; i++) {
        f[i] = INT_MAX;
        for (int j = 1; j <= n; j++) {
            if (C[j] <= i)
                f[i] = min(f[i], f[i - C[j]] + 1);
        }
        for (int j = 1; j <= n; j++) {
            if (C[j] <= i) {
                g[i] = i / C[j] + g[i % C[j]];
                break;
            }
        }

        if (f[i] != g[i]) {
            found = i;
            break;
        }
    }

    if (!found)
        puts("PASSED!");
    else
        printf("%d\n", found);

    return 0;
}