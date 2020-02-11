#include "framework.h"

#include <cstdio>
#include <cstring>

#include <queue>
#include <vector>
#include <chrono>

constexpr u64 HASH_BASE = 19260817;

inline u64 dist_hash(i64 *dist, int n) {
    u64 ret = 0;
    for (int i = 1; i <= n; i++)
        ret = ret * HASH_BASE + dist[i];
    return ret;
}

template <typename T>
double convert_span(const T &x) {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(x).count() / 1e6;
}

ShortestPath::ShortestPath(int _n, int _m):
    n(_n), m(_m),
    dist(new i64[n + 1]),
    G(new std::vector<Edge>[n + 1]),
    q(new HeapInterface(n, this)) {}

ShortestPath::~ShortestPath() {
    delete[] dist;
    delete[] G;
    delete q;
}

void ShortestPath::link(int u, int v, i64 w) {
    G[u].push_back({u, v, w});
    G[v].push_back({v, u, w});
}

auto ShortestPath::solve(int s) -> Result {
    using namespace std::chrono;

    Result ret;

    memset(dist + 1, INF_BYTE, sizeof(i64) * n);
    dist[s] = 0;
    q->clear();
    q->push(s, 0);

    auto t_start = high_resolution_clock::now();
    do {
        ret.peek = std::max(ret.peek, q->size());
        int u = q->pop();

        for (auto &e : G[u]) {
            i64 neo = dist[u] + e.w;
            if (dist[e.v] > neo) {
                if (dist[e.v] == INF)
                    q->push(e.v, neo);
                else
                    q->decrease(e.v, neo);

                dist[e.v] = neo;
                ret.count++;
            }
        }
    } while (!q->empty());
    auto t_end = high_resolution_clock::now();
    ret.time = convert_span(t_end - t_start);

    ret.hash = dist_hash(dist, n);
    return ret;
}

auto ShortestPath::operator[](int u) -> std::vector<Edge>& {
    return G[u];
}