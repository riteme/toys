/**
 * Lost my precision!
 */

#include <cmath>
#include <cstring>

#include <random>
#include <iomanip>
#include <iostream>
#include <algorithm>

using namespace std;

#define NMAX 500
#define OUTPUT_EPS 1e-10
#define NEWTON_MAX 100
#define NEWTON_EPS 1e-14
#define NEWTON_RANDFACTOR 1e4

typedef long double Rational;

inline Rational randfloat() {
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_real_distribution<Rational> dist(-1, 1);
    return dist(gen);
}

inline bool nearzero(const Rational &x) {
    return -NEWTON_EPS < x && x < NEWTON_EPS;
}

inline void get_derivative(int n, const Rational *f, Rational *g) {
    for (int i = n - 1; i >= 0; i--) {
        g[i] = f[i + 1] * (i + 1);
    }
}

inline Rational evalf(int n, const Rational *f, const Rational &x) {
    Rational r = f[n];

    for (int i = n - 1; i >= 0; i--) {
        r = r * x + f[i];
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
    r = pow(fabs(f[0] / f[n]), 1.0 / n) * randfloat();

    for (int t = NEWTON_MAX; t; t--) {
        Rational a = evalf(n, f, r), b = evalf(n - 1, g, r);

        if (nearzero(a))
            break;
        while (nearzero(b)) {
            r += randfloat() / NEWTON_RANDFACTOR;
            a = evalf(n, f, r);
            b = evalf(n - 1, g, r);
        }

        r -= a / b;
    }

    return nearzero(evalf(n, f, r));
}

void deflation(const Rational &r) {
    for (int i = n - 1; i > 0; i--) {
        f[i] += r * f[i + 1];
    }

    memmove(f, f + 1, sizeof(Rational) * n);
    n--;
}

int main() {
    initialize();

    for (Rational r; n; ) {
        get_derivative(n, f, g);
        if (!find_root(r))
            break;
        deflation(r);
        s[cnt++] = r;
    }

    sort(s, s + cnt);
    cnt = unique(s, s + cnt,
        [](const Rational &a, const Rational &b) {
            return fabs(a - b) < OUTPUT_EPS;
        }) - s;

    cout << setprecision(9) 
         << "Foound " << cnt << " rational roots." << endl;
    for (int i = 0; i < cnt; i++) {
        cout << s[i] << " ";
    }
    cout << endl;

    return 0;
}
