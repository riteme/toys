#ifndef LCT_H
#define LCT_H

#define NMAX 400000
#define ROOT 1

class LCT {
  public:
    auto splice(int x) -> int;
    void expose(int x);
    void link(int x, int y);
    void cut(int x);
    void fastcut(int x);
    void evert(int x);
    void set(int x, int v);
    void toggle(int x);
    auto count(int x, int y) -> int;
};

#endif  // LCT_H
