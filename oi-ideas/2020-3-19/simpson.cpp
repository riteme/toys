#include <cmath>
#include <cstdio>

#include <algorithm>

using namespace std;

typedef double d;

#define EPS 1e-5

d f(d x);

// from WoodCube
d simpson(d fl, d fr, d fm, d l, d r) {
    return (fl + fr + 4 * fm) * (r - l) / 6;
}

d rsimpson(d slr, d fl, d fr, d fm, d l, d r) {
    d m = (l + r) / 2, fml = f((l + m) / 2), fmr = f((m + r) / 2);
    d slm = simpson(fl, fm, fml, l, m);
    d smr = simpson(fm, fr, fmr, m, r);
    if (abs(slr - smr - slm) < EPS * slr)
        return slm + smr;
    return rsimpson(slm, fl, fm, fml, l, m) + rsimpson(smr, fm, fr, fmr, m, r);
}

double r1, r2, r3;

double f(double t) {
    double l = sqrt(r2 * r2 + r3 * r3 - 2 * r2 * r3 * cos(t));
    double S = r2 * r3 * sin(t) / 2;
    double h = 2 * S / l;
    if (h < r1)
        return (1 - 2 / M_PI * acos(h / r1)) * S;
    return S;
}

double eval() {
    double fm = f(M_PI / 2);
    double ret = simpson(0, 0, fm, 0, M_PI);
    ret = rsimpson(ret, 0, 0, fm, 0, M_PI) / M_PI;
    return ret;
}

int main() {
    int T;
    scanf("%d", &T);

    while (T--) {
        double r[3];
        scanf("%lf%lf%lf", r, r + 1, r + 2);
        sort(r, r + 3);

        double ans = 0;
        r1 = r[0]; r2 = r[1]; r3 = r[2];
        ans += eval();
        r1 = r[1]; r2 = r[0]; r3 = r[2];
        ans += eval();
        r1 = r[2]; r2 = r[0]; r3 = r[1];
        ans += eval();

        printf("%.12lf\n", ans);
    }
    return 0;
}