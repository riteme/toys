#!/usr/bin/env pypy

from sys import argv
from math import *

import animate

w = int(argv[1])
h = int(argv[2])
L = map(int, argv[3:])
n = w * h

def position(x):
    x -= 1
    return (x % w + 1, h - x / w)

animate.reset(w + 1, h + 1)

for i in xrange(n):
    x, y = position(L[i])
    animate.push("Disk[{%s, %s}, 0.05]" % (x, y))

animate.save()
animate.submit()

animate.push("Circle[{%s, %s}, 0.2]" % position(L[0]))
animate.save()
animate.submit()

for i in xrange(1, n):
    animate.push("Circle[{%s, %s}, 0.2]" % position(L[i]))
    animate.save()

    animate.push("Red")
    animate.push("Thick")
    animate.push("Arrowheads[0.1]")
    animate.push("Arrow[{{%s, %s}, {%s, %s}}]" % (position(L[i - 1]) + position(L[i])))
    animate.submit()

    animate.push("Line[{{%s, %s}, {%s, %s}}]" % (position(L[i - 1]) + position(L[i])))
    animate.save()

animate.submit()
print animate.export()
