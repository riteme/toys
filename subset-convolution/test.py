#!/usr/bin/env pyp

from math import *

def readseq():
    return map(int, raw_input().split())

def mul(A, B):
    return [A[i] * B[i] for i in xrange(len(A))]

def add(A, B):
    return [A[i] + B[i] for i in xrange(len(A))]

def sub(A, B):
    return [A[i] - B[i] for i in xrange(len(A))]

def _fwt(X, T):
    n = len(X)

    if n == 1:
        return X

    m = n >> 1

    A = _fwt(X[:m], T)
    B = _fwt(X[m:], T)
    C = [0] * n
    for i in xrange(m):
        C[i] = A[i] * T[0][0] + B[i] * T[0][1]
        C[i + m] = A[i] * T[1][0] + B[i] * T[1][1]

    return C

def fwt(A, B, T, Tinv):
    X = _fwt(A, T)
    Y = _fwt(B, T)
    X = mul(X, Y)
    return _fwt(X, Tinv)

def show(X):
    print ' '.join(map(str, X))

s5 = sqrt(5)

Tor = [[1, 0], [1, 1]]
Torinv = [[1, 0], [-1, 1]]
Txor = [[1, 1], [1, -1]]
Txorinv = [[0.5, 0.5], [0.5, -0.5]]
Tall = [[1, (1 + s5) / 2], [1, (1 - s5) / 2]]
Tallinv = [
    [-s5 / 10 + 0.5, s5 / 10 + 0.5],
    [s5 / 5, -s5 / 5]
]

n = int(raw_input())
F = readseq()
G = readseq()
Hor = fwt(F, G, Tor, Torinv)
Hxor = fwt(F, G, Txor, Txorinv)
Hall = fwt(F, G, Tall, Tallinv)

H = sub(add(Hor, Hxor), Hall)
show(H)
