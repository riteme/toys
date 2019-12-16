#include <cstdio>

#define NMAX 100000

static int n;
static char s[NMAX + 10];

int main() {
    scanf("%d%s", &n, s + 1);
    s[n + 1] = '$';
    int cnt[2] = {0, 1}, t = 1;
    for (int i = n; i >= 1; i--) {
        if (s[i] != s[i + 1])
            t = s[i] < s[i + 1];
        cnt[t]++;
    }
    // cnt[0] for L-type, cnt[1] for S-type
    printf("%d %d\n", cnt[1], cnt[0]);
    return 0;
}