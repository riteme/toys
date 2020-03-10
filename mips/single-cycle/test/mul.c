#include <stdio.h>

int mul(int x, int y) {
    int r = 0;
    for ( ; x; y <<= 1, x >>= 1)
        if (x & 1)
            r += y;
    return r;
}

int main() {
    int x, y;
    scanf("%d%d", &x, &y);
    printf("%d", mul(x, y));

    return 0;
}