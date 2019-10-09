#!/usr/bin/env pypy

from random import *
from sys import argv

n, m, c = map(int, argv[1:])
print n, m, randint(1, n)
for i in xrange(m):
    print randint(1, n), randint(1, n), randint(1, c)
