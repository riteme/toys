#!/usr/bin/env pypy

from os import *
from sys import *
import io

def sh(x):
    assert not system(x)

if len(argv) < 3:
    print "%s [source] [data]" % argv[0]
    exit(-1)

def run():
    sh("./a.out < data.in > /dev/null 2> time.out")
    with io.open("time.out", "r") as reader:
        t = float(reader.read())
    return t

source, data = argv[1:]
sh("cp main.cpp /tmp")
sh("cp lct.h /tmp")
sh("cp %s.cpp /tmp/source.cpp" % source)
sh("cp data/%s.in /tmp/data.in" % data)
sh("cp data/%s.ans /tmp/data.ans" % data)
chdir("/tmp")
sh("clang++ -std=c++14 -O3 -Ofast -DNDEBUG main.cpp source.cpp")
sh("./a.out < data.in > a.ans 2> /dev/null")
sh("diff -Bb a.ans data.ans")

n1, n2 = 5, 100
for i in xrange(n1):
    print "(%s)" % run()
s = 0.0
for i in xrange(n2):
    t = run();
    s += t
    print t
print "average =", s / n2
