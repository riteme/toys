#!/usr/bin/pypy

from sys import *
from random import *

n = int(argv[1])
S = "BCDFGTVXYZ"
print ''.join(choice(S) for i in xrange(n))
