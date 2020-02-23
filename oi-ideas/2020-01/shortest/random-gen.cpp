#include <ctime>
#include <cmath>
#include <cassert>
#include <cstdlib>

#include <set>
#include <vector>
#include <iostream>

using namespace std;

#define NMAX 50000

typedef long long i64;
typedef long double d8;
typedef pair<int, int> Edge;

d8 randf() {
    return (d8) rand() / RAND_MAX;
}

Edge S(i64 n) {
    i64 k = 2, r = NMAX;
    while (k < r) {
        i64 m = (k + r) / 2;
        i64 lo = (m - 1) * (m - 2) / 2;
        i64 hi = m * (m - 1) / 2;
        if (lo < n && n <= hi)
            k = r = m;
        else if (n <= lo)
            r = m - 1;
        else
            k = m + 1;
    }

    int u = n - (k - 1) * (k - 2) / 2;
    int v = k;
    return make_pair(u, v);
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    ios::sync_with_stdio(false);

    if (argc < 5) {
        cerr << argv[0] << ": [n] [m] [q] [MAXC]\n";
        return -1;
    }

    i64 n, m, q, C;
    n = atoi(argv[1]);
    m = atoi(argv[2]);
    q = atoi(argv[3]);
    C = atoi(argv[4]);

    i64 M = n * (n - 1) / 2;
    assert(m <= M);

    i64 i;
    vector<Edge> E;
    for (i = 1; i <= m; i++)
        E.push_back(S(i));

    d8 W = exp(log(randf()) / m);
    while (true) {
        i += i64(log(randf()) / log(1 - W)) + 1;
        if (i <= M) {
            E[rand() % m] = S(i);
            W = W * exp(log(randf()) / m);
        } else break;
    }

    cout << n << ' ' << m << '\n';
    for (int i = 0; i < m; i++) {
        int u = E[i].first, v = E[i].second;
        if (rand() & 1)
            swap(u, v);
        cout << u << ' ' << v << ' ' << (rand() % C + 1) << '\n';
    }

    cout << q << '\n';
    for (int i = 0; i < q; i++) {
        int u = rand() % n + 1;
        int v = rand() % n + 1;
        cout << u << ' ' << v << '\n';
    }

    return 0;
}