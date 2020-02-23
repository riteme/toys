#!/usr/bin/pypy

from sys import *
from random import *

m, K = map(int, argv[1:])

n = 4 * m
idx = range(1, 4 * m + 1)
d = [0] * n
E = []

def link(u, v):
    global d
    global E

    E.append((u, v))
    d[u] += 1
    d[v] += 1

for i in xrange(0, n, 4):
    link(i, i + 1)
    link(i, i + 2)
    link(i + 1, i + 3)

for i in xrange(0, n, 4):
    for j in xrange(i + 4, n, 4):
        for x in range(0, 4):
            for y in range(0, 4):
                if not (i + 4 == j and x == 0 and y == 1):
                    link(i + x, j + y)

a = max(d[0], d[1], d[2], d[3] + 1)
for i in xrange(4, n):
    if i % 4 in [1, 3]:
        a = max(a, d[i] + 1)
    else:
        a = max(a, d[i])
a += K
# print 'a =', a

lp = n
n0 = n + 1
idx += [n0]
d += [0]

def feed(i, t):
    global d
    global idx
    global a
    global n0
    global lp

    while d[lp] >= a - 3:
        lp += 1
        while n0 <= lp:
            n0 += 1
            idx += [n0]
            d += [0]
    v = lp
    while d[i] < t:
        while n0 <= v:
            n0 += 1
            idx += [n0]
            d += [0]
        link(i, v)
        v += 1

feed(0, a)
feed(1, a)
feed(2, a)
feed(3, a - 1)
for i in xrange(4, n):
    t = a - 1 if i % 4 in [1, 3] else a
    feed(i, t)

shuffle(idx)
#idx = list(reversed(idx))
print 2 * a
for u, v in E:
    print idx[u], idx[v]
# print d, idx
