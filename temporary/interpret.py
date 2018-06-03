#!/usr/bin/env pypy

from sys import argv
from math import *

w = int(argv[1])
h = int(argv[2])
L = map(int, argv[3:])
n = w * h

def position(x):
    x -= 1
    return (x % w + 1, h - x / w)

G = []

G.append("Thickness[0.005]")
for i in xrange(n - 1):
    x1, y1 = position(L[i])
    x2, y2 = position(L[i + 1])
    G.append("ColorData[\"DarkRainbow\"][%s]" % (float(i + 1) / n))
    G.append("Arrow[{{%s, %s}, {%s, %s}}]" % (x1, y1, x2, y2))

G.append("Black")
for i in xrange(n):
    x, y = position(L[i])
    G.append("Disk[{%s, %s}, 0.05]" % (x, y))

G.append("Red")
for i in xrange(n):
    x, y = position(i + 1)
    G.append("Text[Style[%s, Bold, FontSize -> 14], {%s, %s}]" % (i + 1, x + 0.09, y - 0.1))

print "Graphics[{%s}, PlotRange -> {{0.8, %s + 0.2}, {0.8, %s + 0.2}}]" % (", ".join(G), w, h)
