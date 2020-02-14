#include "framework.h"

#include <queue>
#include <vector>

using namespace std;

namespace HeapInterface {

static queue<int> q;
static vector<bool> in;

void clear() {
    while (!q.empty()) q.pop();
    in.resize(n + 1, false);
}

void push(int x) {
    if (!in[x]) {
        q.push(x);
        in[x] = true;
    }
}

void decrease(int x) {
    push(x);
}

auto pop() -> int {
    int u = q.front();
    q.pop();
    in[u] = false;
    return u;
}

auto empty() -> bool {
    return q.size() == 0;
}

}