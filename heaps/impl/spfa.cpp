#include "../framework.h"

#include <queue>
#include <vector>

using namespace std;

static queue<int> q;
static vector<bool> in;

void HeapInterface::clear() {
    while (!q.empty()) q.pop();
    in.resize(n + 1, false);
}

void HeapInterface::push(int x, i64 v) {
    if (!in[x]) {
        q.push(x);
        in[x] = true;
    }
}

void HeapInterface::decrease(int x, i64 v) {
    push(x, v);
}

auto HeapInterface::pop() -> int {
    int u = q.front();
    q.pop();
    in[u] = false;
    return u;
}

auto HeapInterface::size() -> int {
    return q.size();
}

auto HeapInterface::empty() -> bool {
    return size() == 0;
}