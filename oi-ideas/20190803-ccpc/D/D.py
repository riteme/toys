#!/usr/bin/python3

from gmpy2 import *

get_context().precision = 10000
for i in range(1, 101):
    print(0, 1 / mpfr(i))
