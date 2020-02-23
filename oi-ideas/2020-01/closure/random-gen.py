#!/usr/bin/python

from sys import *
from random import *

n, m = map(int, argv[1:])
for i in xrange(m):
    u = randint(1, n - 1)
    v = randint(u + 1, n)
    print u, v