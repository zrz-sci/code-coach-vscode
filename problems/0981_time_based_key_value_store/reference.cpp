/*
 * LeetCode 981: 基于时间的键值存储 (Time Based Key-Value Store)
 *
 * 【题目本质】
 * 设计一个数据结构：每个 key 下存多个 (timestamp, value) 对，
 * get 操作要找 <= 给定 timestamp 的最大时间戳对应的值。
 * 核心操作 = 在有序序列上做 "floor 查询"（<= target 的最大值）。
 *
 * 【解法总览】
 * 解法1: HashMap + 线性扫描     — set O(1) / get O(N)   / 空间 O(N) — 最直觉
 * 解法2: HashMap + 手写二分查找  — set O(1) / get O(logN) / 空间 O(N) — 面试首选 ⭐
 * 解法3: HashMap + STL upper_bound — set O(1) / get O(logN) / 空间 O(N) — STL 简洁版
 * 解法4: HashMap + std::map     — set O(logN) / get O(logN) / 空间 O(N) — 通用方案
 */

// ============================================================
// 解法1: HashMap + 线性扫描
// set: O(1)  get: O(N)  空间: O(N)
//
// 【思路】
// 最直觉的做法：每个 key 对应一个 vector<(timestamp, value)>。
// 因为 set 的时间戳严格递增，vector 天然有序。
// get 时从后往前扫描，找第一个 ts <= timestamp 的记录。
//
// 为什么从后往前？因为越靠后的时间戳越大，
// 如果查询时间戳较新（常见场景），可以更快命中。
// 但最坏情况（查很老的时间戳或 key 不存在匹配）仍然是 O(N)。
//
// 数据结构示意：
//   store["foo"] = [(1,"bar"), (4,"bar2"), (7,"bar3")]
//                    ↑ 有序，按 timestamp 递增
//
//   get("foo", 5):
//     从后往前: 7>5 跳过, 4<=5 命中 → "bar2"
// ============================================================
class TimeMap1 {
public:
    unordered_map<string, vector<pair<int, string>>> store;

    TimeMap1() {}

    void set(string key, string value, int timestamp) {
        // 时间戳严格递增，直接 push_back 即可保持有序
        store[key].emplace_back(timestamp, value);
    }

    string get(string key, int timestamp) {
        // 先检查 key 是否存在，避免 store[key] 创建空 entry
        auto it = store.find(key);
        if (it == store.end()) return "";

        auto& vec = it->second;
        // 从后往前线性扫描
        for (int i = (int)vec.size() - 1; i >= 0; i--) {
            if (vec[i].first <= timestamp) {
                return vec[i].second;
            }
        }
        // 所有时间戳都 > timestamp，没有匹配
        return "";
    }
};

// ============================================================
// 解法2: HashMap + 手写二分查找 ⭐ 面试首选
// set: O(1)  get: O(log N)  空间: O(N)
//
// 【思路】
// 解法1 的瓶颈：get 线性扫描 O(N)。
// 既然 vector 已经按 timestamp 有序（题目保证递增），
// 我们可以用二分查找把 get 从 O(N) 优化到 O(log N)。
//
// 目标：找 vector 中最后一个 ts <= timestamp 的位置。
// 等价于：upper_bound(timestamp) 的前一个位置。
//
// 手写二分的好处：面试中展示你理解二分的本质，不依赖 STL。
//
// 二分搜索空间收缩过程：
//   records = [(1,"bar"), (4,"bar2"), (7,"bar3")]
//
//   get(timestamp=5): 找 <= 5 的最大
//   ────────────────────────────────
//   Round1: lo=0, hi=2, mid=1
//           [(1), (4), (7)]
//                  ↑ 4<=5 → ans=1, lo=2
//   Round2: lo=2, hi=2, mid=2
//           [(1), (4), (7)]
//                        ↑ 7>5 → hi=1
//   lo=2 > hi=1 → 结束, ans=1 → "bar2" ✓
//
//   get(timestamp=0): 找 <= 0 的最大
//   ────────────────────────────────
//   Round1: lo=0, hi=2, mid=1
//           4>0 → hi=0
//   Round2: lo=0, hi=0, mid=0
//           1>0 → hi=-1
//   lo=0 > hi=-1 → 结束, ans=-1 → "" ✓
// ============================================================
class TimeMap2 {
public:
    unordered_map<string, vector<pair<int, string>>> store;

    TimeMap2() {}

    void set(string key, string value, int timestamp) {
        store[key].emplace_back(timestamp, value);
    }

    string get(string key, int timestamp) {
        auto it = store.find(key);
        if (it == store.end()) return "";

        auto& vec = it->second;

        // 二分查找：找最后一个 ts <= timestamp 的位置
        int lo = 0, hi = (int)vec.size() - 1;
        int ans = -1;  // -1 表示没找到任何 <= timestamp 的记录

        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;  // 防溢出写法
            if (vec[mid].first <= timestamp) {
                // mid 满足条件，记录为候选答案
                // 但可能有更大的 timestamp 也满足，继续往右找
                ans = mid;
                lo = mid + 1;
            } else {
                // vec[mid].first > timestamp，mid 及其右边都不行
                hi = mid - 1;
            }
        }

        return ans == -1 ? "" : vec[ans].second;
    }
};

// ============================================================
// 解法3: HashMap + STL upper_bound
// set: O(1)  get: O(log N)  空间: O(N)
//
// 【思路】
// 和解法2 完全相同的逻辑，但用 STL 的 upper_bound 代替手写二分。
// upper_bound 返回第一个 > timestamp 的迭代器，
// 它的前一个就是 <= timestamp 的最大值。
//
// 注意 upper_bound 的自定义比较函数：
// 对于 upper_bound(begin, end, value, comp)，
// comp 的签名是 comp(value, element)，即 (int, pair<int,string>)。
// 这个参数顺序很容易写反！
// ============================================================
class TimeMap3 {
public:
    unordered_map<string, vector<pair<int, string>>> store;

    TimeMap3() {}

    void set(string key, string value, int timestamp) {
        store[key].emplace_back(timestamp, value);
    }

    string get(string key, int timestamp) {
        auto it = store.find(key);
        if (it == store.end()) return "";

        auto& vec = it->second;

        // upper_bound 找第一个 timestamp(element) > timestamp(value) 的位置
        // 自定义比较：comp(value, element) → value < element.first
        auto ub = upper_bound(vec.begin(), vec.end(), timestamp,
            [](int ts, const pair<int, string>& p) {
                return ts < p.first;  // ts 是 value，p 是 element
            });

        // 如果 ub == begin()，说明所有记录的时间戳都 > timestamp
        if (ub == vec.begin()) return "";

        // 前一个位置就是 <= timestamp 的最大时间戳
        return prev(ub)->second;
    }
};

// ============================================================
// 解法4: HashMap + std::map（通用方案）
// set: O(log N)  get: O(log N)  空间: O(N)
//
// 【思路】
// 如果题目不保证 set 的时间戳递增怎么办？
// 用 std::map<int, string> 代替 vector，自动维护 timestamp 有序。
// map::upper_bound 直接可用，代码最简洁。
//
// 代价：set 从 O(1) 变成 O(log N)（红黑树插入），
// 且 map 的节点分散在堆上，缓存不友好。
// 在本题时间戳已递增的条件下，解法2/3 更优。
//
// 但在面试中，如果面试官追问"时间戳不递增怎么办"，
// 这就是你的答案。
// ============================================================
class TimeMap4 {
public:
    unordered_map<string, map<int, string>> store;

    TimeMap4() {}

    void set(string key, string value, int timestamp) {
        // O(log N) 插入红黑树
        store[key][timestamp] = value;
    }

    string get(string key, int timestamp) {
        auto it = store.find(key);
        if (it == store.end()) return "";

        auto& m = it->second;

        // upper_bound 找第一个 key > timestamp 的位置
        auto ub = m.upper_bound(timestamp);

        // 如果 ub == begin()，所有时间戳都 > timestamp
        if (ub == m.begin()) return "";

        // prev(ub) 指向 <= timestamp 的最大时间戳
        return prev(ub)->second;
    }
};

// ============================================================
// 【解法对比】
//
// | 维度         | 解法1:线性扫描 | 解法2:手写二分 | 解法3:STL二分 | 解法4:std::map |
// |-------------|---------------|---------------|-------------|---------------|
// | set 时间     | O(1)          | O(1)          | O(1)        | O(log N)      |
// | get 时间     | O(N)          | O(log N) ⭐    | O(log N)    | O(log N)      |
// | 空间效率      | vector 连续   | vector 连续   | vector 连续  | 红黑树分散     |
// | 代码量       | 最简单        | 中等           | 简洁         | 最简洁        |
// | ts不递增时    | 需额外排序    | 需额外排序     | 需额外排序   | ✓ 自动有序    |
//
// 面试推荐：解法2（手写二分展示理解），然后提一句解法4 应对追问。
//
// 【易错点】
//
// 1. store[key] 在 key 不存在时会创建空 entry:
//    ✗ auto& vec = store[key];  // key 不存在时插入空 vector，浪费内存
//    ✓ auto it = store.find(key); if (it == store.end()) return "";
//
// 2. 二分找到的 ans 忘记判断 -1:
//    ✗ return vec[ans].second;  // 当所有 ts > timestamp 时 ans=-1，越界！
//    ✓ return ans == -1 ? "" : vec[ans].second;
//
// 3. upper_bound 自定义比较函数参数顺序写反:
//    ✗ [](const pair<int,string>& p, int ts) { return p.first < ts; }
//       这是 lower_bound 的比较函数签名！upper_bound 要求 comp(value, element)
//    ✓ [](int ts, const pair<int,string>& p) { return ts < p.first; }
//
// 4. upper_bound 返回 begin() 时忘记特判:
//    ✗ return prev(ub)->second;  // ub==begin() 时 prev 越界！
//    ✓ if (ub == vec.begin()) return ""; return prev(ub)->second;
//
// 5. 手写二分中 mid 计算溢出:
//    ✗ int mid = (lo + hi) / 2;  // lo+hi 可能溢出 int（虽然本题数据量不大）
//    ✓ int mid = lo + (hi - lo) / 2;
//
// 【面试追问链】
//
// Q1(基础理解): 为什么 get 不能用哈希表直接 O(1) 查？
// → 因为 get 不是精确匹配 timestamp，而是 "floor 查询"（<= timestamp 的最大值）。
//   哈希表只能精确匹配，范围查询需要有序结构。
//
// Q2(优化): 为什么能用二分？如果 set 的时间戳不递增呢？
// → 题目保证时间戳严格递增 → vector 天然有序 → 二分可行。
//   如果不递增，用 std::map（自动排序）或每次 set 后用插入排序维护有序。
//
// Q3(变体): 如果要支持 delete(key, timestamp) 怎么办？
// → vector 删除中间元素 O(N)，不合适。改用 std::map（O(logN) 删除），
//   或者 vector + 惰性删除标记（set 一个 tombstone 值）。
//
// Q4(扩展): 如果要查 key 在 [t1, t2] 范围内的所有值？
// → 两次二分找左右边界，遍历区间。时间 O(logN + K)，K 是结果数。
//   用 lower_bound(t1) 和 upper_bound(t2) 确定范围。
// ============================================================
