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
    G(new std::vector<Edge>[n + 1]) {
    HeapInterface::initialize(this);
}

ShortestPath::~ShortestPath() {
    delete[] dist;
    delete[] G;
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

    HeapInterface::_aux = 0;
    HeapInterface::clear();
    HeapInterface::push(s);

    auto t_start = high_resolution_clock::now();
    do {
        int u = HeapInterface::pop();

        for (auto &e : G[u]) {
            if (dist[e.v] > dist[u] + e.w) {
                bool neo = dist[e.v] == INF;
                dist[e.v] = dist[u] + e.w;

                if (neo)
                    HeapInterface::push(e.v);
                else
                    HeapInterface::decrease(e.v);

                ret.count++;
            }
        }
    } while (!HeapInterface::empty());
    auto t_end = high_resolution_clock::now();
    ret.time = convert_span(t_end - t_start);

    ret.hash = dist_hash(dist, n);
    ret.aux = HeapInterface::_aux;
    return ret;
}

auto ShortestPath::operator[](int u) -> std::vector<Edge>& {
    return G[u];
}