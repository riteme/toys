#include "hackers.h"

#include <list>
#include <deque>
#include <random>
#include <algorithm>
#include <unordered_map>

using std::vector;

namespace hack {

typedef vector<std::unordered_map<int, i64>> HashMap;

static i64 seed;
static std::mt19937_64 randi;

inline i64 eid(int u, int v) {
    return (static_cast<i64>(u) << 32) | v;
}

inline i64 randint(i64 l, i64 r) {
    return randi() % (r - l + 1) + l;
}

void initiailize(i64 _seed) {
    seed = _seed;
    randi = std::mt19937_64(seed);
}

auto get_hacker(const std::string &name) -> Hacker {
    if (name == "uniform")
        return uniform;
    if (name == "span")
        return span;
    if (name == "stack")
        return stack;

    fprintf(stderr, "No hacker named \"%s\".\n", name.c_str());
    abort();
}

void uniform(Graph &G, int s) {
    HashMap mp(G.n + 1);

    for (int u = 1; u <= G.n; u++)
    for (auto &e : G[u]) if (e.u < e.v)
        mp[e.u][e.v] =  e.w = randint(1, WMAX);

    for (int u = 1; u <= G.n; u++)
    for (auto &e : G[u]) if (e.u > e.v)
        e.w = mp[e.v][e.u];
}

inline void random_spanning_tree(
    Graph &G, int s,
    vector<int> &id,
    vector<int> &sequence
) {
    int n = G.n;
    id.resize(n + 1);
    sequence.resize(n);

    std::deque<int> q;
    q.push_back(s);
    vector marked(n + 1, false);
    marked[s] = true;
    for (int i = 0; i < n; i++) {
        int u = q.front();
        q.pop_front();
        sequence[i] = u;

        for (auto &e : G[u]) if (!marked[e.v]) {
            marked[e.v] = true;
            q.push_back(e.v);
            std::swap(q[randi() % q.size()], q.back());
        }
    }

    for (int i = 0; i < n; i++)
        id[sequence[i]] = i;
}

inline void assign_weights(
    Graph &G,
    vector<int> &id,
    HashMap &mp
) {
    for (int u = 1; u <= G.n; u++)
    for (auto &e : G[u]) if (id[u] > id[e.v])
        e.w = mp[e.v][u];
}

void span(Graph &G, int s) {
    constexpr i64 PMAX = 1'00000'00000'00000;

    // assume this hacker run on the same graph
    static vector<int> id, sequence;
    static vector last(G.n + 1, 0);
    static vector<vector<i64>> levels(G.n + 1);
    static HashMap mp(G.n + 1);

    random_spanning_tree(G, s, id, sequence);

    // determine last entrance vertex & levels
    for (int x : sequence) {
        if (x != s)
            levels[x][0] = id[x];
        sort(levels[x].begin(), levels[x].end());

        for (auto &e : G[x])
        if (id[x] < id[e.v]) {
            last[e.v] = x;
            levels[e.v].push_back(randint(id[e.v] + 1, PMAX));
        }
    }

    // compute edge weights
    vector dist(G.n + 1, INF);
    dist[s] = 0;
    for (int u : sequence)
    for (auto &e : G[u]) if (id[u] < id[e.v]) {
        int v = e.v;
        dist[v] = levels[v].back();
        levels[v].pop_back();
        mp[u][v] = e.w = dist[v] - id[u];
    }

    assign_weights(G, id, mp);
}

void stack(Graph &G, int s) {
    constexpr i64 PMAX = 1'000000'000000'000000;

    vector<int> stk, id(G.n + 1, 0), sequence;
    vector<i64> dist(G.n + 1);
    static vector<vector<int>> levels(G.n + 1);
    static HashMap mp(G.n + 1);
    sequence.reserve(G.n);

    // simulate to generate traces & levels
    stk.push_back(s);
    int cur = 0;
    do {
        int u = stk.back();
        stk.pop_back();
        levels[u].push_back(cur++);

        if (id[u])
            continue;

        sequence.push_back(u);
        id[u] = cur;

        int cnt = 0;
        for (auto &e : G[u]) if (!id[e.v]) {
            stk.push_back(e.v);
            cnt++;
        }

        // uncomment the next line to obtain a randomized version.
        // std::shuffle(stk.end() - cnt, stk.end(), randi);
    } while (!stk.empty());

    // evaluate edge weights
    i64 S = PMAX / G.m;
    for (int u : sequence)
    for (auto &e : G[u]) if (id[u] < id[e.v]) {
        int v = e.v;
        dist[v] = S * levels[v].back();
        levels[v].pop_back();
        mp[u][v] = e.w = dist[v] - dist[u];
    }

    assign_weights(G, id, mp);
}

}