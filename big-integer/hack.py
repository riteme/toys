#!/usr/bin/python3

from os import *
from sys import *

def sh(x):
    # print('>', x)
    assert not system(x)

sh('cp BigInteger.cpp /tmp/a.cpp')
sh('cp gen.py /tmp')
chdir('/tmp')
sh('g++ a.cpp -O3 -fsanitize=address,undefined -DNOP')
cnt = 0
while True:
    cnt += 1
    print(cnt)
    sh(f'./gen.py {" ".join(argv[1:])} > data.in 2> b.ans')
    sh('./a.out < data.in > a.ans')
    sh('diff a.ans b.ans')