count = 0
P = None

def set_target(p):
    global P
    P = p

def evaluate(x):
    global count
    global P
    count += 1

    if x == 0:
        print 'Hey!'

    r = 0
    for c in P:
        r = r * x + c
    return r
