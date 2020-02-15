#include "framework.h"

#include <algorithm>

#include <ext/pb_ds/priority_queue.hpp>

using namespace std;

namespace HeapInterface {

using Pair = pair<i64, int>;
using PriorityQueue = __gnu_pbds::priority_queue<Pair, std::greater<Pair>>;

static PriorityQueue q;
static vector<PriorityQueue::point_iterator> it;

void clear() {
    q.clear();
    it.resize(n + 1);
}

void push(int x) {
    it[x] = q.push({w[x], x});
}

void decrease(int x) {
    q.modify(it[x], {w[x], x});
}

auto pop() -> int {
    int u = q.top().second;
    q.pop();
    return u;
}

auto empty() -> bool {
    return q.empty();
}

}