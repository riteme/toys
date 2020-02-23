#!/usr/bin/python3

from sys import *
from random import *

n, q, C = map(int, argv[1:])

def randz(l):
    Σ = "0123456789"
    m = randint(1, l)
    σ = choice(Σ[1:]) + ''.join(choice(Σ) for i in range(m - 1))
    return int(σ)

def mag(num):
    return len(str(num))

x = randz(n)
print(x)
i = 0
while i < q:
    i += 1
    Γ = ['+', '-', '*', '+', '-', '*', '+', '*', '/', '<<', '>>', '%']
    ρ = choice(Γ)
    assert x >= 0
    l = mag(x)
    if l > C * n and ρ in ['+', '*', '<<']:
        i -= 1
        continue
    if ρ == '+':
        v = randz(2 * l)
        print('+', v)
        x += v
    elif ρ == '-':
        l1 = randint(1, l)
        if l1 == l:
            v = randint(0, x)
        else:
            v = randz(l1)
        print('-', v)
        x -= v
    elif ρ == '*':
        v = randz(2 * l)
        print('*', v)
        x *= v
    elif ρ == '/':
        v = randz(l)
        print('/', v)
        x //= v
    elif ρ == '<<':
        v = randint(0, l // 2)
        print('<<', v)
        x <<= v
    elif ρ == '>>':
        v = randint(0, l // 2)
        print('>>', v)
        x >>= v

    if ρ == '%':
        v = randz(l)
        print('%', v)
        stderr.write(f'{x % v}\n')
    else:
        stderr.write(f'{x}\n')
