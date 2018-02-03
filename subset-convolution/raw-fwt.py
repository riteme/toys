#!/usr/bin/env pypy

n = int(raw_input())
assert n & (n - 1) == 0, 'Argument "n" must be a power of 2.'

f = map(int, raw_input().split())
g = map(int, raw_input().split())

def prefix(X):
    if len(X) == 1:
        return X

    m = len(X) >> 1
    A, B = prefix(X[:m]), prefix(X[m:])
    for i in xrange(m):
        A.append(A[i] + B[i])
    return A

def unprefix(X):
    if len(X) == 1:
        return X

    m = len(X) >> 1
    A, B = unprefix(X[:m]), unprefix(X[m:])
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

F = prefix(f)
G = prefix(g)
F1 = sub(F, f)
G1 = sub(G, g)
mu = [(-1)**cnt(x) for x in xrange(n)]

A = prefix(mu)
B = prefix(mul(mu, F1))
C = prefix(mul(mu, G1))
D = prefix(mul(mu, mul(F1, G1)))

H = [F[s] * G[s] * A[s] - G[s] * B[s] - F[s] * C[s] + D[s] for s in xrange(n)]

show(H)
show(unprefix(H))
