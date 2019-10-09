//fail
#include <cstdio>
#include <cstring>

#include <algorithm>

using namespace std;

#define NMAX 100000
#define Q 1
#define W 2
#define E 3

static int mp[256][100];
static int len[256][6][256], dp[256][6][256];
constexpr int P[] = {3, 3 << 2, 3 << 4, (1 << 6) - 1};

inline int cat(int a, int b, int c) {
    return a | (b << 2) | (c << 4);
}

inline int cut(int s, int l, int r) {
    int mask = (1 << (2 * (r + 1))) - 1;
    s &= mask;
    s = ((s << (2 * l)) & mask) >> (2 * l);
    return s;
}

inline void initialize() {
    mp['Y'][0] = cat(Q, Q, Q);
    mp['V'][0] = cat(Q, Q, W);
    mp['G'][0] = cat(Q, Q, E);
    mp['C'][0] = cat(W, W, W);
    mp['X'][0] = cat(Q, W, W);
    mp['Z'][0] = cat(W, W, E);
    mp['T'][0] = cat(E, E, E);
    mp['F'][0] = cat(Q, E, E);
    mp['D'][0] = cat(W, E, E);
    mp['B'][0] = cat(Q, W, E);
    for (int s = 0; s < 256; s++) if (mp[s][0]) {
        int a = mp[s][0] & P[0];
        int b = (mp[s][0] & P[1]) >> 2;
        int c = (mp[s][0] & P[2]) >> 4;
        mp[s][0] = cat(a, b, c);
        mp[s][1] = cat(b, a, c);
        mp[s][2] = cat(b, c, a);
        mp[s][3] = cat(c, b, a);
        mp[s][4] = cat(c, a, b);
        mp[s][5] = cat(a, c, b);
    }

    for (int s = 0; s < 256; s++) for (int i = 0; i < 6; i++)
    for (int t = 0; t < 256; t++) for (int j = 0; j < 6; j++) {
        if (mp[s][i] == mp[t][j] && len[s][i][t] < 3) {
            len[s][i][t] = 3;
            dp[s][i][t] = j;
        } else if (cut(mp[s][i], 1, 2) == cut(mp[t][j], 0, 1) && len[s][i][t] < 2) {
            len[s][i][t] = 2;
            dp[s][i][t] = j;
        } else if (cut(mp[s][i], 2, 2) == cut(mp[t][j], 0, 0) && len[s][i][t] < 1) {
            len[s][i][t] = 1;
            dp[s][i][t] = j;
        }
    }
}

static char str[NMAX + 10];

int main() {
    initialize();
    scanf("%s", str + 1);
    int n = strlen(str + 1);
    int ans = 4 * n;
    for (int i0 = 0; i0 < 6; i0++) {
        int i = i0, sum = 4 * n;
        for (int k = 1; k < n; k++) {
            sum -= len[str[k]][i][str[k + 1]];
            i = dp[str[k]][i][str[k + 1]];
        }
        ans = min(ans, sum);
    }
    printf("%d\n", ans);
    return 0;
}
