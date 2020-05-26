#include <cstdio>
#include <cassert>
#include <cstring>

#include <random>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

int query(const vector<int> &);

vector<bool> rax;

using vi = vector<int>;

void solve(vi li, int c) {
    if (c == li.size()) {
        for (int i : li) {
            rax[i] = 1;
        }
    } else if (c == 0) {
        for (int i : li) {
            rax[i] = 0;
        }
    } else {
        random_shuffle(li.begin(), li.end());
        int m = li.size() / 2;
        vi L, R;
        for (int i = 0; i < m; i++) {
            L.push_back(li[i]);
        }
        for (int i = m; i < li.size(); i++) {
            R.push_back(li[i]);
        }

        int cL = query(L);
        int cR = c - cL;
        solve(L, cL);
        solve(R, cR);
    }
}

void solve(const vi &li) {
    solve(li, query(li));
}

int n, cnt;
vector<bool> rdx;

int randbit() {
    static random_device rd;
    static mt19937_64 gen(rd());
    static uniform_int_distribution r(0, 1);
    return r(gen);
}

int query(const vector<int> &li) {
    cnt++;
    int ret = 0;
    for (int i : li)
        ret += rdx[i];
    return ret;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [n]\n", argv[0]);
        return -1;
    }

    n = atoi(argv[1]);
    rdx.resize(n);
    rax.resize(n);
    generate_n(rdx.begin(), n, randbit);
    vector li(n, 0);
    iota(li.begin(), li.end(), 0);
    solve(li);

    assert(rax == rdx);

    long double ratio = static_cast<long double>(cnt) / n;
    printf("cnt = %d\n", cnt);
    printf("ratio = %.8Lf\n", ratio);
    return 0;
}
