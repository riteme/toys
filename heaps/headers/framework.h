#pragma once

#include <vector>

using std::vector;

typedef long long i64;
typedef unsigned long long u64;

constexpr i64 INF = 0x3f3f3f3f'3f3f3f3f;
constexpr int INF_BYTE = 0x3f;
constexpr i64 WMAX = 1'000'000'000;

class ShortestPath;

namespace HeapInterface {
    /**
     * @brief associated instance of Dijkstra'a algorithm
     */
    extern ShortestPath *instance;

    /**
     * @brief number of vertices
     */
    extern int n;

    /**
     * @brief auxiliary counter
     */
    extern int _aux;

    /**
     * @brief key values (distances)
     */
    extern i64 *w;

    /**
     * @brief initialize environment
     *
     * @param _instance associated instance
     */
    void initialize(ShortestPath *_instance);

    /**
     * @brief initialize & clean up heap structure
     */
    void clear();

    /**
     * @brief push new vertex into heap
     *
     * @param x new vertex
     */
    void push(int x);

    /**
     * @brief notify that the key of vertex x has been decreased
     *
     * @param x target vertex
     */
    void decrease(int x);

    /**
     * @brief remove the vertex of minimum key and return it
     *
     * @return int minimum vertex
     */
    auto pop() -> int;

    /**
     * @brief return whether the heap is empty
     *
     * @return true  the heap is empty
     * @return false the heap is not empty
     */
    auto empty() -> bool;
}

class ShortestPath {
public:
    struct Edge {
        int u, v;
        i64 w;
    };

    struct Result {
        u64 hash = 0;   // hash for distance array
        u64 trace = 0;  // hash for access sequence
        double time;    // in milliseconds
        int count = 0;  // number of decrease_key operations
        int aux = 0;    // auxiliary counter
    };

    ShortestPath(int _n, int _m);
    ~ShortestPath();

    void link(int u, int v, i64 w = 0);
    auto solve(int s) -> Result;

    int n, m;

    auto operator[](int u) -> vector<Edge>&;

private:
    friend void HeapInterface::initialize(ShortestPath*);

    ShortestPath(const ShortestPath&) = delete;
    ShortestPath(ShortestPath&&) = delete;
    auto operator=(const ShortestPath&) = delete;
    auto operator=(ShortestPath&&) = delete;

    i64 *dist;
    vector<Edge> *G;
};