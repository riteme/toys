#include <cstdio>
#include <cstring>

#include <algorithm>

using namespace std;

typedef long long i64;

int main() {
    i64 D, tm, lm, ts, ls, te, le;
    scanf("%lld%lld%lld%lld%lld%lld%lld",
          &D, &tm, &lm, &ts, &ls, &te, &le);
    i64 c2, c3, c5;
    for (c2 = 0; D % 2 == 0; D /= 2) c2++;
    for (c3 = 0; D % 3 == 0; D /= 3) c3++;
    for (c5 = 0; D % 5 == 0; D /= 5) c5++;
    i64 mi = tm * lm;
    if (D == 1) {
        i64 t = 0, l = 0;
        if (c5) {
            t += c5 * te;
            l += c5 * le;
        }
        if (c3) {
            t += ((c3 + 1) / 2) * te;
            l += ((c3 + 1) / 2) * le;
        }
        if (c2) {
            i64 t0 = t + ts, l0 = l + ls;
            mi = min(mi, t0 * l0);
            i64 c = c2 / 3 + (c2 % 3 > 0);
            t += c * te;
            l += c * le;
            mi = min(mi, t * l);
        } else
            mi = min(mi, t * l);
    }
    i64 p = tm * lm;
    i64 r = __gcd(p, mi);
    printf("%lld/%lld\n", p / r, mi / r);
    return 0;
}