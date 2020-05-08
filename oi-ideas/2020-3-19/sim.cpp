#include <cmath>
#include <cstdio>

#include <random>

using namespace std;

typedef long double d8;

d8 randf() {
    static random_device rd;
    static mt19937_64 gen(rd());
    static uniform_real_distribution<d8> dis(0.0L, 2.0L * M_PI);
    return dis(gen);
}

d8 det(d8 x1, d8 y1, d8 x2, d8 y2) {
    return x1 * y2 - x2 * y1;
}

int main() {
    d8 r1, r2, r3;
    int T;
    scanf("%Lf%Lf%Lf%d", &r1, &r2, &r3, &T);

    d8 sum = 0;
    for (int i = 0; i < T; i++) {
        d8 t1 = randf();
        d8 t2 = randf() / 2;
        d8 t3 = randf();
        d8 x1 = r1 * cosl(t1), y1 = r1 * sinl(t1);
        d8 x2 = r2 * cosl(t2), y2 = r2 * sinl(t2);
        d8 x3 = r3 * cosl(t3), y3 = r3 * sinl(t3);
        // d8 x3 = r3, y3 = 0;

        sum += 0.5L * abs(det(x1, y1, x2, y2) + det(x2, y2, x3, y3) + det(x3, y3, x1, y1));
        // d8 area = 0.5L * abs(det(x2, y2, x3, y3));
        // sum += area;
        // sum += det(x1 - x3, y1 - y3, x2 - x3, y2 - y3) < 0 ?
            // area : -area;
    }

    d8 aver = sum / T;
    printf("%.12Lf\n", aver);

    return 0;
}
