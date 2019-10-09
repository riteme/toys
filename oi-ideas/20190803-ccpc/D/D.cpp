#include <cstdio>
static char ans[] = "1101100101000001000100001000000100000001000000000100000000000001000000000000000100000000000000000001";
int main() {
    int n, T;
    scanf("%d", &T);
    while (T--) {
        scanf("%d", &n);
        puts(ans[n - 1] == '0' ? "Yes" : "No");
    }
    return 0;
}
