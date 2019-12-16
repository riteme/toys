#include <cassert>
#include <cstdio>

#include <algorithm>

using namespace std;

#define NMAX 2000

struct Point {
    int x, y;

    Point operator-(const Point &z) const {
        return {x - z.x, y - z.y};
    }

    Point &operator*=(int k) {
        x *= k;
        y *= k;
        return *this;
    }
};

int cross(const Point &u, const Point &v) {
    return u.x * v.y - u.y * v.x;
}

static int width, height, num;
static bool M[NMAX + 10][NMAX + 10];

void fill(const Point &a, const Point &b, const Point &c) {
    assert(cross(b - a, c - a) != 0);
    for (int x = 1; x <= width; x++)
    for (int y = 1; y <= height; y++) {
        Point p = {x, y};
        int area = abs(cross(b - a, c - a));
        int sum = abs(cross(p - a, p - b)) + abs(cross(p - b, p - c)) + abs(cross(p - c, p - a));
        if (area == sum)
            M[y][x] = true;
    }
}

int main() {
    scanf("%d%d%d", &width, &height, &num);
    while (num--) {
        int x1, y1, x2, y2, x3, y3;
        scanf("%d%d%d%d%d%d", &x1, &y1, &x2, &y2, &x3, &y3);
        fill({x1, y1}, {x2, y2}, {x3, y3});
    }

    for (int i = 1; i <= height; i++) {
        for (int j = 1; j <= width; j++)
            putchar(M[i][j] ? '#' : '.');
        puts("");
    }

    return 0;
}