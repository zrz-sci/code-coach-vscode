/*
 * LeetCode 706: 设计哈希映射 (Design HashMap)
 *
 * 【题目本质】
 * 从零实现哈希表：设计哈希函数 + 处理哈希冲突，支持 put/get/remove 操作。
 *
 * 【解法总览】
 * 解法1: 超大数组（直接寻址） — O(1) / O(10^6) — 最简单，回避了哈希设计
 * 解法2: 拉链法（取模+链表）  — O(N/B) / O(B+N) — 面试首选 ⭐
 * 解法3: 开放寻址法（线性探测）— O(1)均摊 / O(B) — 加分项，工业常用
 */

// ============================================================
// 解法1: 超大数组 — key 直接作为数组下标
// 时间: O(1)  空间: O(10^6)
//
// 【思路】
// key 范围是 [0, 10^6]，最直觉的做法：开一个大小 10^6+1 的数组，
// 用 key 直接做下标。-1 表示"该 key 不存在"。
// 完全不需要哈希函数，也没有冲突问题。
// 但这不是真正的"设计哈希映射"，只是利用了 key 范围小的特点。
//
// 示意:
//   data[0] = -1  (不存在)
//   data[1] = 1   (key=1, value=1)
//   data[2] = 2   (key=2, value=2)
//   ...
//   data[1000000] = -1
// ============================================================
class MyHashMap1 {
    vector<int> data;
public:
    MyHashMap1() : data(1000001, -1) {}
    
    void put(int key, int value) {
        data[key] = value;
    }
    
    int get(int key) {
        return data[key];
    }
    
    void remove(int key) {
        data[key] = -1;
    }
};

// ============================================================
// 解法2: 拉链法（Separate Chaining）— 面试首选 ⭐
// 时间: 均摊 O(N/B)  空间: O(B + N)
// 其中 B=桶数, N=元素数
//
// 【思路】
// 真正的哈希表设计：
// 1. 哈希函数: h(key) = key % B，把 key 映射到 [0, B-1]
// 2. 冲突处理: 每个桶维护一个链表，冲突的元素串在同一个桶里
// 3. put: 算桶号 → 遍历链表看 key 是否存在 → 存在则更新，不存在则追加
// 4. get: 算桶号 → 遍历链表查找
// 5. remove: 算桶号 → 遍历链表找到并删除
//
// 为什么选 769 作为桶大小？
// - 质数取模能让哈希值分布更均匀
// - 10^4 次操作 / 769 ≈ 13，平均链表长度很短
//
// 内部结构示意:
//   buckets[]:
//     [0] → null
//     [1] → (1, 1) → (770, 7) → null    ← 1%769=1, 770%769=1
//     [2] → (2, 2) → null
//     ...
//     [768] → null
// ============================================================
class MyHashMap {
    static const int BUCKET_SIZE = 769; // 质数，减少冲突
    vector<list<pair<int, int>>> buckets;

public:
    MyHashMap() : buckets(BUCKET_SIZE) {}
    
    void put(int key, int value) {
        int idx = key % BUCKET_SIZE;
        // 关键：先查找 key 是否已存在，已存在则更新（不是直接追加！）
        for (auto& [k, v] : buckets[idx]) {
            if (k == key) {
                v = value;
                return;
            }
        }
        // key 不存在，追加到链表末尾
        buckets[idx].emplace_back(key, value);
    }
    
    int get(int key) {
        int idx = key % BUCKET_SIZE;
        for (const auto& [k, v] : buckets[idx]) {
            if (k == key) return v;
        }
        return -1;
    }
    
    void remove(int key) {
        int idx = key % BUCKET_SIZE;
        auto& bucket = buckets[idx];
        // 用显式迭代器遍历，找到后 erase 并 return
        // 不能用范围 for + erase，那是未定义行为
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->first == key) {
                bucket.erase(it);
                return;
            }
        }
    }
};

// ============================================================
// 解法3: 开放寻址法（Linear Probing）
// 时间: 均摊 O(1)（低负载因子时）  空间: O(B)
//
// 【思路】
// 与拉链法不同，开放寻址法不用额外的链表。
// 冲突时，在数组内部向后探测，找到下一个空位。
//
// 核心要点：
// - 桶要开得足够大（远大于元素数），保证低负载因子
// - 删除不能用 EMPTY，必须用 DELETED 墓碑标记
//   否则会打断探测链，导致后面的元素找不到
//
// 探测过程示意:
//   put(5, 'A'):   hash=5 → slot[5]空 → 放入
//   put(10012,'B'): hash=10012%10007=5 → slot[5]被占
//                   → 探测slot[6]空 → 放入
//   get(10012):    hash=5 → slot[5]是key=5≠10012
//                  → 探测slot[6]是key=10012 → 返回'B'
//   remove(5):     hash=5 → slot[5]是key=5 → 标记DELETED
//                  (不能用EMPTY! 否则get(10012)在slot[5]就停止了)
// ============================================================
class MyHashMap3 {
    static const int BUCKET_SIZE = 20011; // 大质数，保证低负载因子
    static const int EMPTY = -1;
    static const int DELETED = -2;
    
    vector<int> keys;
    vector<int> values;

public:
    MyHashMap3() : keys(BUCKET_SIZE, EMPTY), values(BUCKET_SIZE, 0) {}
    
    void put(int key, int value) {
        int idx = key % BUCKET_SIZE;
        int firstDeleted = -1; // 记录第一个墓碑位，用于复用空间
        
        while (keys[idx] != EMPTY) {
            if (keys[idx] == key) {
                values[idx] = value; // 已存在，更新
                return;
            }
            // 记录第一个 DELETED 位置，后续可能用来插入
            if (keys[idx] == DELETED && firstDeleted == -1) {
                firstDeleted = idx;
            }
            idx = (idx + 1) % BUCKET_SIZE;
        }
        
        // key 不存在，优先插入到 firstDeleted 位置（复用墓碑位）
        int insertIdx = (firstDeleted != -1) ? firstDeleted : idx;
        keys[insertIdx] = key;
        values[insertIdx] = value;
    }
    
    int get(int key) {
        int idx = key % BUCKET_SIZE;
        while (keys[idx] != EMPTY) {
            if (keys[idx] == key) return values[idx];
            idx = (idx + 1) % BUCKET_SIZE; // 跳过 DELETED 和不匹配的
        }
        return -1; // 遇到 EMPTY，说明 key 不存在
    }
    
    void remove(int key) {
        int idx = key % BUCKET_SIZE;
        while (keys[idx] != EMPTY) {
            if (keys[idx] == key) {
                keys[idx] = DELETED; // 墓碑标记！不是 EMPTY！
                return;
            }
            idx = (idx + 1) % BUCKET_SIZE;
        }
    }
};

// ============================================================
// 【解法对比】
//
// 超大数组 vs 拉链法:
//   超大数组完全回避了哈希设计，本质是直接寻址表。
//   拉链法才是真正的哈希表实现，面试必须掌握。
//
// 拉链法 vs 开放寻址法:
//   拉链法：实现简单、删除容易、不怕高负载因子
//   开放寻址法：缓存友好（数据连续）、删除需要墓碑标记、
//              负载因子高时性能急剧下降
//   面试选拉链法；工业实现（如 Python dict）常用开放寻址。
//
// 【易错点】
// 1. put 时忘记先检查 key 是否存在，直接追加 → 同一个 key 出现多个节点
//    ✗ buckets[idx].push_back({key, value});  // 没有先查找
//    ✓ 先 for 遍历，找到 key 就更新 value 并 return，找不到才 push_back
//
// 2. 拉链法 remove 时用范围 for + erase → 未定义行为
//    ✗ for (auto& p : bucket) { if (p.first == key) bucket.erase(???); }
//    ✓ 用显式迭代器: for (auto it = begin; it != end; ++it) { erase(it); return; }
//
// 3. 开放寻址法 remove 用 EMPTY 而非 DELETED → 打断探测链
//    ✗ keys[idx] = EMPTY;  // 后面的冲突元素再也找不到了
//    ✓ keys[idx] = DELETED; // 墓碑标记，探测时跳过它继续找
//
// 4. 桶大小选 2 的幂次 → 哈希分布不均匀
//    ✗ key % 1024  // 只看低10位，偶数key全落在偶数桶
//    ✓ key % 769   // 质数取模，分布更均匀
//
// 【面试追问】（递进链）
// Q1: 拉链法最坏时间复杂度是多少？什么时候发生？
//     → O(N)，所有 key 映射到同一个桶时（退化为单链表）
//
// Q2: 如何避免最坏情况？工程中怎么做？
//     → (1) 动态扩容(rehash): 负载因子>0.75时桶数翻倍并重新哈希
//       (2) 链表过长时转红黑树 (Java 8 HashMap: 链表长度>8 转树)
//
// Q3: 如果需要支持多线程并发访问怎么办？
//     → (1) 全局锁: 简单但并发度低
//       (2) 分段锁: 不同桶不同锁 (Java ConcurrentHashMap 早期方案)
//       (3) CAS + 无锁链表: 最高并发度但实现复杂
// ============================================================
