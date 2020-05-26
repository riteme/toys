#include <cmath>
#include <cstdio>
#include <cstring>

#include <algorithm>

using namespace std;

using d8 = long double;

constexpr int NMAX = 100;
constexpr int TMAX = 150;
constexpr d8 EPS = 1e-12;
constexpr d8 INF = 1e300;

template <typename T>
bool eq(T x, T y) {
    return fabsl(x - y) < EPS;
}

struct vec {
    vec() : x(0), y(0) {}
    vec(d8 _x, d8 _y) : x(_x), y(_y) {}

    d8 x, y;

    vec ccw() const {
        return {-y, x};
    }

    vec norm() const {
        d8 l = len();
        return {x / l, y / l};
    }

    d8 len() const {
        return hypotl(x, y);
    }

    vec operator+(const vec &z) const {
        return {x + z.x, y + z.y};
    }

    vec operator-(const vec &z) const {
        return {x - z.x, y - z.y};
    }

    vec operator*(d8 k) const {
        return {x * k, y * k};
    }

    vec &operator/=(d8 k) {
        x /= k;
        y /= k;
        return *this;
    }
};

struct cir {
    vec C;
    d8 r;
};

// https://riteme.site/docmeld/riteme/oh-my-acm/fall20/#17
enum IntersectStatus {
  ISEPARATE = 1,
  IOUTERT,
  ICROSS,
  IINNERT,
  ICONTAIN
};
struct IntersectResult {
  vec u, v;  // counterclockwise w.r.t. circle a
  IntersectStatus stat;
};
IntersectResult intersect(const cir &a, const cir &b) {
  vec p = b.C - a.C;
  d8 d = p.len(), d0 = max(a.r, b.r) - min(a.r, b.r);
  IntersectStatus stat;
  if (a.r + b.r <= d - EPS) stat = ISEPARATE;
  else if (eq(a.r + b.r, d)) stat = IOUTERT;
  else if (d - EPS >= d0) stat = ICROSS;
  else if (eq(d, d0)) stat = IINNERT;
  else stat = ICONTAIN;
  d8
  R2 = a.r * a.r,
  t = (R2 - b.r * b.r) / d,
  L = (d + t) * 0.5L,
  H = sqrtl(max(0.0L, R2 - L * L));
  p /= d;
  vec v = vec(p.y, -p.x) * H;
  vec O = a.C + p * L;
  return {O + v, O - v, stat};
}

#define ACM_BEGIN
int n;
vec P[NMAX + 10], S[NMAX + 10];
cir C[NMAX + 10];
void generate(d8 a) {
    for (int i = 1; i <= n; i++) {
        vec u = P[i % n + 1] - P[i];
        d8 l = u.len();
        d8 r = l / 2 / sinl(a);
        d8 h = l / 2 / tanl(a);
        vec v = u.ccw().norm();
        C[i].C = P[i] + u * 0.5L + v * h;
        C[i].r = r;
}}
int check_x(d8 x) {
    int p; d8 maxl = -INF;
    for (int i = 1; i <= n; i++) {
        cir &c = C[i];
        if (x <= c.C.x - c.r - EPS)
            return +1;
        if (x >= c.C.x + c.r + EPS)
            return -1;
        d8 d = abs(c.C.x - x);
        d8 l = sqrtl(max(0.0L, c.r * c.r - d * d));
        S[i] = {c.C.y - l, c.C.y + l};
        if (S[i].x > maxl)
            maxl = S[i].x, p = i;
    }
    for (int i = 1; i <= n; i++) {
        if (S[i].x - EPS < maxl && maxl < S[i].y + EPS)
            continue;
        // NOTE: 需要判断这两个圆的交的相对位置，而不是圆心
        // intersect 来自板子
        auto t = intersect(C[i], C[p]);
        if (t.stat == IOUTERT || t.stat == ICROSS || t.stat == IINNERT)
            return t.u.x <= x ? -1 : +1;
        else
            return -1;
    } return 0;
}
bool test(d8 a) {
    generate(a);
    d8 l = INF, r = -INF;
    for (int i = 1; i <= n; i++) {
        l = min(l, C[i].C.x - C[i].r);
        r = max(r, C[i].C.x + C[i].r);
    }
    for (int _ = 0; _ < TMAX; _++) {
        d8 m = (l + r) * 0.5L;
        int v = check_x(m);
        v > 0 ? l = m : r = m;
        if (v == 0) return true;
    } return false;
}
#define ACM_END

int main() {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
        scanf("%Lf%Lf", &P[i].x, &P[i].y);
    }

    d8 l = EPS, r = M_PI - EPS;
    for (int _ = 0; _  < TMAX; _++) {
        d8 m = (l + r) * 0.5L;
        if (test(m))
            l = m;
        else
            r = m;
    }

    d8 ans = (l + r) * 0.5L;
    ans = ans / M_PI * 180;
    printf("%.16Lf\n", ans);
    return 0;
}