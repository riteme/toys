#include "framework.h"

#include <queue>

using namespace std;

namespace HeapInterface {

struct State {
    int x;
    i64 t;

    bool operator<(const State &z) const {
        return t > z.t;
    }
};

static int remain;
static priority_queue<State> q;

void clear() {
    remain = 0;
    while (!q.empty()) q.pop();
}

void push(int x) {
    remain++;
    q.push({x, w[x]});
}

void decrease(int x) {
    q.push({x, w[x]});
}

auto pop() -> int {
    remain--;
    State u;
    do {
        u = q.top();
        q.pop();
    } while (u.t > w[u.x]);
    return u.x;
}

auto empty() -> bool {
    return remain == 0;
}

}