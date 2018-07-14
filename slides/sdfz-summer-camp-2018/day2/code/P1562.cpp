#include <cctype>
#include <cstdio>
#include <cstring>

#include <bitset>
#include <algorithm>

using namespace std;

#define SIZE 14
#define NMAX (SIZE * SIZE)
#define MMAX (SIZE * SIZE)

static int n, m, s;
static bitset<MMAX> mat[NMAX + 1];  // rows 1..n, cols 0..m - 1

inline int cpos(int x, int y) {
    return s * (x - 1) + y - 1;
}

inline int rpos(int x, int y) {
    return cpos(x, y) + 1;
}

inline char getch() {
    char c;
    do {
        c = getchar();
    } while (!ispunct(c));
    return c;
}

inline void activate(int x, int y) {
    constexpr int NUM_DIRECTIONS = 8;
    constexpr int dx[] = {-1,-1, 0, 1, 1, 1, 0,-1};
    constexpr int dy[] = { 0, 1, 1, 1, 0,-1,-1,-1};

    int r = rpos(x, y);
    mat[r][cpos(x, y)] = 1;
    for (int p = 0; p < NUM_DIRECTIONS; p++) {
        for (int tx = x + dx[p], ty = y + dy[p];
             1 <= tx && tx <= s && 1 <= ty && ty <= s;
             tx += dx[p], ty += dy[p])
            mat[r][cpos(tx, ty)] = 1;
    }
}

void initialize() {
    scanf("%d", &s);
    n = m = s * s;
    for (int x = 1; x <= s; x++) for (int y = 1; y <= s; y++) {
        char c = getch();
        if (c == '*') activate(x, y);
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
static int size[MMAX];
static Node *ptr[NMAX + 1][MMAX], *cols[MMAX];

void setup_dlx() {
    for (int i = 1; i <= n; i++) {
        Node *dst = nullptr;
        for (int j = m - 1; j >= 0; j--)
            if (mat[i][j]) {
                insert_lr(dst, ptr[i][j] = new Node(i, j));
                dst = ptr[i][j];
            }
    }

    head = new Node;
    for (int j = m - 1; j >= 0; j--) {
        Node *col = new Node(0, j);
        cols[j] = col;
        for (int i = n; i >= 1; i--)
            if (mat[i][j]) {
                insert_ud(col, ptr[i][j]);
                size[j]++;
            }
        insert_lr(head, col);
    }
}

static int pos, ans;
static Node *stk[MMAX * (NMAX + 1) + 1];

inline void del(Node *x) {
    _del(x);
    if (x->x) size[x->y]--;
    stk[pos++] = x;
}

inline void rev() {
    Node *x = stk[--pos];
    if (x->x) size[x->y]++;
    _rev(x);
}

void dfs(int dep = 0) {
    int last = pos;

    Node *col = head->r;
    if (col == head) {
        if (dep == s) ans++;
        return;
    }

    for (Node *cand = col->r; cand != head; cand = cand->r) {
        if (size[cand->y] < size[col->y])
            col = cand;
    }

    if (col->dw == col) return;
    for (Node *row = col->dw; row != col; row = row->dw) {
        while (row->r != row) {
            Node *p = cols[row->r->y];
            if (p->r->l == p) del(p);
            del(row->r);
        }
        del(col);
        del(row);

        dfs(dep + 1);

        while (pos > last) rev();
    }
}

int main() {
    initialize();
    setup_dlx();
    dfs();
    printf("%d\n", ans);

    return 0;
}
