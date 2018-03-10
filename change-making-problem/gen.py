#!/usr/bin/env pypy

from sys import *
from random import *

n, CMAX = map(int, argv[1:])

print n
print '1 ' + ' '.join(map(str, (randint(3, CMAX) for i in xrange(n - 1))))