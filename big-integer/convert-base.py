#!/usr/bin/python

n = int(raw_input(), base=10)
print "%x" % n
while True:
    try:
        op, v = raw_input().split()
    except:
        exit(0)
    v = int(v, base=10)
    print op, "%x" % v