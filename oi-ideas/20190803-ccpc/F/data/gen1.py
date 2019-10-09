#!/usr/bin/pypy

from sys import *
from random import *

n = int(argv[1])
print 3

# empty graph
print n, 0

# tree
print n, n - 1
L = range(1, n + 1)
shuffle(L)
E = []
for i in xrange(1, n):
    E.append((randrange(max(0, i - 15), i), i))
print '\n'.join("%s %s" % (L[u], L[v]) for u, v in E)

# cycle
print n, n
shuffle(L)
for i in xrange(0, n):
    print L[i], L[(i + 1) % n]
