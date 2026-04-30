# 220. 存在重复元素 III (Hard)

**Tags:** Array, Bucket Sort, Ordered Set, Sorting, Sliding Window

## 核心思路

在滑动窗口（大小 indexDiff）内，找两个元素值差不超过 valueDiff。两种经典方案：**有序集合（set/multiset）** 滑动窗口 + lower_bound 查找，或者 **桶排序** 把值域分桶后在相邻桶中查找。

## 思维链

1. **暴力思考**：双重循环，对每个 i 检查 j in [max(0,i-indexDiff), i)，判断 |nums[i]-nums[j]| <= valueDiff。时间 O(N * indexDiff)，TLE。
2. **排序思考**：排序后相邻元素值差最小，但打乱了下标关系，难以维护 indexDiff 约束。
3. **滑动窗口 + 有序集合**：维护大小为 indexDiff 的有序窗口。每次加入 nums[i]，用 `lower_bound(nums[i] - valueDiff)` 查找是否有元素 <= nums[i] + valueDiff。时间 O(N log k)。
4. **桶排序优化**：桶大小 = valueDiff + 1，同桶内元素差一定 <= valueDiff。还需检查相邻桶。窗口滑动时维护桶，时间 O(N)。
5. **溢出处理**：nums[i] 范围为 [-10^9, 10^9]，差值可能溢出 int，需要 long long。
6. **最终方案**：桶排序 O(N) 最优，有序集合 O(N log k) 更直观。

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 桶排序 ⭐ | O(N) | O(indexDiff) | 最优，但理解成本高 |
| 有序集合 (set) | O(N log indexDiff) | O(indexDiff) | 直观，面试推荐 |
| 暴力双循环 | O(N * indexDiff) | O(1) | 超时 |

## 关键提示

### 桶排序思路图解

```
valueDiff = 3, 桶大小 = 4

值:     0  1  2  3 | 4  5  6  7 | 8  9  10  11 | ...
桶号:   [  桶 0   ] [  桶 1   ] [   桶 2     ]

同一个桶内: |a - b| <= 3 (一定满足 valueDiff 条件)
相邻桶:     需要额外检查 |a - b| <= 3

例: nums = [1, 5, 9, 1, 5, 9], indexDiff=2, valueDiff=3
i=0: 1 → 桶0, map={0:1}
i=1: 5 → 桶1, 检查桶0有1, |5-1|=4>3. map={0:1, 1:5}
i=2: 9 → 桶2, 检查桶1有5, |9-5|=4>3. 删除i=0. map={1:5, 2:9}
i=3: 1 → 桶0, 检查桶1有5, |1-5|=4>3. 删除i=1. map={0:1, 2:9}
...无匹配 → false
```

### 有序集合思路图解

```
nums = [1, 2, 3, 1], indexDiff=3, valueDiff=0

i=0: set={1}, 无前驱可比较
i=1: set={1,2}, lower_bound(2-0)=2 找到2, |2-2|<=0 ✗ (自身)
     → 实际应先查再插: lower_bound(2)=end, 但1不满足
i=2: set={1,2,3}, lower_bound(3)=3, |3-3|<=0 ✗
i=3: set={1,2,3}, lower_bound(1-0)=1, |1-1|<=0 ✓ → true!
```

## 解法详解

### 解法1: 桶排序 (最优)

**思路**：桶大小 = valueDiff + 1。同桶元素差 <= valueDiff。还需检查相邻桶。每个桶最多一个元素（否则已找到答案）。

```cpp
class Solution {
public:
    bool containsNearbyAlmostDuplicate(vector<int>& nums, int indexDiff, int valueDiff) {
        int n = nums.size();
        unordered_map<long long, long long> buckets;
        long long w = (long long)valueDiff + 1;  // 桶宽度
        
        for (int i = 0; i < n; i++) {
            long long val = (long long)nums[i];
            // 计算桶号（处理负数：先偏移再整除）
            long long bucket = val >= 0 ? val / w : (val + 1) / w - 1;
            
            // 同桶：差值一定 <= valueDiff
            if (buckets.count(bucket)) return true;
            // 相邻桶：需验证差值
            if (buckets.count(bucket - 1) && val - buckets[bucket - 1] <= valueDiff)
                return true;
            if (buckets.count(bucket + 1) && buckets[bucket + 1] - val <= valueDiff)
                return true;
            
            buckets[bucket] = val;
            // 维护窗口大小
            if (i >= indexDiff) {
                long long old_val = (long long)nums[i - indexDiff];
                long long old_bucket = old_val >= 0 ? old_val / w : (old_val + 1) / w - 1;
                buckets.erase(old_bucket);
            }
        }
        return false;
    }
};
```

### 解法2: 有序集合 (set + lower_bound)

**思路**：维护大小为 indexDiff 的有序窗口。对当前元素 x，查找 set 中 >= x - valueDiff 的最小元素，判断它是否 <= x + valueDiff。

```cpp
class Solution {
public:
    bool containsNearbyAlmostDuplicate(vector<int>& nums, int indexDiff, int valueDiff) {
        set<long long> window;
        
        for (int i = 0; i < (int)nums.size(); i++) {
            long long val = nums[i];
            // 找 >= val - valueDiff 的最小元素
            auto it = window.lower_bound(val - valueDiff);
            // 如果该元素存在且 <= val + valueDiff，则找到答案
            if (it != window.end() && *it <= val + valueDiff) {
                return true;
            }
            
            window.insert(val);
            // 维护窗口大小为 indexDiff
            if (i >= indexDiff) {
                window.erase((long long)nums[i - indexDiff]);
            }
        }
        return false;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 用 int 计算差值 | ✓ 用 `long long` | nums[i] 范围 [-10^9, 10^9]，差值溢出 |
| ✗ 负数桶号用 `val/w` | ✓ `(val+1)/w - 1` | C++ 负数除法向零取整，需特殊处理 |
| ✗ `window.erase(nums[i - indexDiff])` 不转 long long | ✓ `window.erase((long long)nums[i-indexDiff])` | set 中存的是 long long |
| ✗ valueDiff=0 时桶宽度为 0 | ✓ 桶宽度 = valueDiff + 1 | 避免除零错误 |

## 面试追问

**Q1: 为什么桶排序方法中每个桶最多存一个元素？**
→ 如果一个桶中已有元素，新元素进来时差值一定 <= valueDiff，直接返回 true。所以每个桶永远最多一个元素。

**Q2: 如何推广到高维（如二维平面中找距离 <= d 的点对）？**
→ 二维可以用网格桶，桶大小为 d x d，检查周围 9 个桶。这是计算几何中 "桶排序找近邻" 的思想。

**Q3: 有序集合方法中，为什么不用 multiset？**
→ 可以用 multiset，但 set 也行，因为如果有重复元素且 valueDiff >= 0，在插入时就会找到已有的相同元素而返回 true。

## 相关题型

- [217. Contains Duplicate](https://leetcode.com/problems/contains-duplicate/) - 基础版
- [219. Contains Duplicate II](https://leetcode.com/problems/contains-duplicate-ii/) - 只限制 indexDiff
- [315. Count of Smaller Numbers After Self](https://leetcode.com/problems/count-of-smaller-numbers-after-self/) - 有序集合应用
- [352. Data Stream as Disjoint Intervals](https://leetcode.com/problems/data-stream-as-disjoint-intervals/) - 有序集合维护区间
