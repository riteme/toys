#!/usr/bin/python

n = int(raw_input(), base=16)
print n
while True:
    try:
        op, v = raw_input().split()
    except:
        exit(0)
    v = int(v, base=16)
    print op, v