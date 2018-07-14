// #define NDEBUG

#define NAME "polynomial"

#include <cstdio>
#include <cstring>
#include <climits>

#include <vector>
#include <unordered_map>
#include <algorithm>

using namespace std;

#define NMAX 100000
#define QMAX 100000
#define KMAX 101  // After prefix operation, rank + 1
#define NKMAX 256
#define MOD 998244353
#define G 3

typedef long long i64;

inline int nxtp(int x) {
    int k = 1;
    while (k < x) k <<= 1;
    return k;
}

inline i64 qpow(i64 a, int b) {
    if (b < 0)
        b += MOD - 1;
    i64 r = 1;
    for (; b; b >>= 1, a = a * a % MOD)
        if (b & 1)
            r = r * a % MOD;
    return r;
}

inline i64 inv(i64 x) {
    return qpow(x, MOD - 2);
}

void dft(i64 *X, int n, bool rev = false) {
    static i64 tmp[NKMAX];
    memcpy(tmp, X, sizeof(i64) * n);
    int k = 0, x;
    for (int i = 0; i < n; i++) {
        X[i] = tmp[k];
        x = n >> 1;
        while (k & x) {
            k ^= x;
            x >>= 1;
        }
        k |= x;
    }

    for (int s = 2; s <= n; s <<= 1) {
        int l = s >> 1;
        i64 wn = qpow(G, (rev ? -1 : 1) * (MOD - 1) / s);
        for (int i = 0; i < n; i += s) {
            i64 w = 1;
            for (int j = i; j < i + l; j++) {
                i64 t = X[j + l];
                X[j + l] = (X[j] - w * t) % MOD;
                X[j] = (X[j] + w * t) % MOD;
                w = w * wn % MOD;
            }
        }
    }

    if (rev) {
        i64 d = inv(n);
        for (int i = 0; i < n; i++) {
            X[i] = X[i] * d % MOD;
        }
    }
}

template <typename T>
struct FenwickArray {
    FenwickArray(int _n) : n(_n) {}

    int n;
    T a[QMAX + 10];

    void modify(int x, const T &v, bool add = true) {
        if (add)
            for (; x <= n; x += x & (-x))
                a[x] += v;
        else
            for (; x <= n; x += x & (-x))
                a[x] -= v;
    }

    void query(int x, T &dest) {
        for (; x; x -= x & (-x))
            dest += a[x];
    }
};

// the first 100 Bernoulli numbers initially.
static i64 D[NKMAX] = {1, 499122177, 166374059, 0, 565671800, 0, 308980395, 0, 565671800, 0, 892369952, 0, 247549973, 0, 166374060, 0, 624392049, 0, 203902097, 0, 293422811, 0, 947614962, 0, 247463393, 0, 167799576, 0, 56462502, 0, 509507086, 0, 481741584, 0, 535945330, 0, 128883146, 0, 332059229, 0, 289914080, 0, 155408555, 0, 888359911, 0, 904833245, 0, 368749953, 0, 3453945, 0, 694992202, 0, 773373544, 0, 495058659, 0, 185278999, 0, 10126758, 0, 319156225, 0, 549693494, 0, 434007005, 0, 829021404, 0, 790748811, 0, 405173336, 0, 205397248, 0, 738248514, 0, 783042817, 0, 119470640, 0, 104888550, 0, 555592414, 0, 299947194, 0, 770442747, 0, 33109029, 0, 835578572, 0, 25212850, 0, 919529569, 0, 377616162, 0, 409187229, 0};
static i64 fac[KMAX + 10], facinv[KMAX + 10];

struct Polynomial {
    static i64 B[NKMAX], C[NKMAX];

    Polynomial() {
        memset(a, 0, sizeof(a));
    }

    int tm;
    i64 a[KMAX];

    i64 &operator[](const int x) {
        return a[x];
    }

    i64 operator[](const int x) const {
        return a[x];
    }

    Polynomial &operator+=(const Polynomial &b) {
        for (int i = 0; i < KMAX; i++)
            a[i] += b[i];
        return *this;
    }

    Polynomial &operator-=(const Polynomial &b) {
        for (int i = 0; i < KMAX; i++)
            a[i] -= b[i];
        return *this;
    }

    void shift(int c) {
        memset(B, 0, sizeof(B));
        memset(C, 0, sizeof(C));

        i64 cp = 1;
        for (int i = 0; i < KMAX; i++, cp = cp * c % MOD) {
            B[i] = fac[KMAX - i - 1] * a[KMAX - i - 1] % MOD;
            C[i] = cp * facinv[i] % MOD;
        }

        dft(B, NKMAX);
        dft(C, NKMAX);
        for (int i = 0; i < NKMAX; i++) {
            B[i] = B[i] * C[i] % MOD;
        }
        dft(B, NKMAX, true);

        for (int i = 0; i < KMAX; i++) {
            a[i] = B[KMAX - i - 1] * facinv[i] % MOD;
        }
    }

    void prefix() {
        memset(B, 0, sizeof(B));
        for (int i = 0; i < KMAX; i++) {
            B[i] = fac[KMAX - i - 1] * a[KMAX - i - 1] % MOD;
        }

        dft(B, NKMAX);
        for (int i = 0; i < NKMAX; i++) {
            B[i] = B[i] * D[i] % MOD;
        }
        dft(B, NKMAX, true);

        a[0] = 0;
        for (int i = 0; i < KMAX; i++) {
            a[i + 1] = B[KMAX - i - 1] * facinv[i + 1] % MOD;
        }
    }

    i64 eval(i64 x) {
        i64 r = 0;

        for (int i = KMAX - 1; i >= 0; i--) {
            r = (r * x + a[i]) % MOD;
        }

        return r;
    }

    void print(int n) {
        for (int i = 0; i < n; i++) {
            printf("%lld", (a[i] + MOD) % MOD);
            if (i > 0)
                putchar('x');
            if (i > 1)
                printf("^%d", i);
            if (i + 1 != n)
                printf(" + ");
        }
        putchar('\n');
    }
};

i64 Polynomial::B[NKMAX], Polynomial::C[NKMAX];

struct Query {
    int tm;
    i64 ans;
};

enum EventType {
    ADD, QUERYL, QUERYR, DEL
};

struct Event {
    Event(int _pos, EventType _type, void *_data)
        : pos(_pos), type(_type), data(_data) {}

    bool operator<(const Event &b) const {
        return pos < b.pos || (pos == b.pos && type < b.type);
    }

    int pos;
    EventType type;
    void *data;
};

static int n, q, kmax, t, p;
static Polynomial poly[QMAX + 10];
static Query qry[QMAX + 10];
static vector<Event> events;

template <typename T>
inline void hook(int pos, EventType type, T *data) {
    events.push_back(Event(pos, type, reinterpret_cast<void *>(data)));
}

inline char getch() {
    char c;
    do {
        c = getchar();
    } while (!isalpha(c));
    return c;
}

void initialize() {
    fac[0] = 1;
    for (int i = 1; i <= KMAX; i++) {
        fac[i] = fac[i - 1] * i % MOD;
    }

    facinv[KMAX] = inv(fac[KMAX]);
    for (int i = KMAX - 1; i >= 0; i--) {
        facinv[i] = facinv[i + 1] * (i + 1) % MOD;
    }

    for (int i = 0; i < KMAX; i++) {
        D[i] = D[i] * facinv[i] % MOD;
    }
    dft(D, NKMAX);

    scanf("%d%d%d", &n, &q, &kmax);

    events.reserve(q << 1);
    for (int i = 1; i <= q; i++) {
        char op = getch();
        int l, r;
        scanf("%d%d", &l, &r);

        switch (op) {
            case 'M': {
                t++;
                int k;
                scanf("%d", &k);

                poly[t].tm = t;
                for (int i = 0; i < k; i++) {
                    int x, y;
                    scanf("%d%d", &x, &y);
                    poly[t][y] = x;
                }

                // TODO: Why shifting first is wrong?
                poly[t].prefix();
                poly[t].shift(-l + 1);

                hook(l, ADD, poly + t);
                hook(r, DEL, poly + t);
            } break;

            case 'Q': {
                p++;
                qry[p].tm = t;

                hook(l - 1, QUERYL, qry + p);
                hook(r, QUERYR, qry + p);
            } break;
        }
    }
}

#define CAST(T) auto p = reinterpret_cast<T *>(iter->data)

static FenwickArray<Polynomial> *ds;

inline i64 query(int tm, int x) {
    static Polynomial tmp;
    memset(&tmp, 0, sizeof(tmp));
    ds->query(tm, tmp);
    return tmp.eval(x);
}

int main() {
    // freopen("data.in", "r", stdin);
#ifdef NDEBUG
    freopen(NAME ".in", "r", stdin);
    freopen(NAME ".out", "w", stdout);
#endif
    initialize();

    ds = new FenwickArray<Polynomial>(t);
    // ds->a[1].print(0);
    sort(events.begin(), events.end());

    for (auto iter = events.begin(); iter != events.end();) {
        int pos = iter->pos;
        for (; iter != events.end() && iter->pos == pos; iter++) {
            switch (iter->type) {
                case ADD: {
                    CAST(Polynomial);
                    ds->modify(p->tm, *p);
                } break;

                case QUERYL: {
                    CAST(Query);
                    p->ans = query(p->tm, pos);
                } break;

                case QUERYR: {
                    CAST(Query);
                    p->ans = (query(p->tm, pos) - p->ans) % MOD;
                } break;

                case DEL: {
                    CAST(Polynomial);
                    p->a[0] -= p->eval(pos);
                    ds->modify(p->tm, *p, false);
                } break;
            }
        }
    }

    for (int i = 1; i <= p; i++) {
        printf("%lld\n", (qry[i].ans + MOD) % MOD);
    }

    return 0;
}
