#!/usr/bin/env pypy

from os import system as sh

sh("g++ n3.cpp -O3 -std=c++11 -o n3.out")
sh("g++ naive.cpp -O3 -std=c++11 -o naive.out")

cnt = 0
while True:
    cnt += 1
    print cnt

    sh("./gen.py 100 1000000 > data.in")
    if sh("./n3.out < data.in > a.out") or sh("./naive.out < data.in > b.out") or sh("diff -Bb a.out b.out"):
        print "Hacked!"
        break