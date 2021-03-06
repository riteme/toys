# 解题报告

<br>

## riteme

<img alt="Creative Commons License" class="cc" src="assets/cc4-by-nc-sa.png">

***

### 序列

实际上是计算数论函数 $1(n) = 1$ 的 $m$ 次 Dirichlet 卷积 $1^m$ 的前 $n$ 项和。

---

<br>

记 $f(n) = 1^m(n)$。设 $n = p_1^{k_1}p_2^{k_2}\cdots p_t^{k_t}$，则对于每个质数相当于是一个插板法，则：

$$
f(n) = \prod_{i = 1}^t {m + k_i - 1 \choose k_i}
$$

---

$f(n)$ 显然是一个积性函数。对于题目的数据范围使用线性筛即可。理论上应该可以 min25 筛。

***

### 闭包

这个题我构造了一些数据，如果有乱搞过掉了本人甘拜下风。

---

<br>

从空图开始，用一个队列存放所有原图中的边，依次加入到图中。算法应该在加边的过程中寻找有没有需要新增的边。

---

根据图中点当前的度数将所有点放入 $n$ 个桶中。每加入一条边 $uv$ 时，依次将 $d(u)$ 和 $d(v)$ 加 $1$。

---

考虑到 $d(u)$ 加 $1$ 后，$u$ 会从桶 $d(u) - 1$ 移动到桶 $d(u)$，此时需要检查桶 $K - d(u)$ 内所有点，将当前该桶内不与 $u$ 相连的点与 $u$ 连边。新加的边放入队列中。

---

算法终止时就得到了原图的闭包。时间复杂度 $\mathrm O(n^2)$。

***

### 最短路

瞎搞题，你们爱咋搞咋搞，比标程厉害就行。

---

标程干了啥：

---

* 快读。使用 `mmap`。
---
* 询问时是双向 A\*。就是课上提到的那个算法。
---
* 估价函数是预处理时随机找了几个点做最短路算的。每次询问前用估计得最好的几个最短路来计算估价函数。
---
* 预处理的最短路的同时顺便用 Tarjan LCA 算法删掉了一些过长的边。
---
* 预处理的最短路是用了 SLF 优化的 SPFA。
---
* 双向 A\* 的堆用的是配对堆。

---

另外标程其实常数应该挺大的，毕竟我没有特意去卡常数。题目的时限是标程时间取上整后加 $1$ 秒。