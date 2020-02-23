#!/usr/bin/python

from sys import *
from random import *

n, m = map(int, argv[1:])
idx = range(1, n * m + 1)
shuffle(idx)
M = []
for x in xrange(n):
    M.append([])
    for y in xrange(m):
        i = x * m + y
        M[-1].append(idx[i])

print 8
for x in xrange(n):
    for y in xrange(m):
        for dx, dy in [(1, 0), (0, 1)]:
            tx, ty = x + dx, y + dy
            if 0 <= tx < n and 0 <= ty < m:
                print M[x][y], M[tx][ty]