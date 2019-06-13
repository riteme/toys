#include <cstdio>
#include <cstring>

#include <random>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 5) return fprintf(stderr, "Usage: %s [c] [m] [K] [B]\n", argv[0]), -1;
    int BLOCK = atoi(argv[4]), c = atoi(argv[1]), n = BLOCK * c, m = atoi(argv[2]), K = atoi(argv[3]);

    random_device rd;
    srand(rd());
    printf("%d %d\n", n, m);
    for (int i = 0; i < m; i++) {
        if (i % 10000 == 0) srand(rd());
        int j = rand() % c;
        int u = rand() % BLOCK + j * BLOCK + 1, v;
        do v = rand() % BLOCK + j * BLOCK + 1; while (u == v);
        printf("%d %d %d\n", u, v, rand() % K + 1);
    }

    return 0;
}