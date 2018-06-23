#include <cstdio>
#include <cstring>

#include <algorithm>

using namespace std;

typedef long long i64;

i64 pow(i64 x, i64 p, i64 m) {
    i64 ret = 1;
    for (i64 i = 0; i < p; i++)
        ret = ret * x % m;
    return ret;
}

int main() {
    // x^p mod m = y
    i64 p, m, y;
    scanf("%lld%lld%lld", &p, &m, &y);
    for (i64 x = 0; x < m; x++) {
        if (pow(x, p, m) == y)
            printf("%lld ", x);
    }
    puts("");
    return 0;
}
