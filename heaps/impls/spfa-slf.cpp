#include "framework.h"

#include <deque>
#include <vector>

using namespace std;

namespace HeapInterface {

static deque<int> q;
static vector<bool> in;

void clear() {
    q.clear();
    in.resize(n + 1, false);
}

void push(int x) {
    if (!in[x]) {
        if (q.empty() || w[x] <= w[q[0]])
            q.push_front(x);
        else
            q.push_back(x);
        in[x] = true;
    }
}

void decrease(int x) {
    push(x);
}

auto pop() -> int {
    int u = q[0];
    q.pop_front();
    in[u] = false;
    return u;
}

auto empty() -> bool {
    return q.empty();
}

}