#include <cassert>
#include <cstdio>
#include <cstring>

#include <vector>
#include <algorithm>
#include <type_traits>

using namespace std;

#define NMAX 100000
#define LOGN 17

template <typename T>
struct Node {
    Node(const T &_val)
        : val(_val), ch(NULL), nxt(NULL) {}

    T val;
    Node *ch, *nxt;
};

template <typename T>
struct Heap {
    typedef Node<T> *TNode;

    Heap() {
        memset(tr, 0, sizeof(tr));
    }

    TNode tr[LOGN];

    auto operator[](int i) -> TNode& {
        return tr[i];
    }

    auto operator[](int i) const -> TNode {
        return tr[i];
    }
};

template <typename TNode>
auto fuse(TNode x, TNode y) -> TNode {
    if (y->val < x->val)
        swap(x, y);
    y->nxt = x->ch;
    x->ch = y;
    return x;
}

template <typename THeap>
auto meld(THeap &u, THeap &v) -> THeap {
    THeap z;
    for (int i = 0; i < LOGN; i++) {
        if (z[i] && u[i]) {
            z[i + 1] = fuse(z[i], u[i]);
            z[i] = NULL;
        } else if (u[i]) z[i] = u[i];
        if (z[i] && v[i]) {
            z[i + 1] = fuse(z[i], v[i]);
            z[i] = NULL;
        } else if (v[i]) z[i] = v[i];
    }
    return z;
}

//template <typename THeap>
//auto meld(THeap &u, THeap &v) -> THeap& {
//    typename THeap::TNode t = NULL;
//    for (int i = 0; i < LOGN; i++) {
//        if (t && u[i]) {
//            t = fuse(t, u[i]);
//            u[i] = NULL;
//        } else if (t) {
//            u[i] = t;
//            t = NULL;
//        }
//        if (u[i] && v[i]) {
//            t = fuse(u[i], v[i]);
//            u[i] = NULL;
//        } else if (v[i]) u[i] = v[i];
//    }
//    return u;
//}

template <typename T>
struct HeapInterface {
    typedef Heap<T> THeap;
    typedef typename THeap::TNode TNode;

    HeapInterface() : size(0) {}

    THeap q;
    int size;

    void pop() {
        assert(!empty());
        int t = _top(), i;
        TNode x = q[t];
        q[t] = NULL;
        THeap p;
        for (i = 0, x = x->ch; x; x = x->nxt, i++)
            p[i] = x;
        reverse(&p[0],&p[i]);
        q = meld(q, p);
        size--;
    }

    void push(const T &x) {
        THeap p;
        p[0] = new typename remove_pointer<TNode>::type(x);
        q = meld(q, p);
        size++;
    }

    auto _top() const -> int {
        int t = 0;
        for (int i = 1; i < LOGN; i++)
            if (!q[t] || (q[i] && q[i]->val < q[t]->val))
                t = i;
        return t;
    }

    T top() const {
        assert(!empty());
        return q[_top()]->val;
    }

    bool empty() const {
        return size == 0;
    }
};

struct Edge {
    int v, w;
};

static int n, m, s;
static int dist[NMAX + 10];
static vector<Edge> G[NMAX + 10];

void shortest() {
    struct State {
        int u, t;

        bool operator<(const State &z) const {
            return t < z.t;
        }
    };

    HeapInterface<State> q;
    memset(dist, 0x3f, sizeof(dist));
    dist[s] = 0;
    q.push({s, 0});
    while (!q.empty()) {
        auto s = q.top();
        q.pop();
        if (dist[s.u] < s.t) continue;
        int u = s.u;
        for (auto &e : G[u]) {
            int v = e.v;
            if (dist[v] > dist[u] + e.w) {
                dist[v] = dist[u] + e.w;
                q.push({v, dist[v]});
            }
        }
    }
}

int main() {
    scanf("%d%d%d", &n, &m, &s);
    for (int i = 0; i < m; i++) {
        int u, v, w;
        scanf("%d%d%d", &u, &v, &w);
        G[u].push_back({v, w});
    }
    shortest();
    for (int i = 1; i <= n; i++)
        printf("%d ", dist[i]);
    return 0;
}
