#!/usr/bin/pypy

from sys import *
from random import *

n, m, C = map(int, argv[1:])

P = range(1, n + 1)
shuffle(P)
print n, m, P[0]
for i in xrange(1, n):
    u = P[randint(0, i - 1)]
    print u, P[i], randint(1, C)
for i in xrange(n - 1, m):
    print randint(1, n), randint(1, n), randint(1, C)
