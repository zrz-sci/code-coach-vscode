# 624. 数组列表中的最大距离 (Maximum Distance in Arrays)

## 核心思路

在 m 个已排序数组中，从**不同数组**各取一个数，使绝对差最大。关键洞察：每个排序数组的最小值是首元素、最大值是末元素。维护**全局最小值**和**全局最大值**，遍历时用当前数组的 max 与之前的 globalMin 做差、当前数组的 min 与之前的 globalMax 做差，取最大值。**一次遍历 O(m)** 解决。

## 思维链

1. **简化问题**：每个数组已排序，最小值 = 首元素，最大值 = 末元素
2. **约束条件**：两个数必须来自**不同**数组，不能取同一个数组的 min 和 max
3. **贪心思路**：最大距离 = 某个数组的 max - 另一个数组的 min（或反过来）
4. **遍历维护**：用 globalMin 和 globalMax 记录之前遍历过的数组中的极值
5. **逐步更新**：对每个新数组，先用它的极值与之前的全局极值算距离，再更新全局极值
6. **顺序保证不同数组**：先计算再更新，保证 globalMin/globalMax 不来自当前数组

## 解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|------|-----------|-----------|------|
| 贪心一次遍历 | 维护全局 min/max，逐步更新 | O(m) | O(1) | ⭐ |
| 暴力枚举 | 枚举所有数组对的 min/max 组合 | O(m^2) | O(1) | |

## 关键提示

1. **先计算后更新**：处理每个数组时，先用它的极值与全局极值算距离，然后再更新全局极值。这保证了 globalMin/globalMax 与当前数组来自不同数组
2. **初始化技巧**：用第一个数组的 min/max 初始化 globalMin 和 globalMax，从第二个数组开始遍历
3. **两个方向**：`curMax - globalMin` 和 `globalMax - curMin` 都要考虑
4. **不需要记录来源**：不需要记录全局极值来自哪个数组，因为遍历顺序天然保证了不同数组

## 解法详解

### 解法一：贪心一次遍历（推荐）

```cpp
class Solution {
public:
    int maxDistance(vector<vector<int>>& arrays) {
        int result = 0;
        // 用第一个数组初始化全局 min/max
        int globalMin = arrays[0].front();
        int globalMax = arrays[0].back();

        for (int i = 1; i < arrays.size(); i++) {
            int curMin = arrays[i].front();
            int curMax = arrays[i].back();

            // 当前数组的 max 与之前所有数组的 min 做差
            result = max(result, curMax - globalMin);
            // 之前所有数组的 max 与当前数组的 min 做差
            result = max(result, globalMax - curMin);

            // 更新全局极值 (放在计算之后!)
            globalMin = min(globalMin, curMin);
            globalMax = max(globalMax, curMax);
        }

        return result;
    }
};
```

**复杂度**：时间 O(m)，空间 O(1)

### 解法二：暴力枚举（仅作理解参照）

```cpp
class Solution {
public:
    int maxDistance(vector<vector<int>>& arrays) {
        int result = 0;
        for (int i = 0; i < arrays.size(); i++) {
            for (int j = i + 1; j < arrays.size(); j++) {
                result = max(result, abs(arrays[i].front() - arrays[j].back()));
                result = max(result, abs(arrays[i].back() - arrays[j].front()));
            }
        }
        return result;
    }
};
```

**复杂度**：时间 O(m^2)，空间 O(1)。m 可达 10^5，会超时。

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 先更新 globalMin/globalMax 再计算 | ✓ 先计算距离再更新极值 | 先更新会导致 globalMin/Max 可能来自当前数组 |
| ✗ 只考虑 `curMax - globalMin` | ✓ 同时考虑 `globalMax - curMin` | 最大距离可能出现在两个方向 |
| ✗ `abs(globalMax - globalMin)` | ✓ 分别对比不同数组的极值 | 全局 max 和 min 可能来自同一数组 |
| ✗ 从 i=0 开始遍历，初始 globalMin=INT_MAX | ✓ 用 arrays[0] 初始化，从 i=1 开始 | 初始化为 INT_MAX/INT_MIN 也可以但要确保第一次计算正确 |
| ✗ 使用 `arrays[i][0]` 取最大值 | ✓ 使用 `arrays[i].back()` 取最大值 | 数组升序排列，最后一个元素才是最大值 |

## 面试追问

**Q1: 为什么 "先计算后更新" 能保证两个数来自不同数组？**

A: 遍历到第 i 个数组时，globalMin 和 globalMax 只包含前 i-1 个数组的信息。因此 `curMax - globalMin` 中的 curMax 来自第 i 个数组，globalMin 来自前 i-1 个数组中的某个，它们一定是不同数组。计算完成后再更新全局极值，不会影响当前轮的正确性。

**Q2: 如果数组没有预排序，该怎么解决？**

A: 对每个数组只需要知道最小值和最大值，可以 O(k) 扫描得到（k 为数组长度）。总时间复杂度 O(sum of all array lengths)。排序只是让我们用 O(1) 就能获取极值（首尾元素），但核心贪心逻辑不变。

**Q3: 如果要求返回最大距离对应的两个元素的数组索引，怎么修改？**

A: 在更新 result 时记录 bestI 和 bestJ。需要在 globalMin/globalMax 中额外记录它们来自哪个数组的索引 minIdx 和 maxIdx。当 `curMax - globalMin` 更新 result 时，bestI = minIdx, bestJ = i；当 `globalMax - curMin` 更新 result 时，bestI = maxIdx, bestJ = i。

## 相关题型

- [53. 最大子数组和](../0053_maximum_subarray/) - 一次遍历维护状态的贪心思想
- [121. 买卖股票的最佳时机](../0121_best_time_to_buy_and_sell_stock/) - 维护历史最小值求最大差值
- [1200. 最小绝对差](../1200_minimum_absolute_difference/) - 排序数组中的差值问题
