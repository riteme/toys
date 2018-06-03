// #define NDEBUG

#include <cassert>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#define NMAX 21
#define SMAX (1 << (NMAX))
#define SQRTMAX 100

typedef double Float;
#define FLOAT_LITERAL "%f"

static int n, w, h, X[NMAX], Y[NMAX];
static int cover[NMAX][NMAX];
static Float SQRT[SQRTMAX + 1];

// Determine whether Pk is on segment PiPj.
inline bool on_segment(int i, int j, int k) {
    if (i == j || j == k || i == k)
        return false;

    int dx1 = X[i] - X[k], dy1 = Y[i] - Y[k];
    int dx2 = X[j] - X[k], dy2 = Y[j] - Y[k];
    return dx1 * dy2 == dx2 * dy1 && dx1 * dx2 < 0;
}

void initialize() {
    for (int x = 1; x <= h; x++) for (int y = 1; y <= w; y++, n++) {
        X[n] = x;
        Y[n] = y;
    }

    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) {
        int s = 0;
        for (int k = 0; k < n; k++) {
            if (on_segment(i, j, k))
                s |= 1 << k;
        }
        cover[i][j] = s;
    }

    for (int i = 1; i <= SQRTMAX; i++) {
        SQRT[i] = sqrt(i);
    }
}

inline Float dist(int i, int j) {
    int dx = X[i] - X[j], dy = Y[i] - Y[j];
    return SQRT[dx * dx + dy * dy];
}

static Float f[SMAX][NMAX];
static int g[SMAX][NMAX];

void show(int S, int x) {
    if (x > n)
        return;

    assert(S & (1 << x));
    show(S ^ (1 << x), g[S][x]);
    printf("%d ", x + 1);
    if (S + 1 == (1 << n))
        putchar('\n');
}

int main(int argc, char *argv[]) {
    assert(argc > 2);
    w = atoi(argv[1]);
    h = atoi(argv[2]);
    initialize();

    memset(g, 0x3f, sizeof(g));
    for (int S = 0; S < (1 << n); S++) {
        for (int i = 0; i < n; i++) {
            if (!((S >> i) & 1))
                continue;

            Float &ans = f[S][i];
            int R = S ^ (1 << i);
            if (!R) {
                ans = 0;
                break;
            }

            for (int j = 0; j < n; j++) {
                if (!((R >> j) & 1) || ((~S) & cover[i][j]))
                    continue;

                Float ndist = f[R][j] + dist(i, j);
                if (ndist > ans) {
                    ans = ndist;
                    g[S][i] = j;
                }
            }
        }
    }

    Float ans = 0;
    int A = (1 << n) - 1, x;
    for (int i = 0; i < n; i++) {
        if (f[A][i] > ans) {
            ans = f[A][i];
            x = i;
        }
    }

    show(A, x);
    printf(FLOAT_LITERAL "\n", ans);

    return 0;
}
