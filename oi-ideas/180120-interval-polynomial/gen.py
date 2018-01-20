#!/usr/bin/pypy

from sys import *
from random import *

N, Q, K, C = map(int, argv[1:])

print N, Q, K
OP = ['M'] * 7 + ['Q'] * 0
RANK = range(K)
for i in xrange(Q):
    op = choice(OP)
    l = randint(1, N)
    r = randint(l, N)

    if op == 'M':
        k = randint(1, K)
        c = sorted(sample(RANK, k))
        data = ['%s %s' % (randint(1, C), x) for x in c]
        cmd = 'M %s %s %s %s' % (l, r, k, ' '.join(data))
    else:
        cmd = 'Q %s %s' % (l, r)

    print cmd
