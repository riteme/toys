#include <cstdio>
#include <cstring>

#include <vector>
#include <algorithm>

using namespace std;

using i64 = long long;

constexpr int NMAX = 200000;
constexpr int KMAX = 20;
constexpr i64 MOD = 1000000007;

int n, q, K;

void add(i64 *a, int x) {
    static i64 b[KMAX];
    memcpy(b, a, sizeof(b));
    for (int i = 0; i < K; i++) {
        int j = i + x;
        if (j >= K)
            j -= K;
        b[j] += a[i];
        if (b[j] >= MOD)
            b[j] -= MOD;
    }
    memcpy(a, b, sizeof(b));
}

void meld(i64 *a, i64 *b, i64 *c) {
    static i64 d[KMAX];
    memset(d, 0, sizeof(d));
    for (int i = 0; i < K; i++)
    for (int j = 0; j < K; j++) {
        int k = i + j;
        if (k >= K)
            k -= K;
        d[k] = (d[k] + a[i] * b[j]) % MOD;
    };
    memcpy(c, d, sizeof(d));
}

i64 ans[NMAX + 10];
int a[NMAX + 10];
int lb[NMAX + 10], rb[NMAX + 10];
i64 b[NMAX + 10][KMAX];

struct Node {
    int l, r, m;
    vector<int> vs;
    Node *lch, *rch;

    Node(int L, int R)
        : l(L), r(R), m((L + R) / 2), lch(NULL), rch(NULL) {
        if (L < R) {
            lch = new Node(l, m);
            rch = new Node(m + 1, r);
        }
    }

    void put(int L, int R, int i) {
        if (l == r)
            vs.push_back(i);
        else if (L <= m && m < R)
            vs.push_back(i);
        else if (R <= m)
            lch->put(L, R, i);
        else
            rch->put(L, R, i);
    }

    void solve() {
        if (l == r) {
            for (int i : vs) {
                ans[i] = a[l] == 0 ? 2 : 1;;
            }
            return;
        }

        memset(b[m], 0, sizeof(b[0]));
        b[m][0] = 1;
        b[m][a[m]]++;
        for (int i = m - 1; i >= l; i--) {
            memcpy(b[i], b[i + 1], sizeof(b[0]));
            add(b[i], a[i]);
        }

        memset(b[m + 1], 0, sizeof(b[0]));
        b[m + 1][0] = 1;
        b[m + 1][a[m + 1]]++;
        for (int i = m + 2; i <= r; i++) {
            memcpy(b[i], b[i - 1], sizeof(b[0]));
            add(b[i], a[i]);
        }

        static i64 c[KMAX];
        for (int i : vs) {
            memcpy(c, b[lb[i]], sizeof(c));
            meld(c, b[rb[i]], c);
            ans[i] = c[0];
        }

        lch->solve();
        rch->solve();
    }
};

int main() {
    scanf("%d%d", &n, &K);
    for (int i = 1; i <= n; i++) {
        scanf("%d", a + i);
        a[i] %= K;
    }

    auto tr = new Node(1, n);
    scanf("%d", &q);
    for (int i = 1; i <= q; i++) {
        scanf("%d%d", lb + i, rb + i);
        tr->put(lb[i], rb[i], i);
    }

    tr->solve();

    for (int i = 1; i <= q; i++) {
        printf("%lld\n", ans[i]);
    }

    return 0;
}