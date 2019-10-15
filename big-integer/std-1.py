#!/usr/bin/python3

BASE = 10

x = int(input(), base=BASE)
while True:
    try:
        op, v = input().split()
        # print("⇒ ", op, v)
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
        x //= v
    elif op == '%':
        print("%s" % (x % v))
        continue
    print("%s" % x)