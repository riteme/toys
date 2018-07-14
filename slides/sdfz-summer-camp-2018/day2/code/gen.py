#!/usr/bin/env pypy

from sys import argv
from random import *

n = int(argv[1])
m = int(argv[2])
p = float(argv[3])

print n, m
for i in xrange(n):
    print "".join([('.', '1')[random() < p] for j in xrange(m)])
