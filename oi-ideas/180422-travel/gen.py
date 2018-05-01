#!/usr/bin/env pypy

from sys import *
from random import *

n, m = map(int, argv[1:])
print n, m

for i in xrange(m):
    print randint(1, n), randint(1, n)
