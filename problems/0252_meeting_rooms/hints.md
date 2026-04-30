# 252. Meeting Rooms (会议室)

**难度**: Easy | **标签**: Array, Sorting

## 核心思路

判断所有会议是否存在时间冲突 = **排序后检查相邻区间是否重叠**。将区间按起始时间排序后，如果任何相邻会议的前一个结束时间晚于后一个开始时间，则无法全部参加。

## 思维链

1. **理解问题本质**: 一个人能参加所有会议 ⟺ 所有会议时间互不重叠
2. **暴力思路**: 两两比较所有区间对，O(n^2)，能否优化？
3. **排序切入**: 按 start 排序后，重叠只可能发生在相邻区间之间
4. **判定条件**: 遍历排序后的区间，若 `intervals[i-1][1] > intervals[i][0]`，则重叠
5. **边界处理**: 空数组或只有一个区间时，直接返回 true
6. **返回结果**: 遍历完无冲突返回 true，否则 false

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| 排序 + 线性扫描 | O(n log n) | O(1) | ⭐⭐⭐⭐⭐ |
| 暴力两两比较 | O(n^2) | O(1) | ⭐⭐ |

## 关键提示

- 排序依据是**起始时间**，起始时间相同时按结束时间排序也可以
- 注意区间端点相等的情况：`[1,5]` 和 `[5,10]` 不算重叠（一个结束另一个才开始）
- 判定条件是严格大于 `>`，不是 `>=`

## 解法详解

### 解法1: 排序 + 线性扫描 (最优解)

**思路**: 按起始时间排序，检查每对相邻区间是否重叠。

```cpp
class Solution {
public:
    bool canAttendMeetings(vector<vector<int>>& intervals) {
        // 按起始时间排序
        sort(intervals.begin(), intervals.end());
        
        // 检查相邻区间是否重叠
        for (int i = 1; i < intervals.size(); i++) {
            if (intervals[i - 1][1] > intervals[i][0]) {
                return false;
            }
        }
        return true;
    }
};
```

**复杂度**: 时间 O(n log n)，空间 O(1)（原地排序）

### 解法2: 暴力两两比较

**思路**: 检查每一对区间是否有重叠。

```cpp
class Solution {
public:
    bool canAttendMeetings(vector<vector<int>>& intervals) {
        int n = intervals.size();
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                // 两个区间重叠的条件
                if (intervals[i][0] < intervals[j][1] && 
                    intervals[j][0] < intervals[i][1]) {
                    return false;
                }
            }
        }
        return true;
    }
};
```

**复杂度**: 时间 O(n^2)，空间 O(1)

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `intervals[i-1][1] >= intervals[i][0]` | ✓ `intervals[i-1][1] > intervals[i][0]` | 端点相等不算重叠 |
| ✗ 忘记处理空数组 | ✓ 空数组直接返回 true | for 循环不执行自然返回 true |
| ✗ 按结束时间排序 | ✓ 按起始时间排序 | 按 start 排序才能正确检测相邻重叠 |

## 面试追问

**Q1**: 如果会议有重叠，最少需要几个会议室？
→ **A**: 这是 LC 253 Meeting Rooms II，使用最小堆或扫描线。

**Q2**: 如何找出所有冲突的会议对？
→ **A**: 排序后，对每个区间向后检查所有与其重叠的区间（因为排好序了，一旦不重叠就可以 break）。

**Q3**: 如果区间是开区间或半开半闭区间怎么办？
→ **A**: 根据区间类型调整比较条件（`>` vs `>=`）。

## 相关题型

- [56. Merge Intervals](https://leetcode.com/problems/merge-intervals/) - 合并重叠区间
- [253. Meeting Rooms II](https://leetcode.com/problems/meeting-rooms-ii/) - 最少会议室数量
- [435. Non-overlapping Intervals](https://leetcode.com/problems/non-overlapping-intervals/) - 移除最少区间使无重叠
