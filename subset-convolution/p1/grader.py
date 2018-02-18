#!/usr/bin/env pypy

import user
import utility

n = int(raw_input())
P = map(int, raw_input().split())
utility.set_target(P)
ret = user.guess(n)

if ret != P[-1]:
    print 'Wrong answer: received %s, expected %s' % (ret, P[-1])
else:
    print 'OK: %s' % utility.count
