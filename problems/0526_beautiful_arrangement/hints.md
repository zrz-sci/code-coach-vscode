# 526. 优美的排列 (Beautiful Arrangement)

**难度**: Medium | **标签**: Bit Manipulation, Array, Dynamic Programming, Backtracking, Bitmask

## 核心思路

经典的**排列计数 + 约束条件**问题。对于每个位置 i，放置的数字 num 必须满足 `num % i == 0 || i % num == 0`。可用**回溯**或**状压 DP** 解决。n <= 15 暗示可用位掩码表示使用状态。

## 思维链

1. **暴力**: 枚举所有 n! 个排列逐一验证 -> 阶乘级别，太慢
2. **回溯剪枝**: 按位置逐个放数，不满足条件时剪枝
3. **状压 DP**: 用 bitmask 表示哪些数字已被使用，dp[mask] = 方案数
4. **位置推导**: mask 中已用的数字个数 = 当前要填的位置

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| 回溯 + 剪枝 | O(n!) 最坏 | O(n) | ⭐⭐⭐⭐ |
| 状压 DP | O(2^n * n) | O(2^n) | ⭐⭐⭐⭐⭐ |

## 关键提示

1. **位置从 1 开始**: 题目是 1-indexed，注意下标处理
2. **状压 DP 中位置推导**: `pos = __builtin_popcount(mask) + 1`，即已选数字的个数 + 1
3. **回溯优化**: 从大位置向小位置填，因为大位置约束更强，剪枝更早
4. **整除条件**: `num % pos == 0 || pos % num == 0`，两个方向都要检查

## 解法详解

### 解法一：状压 DP

```cpp
class Solution {
public:
    int countArrangement(int n) {
        // dp[mask] = 使用 mask 代表的数字集合时的方案数
        vector<int> dp(1 << n, 0);
        dp[0] = 1; // 空集，一种方案
        
        for (int mask = 1; mask < (1 << n); mask++) {
            int pos = __builtin_popcount(mask); // 当前要填的位置
            for (int num = 1; num <= n; num++) {
                // num 必须在 mask 中（已被选）
                if (!(mask & (1 << (num - 1)))) continue;
                // 检查整除条件
                if (num % pos == 0 || pos % num == 0) {
                    dp[mask] += dp[mask ^ (1 << (num - 1))];
                }
            }
        }
        return dp[(1 << n) - 1];
    }
};
```

### 解法二：回溯

```cpp
class Solution {
public:
    int count = 0;
    
    void backtrack(int pos, int n, vector<bool>& used) {
        if (pos > n) {
            count++;
            return;
        }
        for (int num = 1; num <= n; num++) {
            if (!used[num] && (num % pos == 0 || pos % num == 0)) {
                used[num] = true;
                backtrack(pos + 1, n, used);
                used[num] = false;
            }
        }
    }
    
    int countArrangement(int n) {
        vector<bool> used(n + 1, false);
        backtrack(1, n, used);
        return count;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `pos = __builtin_popcount(mask)` | ✓ `pos = __builtin_popcount(mask)` (位置 1-indexed) | 位置从 1 开始，popcount 直接等于位置 |
| ✗ 只检查 `num % pos == 0` | ✓ 还要检查 `pos % num == 0` | 两个方向的整除都算 |
| ✗ 位运算 `1 << num` | ✓ `1 << (num - 1)` | 数字 1~n 对应位 0~(n-1) |
| ✗ 回溯忘记恢复状态 | ✓ `used[num] = false` | 标准回溯模板 |

## 面试追问

**Q1**: 状压 DP 和回溯哪个更好？
> 状压 DP 时间复杂度严格 O(2^n * n)，避免了回溯中重复子问题的计算。回溯在剪枝良好时实际运行可能更快，但最坏情况更慢。

**Q2**: 为什么 `__builtin_popcount(mask)` 就是当前位置？
> mask 中有几个 1 就代表已经选了几个数字放在前面的位置上，所以下一个要填的位置就是 popcount + 0 (0-indexed) 或 popcount (1-indexed 因为先加了 1)。

**Q3**: 如果 n 更大（比如 20），状压 DP 还可行吗？
> n=20 时 2^20 = 10^6，加上 n=20 的内循环约 2*10^7，还在可接受范围。但 n=25+ 就需要其他方法如容斥原理。

## 相关题型

- [46. 全排列](https://leetcode.com/problems/permutations/) - 排列回溯基础
- [691. 贴纸拼词](https://leetcode.com/problems/stickers-to-spell-word/) - 状压 DP
- [1879. 两个数组最小的异或值之和](https://leetcode.com/problems/minimum-xor-sum-of-two-arrays/) - 状压 DP 排列匹配
