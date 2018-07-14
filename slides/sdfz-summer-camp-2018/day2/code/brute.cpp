#include <cstdio>
#include <cstring>

#include <algorithm>

using namespace std;

#define NMAX 30

static int n, m;
static int mat[NMAX];

void initialize() {
    char buf[NMAX + 1];
    scanf("%d%d", &n, &m);
    for (int i = 0; i < n; i++) {
        scanf("%s", buf);
        for (int j = 0; j < m; j++)
            if (buf[j] == '1')
                mat[i] |= 1 << j;
    }
}

bool test(int S) {
    int T = 0;
    for (int i = 0; i < n; i++) {
        if (!((S >> i) & 1)) continue;
        if (mat[i] & T) return false;
        T |= mat[i];
    }
    return T == (1 << m) - 1;
}

void show(int S) {
    static int cnt = 0;
    printf("%d:\n", ++cnt);
    for (int i = 0; i < n; i++) {
        char c = (S >> i) & 1 ? 'O' : '1';
        for (int j = 0; j < m; j++)
            putchar((mat[i] >> j) & 1 ? c : '.');
        puts("");
    }
}

int main() {
    initialize();

    for (int S = 0; S < (1 << n); S++) {
        if (test(S)) show(S);
    }

    return 0;
}
