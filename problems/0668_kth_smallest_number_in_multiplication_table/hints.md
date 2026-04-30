# 668. 乘法表中第k小的数 (Kth Smallest Number in Multiplication Table)

[Hard] | Math, Binary Search

## 核心思路

二分答案。在 `[1, m*n]` 范围内二分一个值 `mid`，然后计算乘法表中 `<= mid` 的数有多少个。如果个数 `>= k`，说明答案在左半边；否则在右半边。对于每一行 `i`，`<= mid` 的数有 `min(mid/i, n)` 个。

---

## 思维链

1. 乘法表太大（最大 3*10^4 * 3*10^4 = 9*10^8），不能直接排序
2. 转换思路：不是"找第 k 小"，而是"猜一个答案 x，验证 x 是否是第 k 小"
3. 验证方法：统计乘法表中 `<= x` 的元素个数 `count(x)`
4. 第 `i` 行的元素是 `i, 2i, 3i, ..., ni`，其中 `<= x` 的有 `min(x/i, n)` 个
5. `count(x) = sum(min(x/i, n))` for i = 1 to m
6. 找最小的 x 使得 `count(x) >= k`

---

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 解法一 | 二分答案 + 逐行计数 | O(m * log(m*n)) | O(1) | ⭐⭐⭐ 唯一最优解 |

---

## 关键提示

### 提示 1 - 不能暴力
m, n 最大 30000，乘法表有 9*10^8 个元素，无法全部存储和排序。

### 提示 2 - 二分答案模式
"第 k 小" 问题的经典套路：二分猜答案 x，验证 `<= x` 的元素个数是否 `>= k`。

### 提示 3 - 快速计数
第 i 行是 `i, 2i, 3i, ...`，其中 `<= x` 的个数是 `min(x / i, n)`（整除）。遍历所有行求和，O(m) 完成。

### 提示 4 - 二分的左右边界
答案一定是乘法表中实际存在的数。左边界 `lo = 1`，右边界 `hi = m * n`。

### 提示 5 - 找左边界
用 `lo < hi` 的模板，`count(mid) >= k` 时 `hi = mid`，否则 `lo = mid + 1`。这样找到的是满足 `count(x) >= k` 的最小 x。

---

## 解法详解

### 解法一: 二分答案

```cpp
class Solution {
public:
    int findKthNumber(int m, int n, int k) {
        int lo = 1, hi = m * n;
        while (lo < hi) {
            int mid = lo + (hi - lo) / 2;
            if (count(m, n, mid) >= k) {
                hi = mid;
            } else {
                lo = mid + 1;
            }
        }
        return lo;
    }

    // 统计乘法表中 <= x 的元素个数
    int count(int m, int n, int x) {
        int cnt = 0;
        for (int i = 1; i <= m; i++) {
            cnt += min(x / i, n);
        }
        return cnt;
    }
};
```

**过程演示** (m=3, n=3, k=5):

```
乘法表:
1 2 3
2 4 6
3 6 9

排序后: 1, 2, 2, 3, 3, 4, 6, 6, 9  -> 第5小 = 3

二分过程:
lo=1, hi=9
  mid=5: count(5) = min(5/1,3)+min(5/2,3)+min(5/3,3) = 3+2+1 = 6 >= 5 -> hi=5
lo=1, hi=5
  mid=3: count(3) = min(3/1,3)+min(3/2,3)+min(3/3,3) = 3+1+1 = 5 >= 5 -> hi=3
lo=1, hi=3
  mid=2: count(2) = min(2/1,3)+min(2/2,3)+min(2/3,3) = 2+1+0 = 3 < 5 -> lo=3
lo=3, hi=3 -> 答案 = 3
```

---

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 用 `count(mid) == k` 判断 | ✓ 用 `count(mid) >= k` 并找左边界 | 乘法表中可能有重复值，count 可能跳过 k |
| ✗ `hi = m * n + 1` 用开区间 | ✓ 用闭区间 `hi = m * n`，配合 `lo < hi` | 开闭区间要和循环条件匹配 |
| ✗ `int mid = (lo + hi) / 2` | ✓ `int mid = lo + (hi - lo) / 2` | 防止溢出 (m*n 可达 9*10^8) |

---

## 面试追问

**Q1: 为什么二分的答案一定在乘法表中？**
→ 因为我们找的是满足 `count(x) >= k` 的最小 x。如果 x 不在乘法表中，那么 `count(x) == count(x-1)`，而 x-1 也满足条件，x 不会是最小的。所以最终答案一定是乘法表中的某个值。

**Q2: count 函数为什么是 `min(x/i, n)` 而不是 `x/i`？**
→ 第 i 行最多有 n 列，所以 `<= x` 的数最多 n 个。当 `x/i > n` 时，整行都 <= x，取 n。

**Q3: 这道题和 "Kth Smallest Element in a Sorted Matrix" (378) 有什么区别？**
→ 378 的矩阵行列都有序，可以用优先队列或二分。668 的乘法表有更强的结构（第 i 行是 i 的倍数），计数可以 O(m) 完成而不需要 O(m+n)。两题的二分答案思路完全相同。

---

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 378 | Kth Smallest Element in a Sorted Matrix | 二分答案 + 矩阵计数 |
| 719 | Find K-th Smallest Pair Distance | 二分答案 + 计数 |
| 786 | K-th Smallest Prime Fraction | 二分答案 |
| 373 | Find K Pairs with Smallest Sums | 优先队列 / 二分 |
