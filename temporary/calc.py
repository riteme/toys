#!/usr/bin/env pypy

from os import *
from sys import *

sh = system
N, P = map(int, argv[1:])

sh('echo -n > data.out')
for n in xrange(2, N + 1, 50):
    print n, P / n
    sh('./a.out %s %s 8 >> data.out' % (n, P / n))
