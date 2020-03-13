#include <stdio.h>
#include <stdbool.h>

#define S 110

typedef unsigned int u32;

u32 pc, pr[S];
u32 phi[S];

u32 mul(u32 x, u32 y) {
    u32 r = 0;
    for ( ; x; y <<= 1, x >>= 1)
        if (x & 1)
            r += y;
    return r;
}

void divmod(u32 n, u32 m, u32 *q, u32 *r) {
    if (m > n) {
        *q = 0;
        *r = n;
        return;
    }

    u32 t = 1;
    while (m <= n) {
        m <<= 1;
        t <<= 1;
    }
    m >>= 1;
    t >>= 1;

    for ( ; t; t >>= 1) {
        if (m <= n) {
            n -= m;
            *q |= t;
        }
        m >>= 1;
    }
    *r = n;
}

u32 div(u32 n, u32 m) {
    u32 q = 0, r = 0;
    divmod(n, m, &q, &r);
    return q;
}

u32 mod(u32 n, u32 m) {
    u32 q = 0, r = 0;
    divmod(n, m, &q, &r);
    return r;
}

u32 c2(u32 n) {
    return n & 1 ? mul((n + 1) >> 1, n) : mul(n >> 1, n + 1);
}

u32 N;
u32 has[S];
u32 mem[S];
u32 F(u32 n) {
    if (n < S) return phi[n];
    u32 pos = div(N, n);
    if (has[pos]) return mem[pos];
    has[pos] = true;

    u32 r = c2(n);
    for (u32 i = 2, l; i <= n; i = l + 1) {
        u32 p = div(n, i);
        l = div(n, p);
        r -= mul(l - i + 1, F(p));
    }

    return mem[pos] = r;
}

void sieve() {
    phi[1] = 1;
    for (u32 i = 2; i < S; i++) {
        if (!phi[i]) {
            pr[pc++] = i;
            phi[i] = i - 1;
        }

        for (u32 j = 0; mul(pr[j], i) < S; j++) {
            u32 p = pr[j];
            u32 t = mul(i, p);
            if (mod(i, p))
                phi[t] = mul(phi[i], (p - 1));
            else {
                phi[t] = mul(phi[i], p);
                break;
            }
        }
    }

    for (u32 i = 2; i < S; i++)
        phi[i] += phi[i - 1];
}

int main() {
    sieve();
    scanf("%u", &N);
    printf("%u\n", F(N));
    return 0;
}