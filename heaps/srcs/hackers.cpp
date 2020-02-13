#include "hackers.h"

#include <deque>
#include <random>
#include <algorithm>
#include <unordered_map>

typedef std::vector<std::unordered_map<int, i64>> HashMap;

static i64 seed;
static std::mt19937_64 randi;

inline i64 eid(int u, int v) {
    return (static_cast<i64>(u) << 32) | v;
}

inline i64 randint(i64 l, i64 r) {
    return randi() % (r - l + 1) + l;
}

void hack::initiailize(i64 _seed) {
    seed = _seed;
    randi = std::mt19937_64(seed);
}

void hack::uniform(ShortestPath &instance, int s) {
    HashMap mp(instance.n + 1);

    for (int u = 1; u <= instance.n; u++)
    for (auto &e : instance[u]) if (e.u < e.v)
        mp[e.u][e.v] =  e.w = randint(1, WMAX);

    for (int u = 1; u <= instance.n; u++)
    for (auto &e : instance[u]) if (e.u > e.v)
        e.w = mp[e.v][e.u];
}

void hack::max_decrease(ShortestPath &instance, int s) {
    constexpr i64 PMAX = 1'00000'00000'00000;

    using std::vector;

    int n = instance.n;
    vector id(n + 1, 0);
    vector last(n + 1, 0);
    vector<int> sequence;
    sequence.reserve(n);

    // assume this hacker run on the same graph
    static vector<vector<i64>> levels(n + 1);
    static HashMap mp(n + 1);

    // generate random traversal sequence
    std::deque<int> q;
    q.push_back(s);
    vector visited(n + 1, false);
    visited[s] = true;
    for (int i = 0; i < n; i++) {
        int u = q.front();
        q.pop_front();
        id[u] = i;
        sequence.push_back(u);

        for (auto &e : instance[u]) if (!visited[e.v]) {
            visited[e.v] = true;
            q.push_back(e.v);
            std::swap(q[randi() % q.size()], q.back());
        }
    }

    // determine last entrance vertex & levels
    for (int x : sequence) {
        if (x != s)
            levels[x][0] = id[x];
        sort(levels[x].begin(), levels[x].end());

        for (auto &e : instance[x])
        if (id[x] < id[e.v]) {
            last[e.v] = x;
            levels[e.v].push_back(randint(id[e.v] + 1, PMAX));
        }
    }

    // compute edge weights
    vector dist(n + 1, INF);
    dist[s] = 0;
    for (int u : sequence)
    for (auto &e : instance[u]) if (id[u] < id[e.v]) {
        int v = e.v;
        dist[v] = levels[v].back();
        levels[v].pop_back();
        mp[u][v] = dist[v] - id[u];
    }

    // assign edge weights
    for (int u = 1; u <= n; u++)
    for (auto &e : instance[u])
        e.w = id[u] < id[e.v] ? mp[u][e.v] : mp[e.v][u];
}