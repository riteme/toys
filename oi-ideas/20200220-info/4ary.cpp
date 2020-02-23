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

int check(const vi &li, int *_c = NULL) {
    int c = _c ? *_c : query(li);

    if (c == 0)
        for (int i : li)
            rax[i] = 0;
    else if (c == li.size())
        for (int i : li)
            rax[i] = 1;
    else return c;
    return -c;
}

void solve(vi &li) {
    if (li.empty())
        return;

    vi aux;
    unordered_map<int, int> mp;
    for (int i = 0; 4*i < li.size(); i++) {
        if (4*i + 3 < li.size()) {
            int a = li[4*i], b = li[4*i + 1];
            int c = li[4*i + 2], d = li[4*i + 3];
            int c1;
            if ((c1 = check({a, b, c, d})) > 0) {
                int c2;
                if ((c2 = check({a, b})) > 0) {
                    aux.push_back(a);
                    mp[b] = a;
                }
                c1 -= abs(c2);
                if (check({c, d}, &c1) > 0) {
                    aux.push_back(c);
                    mp[d] = c;
                }
            }
        } else {
            for (int j = 4*i; j < li.size(); j++)
                rax[li[j]] = query({li[j]});
        }
    }

    solve(aux);

    for (auto &[a, b] : mp)
        rax[a] = rax[b] ^ 1;
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
    printf("cnt = %d\nratio = %.8Lf\n", cnt, ratio);
    return 0;
}