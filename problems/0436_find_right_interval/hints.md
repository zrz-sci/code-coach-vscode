# 436. Find Right Interval

## 核心思路
对每个区间的 end 值，在所有区间的 start 值中找 **大于等于 end 的最小 start**。这是经典的 **排序 + 二分查找** 问题。

## 思维链
1. 每个 start 唯一 → 可以建立 start → 原始下标的映射
2. 对于每个区间 i，需要找 start_j >= end_i 且 start_j 最小
3. 将所有 (start, index) 排序后，对每个 end 做 lower_bound 二分查找
4. 也可以用 map（有序映射）直接 lower_bound

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ 排序 + 二分查找 | O(n log n) | O(n) | 面试首选 |
| TreeMap / map | O(n log n) | O(n) | 代码更简洁 |

## 关键提示
1. **start 唯一**：题目保证每个 start_i 不同，可以安全地用 start 做 key
2. **i 可以等于 j**：自身的 start >= 自身的 end 时，右区间就是自己
3. **lower_bound**：查找第一个 >= target 的位置
4. **需要记住原始下标**：排序后下标会变，用 pair 或 map 保存原始位置

## 解法详解

### 解法一：排序 + 二分查找 ⭐

**思路**：将 (start, 原始下标) 排序，对每个区间的 end 用 lower_bound 查找。

```cpp
vector<int> findRightInterval(vector<vector<int>>& intervals) {
    int n = intervals.size();
    vector<pair<int, int>> starts; // (start, original_index)
    for (int i = 0; i < n; i++) {
        starts.push_back({intervals[i][0], i});
    }
    sort(starts.begin(), starts.end());
    
    vector<int> res(n);
    for (int i = 0; i < n; i++) {
        int target = intervals[i][1];
        auto it = lower_bound(starts.begin(), starts.end(), 
                              make_pair(target, INT_MIN));
        res[i] = (it == starts.end()) ? -1 : it->second;
    }
    return res;
}
```

**复杂度**：时间 O(n log n)，空间 O(n)

### 解法二：使用 map

**思路**：直接用有序 map 存储 start → index，利用 map 的 lower_bound。

```cpp
vector<int> findRightInterval(vector<vector<int>>& intervals) {
    map<int, int> startMap; // start -> index
    for (int i = 0; i < intervals.size(); i++) {
        startMap[intervals[i][0]] = i;
    }
    
    vector<int> res;
    for (auto& interval : intervals) {
        auto it = startMap.lower_bound(interval[1]);
        res.push_back(it == startMap.end() ? -1 : it->second);
    }
    return res;
}
```

## 易错点
- ✗ 二分查找找的是 > end 而非 >= end → ✓ 用 `lower_bound`（>=）而不是 `upper_bound`（>）
- ✗ 排序后忘记保留原始下标 → ✓ 用 pair<start, index> 排序
- ✗ 没找到右区间时返回 0 或不处理 → ✓ 检查 `it == end()` 返回 -1

## 面试追问

**Q1: 如果 start 不唯一怎么办？**
题目保证 start 唯一。如果不唯一，需要用 multimap 或在排序时用 stable_sort，lower_bound 会找到第一个匹配，仍然正确。

**Q2: 能否用 O(n) 解法？**
不能。因为需要对每个 end 找最小的 >= start，本质是 n 次范围查询，下界是 O(n log n)。

**Q3: 如果区间是动态插入的，如何维护右区间关系？**
用平衡 BST（如 C++ 的 set/map）维护 start 集合，每次插入 O(log n)，查询 lower_bound 也是 O(log n)。

## 相关题型
- [352. Data Stream as Disjoint Intervals](https://leetcode.com/problems/data-stream-as-disjoint-intervals/) - 区间合并
- [56. Merge Intervals](https://leetcode.com/problems/merge-intervals/) - 合并区间
- [435. Non-overlapping Intervals](https://leetcode.com/problems/non-overlapping-intervals/) - 不重叠区间
