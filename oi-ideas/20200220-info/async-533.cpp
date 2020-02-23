#include <cstdio>
#include <cassert>
#include <cstring>

#include <queue>
#include <random>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

int query(const vector<int> &);

vector<bool> rax;

using vi = vector<int>;

struct Task;

void submit(int i, bool v);
void _require(int i, Task*, float);

bool attempt(const vi &li, int c) {
    if (c == 0)
        for (int i : li)
            submit(i, 0);
    else if (c == li.size())
        for (int i : li)
            submit(i, 1);
    else return false;
    return true;
}

struct Task {
    Task(int a, int b, int c, int d) {
        // printf("dispatch: %d %d %d %d\n", a, b, c, d);

        memset(this, 0, sizeof(Task));
        v[0] = a;
        v[1] = b;
        v[2] = c;
        v[3] = d;

        random_shuffle(v, v + 4);  // maybe useful
    }

    int t;
    int v[4];
    int _c;

    int c0, c1, c2;

    void require(int i, float p = 0.5f) {
        _require(i, this, p);
    }

    void require(const vi &li, float p = 0.5f) {
        for (int i : li)
            require(i, p);
    }

    void phase1() {
        c0 = query({v[0], v[1], v[2], v[3]});

        switch (c0) {
            case 0:
            case 4: {
                for (int i : v)
                    submit(i, c0);
            } break;

            case 1:
            case 3: {
                require(v[0], c0 / 4.0f);
            } break;

            case 2: {
                require({v[0], v[1]});
            } break;
        }
    }

    void phase2() {
        switch (c0) {
            case 1:
            case 3: {
                if (rax[v[0]] == (c0 == 1)) {
                    int x = rax[v[0]] ^ 1;
                    submit(v[1], x);
                    submit(v[2], x);
                    submit(v[3], x);
                } else require(v[1], (c0 - rax[v[0]]) / 3.0f);
            } break;

            case 2: {
                c2 = 2 - rax[v[0]] - rax[v[1]];
                if (!attempt({v[2], v[3]}, c2))
                    require(v[2]);
            } break;
        }
    }

    void phase3() {
        switch (c0) {
            case 1:
            case 3: {
                if (rax[v[1]] == (c0 == 1)) {
                    int x = rax[v[1]] ^ 1;
                    submit(v[2], x);
                    submit(v[3], x);
                } else require(v[2]);
            } break;

            case 2: {
                submit(v[3], rax[v[2]] ^ 1);
            } break;
        }
    }

    void phase4() {
        assert(c0 == 1 || c0 == 3);
        submit(v[3], rax[v[2]] ^ 1);
    }

    void execute() {
        t++;

        // printf("execute [%d %d %d %d]@%d\n", v[0], v[1], v[2], v[3], t);
        switch (t) {
            case 1: phase1(); break;
            case 2: phase2(); break;
            case 3: phase3(); break;
            case 4: phase4(); break;
            default: abort();
        }
    }

    auto operator[](int i) -> int& {
        return v[i];
    }
};

int total;
static vector<Task*> q;
static vector<vector<Task*>> hooks;
static vi req;

void submit(int i, bool v) {
    // printf("# rax[%d] = %d\n", i, static_cast<int>(v));

    total++;
    rax[i] = v;
    for (auto p : hooks[i]) {
        p->_c--;
        if (p->_c == 0) {
            // printf("push [%d %d %d %d]\n", p->v[0], p->v[1], p->v[2], p->v[3]);

            q.push_back(p);
        }
    }
}

int rcnt[5];
vector<float> pr;

void _require(int i, Task *sender, float p) {
    // printf("[%d %d %d %d] require rax[%d]\n",
    //     sender->v[0], sender->v[1], sender->v[2], sender->v[3], i);

    rcnt[sender->c0]++;
    pr[i] = p;

    sender->_c++;
    req.push_back(i);
    hooks[i].push_back(sender);
}

extern vector<bool> rdx;
int pcnt[5];
void dispatch() {
    static random_device rd;
    static mt19937_64 randi;

    // IMPORTANT!
    int m = req.size(), r = m % 4;
    shuffle(req.begin(), req.end(), randi);
    stable_sort(req.begin(), req.end(), [](int i, int j) {
        return pr[i] < pr[j] || (pr[i] == pr[j] && hooks[i].size() > hooks[j].size());
    });

    for (int i = 0; 4 * i + 3 < req.size(); i++) {
        auto p = new Task(req[4*i], req[4*i+1], req[4*i+2], req[4*i+3]);
        int c = rdx[p->v[0]] + rdx[p->v[1]] + rdx[p->v[2]] + rdx[p->v[3]];
        pcnt[c]++;
        q.push_back(p);
    }

    for (int i = 0; i < r; i++)
        req[i] = req[m - r + i];
    req.resize(r);

    // for (int i : req)
    //     submit(i, query({i}));
    // req.clear();
}

void execute() {
    for (int i = 0; i < q.size(); i++)
        q[i]->execute();
    // printf("%zu\n", q.size());
    q.clear();
}

int fcnt;

void solve(int n) {
    pr.resize(n);

    hooks.resize(n);
    req.resize(n);
    iota(req.begin(), req.end(), 0);

    while (total < n) {
        // printf("⇒ total = %d\n", total);

        if (req.size() < 4) {
            fcnt++;
            submit(req[0], query({req[0]}));
            req.erase(req.begin());

            // fcnt += req.size();
            // for (int i : req)
            //     submit(i, query({i}));
            // req.clear();
        } else dispatch();

        execute();
    }
}

// --- //

int cnt;
vector<bool> rdx;

int randbit() {
    static random_device rd;
    static mt19937_64 gen(rd());
    // static mt19937_64 gen(19260817);
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

    int n = atoi(argv[1]);
    rdx.resize(n);
    rax.resize(n);
    generate_n(rdx.begin(), n, randbit);

    // for (bool v : rdx)
    //     putchar(v ? '1' : '0');
    // puts("");

    solve(n);

    assert(rax == rdx);

    long double ratio = static_cast<long double>(cnt) / n;
    printf("cnt = %d\n", cnt);

    puts("");
    int sum = pcnt[0]+pcnt[1]+pcnt[2]+pcnt[3]+pcnt[4];
    printf("sum = %d\n", sum);
    for (int i = 0; i <= 4; i++)
        printf("t%d: %lf\n", i, static_cast<double>(pcnt[i]) / sum);
    printf("0&4: %lf\n", static_cast<double>(pcnt[0] + pcnt[4]) / sum);
    printf("1&3: %lf\n", static_cast<double>(pcnt[1] + pcnt[3]) / sum);
    printf("2: %lf\n", static_cast<double>(pcnt[2]) / sum);

    puts("");
    printf("fcnt = %d\n", fcnt);
    for (int i = 0; i <= 4; i++)
        printf("[%d]: %lf\n", i, static_cast<double>(rcnt[i]) / pcnt[i]);
    puts("");

    printf("ratio = %.8Lf\n", ratio);
    return 0;
}