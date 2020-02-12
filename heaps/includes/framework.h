#pragma once

#include <vector>

using std::vector;

typedef long long i64;
typedef unsigned long long u64;

constexpr i64 INF = 0x3f3f3f3f'3f3f3f3f;
constexpr int INF_BYTE = 0x3f;
constexpr i64 WMAX = 1'000'000'000;

class ShortestPath {
public:
    struct Edge {
        int u, v;
        i64 w;
    };

    struct Result {
        u64 hash = 0;   // hash for distance array
        double time;    // in milliseconds
        int count = 0;  // number of decrease_key operations
        int peek = 0;   // maximum number of element in the heap
    };

    ShortestPath(int _n, int _m);
    ~ShortestPath();

    void link(int u, int v, i64 w = 0);
    auto solve(int s) -> Result;

    int n, m;

    auto operator[](int u) -> std::vector<Edge>&;

private:
    friend struct HeapInterface;

    ShortestPath(const ShortestPath&) = delete;
    ShortestPath(ShortestPath&&) = delete;
    auto operator=(const ShortestPath&) = delete;
    auto operator=(ShortestPath&&) = delete;

    i64 *dist;
    vector<Edge> *G;
    struct HeapInterface *q;
};

struct HeapInterface {
    HeapInterface(int _n, ShortestPath *_instance):
        n(_n), instance(_instance), w(instance->dist) {}

    void clear();
    void push(int x);
    void decrease(int x);
    auto pop() -> int;
    auto size() -> int;
    auto empty() -> bool;

    int n;
    ShortestPath *instance;
    i64 *w;
};