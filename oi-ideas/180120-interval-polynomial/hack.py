#!/usr/bin/env pypy

from os import *
from shutil import *

sh = system

copy('brute.cpp', '/tmp/brute.cpp')
copy('main.cpp', '/tmp/main.cpp')
copy('gen.py', '/tmp/gen.py')

chdir('/tmp')
if sh('g++ brute.cpp -O3 -o brute'):
    exit(-1)
if sh('g++ main.cpp -std=c++11 -O3 -o main'):
    exit(-1)

cnt = 0
while True:
    cnt += 1
    print cnt
    sh('./gen.py 100 500 100 1000000000 > data.in')
    sh('./brute < data.in > a.out')
    sh('./main < data.in > b.out')

    if sh('diff -Bb a.out b.out'):
        print 'Hacked!'
        break
