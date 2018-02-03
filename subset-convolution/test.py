#!/usr/bin/env pypy

from math import sqrt

n = int(raw_input())
assert n & (n - 1) == 0, 'Argument "n" must be a power of 2.'

f = map(int, raw_input().split())
g = map(int, raw_input().split())

frac = 1 / sqrt(2)
w = frac + frac * 1j

def fwt(X):
    if len(X) == 1:
        return X

    m = len(X) >> 1
    A, B = fwt(X[:m]), fwt(X[m:])
    C = [A[i] + B[i] * w for i in xrange(m)]
    D = [A[i] - B[i] * w for i in xrange(m)]
    return C + D

def rfwt(X):
    if len(X) == 1:
        return X

    m = len(X) >> 1
    A, B = rfwt(X[:m]), rfwt(X[m:])
    C = [(A[i] + B[i]) * 0.5 for i in xrange(m)]
    D = [(A[i] - B[i]) / (2 * w) for i in xrange(m)]
    return C + D

f, g = fwt(f), fwt(g)
h = rfwt([f[i] * g[i] for i in xrange(n)])
print ' '.join(map(lambda x : str(int(x.real + 0.5)), h))
