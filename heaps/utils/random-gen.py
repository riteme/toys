#!/usr/bin/env python
# coding=utf-8

import argparse

from heapq import *
from random import *

def make_edge(u, v):
    """
    Create a tuple representing an undirected edge.

    @param u, v endpoints of the edge

    @return a tuple (u, v) representing the edge. It is guaranteed that u <= v.
    """

    if u > v:
        u, v = v, u
    return (u, v)

def prufer(n):
    """
    generate a tree of size n from a random Prüfer sequence.

    @param n number of vertices
    """

    if n <= 1:
        return

    sequence = [randrange(0, n) for i in xrange(n - 2)]
    degree = [1] * n
    for x in sequence:
        degree[x] += 1

    q = []
    for x in xrange(n):
        if degree[x] == 1:
            heappush(q, x)

    for v in sequence:
        u = heappop(q)
        degree[u] -= 1
        degree[v] -= 1

        yield make_edge(u + 1, v + 1)

        if degree[v] == 1:
            heappush(q, v)

    u = None
    for i in xrange(n):
        if degree[i] == 1:
            if u is None:
                u = i
            else:
                yield make_edge(u + 1, i + 1)

def havel_hakimi(n):
    """
    generate a tree of size n by modified Havel-Hakimi algorithm.
    The tree is induced from a random degree sequence.

    @param n number of vertices
    """

    idx = range(1, n + 1)
    shuffle(idx)
    bucket = []
    for i in xrange(n):
        bucket.append([])

    degree = [1] * n
    for i in xrange(n - 2):
        degree[randrange(0, n)] += 1

    for i, d in enumerate(degree):
        bucket[d].append(i)

    minp, maxp = 0, n - 1
    for i in xrange(n - 1):
        while len(bucket[minp]) == 0:
            minp += 1

        u = bucket[minp].pop()
        for j in xrange(degree[u]):
            while len(bucket[maxp]) == 0:
                maxp -= 1
            v = bucket[maxp].pop()

            yield make_edge(idx[u], idx[v])

            if maxp > 0:
                bucket[maxp - 1].append(v)
                degree[v] -= 1
                if minp >= maxp:
                    minp = maxp - 1

def generate(n, m, tree_generator):
    """
    generate a simple connected graph of size n with m edges
    by taking the union of some trees.

    @param n number of vertices

    @param m number of edges
    """

    E = set()
    cnt = 0
    while cnt < m:
        for e in tree_generator(n):
            if e not in E:
                E.add(e)
                yield e
                cnt += 1
                if cnt >= m:
                    break

def main():
    parser = argparse.ArgumentParser(description='Random connected graph generator')
    parser.add_argument('n', type=int, help='number of vertices')
    parser.add_argument('m', type=int, help='number of edges')
    # parser.add_argument('CMAX', type=int, help='maximum edge weight')
    parser.add_argument(
        '--prufer', '-p', action='store_true', default=False,
        help='use Prüfer sequence tree generator (slower) instead of Havel-Hakimi algorithm (faster).')
    args = parser.parse_args()

    assert 1 <= args.n
    assert args.n - 1 <= args.m <= args.n * (args.n - 1) / 2
    # assert 1 <= args.CMAX

    generator = prufer if args.prufer else havel_hakimi
    print args.n, args.m
    for (u, v) in generate(args.n, args.m, generator):
        # print u, v, randint(1, args.CMAX)
        print u, v

if __name__ == '__main__':
    main()