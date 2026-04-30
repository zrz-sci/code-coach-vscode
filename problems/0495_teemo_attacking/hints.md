# 495. Teemo Attacking - 提莫攻击

## 核心思路
每次攻击产生 [t, t+duration-1] 的中毒区间。如果两次攻击间隔 >= duration 则贡献完整 duration，否则只贡献间隔时间。最后一次攻击固定贡献 duration。

## 思维链
1. 相邻两次攻击间隔 gap = timeSeries[i] - timeSeries[i-1]
2. 如果 gap >= duration，当前攻击贡献完整 duration
3. 如果 gap < duration，中毒区间重叠，实际只贡献 gap
4. 每次贡献 min(gap, duration)
5. 最后一次攻击额外贡献 duration

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ 线性扫描(间隔法) | O(n) | O(1) | 最优，一次遍历 |
| 区间合并 | O(n) | O(1) | 思路清晰但代码稍多 |

## 关键提示
1. 核心公式：`total += min(gap, duration)`
2. 最后一次攻击要额外加 duration
3. 数组已排序，不需要额外排序
4. duration = 0 时结果为 0

## 解法详解

### 解法一：线性扫描 ⭐
**思路**：遍历相邻攻击的间隔，每次贡献 min(间隔, duration)，最后加上最后一次的 duration。

```cpp
class Solution {
public:
    int findPoisonedDuration(vector<int>& timeSeries, int duration) {
        if (timeSeries.empty()) return 0;
        int total = 0;
        for (int i = 1; i < timeSeries.size(); i++) {
            total += min(timeSeries[i] - timeSeries[i - 1], duration);
        }
        return total + duration;
    }
};
```

### 解法二：区间合并思路
**思路**：维护当前中毒结束时间 end，如果新攻击在 end 之前则延长，否则累加并重置。

```cpp
class Solution {
public:
    int findPoisonedDuration(vector<int>& timeSeries, int duration) {
        if (timeSeries.empty()) return 0;
        int total = 0;
        int end = timeSeries[0] + duration;
        for (int i = 1; i < timeSeries.size(); i++) {
            if (timeSeries[i] < end) {
                total += timeSeries[i] - timeSeries[i - 1];
            } else {
                total += duration;
            }
            end = timeSeries[i] + duration;
        }
        total += duration;
        return total;
    }
};
```

## 易错点
- ✗ 忘记最后一次攻击的 duration
- ✓ 循环结束后 `total += duration`

- ✗ 用 `timeSeries[i+1] - timeSeries[i]` 导致越界
- ✓ 用 `timeSeries[i] - timeSeries[i-1]`，从 i=1 开始

- ✗ duration 为 0 时未处理
- ✓ duration=0 时 min(gap, 0) = 0，结果自然为 0

## 面试追问

**Q1: 如果攻击时间不是有序的怎么办？**
> 先排序 O(n log n)，然后用同样的方法。

**Q2: 如果有多种毒素，持续时间不同，如何计算总中毒时间？**
> 转化为区间合并问题：生成所有中毒区间 [t, t+d)，排序后合并重叠区间，统计合并后的总长度。

**Q3: 如何用 O(1) 空间判断某个时刻 t 是否中毒？**
> 二分查找最后一个 <= t 的攻击时间 ts[i]，检查 t < ts[i] + duration。

## 相关题型
- [LeetCode 56] Merge Intervals（区间合并）
- [LeetCode 252] Meeting Rooms（区间判断）
- [LeetCode 253] Meeting Rooms II（区间重叠计数）
