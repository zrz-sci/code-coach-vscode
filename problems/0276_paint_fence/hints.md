# 276. Paint Fence (栅栏涂色)

**难度**: Medium | **标签**: Dynamic Programming

## 核心思路

经典 DP 问题，关键是分解状态：第 i 个栅栏的涂色方案取决于它与前一个颜色**相同**还是**不同**。用 `same` 和 `diff` 两个变量分别表示「与前一个同色」和「与前一个异色」的方案数。

## 思维链

1. **约束分析**: 不能有连续 3 个同色 → 最多连续 2 个同色
2. **状态定义**: `same[i]` = 第 i 个与第 i-1 个同色的方案数；`diff[i]` = 异色的方案数
3. **转移关系**:
   - `same[i] = diff[i-1]`（只有上一个是异色时，这次才能同色，避免三连同色）
   - `diff[i] = (same[i-1] + diff[i-1]) * (k-1)`（无论上次同色还是异色，这次选不同颜色）
4. **空间优化**: 只需前一步的 same 和 diff，O(1) 空间

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|-----------|-----------|------|
| DP (same/diff 分离) | O(n) | O(1) | ⭐ |
| DP (合并公式) | O(n) | O(1) | |
| 递归+记忆化 | O(n) | O(n) | |

## 关键提示

```
状态转移示意 (n=4, k=3):

栅栏:  [1]  [2]    [3]      [4]
        |    |      |        |
same:   0    k=3    diff[2]  diff[3]
diff:   k=3  k*(k-1) (same[2]+diff[2])*(k-1)
                      =(3+6)*2=18

详细推导 (n=3, k=2):
  i=1: same=0, diff=2       total=2
  i=2: same=2, diff=2*1=2   total=4
  i=3: same=2, diff=(2+2)*1=4  total=6 ✓

为什么 same = diff[i-1] ?
  如果 i 与 i-1 同色，那么 i-1 必须与 i-2 异色
  (否则 i-2, i-1, i 三个同色，违规!)
  
  i-2   i-1   i
  [R]   [B]   [B]  ← i-1与i-2异色(diff)，i与i-1同色(same)
  [R]   [R]   [R]  ← 违规! 三连同色!
```

## 解法详解

### 解法一: DP same/diff 分离 (推荐)

```cpp
class Solution {
public:
    int numWays(int n, int k) {
        if (n == 0) return 0;
        if (n == 1) return k;
        
        // same: 当前与前一个同色的方案数
        // diff: 当前与前一个异色的方案数
        int same = k;       // i=2: 同色方案 = k (第1个k种，第2个同色)
        int diff = k * (k - 1); // i=2: 异色方案 = k*(k-1)
        
        for (int i = 3; i <= n; i++) {
            int prevDiff = diff;
            diff = (same + diff) * (k - 1);  // 异色: 任意前状态 * (k-1)种选择
            same = prevDiff;                   // 同色: 只能从上一步异色转移
        }
        
        return same + diff;
    }
};
```

### 解法二: DP 合并公式

```cpp
class Solution {
public:
    int numWays(int n, int k) {
        if (n == 0) return 0;
        if (n == 1) return k;
        if (n == 2) return k * k;
        
        // dp[i] = dp[i-1]*(k-1) + dp[i-2]*(k-1)
        //       = (dp[i-1] + dp[i-2]) * (k-1)
        int prev2 = k;      // dp[1]
        int prev1 = k * k;  // dp[2]
        
        for (int i = 3; i <= n; i++) {
            int cur = (prev1 + prev2) * (k - 1);
            prev2 = prev1;
            prev1 = cur;
        }
        
        return prev1;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `same = same` (不更新) | ✓ `same = prevDiff` | same 只能从上一步的 diff 转移 |
| ✗ `diff = diff * (k-1)` | ✓ `diff = (same + diff) * (k-1)` | diff 可以从 same 和 diff 都转移 |
| ✗ 初始值 `same=0, diff=k` (i=1起) | ✓ 从 i=2 开始: `same=k, diff=k*(k-1)` | 需要至少两个栅栏才能谈同色/异色 |
| ✗ 忘记 `n==1` 的特殊处理 | ✓ `if (n == 1) return k;` | 一个栅栏有 k 种涂法 |

## 面试追问

**Q1: 如果约束改为不能有连续 K 个同色（而不是3个）?**
--> 维护一个 `same_count` 计数器，或用 same[1], same[2], ..., same[K-1] 表示连续同色的长度。

**Q2: 能否用矩阵快速幂优化到 O(log n)?**
--> 可以。状态转移是线性递推 `[same, diff] = M * [same', diff']`，用矩阵快速幂 O(log n)。

**Q3: 如果 k=1 会怎样?**
--> n <= 2 时返回 1，n >= 3 时返回 0（三个同色必违规）。

## 相关题型

- [198. House Robber](../0198_house_robber/) - 类似的线性 DP
- [213. House Robber II](../0213_house_robber_ii/) - 环形 DP
- [256. Paint House](../0256_paint_house/) - 涂色 DP (3种颜色, 相邻不同色)
- [265. Paint House II](../0265_paint_house_ii/) - 涂色 DP (k种颜色)
