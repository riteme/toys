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
void _require(int i, Task*);

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
    }

    int t;
    int v[4];
    int _c;

    int c0, c1, c2;

    void require(int i) {
        _require(i, this);
    }

    void require(const vi &li) {
        for (int i : li)
            require(i);
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
                c1 = query({v[0], v[1]});
                c2 = c0 - c1;
                if (!attempt({v[0], v[1]}, c1))
                    require(v[0]);
                if (!attempt({v[2], v[3]}, c2))
                    require(v[2]);
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
                if (c1 == 1)
                    submit(v[1], rax[v[0]] ^ 1);
                else
                    submit(v[3], rax[v[2]] ^ 1);
            } break;

            case 2: {
                c2 = 2 - rax[v[0]] - rax[v[1]];
                if (!attempt({v[2], v[3]}, c2))
                    require(v[2]);
            } break;

            default: abort();
        }
    }

    void phase3() {
        assert(c0 == 2);
        submit(v[3], rax[v[2]] ^ 1);
    }

    void execute() {
        t++;

        // printf("execute [%d %d %d %d]@%d\n", v[0], v[1], v[2], v[3], t);
        switch (t) {
            case 1: phase1(); break;
            case 2: phase2(); break;
            case 3: phase3(); break;
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

void _require(int i, Task *sender) {
    // printf("[%d %d %d %d] require rax[%d]\n",
    //     sender->v[0], sender->v[1], sender->v[2], sender->v[3], i);

    sender->_c++;
    req.push_back(i);
    hooks[i].push_back(sender);
}

void dispatch() {
    static random_device rd;
    static mt19937_64 randi;

    // IMPORTANT!
    shuffle(req.begin(), req.end(), randi);

    for (int i = 0; 4 * i + 3 < req.size(); i++) {
        auto p = new Task(req[4*i], req[4*i+1], req[4*i+2], req[4*i+3]);
        q.push_back(p);
    }

    int m = req.size(), r = m % 4;
    for (int i = 0; i < r; i++)
        req[i] = req[m - r + i];
    req.resize(r);
}

void execute() {
    for (int i = 0; i < q.size(); i++)
        q[i]->execute();
    // printf("%zu\n", q.size());
    q.clear();
}

void solve(int n) {
    hooks.resize(n);
    req.resize(n);
    iota(req.begin(), req.end(), 0);

    while (total < n) {
        // printf("⇒ total = %d\n", total);

        if (req.size() < 4) {
            for (int i : req)
                submit(i, query({i}));
            req.clear();
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
    printf("ratio = %.8Lf\n", ratio);
    return 0;
}