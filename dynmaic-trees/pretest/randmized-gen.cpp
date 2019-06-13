#include <cstdlib>

#include <random>
#include <algorithm>

using namespace std;

#define NMAX 1048576

int main(int argc, char *argv[]) {
    if (argc < 4) return fprintf(stderr, "Usage: %s [N] [M] [K]\n", argv[0]), -1;
    int n = atoi(argv[1]), m = atoi(argv[2]), K = atoi(argv[3]);

    random_device rd;
    srand(rd());
    printf("%d %d\n", n, m);
    for (int c = 0; c < m; c++) {
        if (c % 10000 == 0) srand(rd());
        int u = rand() % n + 1;
        int v;
        do {
            v = rand() % n + 1;
        } while (u == v);
        printf("%d %d %d\n", u, v, rand() % K + 1);
    }
    return 0;
}