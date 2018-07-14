#include <cstdio>
#include <cstring>

#include <bitest>
#include <algorithm>

using namespace std;

#define NMAX 1000

static int n, m;
static data_t mat[NMAX + 1];

inline bool bit(int x, int y) {
    return (mat[x] >> y) & 1;
}

void initialize() {
    char buf[NMAX + 1];
    scanf("%d%d", &n, &m);
    for (int i = 1; i <= n; i++) {
        scanf("%s", buf + 1);
        for (int j = 1; j <= m; j++)
            if (buf[j] == '1')
                mat[i] |= static_cast<data_t>(1) << j;
    }
}

void show(data_t S) {
    static int cnt;
    printf("%d:\n", ++cnt);
    for (int i = 1; i <= n; i++) {
        char c = (S >> i) & 1 ? 'O' : '1';
        for (int j = 1; j <= m; j++)
            putchar(bit(i, j) ? c : '.');
        puts("");
    }
}

struct Node {
    Node() : x(0), y(0), l(this), r(this), up(this), dw(this) {}
    Node(int _x, int _y)
        : x(_x), y(_y),
          l(this), r(this), up(this), dw(this) {}

    int x, y;
    Node *l, *r, *up, *dw;
};

#define INSERT_FN(name, pre, nxt) \
inline void name(Node *dst, Node *src) { \
    if (!dst) return; \
    src->pre = dst; src->nxt = dst->nxt; \
    dst->nxt->pre = src; dst->nxt = src; \
}

INSERT_FN(insert_lr, l, r)
INSERT_FN(insert_ud, up, dw)

inline void _del(Node *x) {
    x->l->r = x->r; x->r->l = x->l;
    x->dw->up = x->up; x->up->dw = x->dw;
}

inline void _rev(Node *x) {
    x->l->r = x->r->l = x->dw->up = x->up->dw = x;
}

static Node *head;
static Node *ptr[NMAX + 1][NMAX + 1];

void setup_dlx() {
    for (int i = 1; i <= n; i++) {
        Node *dst = nullptr;
        for (int j = m; j >= 1; j--)
            if (bit(i, j)) {
                insert_lr(dst, ptr[i][j] = new Node(i, j));
                dst = ptr[i][j];
            }
    }

    head = new Node;
    for (int j = m; j >= 1; j--) {
        Node *col = new Node;
        for (int i = n; i >= 1; i--)
            if (bit(i, j)) insert_ud(col, ptr[i][j]);
        insert_lr(head, col);
    }
}

static int pos;
static Node *stk[NMAX * (NMAX + 1) + 1];

inline void del(Node *x) {
    _del(x);
    stk[pos++] = x;
}

inline void rev() {
    _rev(stk[--pos]);
}

void dfs() {
    static data_t cur;
    int last = pos;

    Node *col = head->r;
    if (col == head) show(cur);
    else if (col->dw == col) return;
    else {
        for (Node *row = col->dw; row != col; row = row->dw) {
            cur |= 1 << row->x;  // 中央已经钦定了

            Node *p = row;
            do {
                for (Node *q = p->dw; q != p; q = q->dw)
                    if (q->x) while (q->r != q) del(q->r);
                while (p->dw != p) del(p->dw);
                p = p->r;
            } while (p != row);
            while (row->r != row) del(row->r);
            del(row);

            dfs();

            cur ^= 1 << row->x;
            while (pos > last) rev();
        }
    }
}

int main() {
    initialize();
    setup_dlx();
    dfs();

    return 0;
}
