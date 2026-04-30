# 259. 3Sum Smaller (较小的三数之和)

**难度**: Medium | **标签**: Array, Two Pointers, Binary Search, Sorting

## 核心思路

计算满足 `nums[i] + nums[j] + nums[k] < target` 的三元组数量。核心方法是**排序 + 双指针**：固定第一个数后，用双指针在剩余部分找满足条件的对数。关键洞察是：排序后若 `nums[i] + nums[left] + nums[right] < target`，则 `left` 到 `right-1` 之间的所有 right 值都满足条件，一次性计数 `right - left` 个。

## 思维链

1. **暴力思路**: 三重循环 O(n^3)，能否优化？
2. **排序预处理**: 排序不影响三元组计数（只关心数量，不关心下标）
3. **固定一个数**: 外层循环固定 `nums[i]`，转化为两数之和问题
4. **双指针**: 在 `[i+1, n-1]` 范围用 left/right 指针夹逼
5. **批量计数**: 若 `sum < target`，则 `right` 从 `left+1` 到当前 `right` 都满足，计数 `right - left`
6. **指针移动**: sum < target 时 left++（尝试更大的和），否则 right--

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| 排序 + 双指针 | O(n^2) | O(1) | ⭐⭐⭐⭐⭐ |
| 排序 + 二分搜索 | O(n^2 log n) | O(1) | ⭐⭐⭐ |
| 暴力枚举 | O(n^3) | O(1) | ⭐⭐ |

## 关键提示

- 排序后双指针：`sum < target` 时，计数 `right - left` 而非 1
- 与 3Sum 的区别：这里找的是"小于"而非"等于"，且只需计数不需去重
- 排序是合法的，因为题目只问数量，不问具体下标

## 解法详解

### 解法1: 排序 + 双指针 (最优解)

**思路**: 排序后固定 i，用双指针 left/right 找满足 sum < target 的对数。

```cpp
class Solution {
public:
    int threeSumSmaller(vector<int>& nums, int target) {
        int n = nums.size();
        if (n < 3) return 0;
        
        sort(nums.begin(), nums.end());
        int count = 0;
        
        for (int i = 0; i < n - 2; i++) {
            int left = i + 1, right = n - 1;
            while (left < right) {
                int sum = nums[i] + nums[left] + nums[right];
                if (sum < target) {
                    // nums[left] 和 nums[left+1..right] 都满足
                    count += right - left;
                    left++;
                } else {
                    right--;
                }
            }
        }
        return count;
    }
};
```

**复杂度**: 时间 O(n^2)，空间 O(1)

### 解法2: 排序 + 二分搜索

**思路**: 固定 i 和 j，二分查找满足条件的最大 k。

```cpp
class Solution {
public:
    int threeSumSmaller(vector<int>& nums, int target) {
        int n = nums.size();
        if (n < 3) return 0;
        
        sort(nums.begin(), nums.end());
        int count = 0;
        
        for (int i = 0; i < n - 2; i++) {
            for (int j = i + 1; j < n - 1; j++) {
                int remain = target - nums[i] - nums[j];
                // 找最后一个 < remain 的位置
                int k = lower_bound(nums.begin() + j + 1, nums.end(), remain) 
                        - nums.begin() - 1;
                if (k > j) {
                    count += k - j;
                }
            }
        }
        return count;
    }
};
```

**复杂度**: 时间 O(n^2 log n)，空间 O(1)

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `count++` | ✓ `count += right - left` | 满足条件时所有 [left+1,right] 的 right 都合法 |
| ✗ sum <= target | ✓ sum < target | 题目要求严格小于 |
| ✗ 不排序直接双指针 | ✓ 先排序 | 双指针依赖有序性 |
| ✗ `i < n` | ✓ `i < n - 2` | 至少需要三个元素 |

## 面试追问

**Q1**: 为什么 `sum < target` 时可以一次加 `right - left`？
→ **A**: 排序后，固定 left，right 从当前往 left+1 移动，和只会变小，全都 < target。

**Q2**: 如果要返回具体的三元组呢？
→ **A**: 在 `sum < target` 时，遍历 `left+1` 到 `right` 收集所有三元组。

**Q3**: 与 3Sum (LC 15) 的双指针有什么区别？
→ **A**: 3Sum 找等于，移动两个指针跳过重复；这里找小于，left++ 后批量计数。

## 相关题型

- [15. 3Sum](https://leetcode.com/problems/3sum/) - 三数之和（等于零）
- [16. 3Sum Closest](https://leetcode.com/problems/3sum-closest/) - 最接近的三数之和
- [611. Valid Triangle Number](https://leetcode.com/problems/valid-triangle-number/) - 有效三角形个数（类似双指针计数）
