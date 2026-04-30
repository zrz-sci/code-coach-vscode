/*
 * LeetCode 1146: 快照数组 (Snapshot Array)
 *
 * 【题目本质】
 * 设计一个数据结构，支持数组的修改(set)、拍快照(snap)、和查询历史快照(get)。
 * 核心挑战：如何避免每次快照都 O(n) 复制整个数组？
 *
 * 【解法总览】
 * 解法1: 暴力全量复制   — set O(1) / snap O(n) / get O(1)  / 空间 O(n×S)  — 最直觉但超时超内存
 * 解法2: vector+二分查找 — set O(1) / snap O(1) / get O(logS) / 空间 O(修改次数) — ⭐面试首选
 * 解法3: map 版本       — set O(logS) / snap O(1) / get O(logS) / 空间 O(修改次数) — 代码更简洁
 */

// ============================================================
// 解法1: 暴力全量复制
// set O(1), snap O(n), get O(1), 空间 O(n × S)
//
// 【思路】
// 最直觉的做法：snap 时把整个数组复制一份存起来。
// 问题：length=50000, snap调用50000次 → 存储 2.5×10^9 个 int，直接 MLE。
// 这个解法只用来展示思路，实际无法通过。
//
// 操作过程:
//   SnapshotArray(3) → arr = [0, 0, 0]
//   set(0, 5)        → arr = [5, 0, 0]
//   snap()           → snapshots[0] = [5, 0, 0], 返回 0
//   set(0, 6)        → arr = [6, 0, 0]
//   get(0, 0)        → snapshots[0][0] = 5
// ============================================================
class SnapshotArray1 {
    vector<int> arr;
    vector<vector<int>> snapshots;
public:
    SnapshotArray1(int length) : arr(length, 0) {}

    void set(int index, int val) {
        arr[index] = val;
    }

    int snap() {
        snapshots.push_back(arr);  // O(n) 全量复制 — 这就是性能瓶颈
        return (int)snapshots.size() - 1;
    }

    int get(int index, int snap_id) {
        return snapshots[snap_id][index];  // O(1) 直接查
    }
};


// ============================================================
// 解法2: 每个 index 存变更历史 + 二分查找 (⭐ 面试首选)
// set O(1) 均摊, snap O(1), get O(log S), 空间 O(修改次数)
//
// 【思路】
// 解法1的瓶颈：每次 snap 复制整个数组，但大部分元素没变。
// 核心优化：不存"某个时刻的完整数组"，而是存"每个元素的变更历史"。
//
// 数据结构:
//   history[index] = [(snap_id_0, val_0), (snap_id_1, val_1), ...]
//   每个 index 独立维护一个按 snap_id 有序的变更列表。
//
// set(index, val): 在 history[index] 末尾追加 (cur_snap, val)
//                  如果当前 snap 已有记录则覆盖（同一快照内多次 set）
// snap(): 仅递增 cur_snap，O(1)！不需要遍历任何东西
// get(index, snap_id): 在 history[index] 中二分查找 ≤ snap_id 的最后一条记录
//
// 二分查找过程示例:
//   假设 history[0] = [(0,0), (2,5), (5,3), (8,7)]
//
//   get(0, 4): 找 ≤ 4 的最后一条
//     upper_bound(target=(4, INT_MAX))
//     比较: (0,0)✓ (2,5)✓ (5,3)✗ → upper_bound 指向 (5,3)
//                         ↑
//     --it → (2,5) → 返回 5
//
//   get(0, 5): 找 ≤ 5 的最后一条
//     upper_bound(target=(5, INT_MAX))
//     比较: (0,0)✓ (2,5)✓ (5,3)✓ (8,7)✗ → upper_bound 指向 (8,7)
//                                  ↑
//     --it → (5,3) → 返回 3
// ============================================================
class SnapshotArray {
    vector<vector<pair<int,int>>> history;  // history[i]: index=i 的变更记录
    int cur_snap;

public:
    SnapshotArray(int length) : history(length), cur_snap(0) {
        // 每个 index 初始化为 (snap_id=0, value=0)
        // 这保证了 get 在任何情况下都能找到至少一条记录
        for (int i = 0; i < length; i++) {
            history[i].emplace_back(0, 0);
        }
    }

    void set(int index, int val) {
        // 关键去重: 如果当前 snap_id 已经有记录，直接覆盖值
        // 不去重的后果: 同一个 snap_id 多条记录，浪费空间且可能影响二分正确性
        if (history[index].back().first == cur_snap) {
            history[index].back().second = val;
        } else {
            history[index].emplace_back(cur_snap, val);
        }
    }

    int snap() {
        // 注意: return cur_snap++ 先返回当前值再递增
        // 不是 return ++cur_snap (那样 snap_id 会从 1 开始)
        return cur_snap++;
    }

    int get(int index, int snap_id) {
        auto& h = history[index];
        // upper_bound 找第一个严格大于 (snap_id, INT_MAX) 的位置
        // 用 INT_MAX 作为 second 保证所有 first==snap_id 的 pair 都 ≤ target
        auto it = upper_bound(h.begin(), h.end(),
                              pair<int,int>(snap_id, INT_MAX));
        // it 前面一个位置就是 first ≤ snap_id 的最后一条记录
        --it;
        return it->second;
    }
};


// ============================================================
// 解法3: map 版本 — 代码更简洁
// set O(log S), snap O(1), get O(log S), 空间 O(修改次数)
//
// 【思路】
// 和解法2思路完全相同，但用 map<int,int> 代替 vector<pair<int,int>>。
// 优点: map 天然去重（同 key 覆盖），不需要手动处理同 snap_id 多次 set
// 缺点: map 节点开销大（红黑树每个节点约 32-48 字节），缓存不友好
//        set 操作变成 O(log S) 而不是 O(1)
// ============================================================
class SnapshotArray3 {
    vector<map<int,int>> history;  // history[index]: snap_id → value
    int cur_snap;

public:
    SnapshotArray3(int length) : history(length), cur_snap(0) {
        for (int i = 0; i < length; i++) {
            history[i][0] = 0;  // 初始值: snap_id=0, value=0
        }
    }

    void set(int index, int val) {
        // map 的 [] 操作: 如果 key 存在就覆盖，不存在就插入
        // 天然处理了同一 snap_id 多次 set 的去重问题
        history[index][cur_snap] = val;
    }

    int snap() {
        return cur_snap++;
    }

    int get(int index, int snap_id) {
        // map::upper_bound 找第一个 key > snap_id 的迭代器
        auto it = history[index].upper_bound(snap_id);
        // 退一步就是 key ≤ snap_id 的最大 key
        --it;
        return it->second;
    }
};


// ============================================================
// 【解法对比】
//
// | 操作  | 解法1 暴力 | 解法2 vector+二分 | 解法3 map   |
// |-------|-----------|------------------|-------------|
// | set   | O(1)      | O(1) 均摊         | O(log S)    |
// | snap  | O(n)      | O(1)              | O(1)        |
// | get   | O(1)      | O(log S)          | O(log S)    |
// | 空间  | O(n×S)    | O(修改次数)        | O(修改次数)  |
//
// 面试选择:
//   - 先说解法1展示理解，然后分析瓶颈（snap O(n)，空间爆炸）
//   - 给出解法2，重点讲二分查找的设计
//   - 如果面试官追问，可以提到解法3作为替代
//
// 解法2 vs 解法3:
//   - 解法2: set更快(O(1) vs O(logS)), 空间更紧凑(vector连续内存)
//   - 解法3: 代码更简洁(不需要手动去重), 但常数因子大
//   - 实际面试两种都可以，但解法2更能展示编码功底
//
// ============================================================
//
// 【易错点】
//
// 1. snap() 返回值时机:
//    ✗ int snap() { cur_snap++; return cur_snap; }  // snap_id 从1开始
//    ✓ int snap() { return cur_snap++; }             // 先返回0再递增
//
// 2. 同一 snap_id 多次 set 同一 index 不去重:
//    ✗ history[index].push_back({cur_snap, val}); // 无条件追加
//      → 导致 history 中出现多个 first==cur_snap 的记录
//    ✓ 先检查 back().first == cur_snap 再决定覆盖还是追加
//
// 3. upper_bound 的 target pair 的 second 用 0 而不是 INT_MAX:
//    ✗ upper_bound(..., {snap_id, 0})
//      → 如果 history 中恰好有 (snap_id, v) 其中 v > 0, 
//        那么 (snap_id, v) > (snap_id, 0), 它不会被跳过
//        upper_bound 会指向 (snap_id, v) 而不是它之后
//        --it 就会指向前一条记录，漏掉了 snap_id 的记录!
//    ✓ upper_bound(..., {snap_id, INT_MAX})
//      → 保证所有 first==snap_id 的 pair 都 < target
//
// 4. history 未初始化导致 get 空列表 --it 越界:
//    ✗ 构造函数中不初始化 history[i]
//      → 如果 index=5 从未被 set，get(5, 0) 时 h 为空
//         upper_bound 返回 begin(), --it 越界
//    ✓ 构造时 history[i].push_back({0, 0}) 保证非空
//
// ============================================================
//
// 【面试追问】
//
// Q1: 暴力解的瓶颈在哪？能不能不复制整个数组？
//     → snap 需要 O(n) 复制，大量元素没变化也被复制。
//       改为"只存变化过的元素"，这就是增量存储的思想。
//
// Q2: get 操作用二分查找是 O(log S)，能否做到 O(1)？
//     → 可以，如果在 set/snap 时多做工作（如 snap 时对每个脏位记录快照值），
//       但那就回到了 snap O(dirty_count) 的问题，是时间-空间的 tradeoff。
//       当前方案在 set/snap/get 之间取得了最好的平衡。
//
// Q3: 如果这个系统需要支持"回滚到某个快照"操作怎么做？
//     → 回滚即把 cur_snap 设回目标 snap_id，然后后续 set 覆盖历史。
//       但需要清理目标 snap_id 之后的记录，否则 get 结果不正确。
//       可以考虑"版本分支"模型（类似 Git），更复杂但更灵活。
//
// Q4: 这个设计和数据库的 MVCC 有什么关系？
//     → 本质相同！MVCC 也是为每行数据维护多版本(version_id, value)，
//       读操作根据事务开始时间找到对应版本，不需要加锁。
//       本题的 snap_id 相当于 MVCC 中的 transaction_id。
//
// ============================================================
