#!/usr/bin/env pypy

from os import *

def sh(x):
    assert not system(x)

sh("cp ./brute.cpp /tmp")
sh("cp ./bit.cpp /tmp")
sh("cp ./gen.py /tmp")
chdir("/tmp")
sh("g++ brute.cpp -O3 -o b.out")
sh("g++ bit.cpp -O3")

cnt = 0
while True:
    cnt += 1
    print cnt
    sh("./gen.py 100000 100000 1000000000 > data.in")
    sh("./a.out < data.in > a.ans")
    sh("./b.out < data.in > b.ans")
    sh("diff -Bb a.ans b.ans")
