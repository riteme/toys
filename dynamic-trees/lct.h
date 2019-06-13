#ifndef LCT_H
#define LCT_H

#include <cassert>

#define NMAX (1048576*2)
#define ROOT 1

template <typename T>
inline void chkmax(T &a, const T &b) {
    if (a.mx < b.mx) {
        a.mx = b.mx;
        a.pos = b.pos;
    }
}

struct Allocator {
    Allocator(int n) {
        seq = new int[n];
    }

    ~Allocator() {
        delete[] seq;
    }

    int *seq, head = 0;

    void reset(int l, int r) {
        head = 0;
        for (int i = 0; l + i <= r; i++)
            seq[i] = l + i;
    }

    void release(int x) {
        assert(head);
        seq[--head] = x;
    }

    auto alloc() -> int {
        assert(head < NMAX);
        int r = seq[head++];
        return r;
    }
};

class LCT {
  public:
    void init(int n);
    auto splice(int x) -> int;
    void expose(int x);
    void link(int x, int y);
    void cut(int x);
    void fastcut(int x);
    void evert(int x);
    void set(int x, int v);
    auto query(int x, int y) -> int;
    auto get(int x) -> int;

    void debug();
};

#endif  // LCT_H
