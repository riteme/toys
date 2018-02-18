def fwt(X, T):
    if len(X) == 1:
        return vector(X)

    m = len(X) >> 1
    A = fwt(X[:m], T)
    B = fwt(X[m:], T)
    return vector(tuple(T[0][0] * A + T[0][1] * B) +
                  tuple(T[1][0] * A + T[1][1] * B))

def mul(A, B):
    return vector([A[k] * B[k] for k in range(len(A))])

def real_expand(M):
    return [expand(x) for x in M]

def bin(x, a):
    s = x.binary()
    return '0' * (a - len(s)) + s

def genseq(name, n):
    L = []
    l = len((n - 1).binary())
    i = 0
    while i < n:
        L.append(name + bin(i, l))
        i += 1
    return vector(var(' '.join(L)))

def wn(n):
    return e^(2*pi*I/n)

def test(n, T):
    Ti = T.inverse()
    X = fwt(genseq('a', n), T)
    Y = fwt(genseq('b', n), T)
    Z = mul(X, Y)
    return real_expand(fwt(Z, Ti))
