# 区间加多项式

输入格式：

```
n q kmax
[Q l r]
[M l r k (x, y)... x k]
```

长度为 $n$ 初始全为 $0$ 的序列，$q$ 次操作：

1.  `M l r k P`: 区间 $[l, r]$ 加通项公式为多项式的数列，多项式 $P$ 中有 $k$ 项，每项一个数对 $a,b$ 表示 $P$ 中的 $a \cdot x^b$ 这一项。
2.  `Q l r`: 查询 $[l,r]$ 的区间和。

`brute.cpp`: 纯暴力

`main.cpp`: $O(qk \log q)$ 的离线算法