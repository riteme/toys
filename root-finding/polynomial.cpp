#include <cmath>

#include <random>
#include <iostream>
#include <algorithm>

using namespace std;

#define NMAX 500
#define NEWTON_MAX 50
#define EPS 1e-9
#define RANDFACTOR 1e4

typedef double Rational;

inline Rational rand() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_real_distribution<Rational> dist(-1, 1);
    return dist(gen);
}

inline bool nearzero(const Rational &x) {
    return -EPS < x && x < EPS;
}

inline void get_derivative(int n, const Rational *f, Rational *g) {
    for (int i = n - 1; i >= 0; i--) {
        g[i] = f[i + 1] * (i + 1);
    }
}

inline Rational evalf(int n, const Rational *f, const Rational &x) {
    Rational r = 0;
    
    for (int i = n; i >= 0; i--) {
        r = (r + f[i]) * x;
    }

    return r;
}

static int n, cnt;
static Rational f[NMAX], g[NMAX], s[NMAX];

void initialize() {
    ios::sync_with_stdio(false);

    cin >> n;
    for (int i = n; i >= 0; i--) {
        cin >> f[i];
    }
}

bool find_root(Rational &r) {
    r = pow(fabs(f[0] / f[n]), 1.0 / n) * rand();

    for (int t = NEWTON_MAX; t; t--) {
        Rational a = evalf(n, f, r), b = evalf(n - 1, g, r);

        if (nearzero(a))
            break;
        if (nearzero(b))
            r += rand() / RANDFACTOR;
    }

    return nearzero(evalf(n, f, r));
}

void deflation(const Rational &r) {
}

int main() {
    initialize();

    while (n) {
    }

    return 0;
}
