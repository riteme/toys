#!/usr/bin/env pypy

n = int(raw_input())
assert n & (n - 1) == 0, 'Argument "n" must be a power of 2.'

f = map(int, raw_input().split())
g = map(int, raw_input().split())

h = [0] * n
for s in range(0, n):
    t = s
    while True:
        h[s] += f[s ^ t] * g[t]
        if t == 0:
            break
        t = (t - 1) & s

print ' '.join(map(str, h))
