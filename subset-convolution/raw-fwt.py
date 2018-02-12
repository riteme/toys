#!/usr/bin/env pypy

n = int(raw_input())
assert n & (n - 1) == 0, 'Argument "n" must be a power of 2.'

f = map(int, raw_input().split())
g = map(int, raw_input().split())

def fwt(X):
    if len(X) == 1:
        return X

    m = len(X) >> 1
    A, B = fwt(X[:m]), fwt(X[m:])
    for i in xrange(m):
        A.append(A[i] + B[i])
    return A

def rfwt(X):
    if len(X) == 1:
        return X

    m = len(X) >> 1
    A, B = rfwt(X[:m]), rfwt(X[m:])
    for i in xrange(m):
        A.append(B[i] - A[i])
    return A

def cnt(x):
    r = 0
    while x:
        x ^= x & (-x)
        r += 1
    return r

def add(A, B):
    return [A[x] + B[x] for x in xrange(len(A))]

def sub(A, B):
    return [A[x] - B[x] for x in xrange(len(A))]

def mul(A, B):
    return [A[x] * B[x] for x in xrange(len(A))]

def show(X):
    print ' '.join(map(str, X))

k = cnt(n - 1) + 1
F = [[0] * n for i in xrange(k)]
G = [[0] * n for i in xrange(k)]
H = [[0] * n for i in xrange(k)]

for i in xrange(n):
    c = cnt(i)
    F[c][i] = f[i]
    G[c][i] = g[i]

for i in xrange(k):
    F[i] = fwt(F[i])
    G[i] = fwt(G[i])

for i in xrange(k):
    for j in xrange(k - i):
        print i, j
        H[i + j] = add(H[i + j], mul(F[i], G[j]))

for i in xrange(k):
    H[i] = rfwt(H[i])

R = [0] * n
for i in xrange(n):
    c = cnt(i)
    R[i] = H[c][i]

show(R)