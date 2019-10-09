// 3 * n * 64 * 6
#include <cstdio>
#include <cstring>

#include <algorithm>

using namespace std;

#define NMAX 100000
#define INF 0x3f3f3f3f
#define Q 1
#define W 2
#define E 3

constexpr int P[] = {3, 3 << 2, 3 << 4, (1 << 6) - 1};
static int aha[256][100];

static int n;
static char str[NMAX + 10];
static int dp[3 * NMAX + 10][64];

inline int cat(int a, int b, int c) {
    return a | (b << 2) | (c << 4);
}

inline void initialize() {
    aha['Y'][0] = cat(Q, Q, Q);
    aha['V'][0] = cat(Q, Q, W);
    aha['G'][0] = cat(Q, Q, E);
    aha['C'][0] = cat(W, W, W);
    aha['X'][0] = cat(Q, W, W);
    aha['Z'][0] = cat(W, W, E);
    aha['T'][0] = cat(E, E, E);
    aha['F'][0] = cat(Q, E, E);
    aha['D'][0] = cat(W, E, E);
    aha['B'][0] = cat(Q, W, E);
    for (int s = 0; s < 256; s++) if (aha[s][0]) {
        int a = aha[s][0] & P[0];
        int b = (aha[s][0] & P[1]) >> 2;
        int c = (aha[s][0] & P[2]) >> 4;
        aha[s][0] = cat(a, b, c);
        aha[s][1] = cat(b, a, c);
        aha[s][2] = cat(b, c, a);
        aha[s][3] = cat(c, b, a);
        aha[s][4] = cat(c, a, b);
        aha[s][5] = cat(a, c, b);
    }
}

int main() {
    initialize();
    scanf("%s", str + 1);
    n = strlen(str + 1);
    int ans = INF;
    memset(dp, 0x3f, sizeof(dp));
    dp[0][0] = 0;
    for (int i = 0; i < 3 * n; i++)
    for (int s = 0; s <= P[3]; s++)
    if (dp[i][s] < INF) {
        int p = dp[i][s] + 1;
        //fprintf(stderr, "dp[%d][%.3o] = %d\n", i, s, dp[i][s]);
        for (int c = 1; c <= 3; c++) {
            int ns = ((s << 2) & P[3]) | c, j;
            if (dp[i + 1][ns] == INF) dp[i + 1][ns] = 0;
            for (j = p; j <= n; j++) {
                bool ok = false;
                for (int k = 0; !ok && k < 6; k++)
                    ok |= aha[str[j]][k] == ns;
                if (!ok) break;
            }
            dp[i + 1][ns] = max(dp[i + 1][ns], j - 1);
            if (dp[i + 1][ns] >= n) ans = min(ans, i + 1);
        }
    }
    printf("%d\n", ans + n);
    return 0;
}
