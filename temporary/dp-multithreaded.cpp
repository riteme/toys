/**
 * Requirements:
 * 1. FXT library (https://www.jjj.de/fxt/fxtpage.html#fxt)
 * 2. G++ compiler with C++11 support
 */

// #define NDEBUG
#include <cassert>

#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <vector>
#include <thread>

#include <fxt/comb-all.h>
#include <fxt/bits-all.h>

#define cnt(x) __builtin_popcount(x)
#define ctz(x) __builtin_ctz(x)

#define NMAX 25
#define SMAX (1 << (NMAX))
#define SQRTMAX 100
#define WORDSIZE 5
#define NUM_THREAD 4

typedef float Float;
#define FLOAT_LITERAL "%f"

// 128bit string
template <unsigned L>
struct CompressedString {
    typedef unsigned long long u64;
    typedef __uint128_t u128;
    constexpr static u128 mask = (1 << L) - 1;

    CompressedString() : _data(0) {}

    void append(int x) {
        _data = (_data << L) | x;
    }

    void editlast(int x) {
        _data = (_data & (~mask)) | x;
    }

    void extract(int *X, int len) {
        while (len) {
            X[--len] = _data & mask;
            _data >>= L;
        }
    }

private:
    u128 _data;
};

struct Data {
    Data() : len(0) {
        sol.append(0);
    }

    Float len;
    CompressedString<WORDSIZE> sol;
};

struct State {
    State(int n) {
        _data = new Data[n];
    }

    ~State() {
        delete[] _data;
    }

    Data &operator[](const size_t i) {
        return _data[i];
    }

private:
    Data *_data;
};

static int n, w, h, X[NMAX], Y[NMAX];
static int cover[NMAX][NMAX];
static Float SQRT[SQRTMAX + 1];

// Determine whether Pk is on segment PiPj.
inline bool on_segment(int i, int j, int k) {
    if (i == j || j == k || i == k)
        return false;

    int dx1 = X[i] - X[k], dy1 = Y[i] - Y[k];
    int dx2 = X[j] - X[k], dy2 = Y[j] - Y[k];
    int p = dx1 ? dx1 * dx2 : dy1 * dy2;
    return dx1 * dy2 == dx2 * dy1 && p < 0;
}

inline Float dist(int i, int j) {
    int dx = X[i] - X[j], dy = Y[i] - Y[j];
    return SQRT[dx * dx + dy * dy];
}

static State *ref[SMAX];
static std::vector<int> task;

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

    for (int i = 0; i < n; i++) {
        State *p = ref[1 << i] = new State(1);
        (*p)[0].sol.append(i);
    }
}

void dp(int S, State &s) {
    for (int i = 0, R = S, x; R; i++, R ^= (1 << x)) {
        // i: real index, x: id
        x = ctz(R);
        s[i].sol.append(x);

        for (int j = 0, k = 0; j < n; j++) {
            // k: real index, j: id
            if ((S >> j) & 1) {
                k++;
                continue;
            }

            int T = S | (1 << j);
            if (!ref[T])
                ref[T] = new State(cnt(T));

            State &t = *ref[T];
            if (cover[x][j] & (~T))
                continue;

            Float nwdst = s[i].len + dist(x, j);
            if (nwdst > t[k].len) {
                t[k].len = nwdst;
                t[k].sol = s[i].sol;
            }
        }
    }

    delete ref[S];
}

void process(int l, int r) {
    auto beg = task.begin() + l, ed = task.begin() + r;
    for (; beg != ed; beg++) {
        dp(*beg, *ref[*beg]);
    }
}

int main(int argc, char *argv[]) {
    assert(argc > 2);
    w = atoi(argv[1]);
    h = atoi(argv[2]);
    initialize();

    int size = 1;
    for (int i = 1; i < n; i++) {
        // printf("Stage %d...\n", i);
        size = size * (n - i + 1) / i;
        task.clear();
        task.reserve(size);

        ksubset_gray gen(n, i, i);
        gen.first();
        auto ptr = gen.data();
        do {
            task.push_back(set2bitset(ptr, gen.num(), 1));
        } while (gen.next());

        int aver = task.size() / NUM_THREAD;
        std::thread *tr[NUM_THREAD];
        for (int j = 0; j < NUM_THREAD; j++) {
            tr[j] = new std::thread([j, aver]() {
                process(
                    j * aver,
                    j + 1 == NUM_THREAD ? task.size() : (j + 1) * aver
                );
            });
        }

        for (int j = 0; j < NUM_THREAD; j++) {
            tr[j]->join();
            delete tr[j];
        }
    }

    int A = (1 << n) - 1, x;
    State &s = *ref[A];
    Float ans = 0;
    for (int i = 0; i < n; i++) {
        if (s[i].len > ans) {
            ans = s[i].len;
            x = i;
        }
    }

    int tmp[n];
    s[x].sol.extract(tmp, n - 1);
    tmp[n - 1] = x;
    for (int i = 0; i < n; i++) {
        printf("%d ", tmp[i] + 1);
    }
    printf("\n" FLOAT_LITERAL "\n", ans);

    return 0;
}
