#!/usr/bin/python

from sys import *
from random import *

K, C, M = map(int, argv[1:])
E = []
m = K + 1
cnt = [0]
cov = [m]
full = 0
for i in xrange(C + 1, K + 1):
    while cnt[full] >= C:
        full += 1
    for j in xrange(i):
        if len(cnt) == full + j:
            m += 1
            cnt.append(0)
            cov.append(m)
        E.append((i, cov[full + j]))
        cnt[cov[full + j] - K - 1] += 1

for i in xrange(M / 2):
    E.append((randint(m + 1, m + M), randint(m + 1, m + M)))
m += M

#idx = range(1, m - C + 1)
idx = range(m - C, 0, -1)
shuffle(idx)

print 2 * K - 1
for u, v in E:
    print idx[u - C - 1], idx[v - C - 1]
