#!/usr/bin/pypy

from sys import *
from random import *

n, c, core = map(int, argv[1:])

print 3
for T in xrange(3):
    P = range(1, n + 1)
    shuffle(P)
    L = sorted(sample(range(1, n - 1), c - 1)) + [n]
    G = []
    s = 0
    for t in L:
        E = []
        M = []
        for i in xrange(1, min(t - s, core)):
            E.append((randrange(max(0, i - 15), i), i))
            M.append(0)
        for i in xrange(core, t - s):
            j = randrange(0, len(E))
            u, v = E[j]
            E.append((i, v))
            M.append(1)
            if M[j]:
                E[j] = (u, i)
            else:
                M[j] = 1
                E.append((u, i))
                M.append(1)
        #G += ["%s %s" % (s + u, s + v) for u, v in E]
        G += ["%s %s" % (P[s + u], P[s + v]) for u, v in E]
        s = t
    print n, len(G)
    stderr.write("len(G) = " + str(len(G)) + '\n')
    shuffle(G)
    print '\n'.join(G)
