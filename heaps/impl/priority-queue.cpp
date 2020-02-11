#include "../framework.h"

#include <queue>

using namespace std;

struct State {
    int x;
    i64 t;

    bool operator<(const State &z) const {
        return t > z.t;
    }
};

static int remain;
static priority_queue<State> q;

void HeapInterface::clear() {
    remain = 0;
    while (!q.empty()) q.pop();
}

void HeapInterface::push(int x, i64 v) {
    remain++;
    q.push({x, v});
}

void HeapInterface::decrease(int x, i64 v) {
    q.push({x, v});
}

auto HeapInterface::pop() -> int {
    remain--;
    State u;
    do {
        u = q.top();
        q.pop();
    } while (u.t > instance->dist[u.x]);
    return u.x;
}

auto HeapInterface::size() -> int {
    return q.size();
}

auto HeapInterface::empty() -> bool {
    return remain == 0;
}