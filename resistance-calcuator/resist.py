#!/usr/bin/env python3

from sympy import solve
from sympy.core.numbers import Zero, One
from sympy.core.symbol import Symbol as sym

def calc(n, start, end, E, generate_dot = False):
    """Calculate the resistance of a network.
    A node represent a crossroads of electric lines,
    each segment has a resistance

    Parameters:
    n     (int): the number of nodes in the network, which are
        labelled from 1 to n.
    start (int): the node with input current.
    end   (int): the node with output current.
    E (list[(u, v, r)]): network data. (u, v, r) represent
        a segment between u and v with a resistance of r.

    Flags:
    generate_dot (bool, False): If set to True, this function will return
        the result in dot script for analysis.

    Return values:
    (sympy.core.numbers.Rational / str)
    the resistance of the network represented in rational.
    When `generate_dot` is set, return str instead.
    """

    start, end = start - 1, end - 1

    # Set up a graph, whose edges are bidirectional.
    # Each pair of directed edge has different coefficients (1 and -1).
    G = [[] for i in range(n)]
    for i in range(len(E)):
        u, v, r = E[i]
        u, v = u - 1, v - 1
        s = sym('I' + str(i))
        G[u].append((v, 1, r, s))
        G[v].append((u, -1, r, s))

    # Set up equations.
    A = []
    for v in range(n - 1):
        if v == start:
            eq = One()
        elif v == end:
            eq = -One()
        else:
            eq = Zero()

        for u, f, r, s in G[v]:
            eq -= f * s

        A.append(eq)

    father, depth = [None] * n, [0] * n
    def dfs(A, G, father, tm, x):
        for v, f, r, s in G[x]:
            if s == father[x][3]:
                continue
            if depth[v] and depth[v] < depth[x]:
                eq, u = f * r * s, x
                while u != v:
                    u, f1, r1, s1 = father[u]
                    eq += f1 * r1 * s1
                A.append(eq)
            elif not depth[v]:
                father[v] = (x, f, r, s)
                depth[v] = depth[x] + 1
                dfs(A, G, father, depth, v)

    # Solve it.
    father[start], depth[start] = (0, ) * 4, 1
    dfs(A, G, father, depth, start)
    sol = solve(A)

    # Calculate the resistance.
    ret, x = 0, end
    while x != start:
        x, f, r, s = father[x]
        ret += f * r * sol[s]

    if generate_dot:
        buf = ['digraph {', '    rankdir = "LR";']

        buf.append('    r [shape = rectangle, label = "%sΩ"];' % ret)
        for i in range(1, n + 1):
            if i == start + 1:
                c = 'green'
            elif i == end + 1:
                c = 'red'
            else:
                c = 'blue'

            buf.append('    %s [shape = point, color = %s];' % (i, c))

        for u in range(1, n + 1):
            for v, f, r, s in G[u - 1]:
                if f < 0:
                    continue

                I, v = sol[s], v + 1
                if I < 0:
                    e = '%s -> %s [' % (v, u)
                elif I > 0:
                    e = '%s -> %s [' % (u, v)
                else:
                    e = '%s -> %s [arrowhead = none, ' % (u, v)

                buf.append('    %slabel = "%sA,%sΩ"];' % (e, abs(I), r))

        buf += '}'
        return '\n'.join(buf)
    else:
        return ret

# Samples
# print(calc(3, 2, 1, [(1, 2, 1), (3, 1, 10)], generate_dot = True))  # 1
# print(calc(2, 1, 2, [(1, 2, 2), (1, 2, 3)], generate_dot = True))  # 6/5
# print(calc(4, 1, 4, [
#     (1, 2, 1), (2, 4, 2), (2, 3, 3), (1, 3, 4), (3, 4, 5)
# ]))  # 159/71

# 3x3 grid network
print(calc(16, 1, 16, [
    (1, 3, 1), (1, 2, 1), (3, 4, 1), (3, 5, 1),
    (2, 5, 1), (2, 6, 1), (4, 10, 1), (4, 9, 1),
    (5, 9, 1), (5, 8, 1), (6, 8, 1), (6, 7, 1),
    (10, 11, 1), (9, 11, 1), (9, 12, 1), (8, 12, 1),
    (8, 13, 1), (7, 13, 1), (11, 15, 1), (12, 15, 1),
    (12, 14, 1), (13, 14, 1), (15, 16, 1), (14, 16, 1)
], generate_dot = True))  # 13/7

# Different sources & sinks
# print(calc(5, 1, 3, [
#     (1, 2, 1), (1, 3, 1), (1, 4, 1), (2, 3, 1),
#     (2, 5, 1), (3, 4, 1), (3, 5, 1), (4, 5, 1)
# ]))  # 7/15
# print(calc(5, 1, 5, [
#     (1, 2, 1), (1, 3, 1), (1, 4, 1), (2, 3, 1),
#     (2, 5, 1), (3, 4, 1), (3, 5, 1), (4, 5, 1)
# ]))  # 2/3
# print(calc(5, 1, 2, [
#     (1, 2, 1), (1, 3, 1), (1, 4, 1), (2, 3, 1),
#     (2, 5, 1), (3, 4, 1), (3, 5, 1), (4, 5, 1)
# ]))  # 8/15
