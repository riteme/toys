#include <cstdio>

using i64 = long long;

i64 cnt;
constexpr i64 p[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97};
int ds[100];

void dfs(int i, i64 n, int d) {
    if (n < 1)
        return;

    cnt++;

    if (i > 0) {
        for (int j = 0; j < i; j++) {
            printf("%lld^%d%c", p[j], ds[j], j + 1 == i ? '\n' : '*');
        }
    } else
        puts("1");

    i64 a = p[i];
    for (int k = 1; k <= d; k++) {
        n /= a;
        ds[i] = k;
        dfs(i + 1, n, k);
    }
}

int main() {
    i64 n;
    while (scanf("%lld", &n) != EOF) {
        cnt = 0;
        dfs(0, n, 100);
        printf("count=%lld\n", cnt);
    }
    return 0;
}