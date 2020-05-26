from random import *

n = 100
K = 20
C = 19

print(n, K)
print(*[randint(0, C) for i in range(n)])
print(n)
for i in range(n):
    l = randint(1, n)
    r = randint(l, n)
    print(l, r)