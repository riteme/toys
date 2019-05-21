#include <cstdio>
#include <cstring>

#include <thread>
#include <random>
#include <vector>
#include <algorithm>

using namespace std;

#define NMAX 16000L
#define JMAX 8

typedef long long i64;

static std::random_device rd;
static std::mt19937 g(rd());

static int m;
static struct Edge {
    int u, v;
    int ep(int x) const {
        return u == x ? v : u;
    }
} E[NMAX * NMAX];

struct EvalNode {
    EvalNode(int _n) : n(_n) {
        for (int i = 0; i < m; i++) id[i] = i;
    }

    int n, p, len;
    int fa[NMAX + 1];
    int id[NMAX * NMAX];
    vector<Edge*> G[NMAX + 1];

    void setup() {
        memset(fa + 1, 0, sizeof(int) * n);
        shuffle(id, id + m, g);
        for (int i = 1; i <= n; i++) G[i].clear();
    }

    int find(int x) {
        return fa[x] ? fa[x] = find(fa[x]) : x;
    }

    void _dfs(int x, int f, int d) {
        if (d > len) {
            p = x;
            len = d;
        }
        for (auto e : G[x]) {
            int v = e->ep(x);
            if (v == f) continue;
            _dfs(v, x, d + 1);
        }
    }

    void dfs(int x) {
        len = 0;
        _dfs(x, 0, 0);
    }
};

static i64 result[JMAX];

void process(int id, int n, int T) {
    i64 &sum = result[id];
    EvalNode *mem = new EvalNode(n);
    for (int j = 0; j < T; j++) {
        mem->setup();
        for (int i = 0, c = 0; c < n - 1; i++) {
            Edge &e = E[mem->id[i]];
            int idu = mem->find(e.u);
            int idv = mem->find(e.v);
            if (idu == idv) continue;
            c++;
            mem->fa[idu] = idv;
            mem->G[e.u].push_back(&e);
            mem->G[e.v].push_back(&e);
        }
        mem->dfs(1);
        mem->dfs(mem->p);
        sum += mem->len;
    }
    //delete mem;
}

int main(int argc, char *argv[]) {
    if (argc < 4) return fprintf(stderr, "Usage: %s [n] [T] [j]\n", argv[0]), 0;
    int n = atoi(argv[1]);
    int T = atoi(argv[2]);
    int J = atoi(argv[3]);
    for (int u = 1; u <= n; u++) for (int v = u + 1; v <= n; v++)
        E[m++] = {u, v};

    vector<thread*> ts;
    for (int j = 0; j < J; j++)
        ts.push_back(new thread(process, j, n, T / J + (j < T % J ? 1 : 0)));
    i64 sum = 0;
    for (int j = 0; j < J; j++) {
        ts[j]->join();
        sum += result[j];
    }
    printf("%.6Lf\n", (long double)sum / T);
    return 0;
}
