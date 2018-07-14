#include <cstdio>
#include <cstring>

#include <algorithm>

using namespace std;

#define NMAX 100

static int n;
static int C[NMAX + 10], V[NMAX + 10], W[NMAX + 10];
static int witW[NMAX + 10], witG[NMAX + 10];

bool check(int *v, int w) {
    int cnt = 0;
    for (int i = 1; i <= n; i++) {
        if (v[i])
            cnt++;
        w -= v[i] * C[i];
    }
    return w == 0;
}

void print(const char *desc, int *v) {
    printf("%s: ", desc);
    int r = 0;
    for (int i = 1; i <= n; i++) {
        if (!v[i])
            continue;
        r += v[i];
        printf("%dx%d ", C[i], v[i]);
    }
    printf("(%d)\n", r);
}

int S(int *v) {
    int r = 0;
    for (int i = 1; i <= n; i++) {
        r += v[i];
    }

    return r;
}

void G(int x) {
    for (int i = 1; i <= n; i++) {
        V[i] = x / C[i];
        x %= C[i];
    }
}

void initialize() {
    scanf("%d", &n);

    for (int i = 1; i <= n; i++) {
        scanf("%d", C + i);
    }

    sort(C + 1, C + n + 1, [](int x, int y) { return x > y; });
}

int main() {
    initialize();

    int found = 0;
    for (int i = 1; i < n; i++) {
        G(C[i] - 1);
        memcpy(W, V, sizeof(W));

        int w = C[i], sz = 1;
        for (int j = i + 1; j <= n; j++) {
            w += (W[j] + 1) * C[j] - C[j - 1];
            sz += W[j];

            G(w);  // Slow here
            if (S(V) > sz && (!found || w < found)) {
                found = w;
                memcpy(witW + 1, W + 1, sizeof(int) * j);
                memset(witW + j + 1, 0, sizeof(int) * (n - j));
                memcpy(witG, V, sizeof(V));
                witW[j]++;
            }
        }
    }

    if (!found)
        puts("PASSED!");
    else {
        printf("%d\n", found);
        // print("   std", witW);
        // print("greedy", witG);

        if (!check(witW, found))
            return -1;
    }

    return 0;
}