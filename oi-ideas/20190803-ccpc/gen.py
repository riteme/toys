#!/usr/bin/pypy

from sys import *
from random import *

T, n, p, K0 = map(int, argv[1:]);

print T
for i in xrange(T):
    E = []
    mark = []
    for i in xrange(2, n * p / 100 + 1):
        E.append((randint(1, i - 1), i))
        mark.append(0)
    for i in xrange(n * p / 100 + 1, n + 1):
        j = randrange(0, len(E))
        u, v = E[j]
        if not mark[j]:
            E.append((u, i))
            mark.append(1)
            mark[j] = 1
        else:
            E[j] = (u, i)
        E.append((i, v))
        mark.append(1)
    shuffle(E)
    print n, len(E), randint(0, K0)
    print '\n'.join(map(lambda x: "%s %s" % x, E))
