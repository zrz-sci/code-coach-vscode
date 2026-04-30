# 352. 将数据流变为多个不相交区间

## 核心思路
设计一个数据结构，支持动态插入整数并随时输出当前所有数字组成的不相交区间列表。核心是用有序容器（如 `map` 或 `set`）维护区间集合，每次插入时通过二分查找定位新数字应该插入的位置，并处理与相邻区间的合并（向前合并、向后合并、或三者合并）。

## 思维链
1. 看到"动态添加 + 查询聚合状态" → 设计题，需要选择合适的数据结构
2. 区间需要有序 → `map<int, int>` 或 `set<pair<int,int>>`
3. 插入新数字 val 时，需要找到它的邻居区间：
   - 是否有区间的 end == val-1？（可以向左扩展）
   - 是否有区间的 start == val+1？（可以向右扩展）
   - 两者都满足则三个合并为一个
4. 用 `map<int, int>` 以 start 为 key，end 为 value → `lower_bound` 定位
5. 或者用 `set` 存所有数字，`getIntervals` 时遍历构建区间（更简单但查询慢）

## 解法概览表

| 解法 | addNum 时间 | getIntervals 时间 | 空间 | 特点 |
|------|------------|-------------------|------|------|
| ⭐ 有序Map维护区间 | O(log n) | O(区间数) | O(n) | 面试首选，高效 |
| Set 存所有数字 | O(log n) | O(n) | O(n) | 简单但 getIntervals 慢 |

## 关键提示
1. **用 `map<int, int>` 维护 `{start -> end}`**：有序性保证区间按 start 排列
2. **`lower_bound(val)` 定位**：找到第一个 start >= val 的区间，它可能是 val 的后继区间
3. **合并逻辑需考虑 4 种情况**：
   - val 已在某区间内 → 忽略
   - val 紧接前一个区间的 end → 扩展前区间
   - val 紧接后一个区间的 start → 扩展后区间
   - 前后都紧接 → 合并三者
   - 都不紧接 → 新建区间 [val, val]
4. **重复数字**：需要检查 val 是否已存在于某个区间中

## 解法详解

### 解法一：有序Map维护区间（推荐）

**思路**：用 `map<int,int>` 存储 `{start: end}` 的区间映射，每次 addNum 通过 lower_bound 定位并合并。

```cpp
class SummaryRanges {
    map<int, int> intervals; // start -> end
public:
    SummaryRanges() {}
    
    void addNum(int val) {
        auto it = intervals.upper_bound(val);
        int start = val, end = val;
        
        // 检查前一个区间是否能合并
        if (it != intervals.begin()) {
            auto prev = prev_fn(it);
            if (prev->second >= val) return; // val 已在区间内
            if (prev->second + 1 == val) {
                start = prev->first;
                intervals.erase(prev);
            }
        }
        
        // 检查后一个区间是否能合并
        if (it != intervals.end() && it->first == val + 1) {
            end = it->second;
            intervals.erase(it);
        }
        
        intervals[start] = max(end, val);
    }
    
    vector<vector<int>> getIntervals() {
        vector<vector<int>> res;
        for (auto& [s, e] : intervals) {
            res.push_back({s, e});
        }
        return res;
    }
};
```

**复杂度**：addNum O(log n)，getIntervals O(区间数)

### 解法二：Set 存所有数字

**思路**：用 `set<int>` 存所有插入过的数字，getIntervals 时遍历有序集合构建连续区间。

```cpp
class SummaryRanges {
    set<int> nums;
public:
    SummaryRanges() {}
    
    void addNum(int val) {
        nums.insert(val);
    }
    
    vector<vector<int>> getIntervals() {
        vector<vector<int>> res;
        int start = -1, end = -1;
        for (int num : nums) {
            if (start == -1) {
                start = end = num;
            } else if (num == end + 1) {
                end = num;
            } else {
                res.push_back({start, end});
                start = end = num;
            }
        }
        if (start != -1) res.push_back({start, end});
        return res;
    }
};
```

**复杂度**：addNum O(log n)，getIntervals O(n)

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| 不处理重复数字 | 检查 val 是否已在某区间内 | 重复插入不应改变状态 |
| 只检查后继区间不检查前驱 | 前驱和后继都要检查 | 可能与前面的区间合并 |
| 合并时只更新 end | start 也可能变（与前区间合并时） | start 取前区间的 start |
| `lower_bound` vs `upper_bound` | 根据具体逻辑选择 | upper_bound 找 > val 的第一个 |

## 面试追问

**Q1: Follow-up — 如果有大量合并且区间数远小于数据量，哪种解法更好？**
> Map 解法更好。因为 map 中只存区间（数量少），addNum 和 getIntervals 都是 O(log(区间数))。而 Set 解法存了所有数字，getIntervals 需要遍历所有数字 O(n)。当区间数 << 数据量时，Map 解法优势明显。

**Q2: 能否用并查集实现？**
> 可以。每个数字作为一个节点，addNum 时将 val 与 val-1 和 val+1 合并（如果它们存在）。用一个 map 记录每个连通分量的最小和最大值。但实现比 Map 解法复杂，面试中不推荐。

**Q3: 如果 addNum 和 getIntervals 调用频率差异很大，如何优化？**
> - addNum 频繁、getIntervals 少：用 Set 解法（addNum 简单，getIntervals 时才构建区间）
> - getIntervals 频繁、addNum 少：用 Map 解法（维护好区间，getIntervals 只需遍历）
> - 两者都频繁：Map 解法更均衡

## 相关题型
- [56. 合并区间](https://leetcode.com/problems/merge-intervals/) - 区间合并基础
- [57. 插入区间](https://leetcode.com/problems/insert-interval/) - 插入一个区间并合并
- [715. Range 模块](https://leetcode.com/problems/range-module/) - 更复杂的区间操作
- [228. 汇总区间](https://leetcode.com/problems/summary-ranges/) - 静态版本的区间汇总
- [986. 区间列表的交集](https://leetcode.com/problems/interval-list-intersections/) - 区间交集
