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

#### P124

$$
a \equiv b \pmod{m} \Leftrightarrow a - b = km,\ k \in \mathbf{Z}
$$

用于判断两数是否同余，仅用一次取模。

证明：

模 $m$ 意义下，如果 $a \equiv b$ 且 $c \equiv d$，则：

$$
a + c \equiv b + d \\
a - c \equiv b - d \\
ac \equiv bd
$$
考虑之前所述的等价关系：$a - b$、$c - d$ 均为 $m$ 的倍数，将其线性组合可以得到上述结论。

（$(a - b) + (c - d) = (a + c) - (b + d)$、$(a - b) - (c - d) = (a - c) - (b - d)$、$(a - b)c + (c - d)b = ac - bd$）
$$
ab \bmod mb = ab - mb\dw{ab \over mb} = b\left(a - m\dw{a \over m}\right) = (a \bmod m)b
$$
如果 $d \neq 0$，可以推出：
$$
\begin{aligned}
a \equiv b \pmod{m} & \Leftrightarrow a \bmod m = b \bmod m \\
& \Leftrightarrow d(a \bmod m) = d(b \bmod m) \\
& \Leftrightarrow ad \equiv bd \pmod{md}
\end{aligned}
$$
综合之前的结论：
$$
ad \equiv bd \pmod{m} \Leftrightarrow a \equiv b \pmod{m / \gcd(m, d)} \:\:\:\: (a,\ b,\ m,\ d \in \mathbf{Z})
$$
换模数大法：如果 $m,\ n > 0$
$$
a \equiv b \pmod{m} \text{ and } a \equiv b \pmod{n} \Leftrightarrow a \equiv b \pmod{\mathrm{lcm}(m,\ n)}
$$

#### P128

独立剩余系：若 $m$ 和 $n$ 互质，用$(x \bmod m,\ x \bmod n)$ 来表示 $x \bmod mn$。

问题已知 $x = (p,\ q)$，如何求出 $x \bmod mn$？（中国剩余定理）

设 $a = (1,\ 0)$ 以及 $b = (0,\ 1)$，则 $x \equiv ap + bq \pmod{mn}$。现在的目标则是求出 $a$ 和 $b$。

Easy！根据扩展欧几里得算法，我们得到：$m'm + n'n = 1$，令 $a = n'n$ 且 $b = m'm$ 即可。

类比向量线性基。

#### P135

$f(n)$ 为积性函数当且仅当 $1 \times f = \sum_{d \mid n} f(d)$ 为积性函数。

证明方法，数学归纳法

#### P138

记欧拉函数前缀和：
$$
\Phi(n) = \sum_{k = 1}^n \varphi(k)
$$
证明：
$$
\sum_{k = 1}^n \Phi\left(\dw{n \over k}\right) = {n(n + 1) \over 2}
$$
$\Phi(n)$ 表示满足 $0 \leqslant a < b \leqslant n$ 的既约分数 $a/b$ 的个数。考虑统计所有满足 $0 \leqslant a < b \leqslant n$ 的分数 $a/b$ 个数（可能没有约分），其中 $\gcd(a,\ b) = r$ 的分数可表示为 $ra'/rb' = a'/b'$，且 $b' \leqslant \dw{n/r}$，所以这样的分数的个数为 $\Phi(\dw{n/r})$。分数的总个数为右式。

#### P139

莫比乌斯反演真实面目：
$$
f(x) = \sum_{k \geqslant 1} g(x/k) \:\Longleftrightarrow\: g(x) = \sum_{k \geqslant 1} \mu(k)f(x/k)
$$
由 $\sum_{d \mid n} \mu(d) = [n = 1]$ 可推出。

#### P143

费马小定理加强版：
$$
\begin{aligned}
n^p & \equiv n & \pmod{p} \\
n^{p^k} & \equiv n^{p^{k - 1}} & \pmod {p}
\end{aligned}
$$
通过数学归纳法 + 二项式定理展开证明