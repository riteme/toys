# 组合数学选讲

<br>

## riteme

<img alt="Creative Commons License" class="cc" src="assets/cc4-by-nc-sa.png">

***

### 鸽笼原理

**基本形式：**$n + 1$ 个球放入 $n$ 个盒子中，至少有一个盒子里面有不少于两个球。

---

**扩展形式：**令 $q_1,\ q_2,\ ...,\ q_n$ 为正整数，则 $\sum_{i = 1}^n q_i - n + 1$ 个球放入 $n$ 个盒子中，则至少有一个 $i$ 满足第 $i$ 个盒子里有 $q_i$ 个球。

---

如果没有，则第 $i$ 个盒子总共只有 $q_i - 1$ 个球，因此总球数上限为 $\sum_{i = 1}^n q_i - n$，矛盾。

---

**推论**　$n$ 个球放入 $m$ 个盒子中，则存在有一个盒子里有至少 $\lceil n / m \rceil$ 个球。

---

**问题：**如果尽可能平均分（即每个盒子要么 $\lfloor n/m \rfloor$ 个，要么 $\lceil n/m \rceil$ 个），有几个盒子会装 $\lceil n/m \rceil$ 个球？

---

**推论**　对于 $n$ 个正整数 $q_1,\ q_2,\ q_3,\ ...,\ q_n$，若它们的平均值大于某个数 $r - 1$，则必有一个 $q_i \geqslant r$。

---

否则 $\sum_{i = 1}^n q_i \leqslant n(r - 1)$，与平均值 $\frac1n \sum_{i = 1}^n q_i > r - 1$ 矛盾。

---

**推论**　在前 $2n$ 个正整数中任取 $n + 1$ 个数，其中必有两个数互质。

---

考虑分组 $\{1,\ 2\},\ \{3,\ 4\},\ ...,\ \{2n - 1,\ 2n\}$，则 $n + 1$ 个数里面必定有两个数在同一组。

---

**推论**　在前 $2n$ 个正整数中任取 $n + 1$ 个数，则必有一个数是另外一个数的倍数。

---

将一个数 $m$ 表示成 $2^k\cdot t$ 的形式，即把 $m$ 所有 $2$ 的质因子提出来，那么 $t$ 是一个奇数。于是前 $2n$ 个数按照 $t$ 分组可以分成 $n$ 组。对于同一组中的两个数 $2^i\cdot t$ 和 $2^j\cdot t$，根据 $i$、$j$ 之间的大小关系，必有一个数是另一个数的倍数。

+++

**推论**　将一大一小两个同心圆盘分为 $2n$ 段。对于大圆盘，任取 $n$ 个染黑色，其余染白色。对于小圆盘，任意黑白染色。旋转小圆盘，则必有一个旋转角度使得大小圆盘对应位置上相同的颜色段的数量不少于 $n$。

---

一共有 $2n$ 种旋转方式，设第 $i$ 种旋转方式相同的颜色段数量为 $q_i$。考虑计算 $\sum_{i = 1}^{2n} q_i$。对于小圆盘上任意一段，在其旋转一周的过程中，必有 $n$ 次颜色相同，以及 $n$ 次颜色不同，于是 $\sum_{i = 1}^{2n} q_i = 2n \cdot n = 2n^2$。因此 $q_i$ 的平均值不小于 $n$，则必有某个 $q_i \geqslant n$。

---

**推论**　对于任意长度为 $n$ 的序列 $\{a_i\}$，必有一个区间内的和被 $n$ 整除。

---

考虑前缀和在模 $n$ 意义下的值即可。

---

**推论**　任取 $n + 2$ 个整数，则其中必有两个数，满足两数之和或者两数之差能被 $2n$ 整除。

---

首先可以假设这些数在模 $2n$ 意义下两两不同。考虑每个数以及自己的相反数，除了 $0$ 和 $n$ 这两个自己等于自己的相反数外，其余 $n$ 个数均不与自己的相反数相同。因此一共拿到 $2n + 2$ 个数，而这些数在模 $2n$ 意义下必定有两者相同，则这两者的差为 $0$，并且根据两者的符号可以知道是原来的两数之和还是原来的两数之差。

---

下面这个结论叫做 Erdős–Szekeres 定理。

**推论**　对于任意长度为 $rs + 1$ 的序列，序列中要么存在一个长度为 $r + 1$ 的单增（单调不减、上升）的子序列，要么存在一个长度为 $s + 1$ 的单减（单调不增、下降）的子序列。

---

如果没有长度为 $r + 1$ 的单增序列，则最长上升子序列长度最多为 $r$。考虑做一个从后往前的 DP，设 $f_i$ 表示以 $i$ 开始的最长上升子序列，则我们可以把原序列分为若干个极长的上升子序列，那么这些子序列的起点则构成一个下降子序列，且长度至少为 $s + 1$。

***

### The Phone Number

<https://codeforces.com/problemset/problem/1017/C>

给定 $n$，要求构造一个排列，使得排列中最长上升子序列的长度与最长下降子序列的长度之和最小。

$n \leqslant 10^5$。

---

设定参数 $L$，构造排列为：

$$
\begin{aligned}
& n - L + 1,\ n - L + 2,\ ...,\ n, \\
& n - 2L + 1,\ n - 2L + 2,\ ...,\ n - L, \\
& ... \\
& 1,\ 2,\ ...,\ n \bmod L
\end{aligned}
$$

例如，当 $n = 22$，$L = 4$ 时：

$$
\begin{aligned}
&19,\ 20,\ 21,\ 22,\\ &15,\ 16,\ 17,\ 18,\\ &11,\ 12,\ 13,\ 14,\\ &7,\ 8,\ 9,\ 10,\\ &3,\ 4,\ 5,\ 6,\\ &1,\ 2
\end{aligned}
$$

此时最长上升子序列为 $L$，而最长下降子序列为 $\lceil n / L \rceil$。

---

枚举 $L$，使得 $L + \lceil n/L \rceil$ 最小，之后再构造即可。

***

### Dirichlet 近似定理

**定理**　设 $α$ 为实数，对于任意正整数 $n$，存在整数 $q$ 和 $[1,\ n]$ 内的正整数 $p$，满足：

$$
|pα - q| < \frac1n
$$

---

设取小数部分的记号为 $\{x\} = x - \lfloor x \rfloor$。考虑 $\{0\cdot α\},\ \{1\cdot α\},\ \{2\cdot α\},\ ...,\ \{n\cdot α\}$，这 $n + 1$ 个值，同时将区间 $[0,\ 1)$ 划分为 $n$ 个等长的子区间 $[0,\ 1/n),\ [1/n,\ 2/n),\ ...,\ [(n - 1)/n,\ 1)$，那么必定有两个下标 $0 \leqslant i < j \leqslant n$，使得 $\{iα\}$ 和 $\{jα\}$ 落在同一个子区间内，则：

$$
\begin{aligned}
1/n & > |\{jα\} - \{iα\}| \\
& = |jα - \lfloor jα \rfloor - (iα - \lfloor iα \rfloor)| \\
& = |(j - i)α - (\lfloor jα \rfloor - \lfloor iα \rfloor)|
\end{aligned}
$$

因此令 $p = j - i$，$q = \lfloor jα \rfloor - \lfloor iα \rfloor$ 即可。

---

稍微变一下形：

$$
\left|α - \frac{q}p\right| < \frac1{np} \leqslant \frac1{p^2}
$$

即有理数能在 “平方” 级别内逼近任意实数（无理数）。寻找最优逼近使用的是 Stern-Brocot 树。

***

### 排列数

$n$ 个不同的球的排列总数为 $n!$，其中任选 $m$ 个球后组成的排列个数为 $P(n,\ m) = n!/(n - m)!$。

---

**问题**　有 $n$ 种球，第 $i$ 种球有 $a_i$ 个，则这些球的总排列个数有多少个？

---

令 $m = \sum_{i = 1}^n a_i$，则为 $m! / \prod_{i = 1}^n a_i!$ 种。可以视为把同种球之间的排列全部去掉了。

---

**问题**　把 $n$ 个球排成一个环，问有多少种不同的的排列。（不考虑对称同构）

---

任意一个环形排列通过旋转总能把 $1$ 放到最上面，这样固定 $1$ 的位置，其余元素就是普通的排列了，因此方案数为 $(n - 1)!$。

---

一般的，如果是取出 $m$ 个做排列，则为 $P(n,\ m) / m$。

---

对于多重排列的环排列个数就比较麻烦了，因为一个多重排列旋转 $n$ 次中不一定每个排列都是不同的，这涉及到一个旋转同构的问题，需要使用 Pölya 计数法。

***

### 组合数

组合数 ${n \choose m}$ 是在一个大小为 $n$ 的有限集中选出 $m$ 个元素的的方案数，即大小为 $m$ 的子集数量。易知 ${n \choose m} = P(n,\ m) / m!$，即不考虑排列的内部顺序，就是组合。因此：

$$
{n \choose m} = {n! \over m!(n - m)!} = {n(n - 1)(n - 2) \cdots (n - m + 1) \over m!}
$$

---

由分母的对称性，易知 ${n \choose m} = {n \choose n - m}$。

---

计算组合数是 OI 中的基本技能。最简单的是使用 Pascal 公式（其实就是个 DP 方程）：

$$
{n \choose m} = {n - 1 \choose m} + {n - 1 \choose m - 1}
$$

考虑第 $n$ 个元素选或不选。

---

可以用一个二维数组把所有答案存下来。这种方法适用于 $n$ 比较小的情况。

---

如果是计算组合数对质数取模的结果，可以考虑计算阶乘以及阶乘的逆元，从而按照定义计算。

$$
\begin{array}{ll}
1& f[i]\colon i! \text{ 的值} \\
2& f^{-1}[i]\colon i! \text{ 在模意义下的乘法逆元} \\
3& M\colon \text{模数} \\
4& \\
5& f[0] := 1 \\
6& \textbf{for }i := 1..n\colon \\
7& \quad f[i] := f[i - 1] \cdot i \bmod M \\
8& f^{-1}[n] := f[i] \text{ 的乘法逆元} \\
9& \textbf{for }i := (n - 1)..0\colon \\
10& \quad f^{-1}[i] := f^{-1}[i + 1] \cdot (i + 1) \bmod M
\end{array}
$$

---

一般用于 $n$ 在 $10^5$ 至 $10^6$ 的时候。如果模数不是质数时限制比较大。

+++

有时候需要组合数的连续的项，例如计算 $\sum_{k = 1}^r {n \choose k}$。考虑相邻几个组合数之间的关系：

$$
\begin{aligned}
{n \choose m} &= {n! \over m! (n - m)!} \\
{n \choose m + 1} &= {n! \over (m + 1)! (n - m - 1)!} \\
{n + 1 \choose m} &= {(n + 1)! \over m! (n + 1 - m)!} \\
{n + 1 \choose m + 1} &= {(n + 1)! \over (m + 1)! (n - m)!}
\end{aligned}
$$

---

于是可以得到：

$$
\begin{aligned}
{n \choose m + 1} &= {n - m \over m + 1}{n \choose m} \\
{n + 1 \choose m} &= {n + 1 \over n - m + 1}{n \choose m} \\
{n + 1 \choose m + 1} &= {n + 1 \over m + 1}{n \choose m}
\end{aligned}
$$

其它的方向也是一样的推法。

---

如果能求出乘法逆元，只要知道一个组合数，就可以依次推出其周围的组合数。通常情况下 ${n \choose 0}$ 和 ${n \choose n}$ 之类的值都是显然的。适用于 $n$ 非常大（例如 $10^9$），但只需要知道部分组合数的值时使用。

***

### 二项式定理

$$
(x + y)^n = \sum_{k = 0}^n {n \choose k} x^k y^{n - k}
$$

**组合意义：**$n$ 括号，每个括号可以选 $x$ 或者选 $y$，因此选择 $k$ 个 $x$ （因此有 $n - k$ 个 $y$）的方案数为 ${n \choose k}$。

也可以归纳法证明。

---

令 $x = y = 1$：

$$
2^n = \sum_{k = 1}^n {n \choose k}
$$

即 $n$ 个元素的集合的子集数量为 $2^n$。

---

如果令 $x = -1$，$y = 1$：

$$
0 = \sum_{k = 0}^n (-1)^k{n \choose k} = \sum_{k = 0}^n {n\choose k}[k \text{ 是偶数}] - \sum_{k = 0}^n{n \choose k}[k \text{ 是奇数}]
$$

即大小为偶数的子集和大小为奇数的子集是一样多的。

---

如果 $n$ 是奇数，设大小为 $n$ 的全集为 $S$，那么对于每个子集 $A$，$S\backslash A$ 的大小的奇偶性一定与 $A$ 不同。如果 $n$ 是偶数？

---

此时可以将某个元素固定住（例如元素 $1$），然后将其它元素取反，这样奇偶性也会变化。从而构造双射证明了这个等式。

+++

令 $y = 1$：

$$
(1 + x)^n = \sum_{k = 0}^n {n \choose k} x^k
$$

之后对两边同时对 $x$ 求导：

$$
n(1 + x)^{n - 1} = \sum_{k = 1}^n {n \choose k} \cdot k x^{k - 1}
$$

---

此时再令 $x = 1$：

$$
n2^{n - 1} = \sum_{k = 1}^n k{n \choose k}
$$

类似的方法可以求 $\sum_{k = 0}^n k^2{n \choose k}$ 和 $\sum_{k = 0}^n k^3 {n \choose k}$ 以及更高次的级数。

+++

**多项式平移：**给定 $n$ 次多项式 $P(x) = \sum_{k = 0}^n a_k x^k$，求将 $P(x)$ 的图像向右平移 $L$ 长度后的多项式的系数表达式。即展开 $P(x - L)$。

---

$$
\begin{aligned}
P(x - L) &= \sum_{k = 0}^n a_k(x - L)^k \\
&= \sum_{k = 0}^n a_k \sum_{j = 0}^k {n \choose j} x^j (-L)^{k - j} \\
&= \sum_{j = 0}^n {n \choose j} x^j \sum_{k = j}^n a_k (-L)^{k - j}
\end{aligned}
$$

即平移后的系数 $a'_k = {n \choose k} \sum_{j = k}^n a_j(-L)^{j - k}$。

---

记 $A_k = a_{n - k}$，则：

$$
\begin{aligned}
a'_k &= \sum_{j = k}^n a_j(-L)^{j - k} \\
&= \sum_{j = 0}^{n - k} a_{j + k}(-L)^{j} \\
&= \sum_{j = 0}^{n - k} A_{n - j - k}(-L)^j
\end{aligned}
$$

于是右边变成了一个卷积的形式，可以使用 FFT 快速计算。

+++

**引理**　若 $p$ 为质数，则 $(1 + x)^{p^k} \equiv 1 + x^{p^k} \pmod p$。

---

考虑二项式展开：

$$
(1 + x)^{p^k} = \sum_{j = 0}^{p^k} {p^k \choose j} x^j
$$

---

我们知道 ${p^k \choose j} = (p^k / j)\cdot {p^k - 1 \choose j - 1}$。由于 ${p^k - 1 \choose j - 1}$ 是一个整数，因此当 $0 < j < p^k$ 时，由于 $p$ 是一个质数，所以 $j$ 最多除掉 $p^k$ 中 $k - 1$ 个质因子，因此等式右边一定是能被 $p$ 整除的。所以 ${p^k \choose j} \equiv 0 \pmod p$。而当 $j = 0$ 或 $j = p^k$ 时，组合数均为 $1$，此时二项式展开变为 $1 + x^{p^k}$。

***

### 多项式定理

扩展二项式定理，将其能够运用到多个项的拆分上：

$$
(x_1 + x_2 + \cdots + x_m)^n = \sum_{k_1 + \cdots + k_m = n} {n \choose k_1~k_2~\cdots~k_m} x_1^{k_1}x_2^{k_2}\cdots x_m^{k_m}
$$

其中 ${n \choose k_1~k_2~\cdots~k_m}$ 是所谓的多项式系数，即从 $n$ 个括号中选出 $k_1$ 个 $x_1$、$k_2$ 个 $x_2$、...、$k_m$ 个 $x_m$ 的方案数。因此：

$$
{n \choose k_1~k_2~\cdots~k_m} = {n! \over k_1!k_2!\cdots k_m!}
$$

这和多重排列的公式是一样的。一般的组合数（二项式系数）${n \choose k}$ 也可以写成 ${n \choose k~n - k}$。

---

实际上多项式系数可以写成一些列组合数的乘积：

$$
{n \choose k_1~k_2~\cdots~k_m} = {n \choose k_1}{n - k_1 \choose k_2} \cdots {n - k_1 - k_2 - \cdots - k_{m - 1} \choose k_m}
$$

***

### 组合恒等式

$$
{n \choose m}{m \choose k} = {n \choose k}{n - k \choose m - k} = {n \choose k~m-k~n-m}
$$

对于一个长度为 $n$ 初始为 $0$ 的数列，最左边相当于是先选 $m$ 个数加 $1$，之后从这 $m$ 个数中再选 $k$ 个数加 $1$。于是最后会得到 $k$ 个数为 $2$，$m - k$ 个数为 $1$，以及 $n - m$ 个数为 $0$，即最右边的三项式系数。

+++

计算：

$$
\sum_{n = l}^r {n \choose m} ={l \choose m} + {l + 1 \choose m} + \cdots + {r \choose m}
$$

---

使用 Pascal 公式可以逐项化简，因为下标总是不变：

$$
\begin{aligned}
& \red{l \choose m + 1} + \green{l \choose m} + {l + 1 \choose m} + \cdots + {r \choose m} \\
=\ & \red{l + 1 \choose m + 1} + \green{l + 1 \choose m} + {l + 2 \choose m} + \cdots + {r \choose m} \\
=\ & \cdots \\
=\ & {r + 1 \choose m + 1}
\end{aligned}
$$

因此原式就是 ${r + 1 \choose m + 1} - {l \choose m + 1}$。有点类似于前缀和。

+++

计算：

$$
\sum_{k = 0}^{r - l} {l + k \choose m + k} = {l \choose m} + {l + 1 \choose m + 1} + \cdots + {r \choose m + r - l}
$$

---

<br>

$$
\begin{aligned}
& \red{l \choose m - 1} + \green{l \choose m} + {l + 1 \choose m + 1} + \cdots + {r \choose m + r - l} \\
=\ & \red{l + 1 \choose m} + \green{l + 1 \choose m + 1} + {l + 2 \choose m + 2} + \cdots + {r \choose m + r - l} \\
=\ & \cdots \\
=\ & {r + 1 \choose m + r - l}
\end{aligned}
$$

因此原式为 ${r + 1 \choose m + r - l} - {l \choose m - 1}$。

+++

计算：

$$
\sum_{k = l}^r {n \choose k} = {n \choose l} + {n \choose l + 1} + \cdots + {n \choose r}
$$

---

抱歉，这个没法算 QAQ。目前这个和式的结果没有简单的公式。

+++

计算：

$$
\sum_{k = 0}^p {n \choose k}{m \choose p - k} = {n \choose 0}{m \choose p} + {n \choose 1}{m \choose p - 1} + \cdots + {n \choose p}{m \choose 0}
$$

---

答案是 ${n + m \choose p}$。可以考虑将 $n + m$ 个球分为两组，一组有 $n$ 个球，另一组有 $m$ 个球。则从原来的 $n + m$ 个球中选出 $p$ 个球就相当于从第一组中选出 $k$ 个球，然后从第二组中选出 $p - k$ 个球。

---

非要用公式推的话，就考虑二项式展开 $(1 + x)^{n + m} = (1 + x)^n(1 + x)^m$ 中 $x^p$ 的系数。这个系数在左边就是答案，在右边就是上述的枚举。

---

令 $n = m$，以及 $p = n$，则：

$$
\sum_{k = 0}^n {n \choose k}{n \choose n - k} = \sum_{k = 0}^n{n \choose k}^2 = {2n \choose n}
$$

***

### Lucas 定理

Lucas 定理可以用于组合数在模一些小质数下的值。

**定理**　对于质数 $p$，将非负整数 $n$ 和 $m$ 用 $p$ 进制表示：

$$
\begin{aligned}
n &= n_kn_{k-1}n_{k-2}\cdots n_0 \\
m &= m_km_{k-1}m_{k-2}\cdots m_0
\end{aligned}
$$

即 $n = \sum_{i = 0}^k n_ip^i$，$m = \sum_{i = 0}^k m_ip^i$，则：

$$
{n \choose m} \equiv \prod_{i = 0}^k {n_i \choose m_i} \pmod p
$$

---

一种简单形式是 ${n \choose m} \equiv {n \bmod p \choose m \bmod p}{\lfloor n / p \rfloor \choose \lfloor m / p \rfloor} \pmod p$，相当于从低位到高位依次取出 $p$ 进制中的每一位。

---

如果 $p$ 比较小（例如只有 $10^5$ 级别），那么无论 $n$、$m$ 有多大都可以计算。

---

证明有点抽象。考虑 $(1 + x)^n$ 的二项式展开在模 $p$ 意义下的结果：

$$
\begin{aligned}
(1 + x)^n &= \prod_{i = 0}^k (1 + x)^{n_i p^i} \\
&\equiv \prod_{i = 0}^k (1 + x^{p^i})^{n_i} ~~~~ \text{因为 }(1 + x)^{p^i} \equiv 1 + x^{p^i} \pmod p \\
&= \prod_{i = 0}^k\sum_{j = 0}^{n_i} {n_i \choose j} x^{j\cdot p^i}
\end{aligned}
$$

---

尝试从里面抽出 $x^m$ 的系数，其实就是所有 $x^{m_i\cdot p^i}$ 的系数 ${n_i \choose m_i}$ 之积。

***

### Less Coin Tosses

<https://codeforces.com/gym/102346/problem/L>

有一枚硬币，其抛出正面的概率是 $p$，抛出反面的概率是 $1 - p$，并且 $p$ 不一定就是 $0.5$，因此这枚硬币不一定是公平的。现在有两个人想用这枚硬币做一个公平的决定。他们决定连续抛 $n$ 次硬币，如果正面朝上，则记录一个 `1`，否则记录一个 `0`。抛完后就得到一个长度为 $n$ 的 `0/1` 串。

记抛出串 $s$ 的概率为 $\mathbf{Pr}[s]$。对于总共 $2^n$ 个 `0/1`，将其中一部分串 $A$ 分给第一个人，将另外一部分串 $B$ 分给另一个人，需要保证无论 $p$ 取什么值，$A$ 中所有串的概率总和必须等于 $B$ 中所有串的概率总和，这样一来就是公平的。如果抛出来的串既不在 $A$ 中，也不在 $B$ 中，则需要重新抛 $n$ 次。重复这个步骤直到确定出是谁。

由于不想过多地重抛，他们希望设计一种分配方案，使得没有用到的串总数最少。输出这个最小值。

$n \leqslant 10^{18}$。

---

如果 $s$ 中有 $k$ 个 `1`，那么 $\mathbf{Pr}[s] = p^k(1 - p)^{n - k}$，因此串的概率只与串中 `1` 的数量有关系。

---

所以对于 `1` 数量相同的一系列串，应该是平均分给两个人。我们知道有 $k$ 个 `1` 的串的总数为 ${n \choose k}$，因此只要 ${n \choose k}$ 不是奇数，就不会有浪费，否则就会有一个的浪费。于是题目变成问：

$$
\sum_{k = 0}^n \left({n \choose k} \bmod 2\right)
$$

---

考虑 Lucas 定理，易知 ${n \choose k}$ 为奇数，当且仅当在二进制中，$n$ 的每个数位都不比 $k$ 的数位小。设 $n$ 的二进制中 $1$ 的数量为 $c$，则答案为 $2^c$。

---

这个 $c$ 可以用 `__builtin_popcount` 来数。

***

### 插板法

插板法是用于解决多重组合数的一个小技巧。所谓多重组合数，就是有 $n$ 种颜色球，每种颜色可以出现任意次，问从中选出 $m$ 个球的方案数。

---

一种等价的表述形式是考虑每种颜色 $i$ 出现的次数 $x_i$。则上述组合的方案数等价于：

$$
x_1 + x_2 + \cdots + x_n = m
$$

这个方程非负整数解的个数。

---

想象一下现在有 $m$ 个未染色的球，然后在这中间插入 $n - 1$ 个板子，板子可以相邻。然后将第一块板子之前的球染为染色 $1$，将第一块板子和第二块板子之间的球染为颜色 $2$......依次类推。注意可能有部分颜色没有染任何球，这是没有问题的，因为是非负整数解。这样每一种解就对应于一个插板的方案。

---

考虑插板的方案总数。一共有 $m + n - 1$ 个位置，其中有 $m$ 个位置是球，其余位置是板子，因此总方案数为 ${m + n - 1 \choose m} = {m + n - 1 \choose n - 1}$。

---

如果每个 $x_i \geqslant l_i$，可以提前处理等式。令 $y_i = x_i - l_i \geqslant 0$。于是：

$$
\sum_{i = 1}^n y_i = \sum_{i = 1}^n (x_i - l_i) = m - \sum_{i = 1}^n l_i
$$

因此变成了求非负整数解 $y_i$ 的数量，计算方法和前面相同。

---

如果每个 $x_i$ 还有上界，即 $x_i \leqslant r_i$，这个时候就不太好办，需要使用容斥原理。

***

### 容斥原理

在 OI 中及其重要的计数思想：<del>看到计数想容斥</del>，正着不行反着来。

---

**基本形式：**$|A \cup B| = |A| + |B| - |A \cap B|$。

---

**扩展形式：**求 $n$ 个集合 $A_1,\ A_2,\ ...,\ A_n$ 的并的大小。

$$
\begin{aligned}
\left| \bigcup_{k = 1}^n A_k \right| = & |A_1| + |A_2| + \cdots + |A_n| \\
& - |A_1 \cap A_2| - |A_1 \cap A_3| - \cdots |A_{n - 1} \cap A_n| \\
& + \cdots \\
& + (-1)^{n + 1} |A_1 \cap A_2 \cap \cdots \cap A_n|
\end{aligned}
$$

---

令 $S$ 表示无限制下所有非负整数解的集合，$A_i$ 表示只有 $x_i \leqslant r_i$ 限制下之前非负整数解的集合，则带限制的多重组合方案数为：

$$
\left|\bigcap_{k = 1}^n A_k\right| = |S| - \left|\bigcup_{k = 1}^n \bar A_k \right| = |S| - \cdots
$$

省略的部分是容斥原理的展开。因此我们只用考虑计算 $|\bar A_{k_1} \cap \bar A_{k_2} \cap \cdots \cap \bar A_{k_m}|$。而 $\bar A_i$ 就是所有满足 $x_i \geqslant r_i + 1$ 的解的集合，因此这个东西变成了之前带下界的问题。

***

### 错位排列

错位排列是容斥原理的经典应用。

**问题**　长度为 $n$ 的所有 $1..n$ 排列中，有多少个排列满足对于所有 $i \in [1,\ n]$，$i$ 不在排列的第 $i$ 个位置上。

---

同样设 $S$ 为所有 $n$ 排列的集合，设 $A_i$ 表示 $i$ 不在第 $i$ 个位置上的排列的集合，则我们想求：

$$
\left|\bigcap_{k = 1}^n A_k \right| = |S| - \left| \bigcup_{k = 1}^n \bar A_k \right|
$$

---

令 $I = {1,\ 2,\ 3,\ ...,\ n}$，尝试将右边用容斥原理展开。注意 $\bar A_i$ 就是 $i$ 固定在位置 $i$ 上的所有方案。

$$
\begin{aligned}
\left| \bigcup_{k = 1}^n \bar A_k \right| &= \sum_{A \subseteq I} (-1)^{|A| + 1} \left| \bigcap_{i \in A} \bar A_i \right| \\
&= \sum_{A \subseteq I} (-1)^{|A| + 1} (n - |A|)! \quad (\text{因为 }A\text{ 中的位置均已固定}) \\
&= \sum_{k = 1}^n (-1)^{k + 1}{n \choose k}(n - k)! \\
&= \sum_{k = 1}^n (-1)^{k + 1} {n! \over k!}
\end{aligned}
$$

---

故答案为：

$$
n! - \sum_{k = 1}^n (-1)^{k + 1} {n! \over k!} = \sum_{k = 0}^n(-1)^k {n! \over k!} = n!\sum_{k = 0}^n{(-1)^k \over k!}
$$

---

顺带一提 $\sum_{k = 0}^\infin ((-1)^k / k!) = 1/\mathrm e$。

***

### Fox 序列的数量

<https://www.51nod.com/Challenge/Problem.html#problemId=1251>

给定 $n$ 和 $m$，问满足

$$
x_1 + x_2 + \cdots + x_m = n
$$

并且 $x_1,\ x_2,\ ...,\ x_m$ 中最大值唯一的非负整数解个数有多少。

$n,\ m \leqslant 10^5$。

---

枚举最大值 $a$，则最大值有 $m$ 种选择。除了最大值外，其余 $m - 1$ 个变量都需要满足 $x_i \leqslant a - 1$。这个的方案数就是：

$$
\sum_{k = 0}^{\min\{m - 1,\ \lfloor n / a \rfloor\}} (-1)^k {m - 1 \choose k} {n - (k + 1)a + m - 2 \choose m - 2}
$$

---

因为最多只能有 $\lfloor n / a \rfloor$ 个达到了最大值 $a$，因此当 $a$ 从 $1$ 到 $n$ 枚举时，复杂度是调和级数 $\mathrm O(n \ln n)$。

***

### Chessboard

给定 $n$ 和 $m$，问有多少个填了数字的方阵（方阵大小没有固定），满足每个元素不小于 $m$，并且从方阵中任意抽取一个排列矩阵（即不存在两个抽取到元素处于同一行或者同一列上）上的元素总和均相同，且总和不超过 $n$。

$1 \leqslant n,\ m \leqslant 2000$。

---

首先枚举方阵的边长 $1 \leqslant k \leqslant n/m$。

---

考虑数字下限的限制，我们可以将方阵中每个数字减去 $m$，这样数字就只用满足非负的条件即可，此时总和的上界限制 $n$ 需要减去 $km$。

---

考虑后面那个奇怪的条件，注意到求和对于每一行和每一列均只有一次，因此可以想象成方阵是由一行 $a_i$ 和一列 $b_i$ 两个数列生成的，其中第 $i$ 行第 $j$ 列的元素为 $b_i + a_j$，这样抽出来的总和实际上就是 $\sum_{i = 1}^k (a_i + b_i)$。因此只用计数序列 $a_i$、$b_i$ 有多少个，就可以知道满足那个奇怪的条件的方阵个数。

---

枚举这个总和 $n' \leqslant n$。由于方阵中每个元素非负，因此 $\min a_i + \min b_i \geqslant 0$。注意，对于一组 $a_i$ 和 $b_i$，将 $a_i$ 全部加 $1$，同时将 $b_i$ 全部减 $1$，生成的方阵是一样的，另一个方向亦然。为了避免算重，我们可以通过这种操作使得 $\min a_i = 0$。那么这样的序列总数就是 $a_i$ 和 $b_i$ 均非负的序列（即 $\min a_i \geqslant 0$）总数减去 $a_i \geqslant 1$，$b_i \geqslant 0$ 的序列（即 $\min a_i \geqslant 1$）的总数，这两个都可以使用插板法计算，即：

$$
{n' + 2k - 1 \choose 2k - 1} - {n' + k - 1 \choose 2k - 1}
$$

---

时间复杂度 $\mathrm O(n^2)$。

***

### 网格路径计数

**问题**　在二维平面上从原点 $(0,\ 0)$ 走到 $(n,\ m)$，每步只能向右和向上走一格，问有多少种不同的路线？

---

无论怎么走，一定会走 $n + m$ 步，其中 $n$ 步是向右走，$m$ 步是向上走，因此总方案数为 ${n + m \choose n}$。

---

如果加上不能穿过对角线 $y = x$ 到对角线上方的话，可以使用一种路径对称的技巧。考虑所有不合法的路径，将其从第一次穿过 $y = x + 1$ 处对称，发现起点会落在一个固定点 $(-1,\ 1)$ 处。因此每条不合法的路径都对应于一条从 $(-1,\ 1)$ 出发到终点的路径，故合法的方案数为：

$$
{n + m \choose m} - {n + m \choose m - 1}
$$

当 $n = m$ 时，这个数被称为 Catalan 数：

$$
{2n \choose n} - {2n \choose n - 1} = \left(1 - {n \over n + 1}\right) {2n \choose n} = \frac1{n + 1} {2n \choose n}
$$

---

模板题：<https://lydsy.com/JudgeOnline/problem.php?id=3907>（但是要高精度 QAQ）

---

用同样的办法可以推出从 $(x_1,\ y_1)$ 到 $(x_2,\ y_2)$ 且不触碰直线 $y = x + b$ 的方案数（$x_1 \leqslant x_2$，$y_1 \leqslant y_2$）：

$$
{x_2 - x_1 + y_2 - y_1 \choose y_2 - y_1} - {x_2 - x_1 + y_2 - y_1 \choose y_2 - y_1 - b}
$$

---

其实也可以从最后一次穿过处对称。这样可以处理不穿过两条直线的方案数。

---

关于组合恒等式 $\sum_{k = 0}^n {n \choose k}^2 = {2n \choose n}$ 也可以用路径计数的方法来解释。${n \choose k}$ 可以看做是走到了斜对角线上的一个点，而第二个则是从斜对角线上走到了右上角。

+++

**问题**　求有多少对从 $(0,\ 0)$ 走到 $(n,\ m)$ 的路径，满足两条路径不相交。路径只能向右和向上走。

---

如果相交，则从第一次相交的位置对称一下，于是变成交换起点的路径对数，因此答案为：

$$
{n + m \choose m}^2 - {n + m \choose m - 1} {n + m \choose m + 1}
$$

***

### 大大走格子

<https://www.51nod.com/Challenge/Problem.html#problemId=1486>

从 $(0,\ 0)$ 走到 $(x,\ y)$，每次只能往右或者往上走一步。但是平面还有 $n$ 个点 $(x_i,\ y_i)$ 是障碍，不能经过。问方案总数。

$x,\ y \leqslant 10^5$，$n \leqslant 2000$。

---

把终点也看做是障碍，然后设 $f_i$ 表示从起点出发到达第 $i$ 个障碍，并且中途不经过其它障碍的方案数。那么只需要从所有起点到第 $i$ 个障碍的路径中减去图中经过了其它障碍的方案数，因此枚举在障碍 $i$ 左下方向的障碍 $j$：

$$
f_i = {x_i + y_i \choose y_i} - f_j {x_i - x_j + y_i - y_j \choose y_i - y_j}
$$

时间复杂度 $\mathrm O(n^2)$。

---

还可以结合之前带限制的网格路径计数。

***

### 多项式差分

对于关于 $n$ 的多项式 $h(n)$，定义一阶差分为 $Δh(n) = h(n + 1) - h(n)$，$k$ 阶差分为 $Δ^{(k)}h(n) = Δ^{(k - 1)}h(n + 1) - Δ^{(k - 1)} h(n)$。

---

**定理**　$k$ 阶多项式 $h(n)$ 的 $k + 1$ 阶差分 $Δ^{(k + 1)}h(n) \equiv 0$。

---

可以发现多项式的差分依然是一个多项式，并且次数至少减一。可以考虑差分中最高次项的系数：

$$
a_k(n + 1)^k - a_kn^k
$$

由二项式定理可知相减后 $n^k$ 的系数会变为 $0$。

---

此外，差分是具有线性性的。

+++

对于 $p$ 阶多项式，把多阶差分写为多行的形式，那么实际上只要知道第一列上的值 $c_0,\ ...,\ c_p$（即每阶差分在 $n = 0$ 处的取值），就可以推出原多项式的序列。这个是基于网格图路径计数的。

$$
h(n) = \sum_{k = 0}^p c_k{n \choose k}
$$

---

利用组合恒等式，可以快速算出多项式的前缀和：

$$
\begin{aligned}
\sum_{j = 0}^n h(j) &= \sum_{j = 0}^n \sum_{k = 0}^p c_k{j \choose k} \\
&= \sum_{k = 0}^p c_k \sum_{j = 0}^n {j \choose k} \\
&= \sum_{k = 0}^p c_k {n + 1 \choose k + 1}
\end{aligned}
$$

即可以 $\mathrm O(p)$ 计算。但关键是要能快速计算 $c_k$。

+++

先考虑一个特殊情况，即当 $h(n) = n^p$ 时。此时：

$$
n^p = \sum_{k = 0}^p {c_k \over k!} n^{\underline k}
$$

---

记 $S(p,\ k) = c_k / (k!)$，这个东西叫做第二类 Stirling 数。注意 $S(0,\ 0) = 1$ 以及 $S(p,\ 0) = 0$（$p > 0$）。

---

因此：

$$
\begin{aligned}
\sum_{j = 0}^n h(j) = \sum_{j = 0}^n j^p & = \sum_{k = 0}^p c_k{n + 1 \choose k + 1} \\
& = \sum_{k = 0}^p {S(p,\ k) \over k + 1}(n + 1)^{\underline{k + 1}}
\end{aligned}
$$

+++

利用 $n^p = \sum_{k = 0}^p S(p,\ k) n^{\underline k}$ 可以推出第二类 Stirling 数的递推式：

$$
\begin{aligned}
n^{p-1} & = \sum_{k = 0}^{p-1} S(p-1,\ k)n^{\underline{k}} \\
n^p = n \cdot n^{p-1} & = n\sum_{k = 0}^{p-1} S(p-1,\ k)n^{\underline{k}} \\
& = \sum_{k = 0}^{p-1} S(p-1,\ k) \cdot n \cdot n^{\underline{k}} \\
& = \sum_{k = 0}^{p-1} S(p-1,\ k) \cdot (n - k + k) \cdot n^{\underline{k}} \\
& = \sum_{k = 0}^{p-1} S(p-1,\ k) n^{\underline{k + 1}} + \sum_{k = 0}^{p-1} kS(p-1,\ k) n^{\underline{k}} \\
& = \sum_{k = 1}^{p} S(p - 1,\ k - 1) n^{\underline{k}} + \sum_{k = 0}^{p-1} kS(p-1,\ k) n^{\underline{k}}
\end{aligned}
$$

对比系数后可以得出 $S(p,\ k) = kS(p - 1,\ k) + S(p - 1,\ k - 1)$。

+++

另外，根据递推式 $S(p,\ k) = kS(p - 1,\ k) + S(p - 1,\ k - 1)$，其实它可以视为是一个 DP 方程，其 $S(p,\ k)$ 的组合意义为将 $p$ 个人分为 $k$ 组的方案数（每组不能为空）。

---

根据这个组合意义，可以使用容斥原理推出计算单个 $S(p,\ k)$ 的公式。设 $A_1,\ A_2,\ A_3,\ ...,\ A_k$ 为第 $1$ 到第 $k$ 个组为空的方案集合，那么我们的答案就是：

$$ S(p,\ k) = \frac1{k!}\left|\bigcap_{i = 1}^k \bar A_i\right| = \frac1{k!}\left(k^p - \left|\bigcup_{i = 1}^k A_i\right|\right) $$

注意在之前的推导里面我们给这 $k$ 个组标了号，而第二类 Stirling 数中是没有标号的，所以要除以 $k!$。

---

考虑如果有 $t$ 个组为空，那么对于每一个人而言，就只有 $k - t$ 个选择，所以方案数就是 $(k - t)^p$。所以：

$$ S(p,\ k) = \frac1{k!}\sum_{t = 0}^k (-1)^t{k \choose t}(k - t)^p $$

***

### Lagrange 多项式插值

给定 $n + 1$ 个横坐标不相同的点值 $(x_0,\ y_0),\ (x_1,\ y_1),\ ...,\ (x_n,\ y_n)$，可以唯一确定一个次数为 $n$ 次的多项式 $p(x)$，使得 $p(x_i) = y_i$。

---

构造基本多项式为：

$$
\ell_i(x) = \prod_{j = 0,\ j \neq i}^n {x - x_j \over x_i - x_j}
$$

注意 $\ell_i(x_i) = 1$，而 $\ell_i(x_j) = 0$（$j \neq i$）。

---

于是插值多项式就是：

$$
p(x) = \sum_{i = 0}^n y_i\ell_i(x)
$$

---

如果计算出了某个 $k$ 次多项式前 $k + 1$ 个自然数处的值 $f(0),\ f(1),\ f(2),\ ...,\ f(k)$ 的话，可以推出一个比较简单的插值形式：

$$
\begin{aligned}
f(n) & = \sum_{i = 0}^{k} f(i) {(n - 0)(n - 1)\cdots[n - (i - 1)][(n - (i + 1)]\cdots(n - k) \over (i - 0)(i - 1)\cdots[i - (i - 1)][(i - (i + 1)]\cdots(i - k)} \\
& = \sum_{i = 0}^{k} f(i) {\prod_{j = 0}^{i - 1} (n - j) \prod_{j = i + 1}^k (n - j) \over i!(-1)^{k - i}(k - i)!} \\
& = \sum_{i = 0}^{k} (-1)^{k - i}f(i) {\prod_{j = 0}^{i - 1} (n - j) \prod_{j = i + 1}^{k} (n - j) \over i!(k - i)!}
\end{aligned}
$$

插值部分的时间复杂度为 $\mathrm O(k)$。

***

### XOR Sum

<https://codeforces.com/gym/259514/problem/M>

定义函数 $f(i,\ k)$ 为 $1,\ 2,\ 3,\ ..., i^k - 1,\ i^k$ 这些数的异或和，然后给定 $t$、$x$ 和 $y$，要求计算：

$$
\sum_{k = 1}^t \sum_{i = x}^y f(i,\ k) \pmod {10^9 + 7}
$$

$1 \leqslant t \leqslant 10^5$，$1 \leqslant x \leqslant y \leqslant 10^{18}$。

---

首先有个结论：$4k \oplus (4k + 1) \oplus (4k + 2) \oplus (4k + 3) = 0$。

+++

利用这个事情可以化简前 $n$ 个数的异或和函数 $S(n)$：

$$
S(n) = \begin{cases}
n & (n = 4k) \\
1 & (n = 4k + 1) \\
n + 1 & (n = 4k + 2) \\
0 & (n = 4k + 3)
\end{cases}
$$

由于 $f(i,\ k) = S(i^k)$，实际上只用考虑 $i^k$ 在模 $4$ 意义下的变化规律。

---

考虑计算 $F(t,\ R) = \sum_{i = 1}^R\sum_{k = 1}^t S(i^k)$。首先取最大的 $r$ 满足 $4r \leqslant R$。对于末尾的几个 $4r + 1$、$4r + 2$、$4r + 3$ 的点值可以特判。之后大力对各种情况进行分类讨论即可。结论：

* $\sum_{k = 1}^t S((4r + 1)^k) = t$，$\sum_{k = 1}^t S((4r + 2)^k) = ((4r + 2)^{t + 1} - 1) / (4r + 1)$，$\sum_{k = 1}^t S((4r + 3)^k) = \lfloor t / 2 \rfloor$
* $R = 0$：$0$
* 对 $k = 1$：$2r(r + 1) + r + 2r^2 + r + 0 = 4r(r + 1)$
* 对 $k > 1$ 且同余 $1$：$r(t - 1)$
* 对 $k > 1$ 且同余 $3$：$r\lfloor t / 2 \rfloor$
* 对 $k > 1$ 且同余 $0$ 或 $2$：$\sum_{i = 1}^{2r} ((2i)^{t + 1} - (2i)^2) / (2i - 1)$。这是一个关于 $r$ 的多项式，插值即可。

***

# Thanks!