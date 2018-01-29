#### P84

令 $f(x)$ 连续单调递增的函数，并且当函数值为整数时，$x$ 也为整数，即：
$$
f(x) \in \mathbf{N} \Rightarrow x \in \mathbf{N}
$$
则有：
$$
\lfloor f(x) \rfloor = \lfloor f(\lfloor x \rfloor) \rfloor \\
\lceil f(x) \rceil = \lceil f(\lceil x \rceil) \rceil
$$

若$x \in \mathbf{R},~n,m \in \mathbf{N}$，则有：
$$
\left\lfloor {x + m \over n} \right\rfloor = \left\lfloor {\lfloor x \rfloor + m \over n} \right\rfloor \\
\left\lceil {x + m \over n} \right\rceil = \left\lceil {\lceil x \rceil + m \over n} \right\rceil
$$
例如，$m = 0$ 时，可知 $\lfloor \lfloor \lfloor x / 10 \rfloor / 10 \rfloor / 10 \rfloor = \lfloor x / 1000 \rfloor$。

#### P87

区间内整数的个数：

*   $[a,b]$: $\lfloor b \rfloor - \lceil a \rceil + 1$
*   $[a,b)$: $\lceil b \rceil - \lceil a \rceil$
*   $(a,b]$: $\lfloor b \rfloor - \lfloor a \rfloor$
*   $(a,b)$: $\lceil b \rceil - \lfloor a \rfloor - 1$

#### P98

对于任意**整数** $n$ 和**正整数** $m$：
$$
n = \sum_{k = 0}^{m - 1} \left\lceil {n - k \over m} \right\rceil = \sum_{k = 0}^{m - 1} \left\lfloor {n + k \over m} \right\rfloor
$$
