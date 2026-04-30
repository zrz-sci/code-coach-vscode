# 154. 寻找旋转排序数组中的最小值 II

## 核心思路
本质是在含重复元素的旋转排序数组中二分查找最小值，关键在于当 nums[mid] == nums[right] 时无法判断最小值在哪半边，只能收缩右边界。

## 思维链
1. **暴力**: 线性扫描找最小值 → O(n)
2. **观察结构**: 旋转数组由两段有序部分组成，最小值是"断崖"处
3. **无重复版(153题)**: 比较 nums[mid] 与 nums[right]，确定最小值在哪半边 → O(log n)
4. **有重复的困境**: 当 nums[mid] == nums[right] 时，无法判断方向
5. **关键洞察**: nums[mid] == nums[right] 时，right-- 安全收缩(不会跳过最小值)
6. **最坏情况**: 全部相同如 [1,1,1,1,1]，退化为 O(n)

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 二分搜索(与right比较) | 三分支判断 | O(log n) ~ O(n) | O(1) | ⭐ 必知 |
| 线性扫描 | 遍历取最小 | O(n) | O(1) | 兜底方案 |

## 关键提示
1. **为什么比较 mid 和 right 而非 left?** 因为最小值可能在左半边，比较left无法正确收缩
2. **三种情况**:
   - `nums[mid] < nums[right]`: 最小值在 [left, mid]，令 right = mid
   - `nums[mid] > nums[right]`: 最小值在 (mid, right]，令 left = mid + 1
   - `nums[mid] == nums[right]`: 无法判断，安全操作 right--
3. **right-- 为什么安全?** 即使 nums[right] 是最小值，nums[mid] 与它相等，mid 还在范围内
4. **最坏 O(n)**: 如 [1,1,1,0,1,1,1] 每次只能 right-- 一步

```
示例: nums = [2, 2, 2, 0, 1]

     left=0  mid=2  right=4
     [2, 2, 2, 0, 1]
     nums[mid]=2 > nums[right]=1
     → 最小值在右半: left = mid+1 = 3

     left=3  mid=3  right=4
     [_, _, _, 0, 1]
     nums[mid]=0 < nums[right]=1
     → 最小值在左半(含mid): right = mid = 3

     left=3  right=3 → 退出
     答案: nums[3] = 0

特殊情况: nums = [3, 3, 1, 3]
     left=0  mid=1  right=3
     nums[mid]=3 == nums[right]=3
     → 无法判断，right-- → right=2

     left=0  mid=1  right=2
     nums[mid]=3 > nums[right]=1
     → left = mid+1 = 2

     left=2  right=2 → 退出
     答案: nums[2] = 1
```

## 解法详解

### 解法1: 二分搜索 (最优解)

**思路**: 用 nums[mid] 与 nums[right] 比较来收缩搜索范围。重复元素时退化为线性收缩。

```cpp
class Solution {
public:
    int findMin(vector<int>& nums) {
        int left = 0, right = nums.size() - 1;
        while (left < right) {
            int mid = left + (right - left) / 2;
            if (nums[mid] < nums[right]) {
                // mid 在右半段有序区，最小值在 [left, mid]
                right = mid;
            } else if (nums[mid] > nums[right]) {
                // mid 在左半段有序区，最小值在 (mid, right]
                left = mid + 1;
            } else {
                // nums[mid] == nums[right]，无法判断
                // right-- 安全收缩: 即使 right 是最小值，mid 还保留着
                right--;
            }
        }
        return nums[left];
    }
};
```

**关键点**:
- 循环条件 `left < right`，结束时 left == right 即答案位置
- `right = mid` 而非 `mid - 1`，因为 mid 可能就是答案
- `right--` 只在相等时使用，保证不跳过最小值

### 解法2: 线性扫描 (兜底)

**思路**: 简单遍历取最小值。当面试官问"如果不用二分"时的答案。

```cpp
class Solution {
public:
    int findMin(vector<int>& nums) {
        int minVal = nums[0];
        for (int i = 1; i < nums.size(); i++) {
            minVal = min(minVal, nums[i]);
        }
        return minVal;
    }
};
```

**关键点**:
- 时间固定 O(n)，当重复元素很多时与二分的最坏情况一样
- 代码简洁，作为验证方案有价值

## 易错点

1. **比较对象选错**
```
✗ if (nums[mid] < nums[left])   // 反例: [3,1,2], mid=1时无法正确判断
✓ if (nums[mid] < nums[right])  // 始终与right比较
```

2. **收缩方向错误**
```
✗ if (nums[mid] < nums[right]) right = mid - 1;  // mid可能是答案，不能跳过
✓ if (nums[mid] < nums[right]) right = mid;       // 保留mid
```

3. **相等时错误操作**
```
✗ if (nums[mid] == nums[right]) left = mid + 1;  // 可能跳过最小值
✓ if (nums[mid] == nums[right]) right--;          // 安全收缩
```

## 面试追问

**Q1**: 这题和153题(无重复)有什么区别？
→ 无重复时严格二分O(log n)。有重复时增加 `nums[mid] == nums[right]` 的 `right--` 分支，最坏退化O(n)。

**Q2**: 为什么不能比较 nums[mid] 和 nums[left]？
→ 考虑 [3,4,5,1,2]，left=0, mid=2, nums[mid]=5 > nums[left]=3，但最小值在右边。而如果用right比较，nums[mid]=5 > nums[right]=2，正确判断 left=mid+1。

**Q3**: 如果要求在旋转数组中搜索特定值(含重复)，怎么做？(LC 81)
→ 同样的三分支框架。先判断哪半边有序，在有序半边用常规二分判断target是否在范围内。相等时同样 right--。

## 相关题型
- [153. 寻找旋转排序数组中的最小值](../0153_find_minimum_in_rotated_sorted_array/) — 无重复版本，纯二分 O(log n)
- [33. 搜索旋转排序数组](../0033_search_in_rotated_sorted_array/) — 无重复，搜索特定值
- [81. 搜索旋转排序数组 II](../0081_search_in_rotated_sorted_array_ii/) — 有重复，搜索特定值
