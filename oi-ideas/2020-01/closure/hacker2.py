#!/usr/bin/python

from sys import *
from random import *

NMAX = 5000

n = int(argv[1])
K = 2 * n - 5

idx = range(1, NMAX + 1)
#idx = range(NMAX, 0, -1)
shuffle(idx)

idx = [0] + idx
print K
for u in xrange(1, n + 1):
    for v in xrange(u + 2, n + 1):
        print idx[u], idx[v]

d = [randint(0, 2) for i in xrange(NMAX + 1)]
for u in xrange(n + 1, NMAX + 1):
    for v in xrange(u + 1, NMAX + 1):
        if d[u] >= n - 5:
            break
        if d[v] >= n - 5:
            continue
        print idx[u], idx[v]
        d[u] += 1
        d[v] += 1
