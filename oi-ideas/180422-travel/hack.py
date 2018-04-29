#!/usr/bin/env pypy

from os import system as sh
from sys import argv

n, m = argv[1:]

if sh("g++ std.cpp -std=c++11 -O3 -o a.out"):
    exit(-1)
if sh("g++ brute.cpp -std=c++11 -O3 -o b.out"):
    exit(-1)

cnt = 0
while True:
    cnt += 1
    print cnt
    sh("./gen.py %s %s > data.in" % (n, m))
    sh("./a.out < data.in > a.ans")
    sh("./b.out < data.in > b.ans")

    if sh("diff -Bb a.ans b.ans"):
        print "Hacked!"
        exit(-2)
