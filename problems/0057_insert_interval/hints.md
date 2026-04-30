# 57. 插入区间

## 核心思路

本质上是在一个**已排序、无重叠**的区间列表中，找出哪些区间与新区间有重叠，把它们合并成一个区间，其余区间原样保留。

## 思维链

1. **读完题第一反应**：既然区间已经排序且无重叠，那新区间插入后只会影响"中间一段"——前面一些区间完全在新区间左边（不重叠），后面一些完全在右边（不重叠），中间那些和新区间有交集需要合并。

2. **怎么判断"不重叠"？** 区间 A 在区间 B 完全左边的条件是 `A.end < B.start`；完全在右边是 `A.start > B.end`。既不在左也不在右，就是有重叠。

3. **怎么合并重叠部分？** 多个重叠区间合并后，`start = min(所有start)`，`end = max(所有end)`。

4. **算法流程**：三步走——(1) 把所有在新区间左边的直接加入结果；(2) 逐个合并所有与新区间重叠的；(3) 把所有在新区间右边的直接加入结果。一次遍历搞定。

5. **另一种思路**：先把新区间插入正确位置（二分查找），然后调用"合并区间"（LeetCode 56）的逻辑。这样更通用，但多了一步。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 三段式一次遍历 | 左段直接加、中段合并、右段直接加 | O(n) | O(n) | ⭐ 必须写出 |
| 插入+合并区间 | 插入后调用 LeetCode 56 合并 | O(n) | O(n) | 能说出即可 |
| 二分查找优化 | 用二分找到左右边界，减少常数 | O(log n + n) | O(n) | 加分项 |

## 关键提示

1. **两个区间不重叠**只有两种情况：`A完全在B左边` 或 `A完全在B右边`。取反就是重叠条件——不需要分多种 case。

2. 合并时不要一个个 if-else，直接用 `min/max` 更新 newInterval 的边界就够了。

3. 注意边界：`intervals` 可能为空（直接返回 `{newInterval}`）；newInterval 可能在所有区间左边或右边。

4. 示意图——以示例2为例：
```
原始区间:  [1,2]  [3,5]  [6,7]  [8,10]  [12,16]
新区间:              [4,--------8]
                     ↑ 重叠区间 ↑

结果:      [1,2]  [3,--------10]         [12,16]
           左段       合并段               右段
```

5. 区分"相邻"和"重叠"：`[1,2]` 和 `[3,5]`，end=2 < start=3，不重叠。但 `[1,3]` 和 `[3,5]`，end=3 >= start=3，算重叠（题目中区间端点相等算重叠）。

## 解法详解

### 解法1: 三段式一次遍历 — O(n) / O(n) ⭐ 面试首选

**思考过程**：区间已排序，新区间把原列表天然分成三部分——完全在左、有重叠、完全在右。我们只需一次遍历，分三个阶段处理。

```cpp
class Solution {
public:
    vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
        vector<vector<int>> result;
        int i = 0, n = intervals.size();
        
        // 阶段1: 把所有在 newInterval 左边（完全不重叠）的区间直接加入
        // 判断条件：当前区间的 end < newInterval 的 start
        while (i < n && intervals[i][1] < newInterval[0]) {
            result.push_back(intervals[i]);
            i++;
        }
        
        // 阶段2: 合并所有与 newInterval 重叠的区间
        // 判断条件：当前区间的 start <= newInterval 的 end（还有交集）
        while (i < n && intervals[i][0] <= newInterval[1]) {
            // 合并：取两者 start 的较小值、end 的较大值
            newInterval[0] = min(newInterval[0], intervals[i][0]);
            newInterval[1] = max(newInterval[1], intervals[i][1]);
            i++;
        }
        result.push_back(newInterval); // 合并后的区间加入结果
        
        // 阶段3: 把所有在 newInterval 右边（完全不重叠）的区间直接加入
        while (i < n) {
            result.push_back(intervals[i]);
            i++;
        }
        
        return result;
    }
};
```

**关键点**：
- 阶段1的退出条件是 `intervals[i][1] < newInterval[0]`（严格小于，等于就算重叠）
- 阶段2的循环条件是 `intervals[i][0] <= newInterval[1]`（注意是用**当前的** newInterval，它可能在合并过程中不断扩展）
- 阶段2结束后，newInterval 已经吸收了所有重叠区间

### 解法2: 插入后合并 — O(n) / O(n)

**思考过程**：如果你已经会做 LeetCode 56（合并区间），那最简单的思路就是——先把 newInterval 插到正确位置，然后当成"合并区间"来做。

```cpp
class Solution {
public:
    vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
        // 找到插入位置（按 start 排序）
        auto it = intervals.begin();
        while (it != intervals.end() && (*it)[0] < newInterval[0]) {
            ++it;
        }
        intervals.insert(it, newInterval);
        
        // 合并区间（LeetCode 56 的逻辑）
        vector<vector<int>> result;
        for (auto& interval : intervals) {
            if (result.empty() || result.back()[1] < interval[0]) {
                // 不重叠，直接加入
                result.push_back(interval);
            } else {
                // 重叠，合并（扩展 end）
                result.back()[1] = max(result.back()[1], interval[1]);
            }
        }
        return result;
    }
};
```

**关键点**：`vector::insert` 在中间插入是 O(n)，所以总体还是 O(n)。这个方法的优势是思路简单、代码可复用 LeetCode 56。

### 解法3: 二分查找优化 — O(log n + n) / O(n)

**从解法1优化**：解法1的阶段1和阶段3本质上是在找"第一个与 newInterval 重叠的区间"和"最后一个重叠的区间"。利用排序性质，可以用二分查找加速这个定位过程。

```cpp
class Solution {
public:
    vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
        int n = intervals.size();
        if (n == 0) return {newInterval};
        
        // 二分找第一个 end >= newInterval[0] 的区间（即第一个可能与新区间重叠的）
        // 搜索空间: [0, n-1] 中找最小的 i 使得 intervals[i][1] >= newInterval[0]
        //
        // 示例: intervals = [[1,2],[3,5],[6,7],[8,10],[12,16]], newInterval = [4,8]
        //   ends: [2, 5, 7, 10, 16]  找第一个 >= 4
        //   Round1: lo=0, hi=4, mid=2, ends[2]=7 >= 4 → hi=2
        //   Round2: lo=0, hi=2, mid=1, ends[1]=5 >= 4 → hi=1
        //   Round3: lo=0, hi=1, mid=0, ends[0]=2 < 4  → lo=1
        //   结果: left=1 (即 [3,5])
        int lo = 0, hi = n - 1, left = n;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (intervals[mid][1] >= newInterval[0]) {
                left = mid;
                hi = mid - 1;
            } else {
                lo = mid + 1;
            }
        }
        
        // 二分找最后一个 start <= newInterval[1] 的区间（最后一个可能重叠的）
        lo = 0; hi = n - 1;
        int right = -1;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (intervals[mid][0] <= newInterval[1]) {
                right = mid;
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
        
        vector<vector<int>> result;
        
        // 左段: [0, left-1]
        for (int i = 0; i < left; i++) {
            result.push_back(intervals[i]);
        }
        
        // 合并段: [left, right] 中的区间与 newInterval 合并
        if (left <= right) {
            newInterval[0] = min(newInterval[0], intervals[left][0]);
            newInterval[1] = max(newInterval[1], intervals[right][1]);
        }
        result.push_back(newInterval);
        
        // 右段: [right+1, n-1]
        for (int i = right + 1; i < n; i++) {
            result.push_back(intervals[i]);
        }
        
        return result;
    }
};
```

**关键点**：
- 当 `left > right` 时，说明没有区间与 newInterval 重叠，newInterval 直接插入即可
- 二分查找把"定位"从 O(n) 优化到 O(log n)，但构建结果数组仍然是 O(n)，所以整体还是 O(n)
- 面试中如果追问"能不能更快定位"，这个思路能展示对二分的熟练度

## 解法对比

| | 解法1: 三段式 | 解法2: 插入+合并 | 解法3: 二分优化 |
|---|---|---|---|
| 核心思想 | 分三阶段处理 | 复用合并区间 | 二分定位边界 |
| 代码量 | 最少，最简洁 | 中等 | 较多 |
| 面试推荐 | ⭐ 首选 | 作为思路提及 | 追问时展示 |
| 优势 | 一次遍历，直觉清晰 | 可复用 LC56 | 展示二分能力 |

三种解法时间复杂度都是 O(n)（因为结果数组需要 O(n) 构建），面试中**解法1**是最佳选择——简洁、正确、好解释。

## 易错点

1. **重叠判断条件写反**
   - ✗ `intervals[i][1] <= newInterval[0]` → 当 end == start 时判为不重叠，但 `[1,3]` 和 `[3,5]` 是重叠的
   - ✓ `intervals[i][1] < newInterval[0]` → 严格小于才是不重叠

2. **阶段2中用原始 newInterval 判断而不是更新后的**
   - ✗ 把 newInterval 拷贝了一份，合并时更新的是拷贝但判断用的是原始值
   - ✓ 直接在 newInterval 上修改，保证循环条件用的是不断扩展后的边界

3. **忘记处理 intervals 为空**
   - ✗ 直接开始遍历 → 三个 while 都不进入，但也没加入 newInterval
   - ✓ 解法1其实自然处理了：三个 while 都不进入，最终 `result.push_back(newInterval)` 一定会执行

4. **合并段忘记 push newInterval**
   - ✗ 阶段2只合并但没把结果加入 result
   - ✓ 阶段2结束后必须 `result.push_back(newInterval)`

## 面试追问

**Q1: 这道题和 LeetCode 56（合并区间）有什么关系？**
→ 56 是给无序区间排序后合并；57 是区间已排序，插入一个新区间后合并。57 可以看做 56 的一个特殊情况——已经排好序，只需处理一个新区间的插入。解法2 就是直接复用 56 的逻辑。

**Q2: 如果频繁插入怎么优化？每次插入都要 O(n) 构建新数组。**
→ 如果需要频繁插入和查询，可以用**平衡 BST（如 C++ 的 `std::map`）**维护区间集合。用 `map<int,int>` 存 `{start: end}`，插入时用 `lower_bound` 定位，合并相邻/重叠区间，每次操作 O(log n + k)，k 是被合并的区间数。

**Q3: 如果不允许创建新数组，要求原地修改 intervals，怎么做？**
→ 原地修改需要处理数组中间的删除和插入，比较 tricky。可以用双指针：一个读指针遍历原数组，一个写指针标记结果位置。合并段的区间被"覆盖"掉，最后 resize 数组。但 vector 的 erase/insert 都是 O(n)，所以时间复杂度不变，只是省了额外空间。

## 相关题型

- **56. 合并区间** — 本题的前置题，区别：56 的输入无序需要先排序，57 已排好序且只插入一个。本题解法2 直接复用 56 的合并逻辑。
- **252/253. 会议室 I/II** — 同样是区间问题，复用"排序+遍历"框架。252 判断是否有重叠（简化版），253 求最大重叠数（用堆/扫描线）。
- **986. 区间列表的交集** — 两个有序区间列表求交集，复用本题的"双指针遍历有序区间"技巧，区别是本题求并集（合并），986 求交集。
- **715. Range 模块（Hard）** — 追问Q2 的完整实现，用有序 map 维护区间集合，支持 O(log n) 的插入/删除/查询。