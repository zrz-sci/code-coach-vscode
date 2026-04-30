# 453. 最小操作次数使数组元素相等 (Minimum Moves to Equal Array Elements)

## 核心思路
每次给 n-1 个元素 +1 等价于给 1 个元素 -1。问题转化为：把所有元素减到最小值需要多少步。

## 思维链
1. 每次操作：n-1 个元素 +1 → 等价于 1 个元素 -1（相对差不变）
2. 目标：所有元素相等 → 等价于所有元素都减到最小值
3. 答案 = sum(nums[i] - min) = sum(nums) - n * min

## 解法概览表

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| Solution1 | 数学等价变换 ⭐ | O(n) | O(1) | 最优解 |
| Solution2 | 排序后逐差累加 | O(n log n) | O(1) | 直观理解 |

## 关键提示
1. **逆向思维**：给 n-1 个 +1 等价于给 1 个 -1
2. **答案公式**：`sum(nums) - n * min(nums)`
3. **溢出风险**：sum 可能溢出 int，但题目保证答案在 32-bit 范围内

## 解法详解

### Solution 1: 数学等价变换 ⭐
**思路**：给 n-1 个元素 +1 相当于给 1 个元素 -1。最终所有元素变为 min，每个元素贡献 nums[i] - min 次操作。

```cpp
class Solution {
public:
    int minMoves(vector<int>& nums) {
        int mn = *min_element(nums.begin(), nums.end());
        int res = 0;
        for (int x : nums) res += x - mn;
        return res;
    }
};
```

### Solution 2: 排序后逐差累加
**思路**：排序后，从小到大逐个元素计算与最小值的差。

```cpp
class Solution {
public:
    int minMoves(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        int res = 0;
        for (int i = 1; i < nums.size(); i++) {
            res += nums[i] - nums[0];
        }
        return res;
    }
};
```

## 易错点
- ✗ 直接模拟每步操作 → ✓ 用数学等价变换 O(1) 推导
- ✗ 认为最终值是 max → ✓ 等价变换后目标是减到 min
- ✗ 忽略整数溢出 → ✓ 累加差值而非先算总和再减，更安全

## 面试追问
**Q1: 为什么 "n-1 个 +1" 等价于 "1 个 -1"？**
A: 关注的是元素间的相对差异。每次操作，未被选中的那个元素相对于其他元素"减少"了 1。最终所有元素相等等价于所有差异消除。

**Q2: 如果改为每次给 n-1 个元素 +k（k 任意正整数）呢？**
A: 等价于每次给 1 个元素 -k，即可以一次减任意正整数。那答案变为 max - min（贪心选最大的减到最小）。

**Q3: 与 462 题的区别？**
A: 462 题是每次给 1 个元素 +1 或 -1，目标值是中位数。本题目标值是最小值。

## 相关题型
- [462. 最小操作次数使数组元素相等 II](https://leetcode.com/problems/minimum-moves-to-equal-array-elements-ii/) - 每次 +1/-1 一个元素
- [2141. 同时运行 N 台电脑的最长时间](https://leetcode.com/problems/maximum-running-time-of-n-computers/) - 类似数学推导
