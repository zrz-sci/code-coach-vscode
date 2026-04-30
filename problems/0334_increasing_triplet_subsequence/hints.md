# 334. Increasing Triplet Subsequence - 递增的三元子序列

## 核心思路

判断数组中是否存在长度为 3 的递增子序列。维护两个变量 `first`（最小值）和 `second`（次小值），遍历时若发现比 `second` 更大的元素，则找到了三元组。本质是 LIS 问题的特化版本（k=3）。

## 思维链

1. **暴力三重循环**：O(n^3) 枚举所有三元组 → 太慢
2. **LIS 思路**：用 patience sorting 维护递增序列，长度达到 3 即返回 → O(n log n)
3. **进一步优化**：k=3 时只需两个变量 first, second → O(n) 时间 O(1) 空间
4. **关键洞察**：`first` 更新不影响正确性 — 即使 first 更新到后面位置，second 之前已有更小的 first 存在

## 解法概览

| 解法 | 时间 | 空间 | 特点 | 推荐 |
|------|------|------|------|------|
| 贪心双变量 | O(n) | O(1) | 最优解，面试首选 | ⭐⭐⭐ |
| LIS (patience sorting) | O(n) | O(1) | 通用 LIS 框架特化 | ⭐ |

## 关键提示

1. `first` = 当前遇到的最小值，`second` = 在 first 之后遇到的最小的"比 first 大"的值
2. `first` 被更新到更后面的位置不影响正确性：因为 `second` 已经隐含了"在它之前存在一个更小的值"
3. 初始值设为 `INT_MAX`，保证第一个元素一定更新 first
4. 遇到 `nums[i] > second` 时直接返回 true

## 解法详解

### 解法一：贪心双变量（推荐）

**思路**：维护 first（最小）和 second（次小且在 first 之后），找到比 second 大的就返回。

```cpp
class Solution {
public:
    bool increasingTriplet(vector<int>& nums) {
        int first = INT_MAX, second = INT_MAX;
        for (int num : nums) {
            if (num <= first) {
                first = num;         // 更新最小值
            } else if (num <= second) {
                second = num;        // 更新次小值
            } else {
                return true;         // 找到第三个：num > second > (某个之前的first)
            }
        }
        return false;
    }
};
```

**复杂度**：时间 O(n)，空间 O(1)

**为什么 first 更新不破坏正确性？**
例如 `[1, 5, 0, 6]`：
- 遍历 1: first=1
- 遍历 5: second=5
- 遍历 0: first=0（first 更新了，但 second=5 仍隐含 "之前有个 < 5 的数"）
- 遍历 6: 6 > second=5 → 返回 true（三元组是 1,5,6 而不是 0,5,6）

### 解法二：前后缀数组

```cpp
class Solution {
public:
    bool increasingTriplet(vector<int>& nums) {
        int n = nums.size();
        if (n < 3) return false;

        vector<int> leftMin(n), rightMax(n);
        leftMin[0] = nums[0];
        for (int i = 1; i < n; i++)
            leftMin[i] = min(leftMin[i-1], nums[i]);

        rightMax[n-1] = nums[n-1];
        for (int i = n-2; i >= 0; i--)
            rightMax[i] = max(rightMax[i+1], nums[i]);

        for (int i = 1; i < n-1; i++)
            if (leftMin[i] < nums[i] && nums[i] < rightMax[i])
                return true;

        return false;
    }
};
```

**复杂度**：时间 O(n)，空间 O(n)

## 易错点

- ✗ 用 `<` 比较 first/second → ✓ 用 `<=`（等于时不应推进到下一层）
- ✗ 认为 first 更新后 second 失效 → ✓ second 隐含了"之前存在比它小的元素"
- ✗ 初始值设为 `nums[0]` → ✓ 设为 `INT_MAX` 更简洁
- ✗ 数组长度 < 3 时未特判 → ✓ 虽然循环也能处理，但面试时显式检查更清晰

## 面试追问

**Q1: 如果要求返回具体的三元组下标，怎么修改？**
→ 额外记录 first_idx 和 second_idx。注意 first 更新时不能覆盖 first_idx（因为 second 对应的前一个 first 可能不同）。需要分别记录。

**Q2: 如果改为长度为 k 的递增子序列呢？**
→ 推广为 LIS 问题，用长度为 k 的 tails 数组 + 二分查找，O(n log k) 时间。

**Q3: 这道题和 LIS 的关系？**
→ 本题等价于判断 LIS >= 3。贪心双变量就是 patience sorting 中 tails 数组长度为 2 的特化。

## 相关题型

- [300. Longest Increasing Subsequence](https://leetcode.com/problems/longest-increasing-subsequence/) - LIS 通用版
- [354. Russian Doll Envelopes](https://leetcode.com/problems/russian-doll-envelopes/) - 二维 LIS
- [456. 132 Pattern](https://leetcode.com/problems/132-pattern/) - 类似的贪心 + 栈思路
