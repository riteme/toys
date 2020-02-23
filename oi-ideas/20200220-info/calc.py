#!/usr/bin/python

from math import factorial as fac
from fractions import Fraction as q

def c(n, m):
    if n < m:
        return 0
    return fac(n) / fac(m) / fac(n - m)

dp = {}
def f(n, a):
    if a == 0 or a == n:
        return (q(0), q(0))
    if n == 2:
        return (q(1), q(0))
    if (n, a) in dp:
        return dp[(n, a)]

    cnt, qcnt = q(0), q(1)
    for k in xrange(max(0, a - n / 2), min(n / 2, a) + 1):
        p = q(c(a, k) * c(n - a, n / 2 - k), c(n, n / 2))
        x, y = f(n / 2, k)
        z, w = f((n + 1) / 2, a - k)
        # print '[',n,a,k,']',p,x,y,z,w
        cnt += p * (x + z)
        qcnt += p * (y + w)

    ret = (cnt, qcnt)
    dp[(n, a)] = ret
    return ret

def ratio(m):
    cnt, qcnt = q(0), q(1)
    for a in xrange(0, m + 1):
        p = q(c(m, a), 2**m)
        x, y = f(m, a)
        # print p, x, y
        cnt += p * x
        qcnt += p * y

    return (qcnt / m) / (1 - cnt / m)

m = int(raw_input())
L = sorted([(i, ratio(i)) for i in xrange(2, m + 1)], key=lambda p:p[1])
for x, y in L:
    print '%-10d %-25s %-30.12f' % (x, y, y)