#!/usr/bin/env python3

from os import *
from sys import *
from collections import defaultdict
import io

BLOCK = 512
TARGETS = ["spaly", "splay", "locally-biased", "treap"]

def sh(x):
    print(f"→ {x}")
    assert not system(x)

if len(argv) < 7:
    print("Usage: %s [T] [S] [Nmin] [Nmax] [K] [Cmax] [gen] [args...]" % argv[0])
    exit(-1)

def compile(x):
    sh(f"clang++ -std=c++14 main.cpp {x}.cpp -O3 -Ofast -DNDEBUG -o {x} -Wno-macro-redefined")

def readtime():
    with io.open("data.err", "r") as reader:
        return float(reader.read())

T, S, Nmin, Nmax, K, Cmax = map(int, argv[1:7])
gen = argv[7]
args = argv[8:]
sh(f"cp {gen} /tmp/gen")
sh("cp kruskal.cpp /tmp/kruskal.cpp")
sh("cp main.cpp /tmp")
sh("cp lct.h /tmp")
for x in TARGETS:
    sh(f"cp {x}.cpp /tmp/{x}.cpp")

chdir("/tmp")
sh(f"clang++ -std=c++14 kruskal.cpp -O3 -Ofast -DNDEBUG -o kruskal")
for x in TARGETS:
    compile(x)

for i in range(Nmin, Nmax + 1):
    print("")
    n = 1 << i
    m = n * K * BLOCK
    print(f"### log n = {i} (n = {n})")
    cnt = defaultdict(lambda: 0.0)
    for t in range(T):
        sh(f"./gen {n} {m} {Cmax} {' '.join(args)} > data.in")
        sh(f"./kruskal {S // m + 1} < data.in > data.ans 2> data.err")
        tm = readtime()
        print(f"= {tm}")
        cnt["kruskal"] += tm
        for x in TARGETS:
            sh(f"./{x} {S // m + 1} < data.in > data.out 2> data.err")
            tm = readtime()
            print(f"= {tm}")
            cnt[x] += tm
            sh("diff -Bb data.out data.ans")
    with io.open("result", "a") as fp:
        fp.write(f"# log n = {i}\n")
        for key, val in cnt.items():
            ret = f"{key:>15}: {val / T:.3g}"
            print(ret)
            fp.write(ret + '\n')