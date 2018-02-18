import utility

from math import sin, cos, pi

get = utility.evaluate

def expi(x):
    return cos(x) + sin(x) * 1j

def wn(n):
    return expi(2 * pi / n)

def guess(n):
    w = wn(n)
    r = sum([get(w**(-i)) for i in xrange(n)]) / n
    return int(r.real)
