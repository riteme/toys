# 硬币兑换问题
$n$ 种不同面值的硬币（通常会有 $1$），检查从大到小的贪心找币策略是否总是成立。若不成立，给出（最小的）反例。

Paper: *A Polynomial-time Algorithm for the Change-Making Problem* (David Pearson, 2005)

## File List

* `naive.cpp`: xjb 暴力
* `n3.cpp`: 论文中的 $\Theta(n^3)$ 算法

## 数据注意事项
* 最小的样例不一定是某个面值的倍数，但随机数据容易出现这种情况。
* 绝大部分都有反例