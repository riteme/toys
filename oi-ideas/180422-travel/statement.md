### Description

Given an **undirected** graph $G = (V,\ E)$ where $V$ and $E$ represent the set of nodes and the set of edges in the graph $G$ respectively, each node is numbered from 1 to n. Denote  $n = |V|$ and $m = |E|$ for convenience. The task comes with $q$ queries, each of which is followed by a triple $(s,\ t,\ u)$. Contestants should write a program that for each query, detects whether a simple path (a path that passes every node at most once) exists in the graph $G$ that starts with $s$ and ends in $t$ and **contains** $u$. Only the answer "yes/no" is required.

### Restrictions

Time limit: $1\ \mathrm{s}$ / Memory limit: $512 \ \mathrm{MB}$

 $1 \leqslant n \leqslant 5 \times 10^5$, $1 \leqslant m \leqslant 10^6$, $1 \leqslant q \leqslant 5 \times 10^5$.