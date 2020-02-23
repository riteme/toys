#!/usr/bin/pypy

n = 0
G = {}

K = int(raw_input())
while True:
    try:
        u, v = map(int, raw_input().split())
    except:
        break
    n = max(n, max(u, v))
    if u not in G:
        G[u] = {}
    if v not in G:
        G[v] = {}
    if u != v:
        G[u][v] = '1'
        G[v][u] = '1'

for u in G.keys():
    for v in xrange(1, n + 1):
        if v not in G[u]:
            G[u][v] = '0'

print n, K
for u in xrange(1, n + 1):
    if u not in G:
        print '0' * n
    else:
        L = []
        for v in xrange(1, n + 1):
            L.append(G[u][v])
        print ''.join(L)