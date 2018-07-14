#!/usr/bin/env pypy

from sys import argv
from random import *

n, q, VMAX = map(int, argv[1:])

print n, q
print " ".join(map(str, [0 for i in xrange(n)]))
for i in xrange(q):
    op, l = randint(1, 2), randint(1, n)
    r = randint(l, n)
    if op == 1:
        print 1, l, r, randint(1, VMAX)
    else:
        print 2, l, r
