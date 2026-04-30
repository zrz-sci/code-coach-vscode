# 532. 数组中的 k-diff 数对 (K-diff Pairs in an Array)

**难度**: Medium | **标签**: Array, Hash Table, Two Pointers, Binary Search, Sorting

## 核心思路

找出所有**唯一**的 k-diff 数对 (a, b)，满足 |a - b| = k。利用哈希表统计频次，对每个元素检查 `num + k` 是否存在（k > 0）或该元素出现次数是否 >= 2（k = 0）。

## 思维链

1. 需要"唯一"数对 → 用 set 或 map 去重
2. k > 0 时：对每个不重复的 num，检查 num + k 是否存在
3. k = 0 时：找出出现次数 >= 2 的元素个数
4. k < 0：绝对值差不可能为负数，直接返回 0（但题目保证 k >= 0）

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| 哈希表计数 | O(n) | O(n) | ⭐⭐⭐⭐⭐ |
| 排序 + 双指针 | O(n log n) | O(1) | ⭐⭐⭐⭐ |
| 排序 + 二分查找 | O(n log n) | O(1) | ⭐⭐⭐ |

## 关键提示

1. "唯一数对"意味着 (1,3) 和 (3,1) 算同一对，且重复元素构成的对只算一次
2. k = 0 是特殊情况，需要找出现次数 >= 2 的元素
3. 用哈希表统计频次可以同时处理 k > 0 和 k = 0 两种情况
4. 只需要查找 `num + k`（不需要同时查 `num - k`），因为遍历所有 num 时每对会被较小的那个找到

## 解法详解

### 解法一：哈希表计数（推荐）

```cpp
class Solution {
public:
    int findPairs(vector<int>& nums, int k) {
        unordered_map<int, int> freq;
        for (int num : nums) freq[num]++;
        
        int result = 0;
        for (auto& [num, cnt] : freq) {
            if (k > 0 && freq.count(num + k)) {
                result++;
            } else if (k == 0 && cnt >= 2) {
                result++;
            }
        }
        return result;
    }
};
```

### 解法二：排序 + 双指针

```cpp
class Solution {
public:
    int findPairs(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int n = nums.size(), result = 0;
        int left = 0, right = 1;
        
        while (left < n && right < n) {
            if (left == right || nums[right] - nums[left] < k) {
                right++;
            } else if (nums[right] - nums[left] > k) {
                left++;
            } else {
                // nums[right] - nums[left] == k
                result++;
                left++;
                // 跳过重复的 left
                while (left < n && nums[left] == nums[left - 1]) left++;
            }
        }
        return result;
    }
};
```

### 解法三：排序 + 二分查找

```cpp
class Solution {
public:
    int findPairs(vector<int>& nums, int k) {
        sort(nums.begin(), nums.end());
        int n = nums.size(), result = 0;
        
        for (int i = 0; i < n; i++) {
            // 跳过重复元素
            if (i > 0 && nums[i] == nums[i - 1]) continue;
            // 在 i+1 之后查找 nums[i] + k
            if (binary_search(nums.begin() + i + 1, nums.end(), nums[i] + k)) {
                result++;
            }
        }
        return result;
    }
};
```

## 易错点

| 错误写法 | 正确写法 |
|---------|---------|
| ✗ 同时查 `num+k` 和 `num-k` 导致重复计数 | ✓ 只查 `num+k`，遍历所有元素时自然覆盖 |
| ✗ 忽略 k=0 的特殊情况 | ✓ k=0 时检查频次 >= 2 |
| ✗ 双指针中 left==right 时没有移动 right | ✓ 需要确保 left != right |
| ✗ 双指针没有跳过重复的 left 值 | ✓ 找到一对后跳过所有相同的 left |

## 面试追问

**Q1: 为什么只查 num+k 不查 num-k？**

> 因为遍历所有元素时，如果 (a, b) 是一对且 a < b，那么遍历到 a 时会查 a+k=b。如果同时查 a-k，会导致同一对被计数两次。

**Q2: k=0 时哈希表做法的时间复杂度？**

> 仍然是 O(n)。频次统计是 O(n)，遍历 map 找 cnt>=2 是 O(unique 元素数) <= O(n)。

**Q3: 如果要输出所有具体的数对而不是数量，怎么改？**

> 在 result++ 的地方改为将 {num, num+k} 加入结果数组即可。

## 相关题型

- [1. 两数之和](https://leetcode.com/problems/two-sum/) - 哈希表查找配对
- [167. 两数之和 II](https://leetcode.com/problems/two-sum-ii-input-array-is-sorted/) - 排序 + 双指针
- [219. 存在重复元素 II](https://leetcode.com/problems/contains-duplicate-ii/) - 哈希表 + 差值
