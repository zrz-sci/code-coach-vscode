# 446. 等差数列划分 II - 子序列 (Arithmetic Slices II - Subsequence)

## 核心思路
对每个位置 i 维护一个哈希表 `dp[i]`，记录"以 i 结尾、公差为 d 的等差子序列（长度 >= 2 的弱序列）有多少个"。遍历所有 (j, i) 对，`dp[i][d] += dp[j][d] + 1`，其中 `dp[j][d]` 贡献的是长度 >= 3 的真等差子序列。

## 思维链
1. 等差子序列需要至少 3 个元素，但 DP 中先追踪长度 >= 2 的"弱序列"
2. dp[i][d] = 以 nums[i] 结尾、公差为 d 的弱等差子序列个数
3. 对每对 (j, i) 其中 j < i，计算 d = nums[i] - nums[j]
4. dp[i][d] += dp[j][d] + 1（+1 是只有 (nums[j], nums[i]) 这个新二元组）
5. dp[j][d] 这部分代表把长度 >= 2 的序列延长到 >= 3，贡献到答案
6. 所以 `ans += dp[j][d]`

## 解法概览表

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| ⭐ 解法1: DP + HashMap | dp[i][d] 记录弱等差子序列数 | O(n^2) | O(n^2) | 唯一正解 |

## 关键提示
1. **公差 d 要用 long**：nums[i] 可达 2^31-1，差值可能超 int
2. `ans += dp[j][d]`：只有 dp[j][d] > 0 时才有长度 >= 3 的真子序列
3. `dp[i][d] += dp[j][d] + 1`：+1 是新产生的二元组 (j, i)
4. 用 `unordered_map<long, int>` 存储每个位置的公差计数

## 解法详解

### 解法1: DP + HashMap ⭐
```cpp
class Solution {
public:
    int numberOfArithmeticSlices(vector<int>& nums) {
        int n = nums.size(), ans = 0;
        vector<unordered_map<long, int>> dp(n);
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < i; j++) {
                long d = (long)nums[i] - nums[j];
                int cnt = dp[j].count(d) ? dp[j][d] : 0;
                ans += cnt;           // cnt 个长度>=2的弱序列 → 延长后成为>=3的真序列
                dp[i][d] += cnt + 1;  // +1 是新二元组 (j, i)
            }
        }
        return ans;
    }
};
```

## 易错点
| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| `int d = nums[i] - nums[j]` | `long d = (long)nums[i] - nums[j]` | 差值溢出 int |
| `ans += dp[i][d]` | `ans += dp[j][d]` (在更新 dp[i][d] 之前) | 贡献来自 j 不是 i |
| `ans += cnt + 1` | `ans += cnt` | +1 的二元组还不够 3 个元素，不算真等差 |
| 用 `map` | 用 `unordered_map` | map 的 O(logn) 查找使总复杂度变成 O(n^2 logn) |

## 面试追问

**Q1: 为什么 ans += dp[j][d] 而不是 dp[i][d]？**
> dp[j][d] 代表以 j 结尾、公差 d 的长度 >= 2 的弱序列数。加上 nums[i] 后变成长度 >= 3 的真等差子序列，所以正好贡献 dp[j][d] 个。

**Q2: 时间复杂度能否优化到 O(n^2) 以下？**
> 目前没有已知的 sub-quadratic 算法。O(n^2) 是此问题的标准上界。

**Q3: 与第 413 题（等差数列划分 I）有什么区别？**
> 413 题只需连续子数组，用简单计数即可 O(n)。446 题是子序列（不要求连续），需要 DP + HashMap。

## 相关题型
- [413. 等差数列划分](https://leetcode.com/problems/arithmetic-slices/) - 连续子数组版本
- [1027. 最长等差子序列](https://leetcode.com/problems/longest-arithmetic-subsequence/) - 求最长而非计数
- [873. 最长的斐波那契子序列的长度](https://leetcode.com/problems/length-of-longest-fibonacci-subsequence/) - 类似 DP+HashMap
