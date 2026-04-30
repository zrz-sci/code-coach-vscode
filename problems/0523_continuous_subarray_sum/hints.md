# 523. 连续的子数组和 (Continuous Subarray Sum)

**难度**: Medium | **标签**: Array, Hash Table, Math, Prefix Sum

## 核心思路

利用**前缀和 + 同余定理**：如果 `prefixSum[j] % k == prefixSum[i] % k`（j > i），则子数组 `[i+1, j]` 的和是 k 的倍数。用哈希表记录每个余数**首次出现**的位置，确保子数组长度至少为 2。

## 思维链

1. **暴力**: 枚举所有子数组并求和 -> O(n^2)，超时
2. **前缀和优化**: `sum(i,j) = prefix[j] - prefix[i]`
3. **同余定理**: `sum(i,j) % k == 0` 等价于 `prefix[j] % k == prefix[i] % k`
4. **哈希加速**: 用 map 记录余数首次出现的索引
5. **长度约束**: `j - i >= 2` 确保子数组长度至少为 2

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| 前缀和 + 哈希表（同余） | O(n) | O(min(n,k)) | ⭐⭐⭐⭐⭐ |
| 暴力枚举 | O(n^2) | O(1) | ⭐⭐ |

## 关键提示

1. **初始化**: 哈希表需要预设 `{0: -1}`，表示前缀和余数为 0 在索引 -1 处出现
2. **长度要求**: 只在 `i - map[remainder] >= 2` 时返回 true
3. **只记录首次出现**: 余数相同时，保留最早的索引以最大化子数组长度
4. **0 的处理**: 0 始终是 k 的倍数，两个连续 0 即满足条件

## 解法详解

### 解法一：前缀和 + 同余定理

```cpp
class Solution {
public:
    bool checkSubarraySum(vector<int>& nums, int k) {
        // 记录余数 -> 首次出现的索引
        unordered_map<int, int> remainderIndex;
        remainderIndex[0] = -1; // 前缀和为 0 的余数在 -1 位置
        
        int prefixSum = 0;
        for (int i = 0; i < nums.size(); i++) {
            prefixSum += nums[i];
            int remainder = prefixSum % k;
            
            if (remainderIndex.count(remainder)) {
                // 子数组长度 >= 2
                if (i - remainderIndex[remainder] >= 2) {
                    return true;
                }
            } else {
                // 只记录首次出现的位置
                remainderIndex[remainder] = i;
            }
        }
        return false;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 不初始化 `{0: -1}` | ✓ `remainderIndex[0] = -1` | 漏掉前缀和本身就是 k 倍数的情况 |
| ✗ 每次都更新余数索引 | ✓ 只在首次出现时记录 | 需要最早的索引以满足长度 >= 2 |
| ✗ `i - map[r] >= 1` | ✓ `i - map[r] >= 2` | 子数组长度至少为 2 |
| ✗ 忘记处理负数余数 | ✓ C++ 中 `%` 对正数无此问题 | 本题 nums[i] >= 0 且 k >= 1 |

## 面试追问

**Q1**: 为什么哈希表初始化 `{0: -1}`？
> 当 prefix[i] % k == 0 时，子数组 [0..i] 的和是 k 的倍数。设虚拟起点为 -1，使得 i - (-1) >= 2 即 i >= 1（至少两个元素）。

**Q2**: 如果 k 可以为 0 怎么办？
> 需要特殊处理：k=0 时变成找连续子数组和为 0。但本题约束 k >= 1。

**Q3**: 这题和"和为 k 的倍数的子数组个数"有什么区别？
> 本题只需判断存在性且有长度约束；计数版本需要累加所有同余的前缀和对数。

## 相关题型

- [560. 和为 K 的子数组](https://leetcode.com/problems/subarray-sum-equals-k/) - 前缀和 + 哈希计数
- [974. 和可被 K 整除的子数组](https://leetcode.com/problems/subarray-sums-divisible-by-k/) - 同余计数
- [525. 连续数组](https://leetcode.com/problems/contiguous-array/) - 前缀和 + 哈希变体
