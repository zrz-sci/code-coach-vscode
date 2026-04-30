# 981. 基于时间的键值存储

## 核心思路

本质上是一个**带时间维度的查找问题**：对于每个 key，存了一系列 `(timestamp, value)` 对，`get` 操作要求找到 `<= timestamp` 的最大时间戳对应的值——这就是经典的**在有序序列中找 upper_bound 的前一个元素**。

## 思维链

1. **读完题第一反应**：每个 key 可以对应多个 `(timestamp, value)` 对。`get` 要找 `<= timestamp` 的最大时间戳。最朴素的做法：对每个 key 存一个列表，`get` 时从后往前线性扫描。

2. **暴力解的瓶颈**：线性扫描一个 key 下的所有记录，如果某个 key 被 `set` 了 N 次，每次 `get` 就是 O(N)。总共 2×10⁵ 次操作，最坏 O(N²) 可能超时。

3. **关键观察**：题目说 **"set 操作中的时间戳都是严格递增的"**！这意味着每个 key 下的 `(timestamp, value)` 列表天然按 timestamp **有序**。有序 → 可以二分查找！

4. **怎么二分**：对于 `get(key, timestamp)`，在 key 对应的有序列表中，找到**最后一个 `ts <= timestamp`** 的位置。这正是 `upper_bound(timestamp)` 的前一个位置。

5. **数据结构选择**：外层用 `unordered_map<string, ...>` 做 key 到列表的映射（O(1) 查找 key）；内层用 `vector<pair<int, string>>` 存有序的 `(timestamp, value)` 对（支持二分）。

6. **结果**：`set` O(1)，`get` O(log N)，整体效率极高。

## 解法概览

| 解法 | 思路 | set 时间 | get 时间 | 空间 | 面试 |
|------|------|----------|----------|------|------|
| 暴力线性查找 | vector + 线性扫描 | O(1) | O(N) | O(N) | 能说出即可 |
| HashMap + 二分查找 | vector + binary search | O(1) | O(log N) | O(N) | ⭐ 必须写出 |
| HashMap + STL map | 用 `std::map` 的 `upper_bound` | O(log N) | O(log N) | O(N) | 加分项 |

> N = 某个 key 下的记录数

## 关键提示

1. **题目的隐藏条件是金矿**：时间戳严格递增 → 列表天然有序 → 不需要排序就能二分。如果没有这个条件，你需要用 `std::map` 来维护有序性。

2. **`get` 的语义是"floor 查询"**：找 `<= timestamp` 的最大值。C++ STL 中 `upper_bound` 返回第一个 `> timestamp` 的迭代器，往前退一步就是答案。

3. **边界情况**：如果 key 不存在，或者所有记录的时间戳都 `> timestamp`，返回空字符串 `""`。

4. **二分搜索收缩过程示意**：
```
// key="foo" 的记录: [(1,"bar"), (4,"bar2"), (7,"bar3")]
// get("foo", 5):
// 找 <= 5 的最大 timestamp
//
// Round1: lo=0, hi=2, mid=1, ts[1]=4 <= 5 → lo=mid+1=2
// Round2: lo=2, hi=2, mid=2, ts[2]=7 > 5  → hi=mid-1=1
// lo > hi → 结束, 答案在 hi=1 → "bar2" ✓
```

5. **STL 方案的巧妙之处**：`std::map` 自身就是有序的，直接用 `upper_bound` + `prev` 即可，代码最短但 `set` 从 O(1) 变成 O(log N)。

## 解法详解

### 解法1: HashMap + 线性扫描 — O(1) set / O(N) get

**思考过程**：最直觉的做法，每个 key 维护一个列表，`get` 时从后往前找第一个 `<= timestamp` 的记录。因为时间戳递增，列表尾部是最大的，从后往前扫可以尽早命中。

```cpp
class TimeMap {
public:
    unordered_map<string, vector<pair<int, string>>> store;

    TimeMap() {}

    void set(string key, string value, int timestamp) {
        store[key].emplace_back(timestamp, value);
    }

    string get(string key, int timestamp) {
        if (store.find(key) == store.end()) return "";
        auto& vec = store[key];
        // 从后往前扫描，找第一个 <= timestamp 的
        for (int i = vec.size() - 1; i >= 0; i--) {
            if (vec[i].first <= timestamp) {
                return vec[i].second;
            }
        }
        return "";
    }
};
```

**关键点**：虽然从后往前扫在平均情况下可能很快（如果查询时间戳接近最新记录），但最坏情况（查询很早的时间戳）仍然是 O(N)。

---

### 解法2: HashMap + 二分查找 — O(1) set / O(log N) get ⭐ 面试首选

**从解法1优化**：解法1的瓶颈是线性扫描。既然列表已经按时间戳有序，直接二分查找 `<= timestamp` 的最大元素。

```cpp
class TimeMap {
public:
    unordered_map<string, vector<pair<int, string>>> store;

    TimeMap() {}

    void set(string key, string value, int timestamp) {
        store[key].emplace_back(timestamp, value);
    }

    string get(string key, int timestamp) {
        auto it = store.find(key);
        if (it == store.end()) return "";

        auto& vec = it->second;
        // 二分查找：找最后一个 ts <= timestamp 的位置
        int lo = 0, hi = (int)vec.size() - 1;
        int ans = -1; // 记录最佳位置
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (vec[mid].first <= timestamp) {
                ans = mid;       // mid 是一个候选答案
                lo = mid + 1;    // 尝试找更大的
            } else {
                hi = mid - 1;    // mid 的时间戳太大，往左找
            }
        }
        return ans == -1 ? "" : vec[ans].second;
    }
};
```

**二分搜索空间收缩过程**：
```
// 示例: key="foo", records = [(1,"bar"), (4,"bar2"), (7,"bar3")]
//
// get("foo", 5) → 找 <= 5 的最大 timestamp
// ─────────────────────────────────────────
// Round1: lo=0, hi=2, mid=1
//         [1, (4), 7]    vec[1].first=4 <= 5 → ans=1, lo=2
//              ↑
// Round2: lo=2, hi=2, mid=2
//         [1, 4, (7)]    vec[2].first=7 > 5  → hi=1
//                 ↑
// lo=2 > hi=1 → 结束, ans=1 → "bar2" ✓
//
// get("foo", 0) → 找 <= 0 的最大 timestamp
// ─────────────────────────────────────────
// Round1: lo=0, hi=2, mid=1
//         [1, (4), 7]    vec[1].first=4 > 0 → hi=0
// Round2: lo=0, hi=0, mid=0
//         [(1), 4, 7]    vec[0].first=1 > 0 → hi=-1
// lo=0 > hi=-1 → 结束, ans=-1 → "" ✓
```

**也可以用 STL 的 `upper_bound`**：

```cpp
class TimeMap {
public:
    unordered_map<string, vector<pair<int, string>>> store;

    TimeMap() {}

    void set(string key, string value, int timestamp) {
        store[key].emplace_back(timestamp, value);
    }

    string get(string key, int timestamp) {
        auto it = store.find(key);
        if (it == store.end()) return "";

        auto& vec = it->second;
        // upper_bound 找第一个 ts > timestamp 的位置
        // 它的前一个就是 <= timestamp 的最大值
        auto ub = upper_bound(vec.begin(), vec.end(), 
                              make_pair(timestamp, string(101, 'z')));
        // 注意：pair 的比较先比 first，所以用一个足够大的 second
        // 更简洁的方式：自定义比较
        if (ub == vec.begin()) return "";
        return prev(ub)->second;
    }
};
```

> **注意**：上面用 `pair` 做 `upper_bound` 时，`second` 需要是一个足够大的字符串，因为 `pair` 按字典序先比 `first` 再比 `second`。更干净的写法是用自定义比较函数：

```cpp
string get(string key, int timestamp) {
    auto it = store.find(key);
    if (it == store.end()) return "";
    auto& vec = it->second;
    // 只按 first（时间戳）比较
    auto ub = upper_bound(vec.begin(), vec.end(), timestamp,
        [](int ts, const pair<int, string>& p) {
            return ts < p.first;
        });
    if (ub == vec.begin()) return "";
    return prev(ub)->second;
}
```

---

### 解法3: HashMap + std::map — O(log N) set / O(log N) get

**不同角度**：如果题目没有保证时间戳递增呢？用 `std::map<int, string>` 自动维护有序性，`upper_bound` 直接可用。

```cpp
class TimeMap {
public:
    unordered_map<string, map<int, string>> store;

    TimeMap() {}

    void set(string key, string value, int timestamp) {
        store[key][timestamp] = value;  // O(log N) 插入红黑树
    }

    string get(string key, int timestamp) {
        auto it = store.find(key);
        if (it == store.end()) return "";

        auto& m = it->second;
        // upper_bound 找第一个 > timestamp 的
        auto ub = m.upper_bound(timestamp);
        if (ub == m.begin()) return "";  // 所有时间戳都 > timestamp
        return prev(ub)->second;         // 前一个就是 <= timestamp 的最大
    }
};
```

**关键点**：代码最简洁，但 `set` 变成 O(log N)（红黑树插入），而且 `std::map` 的常数因子比 `vector` 大（内存不连续，缓存不友好）。在本题时间戳已递增的条件下，解法2更优。

## 解法对比

| | 解法1: 线性扫描 | 解法2: 二分查找 | 解法3: std::map |
|---|---|---|---|
| set 时间 | O(1) | O(1) | O(log N) |
| get 时间 | O(N) | O(log N) | O(log N) |
| 空间效率 | vector 连续内存 | vector 连续内存 | 红黑树节点分散 |
| 代码复杂度 | 最简单 | 中等 | 最简洁 |
| 适用场景 | 数据量小 | ⭐ 本题最优 | 时间戳不保证递增时 |
| 核心依赖 | 无 | 时间戳递增保证有序 | 自动维护有序 |

**面试推荐**：解法2（手写二分），然后提一句"如果时间戳不递增可以用 std::map"。

## 易错点

1. **二分查找的边界判断**：
   - ✗ 忘记处理 `upper_bound` 返回 `begin()` 的情况，直接 `prev(ub)` 导致越界
   - ✓ 先判断 `if (ub == vec.begin()) return "";`

2. **key 不存在时直接用 `store[key]` 访问**：
   - ✗ `auto& vec = store[key];` — 如果 key 不存在会**创建空 entry**，浪费内存
   - ✓ 先用 `store.find(key)` 检查存在性

3. **STL upper_bound 的比较函数方向**：
   - ✗ `[](const pair<int,string>& p, int ts) { return p.first < ts; }` — 参数顺序错误
   - ✓ 当 `value` 类型做第一参数时，lambda 的参数顺序是 `(value, element)`，即 `(int ts, const pair<int,string>& p)`

4. **手写二分时 `ans` 变量忘记初始化为 -1**：
   - ✗ `int ans = 0;` — 当所有时间戳都 > timestamp 时错误返回 vec[0]
   - ✓ `int ans = -1;` 并在结尾判断 `ans == -1 ? "" : ...`

5. **整数溢出计算 mid**：
   - ✗ `int mid = (lo + hi) / 2;` — 极端情况可能溢出
   - ✓ `int mid = lo + (hi - lo) / 2;`

## 面试追问

**Q1: 为什么 `get` 不是简单的哈希查找，而需要二分？**
→ 因为 `get(key, timestamp)` 不是精确匹配 timestamp，而是要找 `<= timestamp` 的最大值（floor 查询）。精确匹配用哈希 O(1)，范围查询需要有序结构 + 二分 O(log N)。

**Q2: 如果 `set` 的时间戳不保证递增怎么办？**
→ 两种方案：
- 用 `std::map<int, string>` 代替 `vector`（解法3），自动排序，`set` 变 O(log N)
- 仍用 `vector`，但每次 `set` 后需要排序或用插入排序维护有序性

**Q3: 如果需要支持 `delete(key, timestamp)` 操作呢？**
→ `vector` 删除中间元素是 O(N)，此时 `std::map` 更合适（O(log N) 删除）。或者用惰性删除标记 + 周期性清理。

**Q4: 如果 `get` 查询量远大于 `set`，有没有进一步优化空间？**
→ 当前 `get` 已经是 O(log N)，很难再快。如果查询的 timestamp 分布有规律（比如集中在最近），可以先检查列表末尾（O(1) 快速路径），不命中再二分。

**Q5: 如果要支持"获取 key 在 [t1, t2] 范围内的所有值"呢？**
→ 用两次二分找到范围的左右边界，然后遍历区间内所有元素。时间 O(log N + K)，K 是结果数量。

## 相关题型

- **LeetCode 35. 搜索插入位置** — 复用二分查找模板，本题的 `get` 本质就是找插入位置的前一个
- **LeetCode 704. 二分查找** — 基础二分，本题是"找 <= target 的最大值"变体
- **LeetCode 146. LRU 缓存** — 同为设计题，区别：LRU 用 HashMap + 双向链表，TimeMap 用 HashMap + 有序数组
- **LeetCode 1146. 快照数组** — 类似思路，每个索引存储 `(snap_id, val)` 对，get 时二分查找，可以直接复用本题的二分模板