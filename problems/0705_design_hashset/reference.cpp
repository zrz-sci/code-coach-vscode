/*
 * LeetCode 705: Design HashSet (设计哈希集合)
 *
 * 【题目本质】
 * 不使用内置哈希表库，自己实现 HashSet，支持 add、remove、contains。
 * 考点：哈希函数设计 + 冲突处理机制（链地址法 / 开放寻址法）。
 *
 * 【解法总览】
 * 解法1: 布尔数组直接映射 — O(1) / O(范围)  — 利用 key 范围有限
 * 解法2: 链地址法         — O(1)均摊 / O(n+B) — 面试推荐，展示哈希原理
 * 解法3: 开放寻址法       — O(1)均摊 / O(B)   — 进阶，缓存友好
 */

// ============================================================
// 解法1: 布尔数组直接映射
// 时间: O(1)  空间: O(10^6)
//
// 【思路】
// key 范围 0~10^6，直接开 bool 数组，用下标表示 key 是否存在。
// 优点：极简，每个操作一行代码。
// 缺点：没有展示哈希表设计能力，面试中仅作为暖场方案提一嘴。
//
// 数据结构示意:
//   data[0..1000000]
//   add(1):      data[1] = true
//   contains(1): return data[1]
//   remove(1):   data[1] = false
// ============================================================
class MyHashSet_v1 {
    vector<bool> data;
public:
    MyHashSet_v1() : data(1000001, false) {}

    void add(int key)      { data[key] = true; }
    void remove(int key)   { data[key] = false; }
    bool contains(int key) { return data[key]; }
};

// ============================================================
// 解法2: 链地址法 (Separate Chaining) — 面试推荐 ⭐
// 时间: 平均 O(1), 最坏 O(n/B)  空间: O(n + B)
//
// 【思路】
// 1. 选质数 BUCKET = 1009 作为桶数
// 2. hash(key) = key % BUCKET，将 key 映射到某个桶
// 3. 每个桶用 list<int> (链表) 存放所有映射到该桶的 key
// 4. 冲突的 key 挂在同一条链表上
//
// 为什么选质数?
//   合数桶数(如1000)会导致有规律的key大量映射到同一批桶。
//   质数(1009)让取模结果更均匀分散。
//   例: key=2,4,6,8... 桶数=1000 → 只用偶数桶
//       key=2,4,6,8... 桶数=1009 → 分散到各桶
//
// 桶数选择:
//   题目最多 10^4 次操作，选 1009 个桶，
//   平均每桶 ~10 个元素，链表遍历代价极小。
//
// 数据结构示意:
//
//   buckets[0..1008]
//   ┌─────┐
//   │  0  │ → [1009] → [2018] → null
//   ├─────┤
//   │  1  │ → [1] → [1010] → null
//   ├─────┤
//   │  2  │ → [2] → null
//   ├─────┤
//   │ ... │
//   └─────┘
//
//   add(1):    hash=1, 桶1为空, push_back(1)
//   add(1010): hash=1, 桶1已有[1], 1010!=1, push_back(1010)
//   contains(1): hash=1, 遍历桶1: 1==1 → true
//   remove(1): hash=1, 从桶1链表中删除1
//   contains(1): hash=1, 遍历桶1: 1010!=1 → false
// ============================================================
class MyHashSet_v2 {
    static const int BUCKET = 1009; // 质数桶数
    vector<list<int>> data;         // 每个桶一条链表

    int hash(int key) { return key % BUCKET; }

public:
    MyHashSet_v2() : data(BUCKET) {}

    void add(int key) {
        int h = hash(key);
        // 先检查是否已存在——HashSet 不允许重复
        // 这一步容易遗漏！如果不检查，同一个 key 会被多次插入
        for (int x : data[h]) {
            if (x == key) return; // 已存在，直接返回
        }
        data[h].push_back(key);
    }

    void remove(int key) {
        int h = hash(key);
        // list::remove(val) 删除链表中所有等于 val 的节点
        // 由于 add 保证了不重复，实际最多删一个
        data[h].remove(key);
    }

    bool contains(int key) {
        int h = hash(key);
        for (int x : data[h]) {
            if (x == key) return true;
        }
        return false;
    }
};

// ============================================================
// 解法3: 开放寻址法 (Open Addressing, Linear Probing)
// 时间: 平均 O(1), 最坏 O(n)  空间: O(B)
//
// 【思路】
// 不用链表，所有元素直接存在数组中。
// 冲突时线性探测下一个空位。
//
// 关键难点：删除操作
//   不能直接置空！否则探测链断裂，后续 contains 会误判。
//   解决方案：用特殊标记 TOMBSTONE 表示"此位曾有元素但已删除"。
//   - contains 遇到 TOMBSTONE 时继续往后探测
//   - add 遇到 TOMBSTONE 时可以在此位插入（复用空间）
//
// 状态:
//   EMPTY     = -1  (从未使用)
//   TOMBSTONE = -2  (已删除)
//   >=0            (有效 key)
//
// 示例 (capacity=7):
//   add(1):  slot = 1%7 = 1, 空位 → data[1] = 1
//   add(8):  slot = 8%7 = 1, 已占 → 探测 slot=2, 空位 → data[2] = 8
//   remove(1): data[1] = TOMBSTONE (不能设 EMPTY!)
//   contains(8): slot=1, TOMBSTONE→继续, slot=2, 8==8 → true
//
// 如果 remove(1) 时设 EMPTY:
//   contains(8): slot=1, EMPTY→停止探测 → false (错误!)
// ============================================================
class MyHashSet_v3 {
    static const int CAP = 20011; // 选比最大元素数大的质数
    static const int EMPTY = -1;
    static const int TOMBSTONE = -2;
    vector<int> data;

    int hash(int key) { return key % CAP; }

    // 找到 key 所在的槽位，或者找到第一个可插入的空位
    // 返回 {found_slot, insert_slot}
    // found_slot = 找到key的位置 (-1 if not found)
    // insert_slot = 可以插入的位置 (EMPTY 或 TOMBSTONE)
    pair<int,int> probe(int key) {
        int h = hash(key);
        int insert_slot = -1;
        for (int i = 0; i < CAP; i++) {
            int slot = (h + i) % CAP;
            if (data[slot] == EMPTY) {
                // 遇到从未使用的空位，key 一定不存在
                if (insert_slot == -1) insert_slot = slot;
                return {-1, insert_slot};
            }
            if (data[slot] == TOMBSTONE) {
                // 记录第一个 tombstone 位置（可复用）
                if (insert_slot == -1) insert_slot = slot;
                continue; // 继续探测，因为 key 可能在后面
            }
            if (data[slot] == key) {
                return {slot, slot}; // 找到了
            }
        }
        return {-1, insert_slot}; // 表满（理论上不会发生）
    }

public:
    MyHashSet_v3() : data(CAP, EMPTY) {}

    void add(int key) {
        auto [found, ins] = probe(key);
        if (found != -1) return; // 已存在
        data[ins] = key;
    }

    void remove(int key) {
        auto [found, ins] = probe(key);
        if (found != -1) {
            data[found] = TOMBSTONE; // 标记为已删除，不是 EMPTY!
        }
    }

    bool contains(int key) {
        auto [found, ins] = probe(key);
        return found != -1;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法          | 时间(均摊) | 空间     | 优势                    | 劣势                     |
// |--------------|-----------|---------|------------------------|--------------------------|
// | 布尔数组      | O(1)      | O(范围)  | 极简，绝对 O(1)          | key范围大时不可行          |
// | 链地址法      | O(1)      | O(n+B)  | 实现直觉，删除简单         | 链表指针开销，缓存不友好    |
// | 开放寻址法    | O(1)      | O(B)    | 缓存友好，空间紧凑         | 删除需 tombstone，写法复杂  |
//
// 面试选择：先提布尔数组方案表明理解题意，然后实现链地址法。
// 如果面试官追问开放寻址法或动态扩容(rehash)，再展开讲。
//
// ============================================================
// 【易错点】
//
// 1. add 忘记去重:
//    ✗ 直接 push_back(key) → 同一个 key 被插入多次
//    ✓ 先遍历检查是否存在，不存在才插入
//
// 2. 桶数选偶数:
//    ✗ BUCKET = 1000 → 偶数key全映射到偶数桶，一半桶浪费
//    ✓ BUCKET = 1009 → 质数，分布更均匀
//
// 3. 开放寻址删除直接置空:
//    ✗ data[slot] = EMPTY → 探测链断裂
//    ✓ data[slot] = TOMBSTONE → 探测时跳过继续找
//
// 4. 混淆 HashSet 和 HashMap:
//    ✗ HashSet 的 add 更新值 → 不对，Set 只存 key，无 value 可更新
//    ✓ HashSet 的 add 遇到已存在的 key 直接跳过
//
// ============================================================
// 【面试追问】
//
// Q1: 为什么选质数作为桶数？
//     合数桶数导致有规律的 key 映射不均匀。
//     例: 桶数=1000, key都是偶数 → 只用偶数桶，负载翻倍。
//     质数桶数让 key%BUCKET 的余数更均匀分散。
//
// Q2: 负载因子超标怎么办？
//     当 元素数/桶数 > 0.75 时，扩容: 桶数翻倍(取下一个质数)，
//     所有元素 rehash 到新桶。均摊 O(1)。
//     这就是 Java HashMap resize 的机制。
//
// Q3: 链地址法 vs 开放寻址法怎么选？
//     链地址法: 删除简单，适合冲突多的场景 (Java HashMap)。
//     开放寻址法: 缓存友好，适合负载因子低的场景 (Python dict)。
//
// Q4: 如何把 HashSet 改成 HashMap？
//     链表节点从 int 变成 pair<int,int>。
//     add 变 put: key已存在时更新value而非跳过。
//     contains 变 get: 返回 value 或 -1。
//     → 这就是 LeetCode 706 的解法。
//
// ============================================================
// 【相关题型】
//
// - 706. 设计哈希映射:
//   从 Set 扩展到 Map，节点存 pair<key,value>，put 时更新 value。
//
// - 146. LRU 缓存:
//   哈希表 + 双向链表。哈希表 O(1) 查找，链表维护访问顺序。
//
// - 380. O(1) 时间插入、删除和获取随机元素:
//   哈希表 + 数组。哈希表 O(1) 查找删除，数组支持 O(1) 随机访问。
// ============================================================
