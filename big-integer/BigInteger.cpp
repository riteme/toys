#include <cassert>
#include <cstdio>
#include <cctype>
#include <cstdint>
#include <cstring>

#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

#define USED __attribute__((used))

typedef uint32_t u32;
typedef uint64_t u64;
constexpr int TMAX = 1000000;
constexpr int CBYTES = sizeof(u32);
constexpr int CBITS = CBYTES * 8;
constexpr u32 MASK = 0xFFFFFFFFu;
static u32 T[TMAX], T0[TMAX];  // temporary storage, T0 for output
static u64 D[TMAX];  // for multiplication
struct Unsigned {
    // 构造函数
    Unsigned() : a(nullptr), len(0) {}
    Unsigned(u32 x) : a(new u32[1]), len(1) { *a = x; }
    Unsigned(int x) : Unsigned((u32) x) {}
    Unsigned(u64 x) : a(new u32[2]), len(2) { a[0] = x & MASK; a[1] = x >> CBITS; shrink(); }
    Unsigned(long long x) : Unsigned((u64) x) {}
    Unsigned(const char *s, bool hex = false) : a(nullptr), len(0) {
        if (hex) {
            if (s[0] == '0' && tolower(s[1]) == 'x') s += 2;
            int slen = strlen(s);
            realloc((slen * 4 + CBITS - 1) / CBITS);
            for (int i = slen - 1, j = 0, k = 0; i >= 0; i--, k += 4) {
                if (k >= CBITS) j++, k = 0;
                if (isdigit(s[i])) a[j] |= u32(s[i] - '0') << k;
                else a[j] |= (tolower(s[i]) - 'a' + 10u) << k;
            }
            shrink();
        } else {
            realloc(1);
            while (!isdigit(*s)) s++;
            for (int i = 0; isdigit(s[i]); i++)
                *this = (*this * 10u) + u32(s[i] - '0');
        }
    }
    Unsigned(const string &s) : Unsigned(s.c_str()) {}
    Unsigned(int _len, int _mem) : a(new u32[_len]), len(_len) { memset(a, _mem, CBYTES * len); }
    Unsigned(int _len, const Unsigned *z) : a(new u32[_len]), len(_len) { if (z) memcpy(a, z->a, CBYTES * z->len); }
    Unsigned(const Unsigned &z) : a(new u32[z.len]), len(z.len) { memcpy(a, z.a, CBYTES * len); }
    Unsigned(Unsigned &&z) : a(z.a), len(z.len) { z.a = nullptr; }
    Unsigned &operator=(const Unsigned &z) {
        if (a && z.len > len) {
            delete[] a;
            a = new u32[z.len];
        }
        len = z.len;
        memcpy(a, z.a, CBYTES * len);
        return *this;
    }
    Unsigned &operator=(Unsigned &&z) {
        if (a) delete[] a;
        a = z.a;
        len = z.len;
        z.a = nullptr;
        return *this;
    }
    ~Unsigned() { if (a) delete[] a; }
    // 结构体成员
    u32 *a; int len;
    // 基本操作
    bool valid() const { return a; }
    void shrink() { while (len > 1 && !a[len - 1]) len--; }
    void realloc(int _len) {
        u32 *b = new u32[_len];
        if (a) {
            memcpy(b, a, CBYTES * min(len, _len));
            delete[] a;
        }
        if (_len > len)
            memset(b + len, 0, CBYTES * (_len - len));
        a = b;
        len = _len;
    }
    void realloc() { realloc(len); }
    u32 operator[](int i) const { return a[i]; }
    u32 &operator[](int i) { return a[i]; }
    operator bool() const {
        for (int i = 0; i < len; i++)
            if (a[i]) return true;
        return false;
    }
    bool operator!() const { return !static_cast<bool>(*this); }
    // 位运算
    Unsigned &operator~() {
        for (int i = 0; i < len; i++)
            a[i] = ~a[i];
        return *this;
    }
    Unsigned &operator<<=(int k) {
        int p = k / CBITS, r = k % CBITS;
        int tlen = len + p + (r > 0);
        memset(T, 0, CBYTES * tlen);
        for (int i = 0; i < len; i++) {
            T[i + p] |= a[i] << r;
            if (r)  T[i + p + 1] = a[i] >> (CBITS - r);
        }
        if (tlen > len) realloc(tlen);
        memcpy(a, T, CBYTES * tlen);
        shrink();
        return *this;
    }
    Unsigned &operator>>=(int k) {
        int p = k / CBITS, r = k % CBITS;
        if (p >= len) {
            len = 1;
            a[0] = 0;
        } else {
            if (r) {
                for (int i = p; i < len - 1; i++)
                    a[i - p] = (a[i] >> r) | (a[i + 1] << (CBITS - r));
                a[len - p - 1] = a[len - 1] >> r;
            } else memmove(a, a + p, CBYTES * (len - p));
            len -= p + !a[len - p - 1];
        }
        return *this;
    }
    Unsigned operator<<(int k) const { return Unsigned(*this) <<= k; }
    Unsigned operator>>(int k) const { return Unsigned(*this) >>= k; }
    bool is_odd() const { return a[0] & 1; }
    // 比较运算。cmp: -1 <, 0 =, 1 >
    int cmp(const Unsigned &z) const {
        if (len < z.len) return -1;
        if (len > z.len) return 1;
        for (int i = len - 1; i >= 0; i--)
            if (a[i] != z[i]) return a[i] < z[i] ? -1 : 1;
        return 0;
    }
    bool operator==(const Unsigned &z) const { return len == z.len && memcmp(a, z.a, CBYTES * len) == 0; }
    bool operator<(const Unsigned &z) const { return cmp(z) < 0; }
    bool operator<=(const Unsigned &z) const { return cmp(z) <= 0; }
    bool operator>(const Unsigned &z) const { return cmp(z) > 0; }
    bool operator>=(const Unsigned &z) const { return cmp(z) >= 0; }
    // 小整型运算
    Unsigned &operator+=(u32 x) {
        u64 c = x;
        for (int i = 0; i < len; i++) {
            T[i] = (c += a[i]) & MASK;
            c >>= CBITS;
        }
        memcpy(a, T, CBYTES * len);
        if (c) {
            realloc(len + 1);
            a[len - 1] = c;
        }
        shrink();
        return *this;
    }
    Unsigned operator+(u32 x) const { return Unsigned(*this) += x; }
    Unsigned &operator-=(u32 x) {
        if (x == 0) return *this;
        u64 c = -x;
        for (int i = 0; i < len; i++) {
            c += (u64) a[i] + (i ? MASK : 0);
            a[i] = c & MASK;
            c >>= CBITS;
        }
        shrink();
        return *this;
    }
    Unsigned operator-(u32 x) const { return Unsigned(*this) -= x; }
    Unsigned &operator*=(u32 x) {
        u64 c = 0;
        for (int i = 0; i < len; i++) {
            c += (u64) a[i] * x;
            T[i] = c & MASK;
            c >>= CBITS;
        }
        memcpy(a, T, CBYTES * len);
        if (c) {
            realloc(len + 1);
            a[len - 1] = c;
        }
        shrink();
        return *this;
    }
    Unsigned operator*(u32 x) const { return Unsigned(*this) *= x; }
    void divide(u32 z, Unsigned *q, u32 *r) const {
        assert(z);
        u64 c = 0;
        for (int i = len - 1; i >= 0; i--) {
            c = (c << CBITS) + a[i];
            if (z <= c) {
                if (q) T[i] = c / z;
                c %= z;
            } else if (q) T[i] = 0;
        }
        if (q) {
            if (!q->valid()) q->realloc(len);
            memcpy(q->a, T, CBYTES * len);
            q->len = len;
            q->shrink();
        }
        if (r) *r = c;
    }
    Unsigned operator/(u32 x) const {
        Unsigned t(len, nullptr);
        divide(x, &t, nullptr);
        return t;
    }
    u32 operator%(u32 x) const {
        divide(x, nullptr, &x);
        return x;
    }
    Unsigned &operator/=(u32 x) {
        divide(x, this, nullptr);
        return *this;
    }
    Unsigned &operator%=(u32 x) {
        divide(x, nullptr, &x);
        return *this = x;
    }
    // 大整型运算
    Unsigned &operator+=(const Unsigned &z) {
        int tlen = max(len, z.len);
        u64 c = 0;
        for (int i = 0; i < tlen; i++) {
            c += (u64) (i < len ? a[i] : 0) + (i < z.len ? z[i] : 0);
            T[i] = c & MASK;
            c >>= CBITS;
        }
        if (c) T[tlen++] = c;
        if (tlen > len) realloc(tlen);
        memcpy(a, T, CBYTES * tlen);
        shrink();
        return *this;
    }
    Unsigned operator+(const Unsigned &z) const { return Unsigned(*this) += z; }
    Unsigned &operator-=(const Unsigned &z) {
        if (a == z.a) *this = Unsigned(0);
        else {
            assert(*this >= z);
            if (!z) return *this;
            u64 c = 1;
            for (int i = 0; i < len; i++) {
                c += (u64) a[i] + (i < z.len ? ~z[i] : MASK);
                a[i] = c & MASK;
                c >>= CBITS;
            }
            shrink();
        }
        return *this;
    }
    Unsigned operator-(const Unsigned &z) const { return Unsigned(*this) -= z; }
    Unsigned &operator*=(const Unsigned &z) {
        // CAUTION: ensure len < 2^32 to avoid overflow
        int tlen = len + z.len;
        memset(D, 0, sizeof(u64) * tlen);
        for (int i = 0; i < len; i++) for (int j = 0; j < z.len; j++) {
            int k = i + j;
            D[k] += (u64) a[i] * z[j];
            if (D[k] >> CBITS) {
                D[k + 1] += D[k] >> CBITS;
                D[k] &= MASK;
            }
        }
        realloc(tlen);
        for (int i = 0; i < tlen; i++) a[i] = D[i];
        shrink();
        return *this;
    }
    Unsigned operator*(const Unsigned &z) const { return Unsigned(*this) *= z; }
    void divide(const Unsigned &z, Unsigned *q, Unsigned *r) const {
        assert(z);
        if (*this < z) {
            if (q) *q = Unsigned(0);
            if (r) *r = *this;
        } else {
            assert(len >= z.len && z[z.len - 1]);
            int lz = CBITS * (len - z.len) + __builtin_clz(z[z.len - 1]);
            int qlen = lz / CBITS + 1;
            // if (q) *q = Unsigned(lz / CBITS + 1, 0);
            Unsigned d = z << lz, R = *this;
            memset(T, 0, CBYTES * qlen);
            for (int s = lz; s >= 0; s--, d >>= 1) {
                int i = s / CBITS, j = s % CBITS;
                if (d <= R) {
                    R -= d;
                    T[i] |= 1u << j;
                }
            }
            if (q) {
                if (!q->valid()) q->realloc(qlen);
                memcpy(q->a, T, CBYTES * qlen);
                q->len = qlen;
                q->shrink();
            }
            if (r) *r = move(R);
        }
    }
    Unsigned operator/(const Unsigned &z) const {
        Unsigned t;
        divide(z, &t, nullptr);
        return t;
    }
    Unsigned operator%(const Unsigned &z) const {
        Unsigned t;
        divide(z, nullptr, &t);
        return t;
    }
    Unsigned &operator/=(const Unsigned &z) {
        divide(z, this, nullptr);
        return *this;
    }
    Unsigned &operator%=(const Unsigned &z) {
        divide(z, nullptr, this);
        return *this;
    }
    // 其它操作
    int popcount() const {
        int ret = 0;
        for (int i = 0; i < len; i++)
            ret += __builtin_popcount(a[i]);
        return ret;
    }
    int ctz() const {
        int ret = 0, i;
        for (i = 0; i < len && !a[i]; i++)
            ret += CBITS;
        return ret + (i < len ? __builtin_ctz(a[i]) : 0);
    }
    int log() const {
        assert(a);
        return CBITS * len - __builtin_clz(a[len - 1]) - 1;
    }
    Unsigned sqrt() const { }  // TODO: integer sqrt
    // 输出
    USED void print() const {
        printf("[len = %d, %lx]\n", len, (unsigned long) a);
        for (int i = len - 1; i >= 0; i--) {
            for (int j = CBYTES - 1; j >= 0; j--) {
                for (int k = 8 * (j + 1) - 1; k >= 8 * j; k--)
                    putchar(((a[i] >> k) & 1 ? '1' : '0'));
                if (j) putchar(' ');
            }
            printf("    [0x%X, %u]\n", a[i], a[i]);
        }
        printf("hex = ");
        print_hex();
        putchar('\n');
    }
    USED void print_dec() const {
        Unsigned t = *this, q;
        int i = 0; u32 r;
        for ( ; t; i++) {
            t.divide(10u, &q, &r);
            T0[i] = r;
            t = move(q);
        }
        for (i--; i >= 0; i--)
            printf("%1u", T0[i]);
    }
    void _print_dec(ostream &os) const {
        Unsigned t = *this, q;
        int i = 0; u32 r;
        for ( ; t; i++) {
            t.divide(10u, &q, &r);
            T0[i] = r;
            t = move(q);
        }
        for (i--; i >= 0; i--)
            os << T0[i];
    }
    USED void print_hex() const {
        printf("0x");
        for (int i = len - 1; i >= 0; i--)
            printf(i == len - 1 ? "%X" : "%.8X", a[i]);
    }
    friend ostream &operator<<(ostream &os, const Unsigned &z);
};
ostream &operator<<(ostream &os, const Unsigned &z) {
    auto flags = os.flags();
    if (flags & os.dec) z._print_dec(os);
    else os << "(unavailable)";  // TODO: hex & oct output for ostream
    return os;
}

// TODO: "Signed" adaptor

int main() {
    char op[8], buf[1024];
    // scanf("%s", buf);
    cin >> buf;
    // Unsigned u = atoll(buf);
    Unsigned u(buf);
    // u.print();
    while (cin >> op >> buf) {
        // printf("⇒ %s %s\n", op, buf);
        Unsigned x(buf);
        // u32 x = strtol(buf, NULL, 10);
        // x.print_hex();
        switch (op[0]) {
            case '+': u += x; break;
            case '-': u -= x; break;
            case '*': u *= x; break;
            case '/': u /= x; break;
            case '%': cout << u % x << endl; continue;
            // case '%': printf("0x%X\n", u % x); continue;
        }
        cout << u << endl;
        // u.print_hex();
        // puts("");
    }
    return 0;
}