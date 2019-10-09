#include <cassert>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdint>

#include <algorithm>

using namespace std;

#define USED __attribute__((used))

typedef uint32_t u32;
typedef uint64_t u64;

struct Integer {
    static constexpr int MAXLEN = 100;
    static constexpr int CBYTES = sizeof(u32);
    static constexpr int CBITS = CBYTES * 8;
    static constexpr u32 MASK = 0xFFFFFFFF;

    Integer(int x) : a((u32*) malloc(CBYTES)), t(0) { *a = x; }
    Integer() : a((u32*) malloc(CBYTES)), t(0) { *a = 0; }
    ~Integer() { if (a) free(a); }

    Integer(const Integer &z) : a((u32*) malloc(z.byte_cnt())), t(z.t) {
        memcpy(a, z.a, byte_cnt());
    }
    Integer(Integer &&z) : t(z.t) {
        if (a) free(a);
        a = z.a;
        z.a = NULL;
    }
    Integer &operator=(const Integer &z) {
        a = (u32*) realloc(a, z.byte_cnt());
        t = z.t;
        memcpy(a, z.a, byte_cnt());
        return *this;
    }
    Integer &operator=(Integer &&z) {
        if (a) free(a);
        a = z.a;
        z.a = NULL;
        t = z.t;
        return *this;
    }

    u32 *a;
    int t;  // t means the highest index of the array a.

    USED int chunk_cnt() const { return t + 1; }
    USED int byte_cnt() const { return chunk_cnt() * CBYTES; }
    USED int bit_cnt() const { return chunk_cnt() * CBITS; }
    USED int msbit() const { return bit_cnt() - 1; }
    USED bool sign() const { return a[t] >> 31; }

    USED bool get(int i) const {
        assert(0 <= i && i < bit_cnt());
        int j = i / CBITS, k = i % CBITS;
        assert(j < chunk_cnt());
        return a[j] & (1 << k);
    }

    // Memory Managements
    USED void shrink() {
        int s = sign(), i = msbit();
        while (i > 0 && get(i - 1) == s)
            i--;
        t = i / CBITS;
        a = (u32*) realloc(a, byte_cnt());
    }

    USED void truncate(int pw) {
        assert(pw < msbit() && pw >= 0);
        t = pw / CBITS;
        //shrink();
    }

    USED void extend(int pw) {
        assert(pw > msbit());
        int _t = pw / CBITS;
        if (_t > t) {
            a = (u32*) realloc(a, (_t + 1) * CBYTES);
            if (sign()) memset(a + t + 1, 0xFF, (_t - t) * CBYTES);
            else memset(a + t + 1, 0, (_t - t) * CBYTES);
            t = _t;
        }
    }

    USED void resize(int pw) {
        if (pw > msbit()) extend(pw);
        else if (pw < msbit()) truncate(pw);
    }

    // Bit Manipulations
    USED void rev() {
        for (int i = 0; i <= t; i++)
            a[i] = ~a[i];
    }

    USED void band(const Integer &z) {
        int p = min(t, z.t);
        for (int i = 0; i <= p; i++)
            a[i] &= z.a[i];
        u32 mask = -u32(z.sign());
        for (int i = p + 1; i <= t; i++)
            a[i] &= mask;
    }

    USED void bor(const Integer &z) {
        int p = min(t, z.t);
        for (int i = 0; i <= p; i++)
            a[i] |= z.a[i];
        u32 mask = -u32(z.sign());
        for (int i = p + 1; i <= t; i++)
            a[i] |= mask;
    }

    USED void bxor(const Integer &z) {
        int p = min(t, z.t);
        for (int i = 0; i <= p; i++)
            a[i] ^= z.a[i];
        u32 mask = -u32(z.sign());
        for (int i = p + 1; i <= t; i++)
            a[i] ^= mask;
    }

    USED bool to_bool() const {
        for (int i = 0; i <= t; i++)
            if (a[i]) return true;
        return false;
    }
    USED bool lnot() const { return !to_bool(); }
    USED bool is_zero() const { return lnot(); }

    USED void lshft(int k) {
        int p = k / CBITS, r = k % CBITS, s = sign();
        a = (u32*) realloc(a, (t + p + 2) * CBYTES);
        if (p) memmove(a + p, a, (t + 1) * CBYTES);
        t += p + 1;
        memset(a, 0, p * CBYTES);
        a[t] = s ? MASK << r : 0;
        if (r) {
            u32 mask = ((1 << r) - 1) << (CBITS - r);
            for (int i = t - 1; i >= p; i--) {
                a[i + 1] |= (a[i] & mask) >> (CBITS - r);
                a[i] <<= r;
            }
        }
        shrink();
    }

    USED void rshft(int k, bool arithmetic = false) {
        int p = k / CBITS, r = k % CBITS;
        if (p > t) {
            a = (u32*) realloc(a, CBYTES);
            *a = 0;
        } else {
            if (p) memmove(a, a + p, (t - p + 1) * CBYTES);
            t -= p;
            u32 mask = (1 << r) - 1;
            if (r) {
                for (int i = 0; i <= t; i++) {
                    a[i] >>= r;
                    if (i < t) a[i] |= (a[i + 1] & mask) << (CBITS - r);
                    else if (arithmetic) a[i] |= mask << (CBITS - r);
                }
            }
            shrink();
        }
    }

    // Arithmetic Operations
    USED void neg() {
        if (is_zero()) return;
        rev();
        int i;
        for (i = 0; a[i] == MASK; i++) {
            assert(i < t);
            a[i] = 0;
        }
        a[i]++;
    }

    USED void add(const Integer &z, bool fixed = false) {
        assert(&z != this);
        if (!fixed) extend(max(msbit(), z.msbit()) + 1);
        u32 c = 0, c0 = 0, s = -u32(z.sign());
        for (int i = 0; i <= t; i++) {
            u32 v = a[i] + (i <= z.t ? z.a[i] : s);
            c0 = v < a[i];
            v += c;
            c = v < c || c0;
            a[i] = v;
        }
        if (!fixed) shrink();
    }

    USED void sub(const Integer &z) {
        assert(&z != this);
        auto tmp = z;
        tmp.neg();
        add(tmp);
    }

    USED void mul(u64 x) {
        assert(!(x >> 32));
        extend((t + 2) * CBITS - 1);
        u64 c = 0;
        for (int i = 0; i <= t; i++) {
            c = a[i] * x + c;
            a[i] = c & MASK;
            c >>= CBITS;
        }
        shrink();
    }

    USED void mul(const Integer &z) {
        static u64 tmp[MAXLEN];
        assert(&z != this);
        if (z.is_zero()) *this = Integer();
    }

    USED void div(u64 x) {
        assert(!(x >> 32));
    }

    USED void div(const Integer &z) {
        assert(&z != this);
    }

    // DEBUG
    USED void print() const {
        printf("[t+1 = %d, %s]\n", t + 1, sign() ? "-" : "+");
        for (int i = t; i >= 0; i--) {
            for (int j = 3; j >= 0; j--) {
                for (int k = 8 * (j + 1) - 1; k >= 8 * j; k--)
                    putchar((a[i] >> k) & 1 ? '1' : '0');
                if (j) putchar(' ');
            }
            printf("   [0x%X, %u]\n", a[i], a[i]);
        }
    }
};

int main() {
    Integer v[100], u[100];
    for (int i = 0; i < 100; i++) {
        v[i] = Integer(i);
        u[i] = Integer(-i);
    }
    v[23].lshft(27);
    v[27].lshft(27);
    //v[23].print();
    //v[27].print();
    v[23].add(v[27]);
    //v[23].print();
    v[23].mul(v[27]);
    //v[23].print();
    v[27].mul(v[23]);
    //v[27].print();
    v[0] = v[27];
    v[0].neg();
    v[0].print();
    v[27].sub(v[1]);
    v[27].print();
    v[27].mul(v[0]);
    v[27].print();
    v[27].neg();
    v[27].print();
    return 0;
}
