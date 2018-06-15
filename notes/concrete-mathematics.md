$$
\text{New Commands Here}
\newcommand{\lf}[]{\lfloor}\newcommand{\rf}[]{\rfloor}\newcommand{\lc}[]{\lceil}\newcommand{\rc}[]{\rceil}
\newcommand{\dw}[1]{\left\lfloor #1 \right\rfloor}\newcommand{\up}[1]{\left\lceil #1 \right\rceil}
$$

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

对于任意**实数** $x$：
$$
\lfloor mx \rfloor = \sum_{k = 0}^{m - 1} \left\lfloor x + \frac{k}m\right\rfloor \\
$$

常用情形：$m = 2$ 时：$\lfloor x \rfloor + \lfloor x + 1/2  \rfloor = \lfloor 2x \rfloor$，$\up{x} + \up{x + 1/2} = \up{2x}$

#### P107

对于任意**正整数** $n$、$m$ 和**实数** $x$：
$$
\sum_{k = 0}^{m -1} \left\lfloor {nk + x \over m} \right\rfloor = d \left\lfloor \frac{x}d \right\rfloor + {(m - 1)(n - 1) \over 2} + {d - 1 \over 2} = \sum_{k = 0}^{n -1} \left\lfloor {mk + x \over n} \right\rfloor
$$
其中 $d = \gcd(m, n)$。P103-P107: 如何找规律

#### P108

3.3
$$
\left\lfloor {\lfloor m \alpha \rfloor n \over \alpha} \right\rfloor = \left\lfloor {(m \alpha - \{m \alpha\}) n \over \alpha} \right\rfloor = mn - 1  \ \ \ \   (m, n \in \mathbf{N}_+, \alpha \in \mathbf{R} - \mathbf{Q}, \alpha > n)
$$
取整的性质：
$$
\lfloor -x \rfloor = -\lceil x \rceil \\
\lceil -x \rceil = -\lfloor x \rfloor
$$
区间内整数个数：
$$
[l,\ r] \rightarrow \lfloor r \rfloor - \lceil l \rceil + 1 \\
[l,\ r) \rightarrow \lceil r \rceil - \lceil l \rceil \\
(l,\ r] \rightarrow \lfloor r \rfloor - \lfloor l \rfloor \\
(l,\ r) \rightarrow \lceil r \rceil - \lfloor l \rfloor - 1 \:\:\:\:  (l < r)
$$

#### P114

$n!$ 的质因数分解中 $2$ 的幂次 $\epsilon_2(n)$：
$$
\epsilon_2(n) = n - \mathrm{popcount}(n)
$$
`popcount` 即 $n$ 的二进制表示中 $1$ 的个数。原因非常简单，因为：
$$
\epsilon_p(n) = \sum_{k = 1}^\infty \dw{n \over p^k}
$$
当 $p = 2$ 时，考虑 $n$ 的二进制，表示 $2^m$ 的二进制上的 $1$ 对 $n$ 的贡献为 $2^m$，而对 $\epsilon_2(n)$ 的贡献为 $\sum_{k = 0}^{m-1} 2^k = 2^m - 1$，因此可以得到之前的等式。