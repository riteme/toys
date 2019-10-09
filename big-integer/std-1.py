#!/usr/bin/python

BASE = 10

x = int(raw_input(), base=BASE)
while True:
    try:
        op, v = raw_input().split()
    except:
        exit(0)
    v = int(v, base=BASE)
    if op == '+':
        x += v
    elif op == '-':
        x -= v
    elif op == '*':
        x *= v
    elif op == '/':
        x /= v
    elif op == '%':
        print "%s" % (x % v)
        continue
    print "%s" % x