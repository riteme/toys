#include <cassert>
#include <cstdio>
#include <cstring>

#include <vector>
#include <algorithm>

using namespace std;

#define NMAX 100000
#define LOGN 17

template <typename T>
struct Heap {
    struct Node {
        Node(const T &_val) : val(_val), ch(NULL), nxt(NULL) {}

        T val;
        Node *ch, *nxt;

        Node *fuse(Node *y) {
            assert(!y->nxt);
            y->nxt = ch;
            ch = y;
        }
    }

    static Node *fuse(Node *x, Node *y) {
        assert(x);
        assert(!x->nxt);
        assert(y);
        assert(!y->nxt);
        if (x->val < y->val) swap(x, y);
        y->nxt = x->ch;
        x->ch = y;
        return x;
    }

    Heap() {
        memset(buc, 0, sizeof(buc));
    }

    Node *buc[LOGN];

    Node *&operator[](int i) {
        return buc[i];
    }

    Heap &operator+=(const Heap &z) {
        Node *carry = NULL;
        for (int i = 0; i < LOGN - 1; i++) {
            if (buc[i] && carry) {
                carry = fuse(buc[i], carry);
                buc[i] = NULL;
            } else {
                buc[i] = carry;
                carry = NULL;
            }
            if (buc[i] && z[i]) {
                carry = fuse(buc[i], z[i]);
                buc[i] = NULL;
            } else {
                buc[i] = z[i];
            }
        }
        buc[LOGN - 1] = carry;
        return *this;
    }
};

template <typename T>
struct HeapInterface {
    void pop() {
        Q.pop();
    }

    void push(const T &x) {
        Q.push(x);
    }

    T top() {
        return Q.top();
    }

    bool empty() const {
        return Q.empty();
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
            return t > z.t;
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
