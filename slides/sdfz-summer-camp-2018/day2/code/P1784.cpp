#include <cstdio>
#include <cstring>

#include <bitset>
#include <algorithm>

using namespace std;

#define BSIZE 3
#define SIZE (BSIZE * BSIZE)
#define NMAX (SIZE * SIZE * SIZE)
#define MMAX (SIZE * SIZE * 4)

static int n, m;
static bitset<MMAX> mat[NMAX + 1];  // rows 1..n, cols 0..m - 1

inline bool bit(int x, int y) {
    return mat[x][y];
}

inline int rplace(int x, int y, int c) {
    constexpr int PREV = 0;
    return PREV + SIZE * SIZE * (x - 1) + SIZE * y + c - 1;
}

inline int crow(int x, int c) {
    constexpr int PREV = SIZE * SIZE * 0;
    return PREV + SIZE * (x - 1) + c - 1;
}

inline int ccol(int y, int c) {
    constexpr int PREV = SIZE * SIZE * 1;
    return PREV + SIZE * y + c - 1;
}

inline int cblock(int x, int y, int c) {
    constexpr int PREV = SIZE * SIZE * 2;
    int idx = (x - 1) / BSIZE, idy = y / BSIZE;
    int id = BSIZE * idx + idy;
    return PREV + SIZE * id + c - 1;
}

inline int cslot(int x, int y) {
    constexpr int PREV = SIZE * SIZE * 3;
    return PREV + SIZE * (x - 1) + y;
}


inline void set(int x, int y, int c) {
    int p = rplace(x, y, c);
    mat[p][crow(x, c)] = 1;
    mat[p][ccol(y, c)] = 1;
    mat[p][cblock(x, y, c)] = 1;
    mat[p][cslot(x, y)] = 1;
}

void initialize() {
    n = NMAX; m = MMAX;
    int c;
    for (int x = 1; x <= SIZE; x++) for (int y = 0; y < SIZE; y++) {
        scanf("%d", &c);

        if (c)
            set(x, y, c);
        else {
            for (int i = 1; i <= SIZE; i++)
                set(x, y, i);
        }
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
static Node *ptr[NMAX + 1][NMAX + 1];

void setup_dlx() {
    for (int i = 1; i <= n; i++) {
        Node *dst = nullptr;
        for (int j = m - 1; j >= 0; j--)
            if (bit(i, j)) {
                insert_lr(dst, ptr[i][j] = new Node(i, j));
                dst = ptr[i][j];
            }
    }

    head = new Node;
    for (int j = m - 1; j >= 0; j--) {
        Node *col = new Node(0, j);
        for (int i = n; i >= 1; i--)
            if (bit(i, j)) {
                insert_ud(col, ptr[i][j]);
                size[j]++;
            }
        insert_lr(head, col);
    }
}

static int pos;
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

void show(bitset<NMAX + 1> &dat) {
    static int map[SIZE][SIZE];
    for (int x = 0; x < SIZE; x++) for (int y = 0; y < SIZE; y++) for (int c = 0; c < SIZE; c++) {
        if (dat[rplace(x + 1, y, c + 1)]) map[x][y] = c + 1;
    }

    for (int x = 0; x < SIZE; x++) {
        for (int y = 0; y < SIZE; y++) {
            printf("%d ", map[x][y]);
        }
        puts("");
    }
    puts("");
}

void dfs() {
    static bitset<NMAX + 1> cur;
    int last = pos;

    Node *col = head->r;
    if (col == head) {
        show(cur);
        return;
    }

    for (Node *cand = col->r; cand != head; cand = cand->r) {
        if (size[cand->y] < size[col->y])
            col = cand;
    }

    if (col->dw == col) return;

    for (Node *row = col->dw; row != col; row = row->dw) {
        cur[row->x] = 1;

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

        cur[row->x] = 0;
        while (pos > last) rev();
    }
}

int main() {
    initialize();
    setup_dlx();
    dfs();

    return 0;
}
