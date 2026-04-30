# 413. Arithmetic Slices (等差数列划分)

## 核心思路
统计数组中所有长度>=3的等差子数组个数。核心观察：如果以 `i` 结尾的连续等差段长度增加1，则新增的等差子数组数量也增加1（递推关系）。

## 思维链
1. 等差子数组要求**连续**且**长度>=3**
2. 如果 `nums[i] - nums[i-1] == nums[i-1] - nums[i-2]`，则 i 延续了等差段
3. 设 dp[i] = 以 i 结尾的等差子数组个数，满足条件时 `dp[i] = dp[i-1] + 1`
4. 答案 = sum(dp[i])
5. dp 只依赖前一个值，可压缩为单变量

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ DP（空间优化） | O(n) | O(1) | 最优解，面试首选 |
| 数学计数 | O(n) | O(1) | 直接统计连续等差段长度 |

## 关键提示
1. dp[i] 代表"以 i 结尾的新增等差子数组数"，不是总数
2. 例：[1,2,3,4] 中 dp = [0,0,1,2]，总数 = 1+2 = 3
3. 段长为 k 的等差段贡献 `(k-1)(k-2)/2` 个等差子数组

## 解法详解

### 解法一：DP 空间优化（推荐）
```cpp
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        int n = nums.size(), dp = 0, total = 0;
        for (int i = 2; i < n; i++) {
            if (nums[i] - nums[i-1] == nums[i-1] - nums[i-2]) {
                dp++;
                total += dp;
            } else {
                dp = 0;
            }
        }
        return total;
    }
};
```

### 解法二：数学计数（统计段长）
```cpp
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        int n = nums.size(), total = 0, len = 2;
        for (int i = 2; i < n; i++) {
            if (nums[i] - nums[i-1] == nums[i-1] - nums[i-2]) {
                len++;
            } else {
                if (len >= 3) total += (len - 1) * (len - 2) / 2;
                len = 2;
            }
        }
        if (len >= 3) total += (len - 1) * (len - 2) / 2;
        return total;
    }
};
```

## 易错点
- ✗ 忘记 dp 重置为 0 → ✓ 公差变化时必须 `dp = 0`
- ✗ 段长公式忘记处理最后一段 → ✓ 循环结束后还要检查 `if (len >= 3)`
- ✗ 混淆子数组和子序列 → ✓ 本题是**连续子数组**（subarray），不是子序列

## 面试追问
**Q1: 如果改成子序列（不要求连续）怎么做？**
A: 那就是 LC 446. Arithmetic Slices II - Subsequence，需要用 `dp[i][diff]` 的哈希表方法，时间 O(n^2)。

**Q2: dp[i] = dp[i-1] + 1 的直觉是什么？**
A: 新加入一个元素延续等差段时，它能和之前所有以 i-1 结尾的等差子数组各延伸一个，再加上新的长度为3的子数组 [i-2,i-1,i]。

**Q3: 能否用滑动窗口？**
A: 可以，维护等差段的左右边界，段长为 k 时贡献 (k-1)(k-2)/2，本质和解法二相同。

## 相关题型
- [446. Arithmetic Slices II - Subsequence](https://leetcode.com/problems/arithmetic-slices-ii-subsequence/) - 子序列版本
- [1027. Longest Arithmetic Subsequence](https://leetcode.com/problems/longest-arithmetic-subsequence/) - 最长等差子序列
- [978. Longest Turbulent Subarray](https://leetcode.com/problems/longest-turbulent-subarray/) - 类似的连续子数组DP
