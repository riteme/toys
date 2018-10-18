#pragma GCC optimize("O9")

#define NDEBUG

#include <cstdio>
#include <cctype>

using namespace std;

#define BUFFERSIZE 65536
static size_t _pos = BUFFERSIZE;
static char _buf[BUFFERSIZE];

inline void getc(char &c) {
    if (_pos == BUFFERSIZE) {
        _pos = 0;
        fread(_buf, 1, BUFFERSIZE, stdin);
    }
    c = _buf[_pos++];
}

template <typename T>
inline void read(T &x) {
    x = 0;
    char c;
    do getc(c); while (!isdigit(c));
    do {
        x = x * 10 + (c - '0');
        getc(c);
    } while (isdigit(c));
}

typedef long long i64;

#ifndef NDEBUG
#define DEBUG(msg, ...) fprintf(stderr, msg "\n", __VA_ARGS__);
#else
#define DEBUG(...)
#endif

#define CMAX 500000
#define FACTORBUF 64
#define MOD 998244353LL

inline i64 trunc(i64 x) {
    return x >= MOD ? x - MOD : (x <= -MOD ? x + MOD : x);
}

inline void add(i64 &a, i64 b) {
    a += b;
    if (a >= MOD) a -= MOD;
    else if (a <= -MOD) a += MOD;
}

static int n, q, f, x, mu[CMAX + 10];
static int pc, pr[CMAX + 10];
static int md[CMAX + 10], pre[CMAX + 10], cnt[CMAX + 10];
static i64 F[CMAX + 10], G[CMAX + 10], pG[CMAX + 10], ans;
static int C[CMAX + 10], dr[FACTORBUF], dc[FACTORBUF], pos;

inline void factor(int m) {
    for (pos = 0; m > 1; m = pre[m]) {
        dc[pos] = cnt[m];
        dr[pos++] = md[m];
    }
}

void dfs(int d, int p, bool v) {
    if (!v) {
        DEBUG("divisor: %d", d);
        add(ans, 2 * f * C[d] * G[d] % MOD);
        C[d] += f;
    }

    if (p == pos) return;
    dfs(d, p + 1, true);
    for (int i = 1, t = dr[p]; i <= dc[p]; t *= dr[p], i++)
        dfs(d * t, p + 1, false);
}

int main() {
    mu[1] = F[1] = G[1] = 1;
    for (int i = 2; i <= CMAX; i++) {
        if (!cnt[i]) {
            pr[pc++] = i;
            mu[i] = -1;
            md[i] = i;
            pre[i] = 1;
            cnt[i] = 1;
        }

        for (int j = 0; i * pr[j] <= CMAX; j++) {
            int p = pr[j], t = i * p;
            md[t] = p;
            if (i % p) {
                mu[t] = -mu[i];
                pre[t] = i;
                cnt[t] = 1;
            } else {
                mu[t] = 0;
                pre[t] = pre[i];
                cnt[t] = cnt[i] + 1;
                break;
            }
        }

        F[i] = trunc(F[i - 1] + F[i - 2]);
        add(G[i], mu[i]);
    }

    // scanf("%d%d", &n, &q);
    read(n); read(q);
    while (n--) {
        // scanf("%d", &x);
        read(x);
        C[x]++;
    }

    for (int i = 1; i <= CMAX; i++) {
        add(pG[i], G[i]);
        for (int j = i + i, t = 2; j <= CMAX; j += i, t++) {
            add(G[j], mu[i] * F[t]);
            add(pG[j], G[i]);
            C[i] += C[j];
        }
        add(ans, G[i] * C[i] % MOD * C[i] % MOD);
    }

    DEBUG("ans = %lld", ans);
    while (q--) {
        // scanf("%d%d", &f, &x);
        read(f); read(x);
        f = f == 1 ? 1 : -1;
        factor(x);
        dfs(1, 0, false);
        add(ans, pG[x]);
        if (ans < 0) ans += MOD;
        printf("%lld\n", ans);
    }

    return 0;
}

