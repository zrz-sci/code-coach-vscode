# 1146. 快照数组

## 核心思路

本质上在问：如何高效地存储数组在多个时间点的历史状态，使得 `set` 和 `get` 操作都尽可能快，同时避免每次快照都完整复制整个数组。

## 思维链

1. **读完题第一反应** → 每次调用 `snap()` 时，把整个数组完整复制一份存起来，`get` 时直接查对应快照的数组。这是最直觉的"全量备份"思路。

2. **暴力解的瓶颈在哪？** → 每次 `snap()` 需要 O(length) 时间和空间复制整个数组。如果数组很大（50000），快照次数也很多（50000），总空间可能达到 O(length × snap次数) = O(2.5×10⁹)，完全不可接受。而且大部分元素在两次快照之间根本没有变化，复制它们是纯浪费。

3. **怎么突破瓶颈？** → 关键观察：**大部分元素在两次快照之间不变**。我们不应该存"整个数组的快照"，而应该只存"每个元素在哪些快照时被修改过"。这是一种"增量存储"思路——对每个 index，只记录它发生变化的 (snap_id, value) 对。

4. **get 怎么查？** → 对于 `get(index, snap_id)`，我们需要在 index 的历史记录中找到"snap_id 或之前最近一次的修改值"。历史记录按 snap_id 有序，所以可以用**二分查找**（upper_bound 找到第一个 > snap_id 的位置，往前退一步）。

5. **set 怎么存？** → `set(index, val)` 时，把 (当前snap_id, val) 记录到 index 的历史中。注意同一个 snap_id 内多次 set 同一个 index，只需保留最后一次的值。

6. **数据结构选择** → 每个 index 维护一个 `vector<pair<int,int>>`（有序的 snap_id → value 列表），查询时二分搜索。

## 解法概览

| 解法 | 思路 | set | snap | get | 空间 | 面试 |
|------|------|-----|------|-----|------|------|
| 暴力全量复制 | 每次 snap 复制整个数组 | O(1) | O(n) | O(1) | O(n×S) | 能说出即可 |
| 每个 index 存 map | 用 map 记录历史 | O(log S) | O(1) | O(log S) | O(修改次数) | 加分项 |
| 每个 index 存 vector + 二分 | vector 存历史 + upper_bound | O(1)* | O(1)* | O(log S) | O(修改次数) | ⭐ 必须写出 |

*n = length, S = snap次数。set 的 O(1) 是均摊的（vector push_back）。snap 的 O(1) 指只需递增计数器。

## 关键提示

1. **不要真的复制整个数组**——想想有多少元素在两次快照之间实际发生了变化？

2. **"找某个时间点的值"等价于"找 ≤ snap_id 的最大 key"**——这是一个经典的二分查找场景。

3. **同一个 snap_id 内多次 set 同一个 index**——你的数据结构需要处理这种去重，否则历史记录会膨胀。

4. **初始值是 0**——如果某个 index 从未被 set 过，`get` 应该返回 0。你的二分查找在"找不到"时要正确返回默认值。

5. 操作过程示意：
```
snap_id:    0        1        2
操作:   set(0,5)  snap()  set(0,6)  snap()  get(0,0)  get(0,1)

index 0 的历史: [(0, 5), (1, 6)]
                         ↑ snap_id=0 时找到 (0,5) → 返回5
                                   ↑ snap_id=1 时找到 (1,6) → 返回6
```

## 解法详解

### 解法1: 暴力全量复制 — O(1)/O(n)/O(1) per operation

**思考过程**: 最直觉的想法——快照就是"拍照"，每次 snap 就把当前数组完整存一份。

```cpp
class SnapshotArray {
    vector<int> arr;
    vector<vector<int>> snapshots; // snapshots[snap_id] = 当时的完整数组
public:
    SnapshotArray(int length) : arr(length, 0) {}
    
    void set(int index, int val) {
        arr[index] = val;
    }
    
    int snap() {
        snapshots.push_back(arr); // O(n) 复制整个数组
        return snapshots.size() - 1;
    }
    
    int get(int index, int snap_id) {
        return snapshots[snap_id][index];
    }
};
```

**关键点**: 
- 空间 O(n × S) 会超出内存限制（50000 × 50000 = 2.5×10⁹）
- snap() 是 O(n)，如果频繁调用会 TLE
- 但思路正确，面试时可以先说这个再优化

### 解法2: 每个 index 存历史记录 + 二分查找 — O(1)/O(1)/O(log S) ⭐ 面试首选

**从解法1优化**: 解法1的瓶颈是每次 snap 复制整个数组，但大部分元素没变。核心思想：**不存快照，存变更历史**。对每个 index 只记录"在哪个 snap_id 被改成了什么值"，查询时用二分查找定位。

```cpp
class SnapshotArray {
    // history[i] 存储 index=i 的所有修改记录: [(snap_id, value), ...]
    // 按 snap_id 有序
    vector<vector<pair<int,int>>> history;
    int cur_snap;
    
public:
    SnapshotArray(int length) : history(length), cur_snap(0) {
        // 每个 index 初始化一条记录: snap_id=0 时值为 0
        for (int i = 0; i < length; i++) {
            history[i].push_back({0, 0});
        }
    }
    
    void set(int index, int val) {
        // 如果当前 snap_id 已经有记录，更新它（同一快照内多次 set）
        // 否则追加新记录
        if (history[index].back().first == cur_snap) {
            history[index].back().second = val;
        } else {
            history[index].push_back({cur_snap, val});
        }
    }
    
    int snap() {
        return cur_snap++;
    }
    
    int get(int index, int snap_id) {
        // 在 history[index] 中找 snap_id 或之前最近的记录
        // upper_bound 找到第一个 snap_id > 目标的位置，往前退一步
        auto& h = history[index];
        // 用 upper_bound 按 pair 的 first 查找
        auto it = upper_bound(h.begin(), h.end(), 
                              pair<int,int>(snap_id, INT_MAX));
        // it 前面一个位置就是 <= snap_id 的最大记录
        --it;
        return it->second;
    }
};
```

**为什么 upper_bound 的 target 是 (snap_id, INT_MAX)?**
- `pair` 的比较是先比 first 再比 second
- `upper_bound` 找第一个严格大于 target 的元素
- `(snap_id, INT_MAX)` 保证了所有 first == snap_id 的 pair 都 ≤ target
- 所以 `--it` 指向的就是 first ≤ snap_id 的最后一个元素

**二分查找过程示意**:
```
// history[0] = [(0,0), (0,5), (2,6), (5,3)]
//                                      ↑ 这些是实际修改发生的 snap_id
//
// get(0, 3): 找 snap_id ≤ 3 的最后一条
// upper_bound(target = (3, INT_MAX))
//   比较: (0,0)<target  (0,5)<target  (2,6)<target  (5,3)>target
//                                                    ↑ upper_bound 指向这里
//   --it → (2,6) → 返回 6
```

### 解法3: 每个 index 用 map — O(log S)/O(1)/O(log S)

**不同角度**: 用 `map<int,int>` 代替 vector，key 是 snap_id，value 是值。查询用 `upper_bound` 一样方便，但 map 的常数因子比 vector 大。

```cpp
class SnapshotArray {
    vector<map<int,int>> history; // history[index]: snap_id → value
    int cur_snap;
    
public:
    SnapshotArray(int length) : history(length), cur_snap(0) {
        for (int i = 0; i < length; i++) {
            history[i][0] = 0; // 初始值
        }
    }
    
    void set(int index, int val) {
        history[index][cur_snap] = val; // map 自动覆盖同 key
    }
    
    int snap() {
        return cur_snap++;
    }
    
    int get(int index, int snap_id) {
        auto it = history[index].upper_bound(snap_id);
        --it; // upper_bound 返回第一个 > snap_id 的，退一步就是 <= snap_id 的
        return it->second;
    }
};
```

**关键点**: map 写起来更简洁（不需要手动处理同一 snap_id 的去重），但 map 的每个节点有额外内存开销（红黑树节点），且缓存不友好。

## 解法对比

| | 解法1 暴力复制 | 解法2 vector+二分 | 解法3 map |
|---|---|---|---|
| set | O(1) | O(1) 均摊 | O(log S) |
| snap | O(n) ← 瓶颈 | O(1) | O(1) |
| get | O(1) | O(log S) | O(log S) |
| 空间 | O(n×S) 爆炸 | O(修改次数) 精简 | O(修改次数) 但常数大 |
| 适用 | 仅说思路 | **面试首选** | 代码更简洁，可作替代 |

**解法2 vs 解法3 怎么选？** 
- 面试时推荐解法2（vector + 二分），性能更好、更能展示二分查找功底
- 如果时间紧张，解法3（map）更快写出来且不易出错

## 易错点

1. **snap() 的返回值 vs 递增时机**:
   - ✗ `int snap() { cur_snap++; return cur_snap; }` → snap_id 从 1 开始了
   - ✓ `int snap() { return cur_snap++; }` → 先返回当前值再递增，snap_id 从 0 开始

2. **同一 snap_id 内多次 set 同一个 index**:
   - ✗ 直接 `push_back({cur_snap, val})` → 同一个 snap_id 出现多条记录，二分查找可能定位错误
   - ✓ 先检查 `back().first == cur_snap` 是否已存在，存在则更新

3. **upper_bound 的 target 写法**:
   - ✗ `upper_bound(..., make_pair(snap_id, 0))` → 如果存在 (snap_id, 0) 这条记录，upper_bound 可能跳过它
   - ✓ `upper_bound(..., make_pair(snap_id, INT_MAX))` → 确保所有 first==snap_id 的记录都在 upper_bound 之前

4. **get 时 index 从未被 set 过**:
   - ✗ history[index] 为空时 `--it` 会越界
   - ✓ 构造函数中为每个 index 初始化 `{0, 0}`，保证永远能找到记录

5. **初始化的时机**:
   - ✗ `history[i].push_back({-1, 0})` 用 snap_id=-1 → 可能影响二分查找逻辑
   - ✓ `history[i].push_back({0, 0})` 用 snap_id=0 → 更自然且正确

## 面试追问

**Q1: 暴力解有什么问题？瓶颈在哪？**
→ 每次 snap 复制 O(n) 的数组，空间 O(n×S)。瓶颈是"大量元素没变也被复制"。

**Q2: 你的优化方案 get 操作为什么用二分查找？能不能 O(1)？**
→ 因为每个 index 的历史记录按 snap_id 有序，要找 ≤ snap_id 的最近一条记录，二分是最自然的选择。如果要 O(1) get，就需要在 snap 时把当前值记录到一个 O(1) 可查的结构中，但那就回到了全量复制的问题。不过可以用**写时复制(Copy-on-Write)**技术做一些平衡。

**Q3: 如果 set 操作极其频繁但 snap 很少，怎么优化？**
→ 当前方案 set 是 O(1) 已经很快了。但如果 snap 非常少，可以考虑只在 snap 时记录"脏位"（哪些 index 被改过），只对改过的 index 存储快照值。这在实际系统中对应"脏页跟踪"机制。

**Q4: 如果并发多线程调用 set/snap/get，需要注意什么？**
→ snap_id 的递增需要原子操作或加锁。set 和 snap 之间需要保证一致性（一个 set 操作要么完全在某个快照之前，要么完全在之后）。get 是只读的，可以无锁并发。这涉及 MVCC（多版本并发控制）的思想。

## 相关题型

- **981. 基于时间的键值存储** — 几乎相同的思路！也是对每个 key 存 (timestamp, value) 列表，查询时二分找 ≤ timestamp 的最新值。本题的每个 index 就是一个 key，snap_id 就是 timestamp。
- **352. 将数据流变为多个不相交区间** — 类似的"增量存储 + 有序查找"设计思路。
- **705/706. 设计哈希集合/映射** — 设计题的基本功，理解数据结构的底层选择。