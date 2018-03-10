#include <cstdio>
#include <cstring>

#define NMAX 10000000

static int f[NMAX + 1];

int eval(int n) {
    int ans = 0;
    for (int i = 1; i <= n; i++) {
        f[i] = f[n % i] + 1;

        if (f[i] > ans)
            ans = f[i];
    }

    return ans;
}

int main() {
    int s, t, j;
    scanf("%d%d%d", &s, &t, &j);

    for (int i = s; i <= t; i += j) {
        printf("{%d,%d},", i, eval(i));
    }

    return 0;
}
