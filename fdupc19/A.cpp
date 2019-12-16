#include <cstdio>

int main() {
    int a, b;
    scanf("%d%d", &a, &b);
    int c = a + b;
    if ((a < 0 && b < 0 && c >= 0) ||
        (a > 0 && b > 0 && c < 0))
        puts("YES");
    else
        puts("NO");
    return 0;
}