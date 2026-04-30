/*
 * LeetCode 146: LRU 缓存 (LRU Cache)
 *
 * 【题目本质】
 * 设计一个数据结构，同时支持 O(1) 的查找（按 key）、O(1) 的插入/更新、
 * O(1) 的删除最久未使用元素。核心是「哈希表 + 双向链表」的经典组合。
 *
 * 【解法总览】
 * 解法1: 暴力（数组+时间戳）   — O(n) per op / O(n) — 理解题意
 * 解法2: HashMap+手写双向链表  — O(1) per op / O(n) — 面试标准答案
 * 解法3: HashMap+STL list     — O(1) per op / O(n) — 简洁写法
 */

// ============================================================
// 解法1: 暴力 — 数组存储，线性查找
// 时间: O(n) per operation  空间: O(n)
//
// 【思路】
// 最直觉的做法：用数组存 (key, value, timestamp)。
// get: 遍历数组找 key，更新 timestamp。
// put: 遍历找 key 更新，或满时遍历找最小 timestamp 驱逐后插入。
// 瓶颈：每个操作都要遍历整个数组 → O(n)。
// 这个解法帮助理解 LRU 语义，但不满足 O(1) 要求。
// ============================================================
class LRUCache1 {
public:
    int cap;
    int timestamp;
    // 每个元素: {key, value, last_used_time}
    vector<tuple<int, int, int>> store;

    LRUCache1(int capacity) : cap(capacity), timestamp(0) {}

    int get(int key) {
        for (auto& [k, v, t] : store) {
            if (k == key) {
                t = ++timestamp;
                return v;
            }
        }
        return -1;
    }

    void put(int key, int value) {
        for (auto& [k, v, t] : store) {
            if (k == key) {
                v = value;
                t = ++timestamp;
                return;
            }
        }
        if ((int)store.size() == cap) {
            // 找时间戳最小的驱逐
            int minIdx = 0;
            for (int i = 1; i < (int)store.size(); i++) {
                if (get<2>(store[i]) < get<2>(store[minIdx]))
                    minIdx = i;
            }
            store.erase(store.begin() + minIdx);
        }
        store.push_back({key, value, ++timestamp});
    }
};

// ============================================================
// 解法2: HashMap + 手写双向链表 — 面试标准答案 ⭐
// 时间: O(1) per operation  空间: O(capacity)
//
// 【思路】
// 解法1的两个瓶颈：
//   (1) 查找 key 需要 O(n) → 用 HashMap O(1)
//   (2) 找最久未使用 + 更新顺序需要 O(n) → 用双向链表 O(1)
//
// 数据结构设计：
//   HashMap: key → 双向链表节点指针（O(1) 定位）
//   双向链表: 维护使用顺序（头部=最近使用，尾部=最久未使用）
//
//  unordered_map<int, DLinkedNode*>
//  ┌──────────────────────────────┐
//  │ key=1 → Node*(k=1,v=1)      │
//  │ key=3 → Node*(k=3,v=3)      │
//  └──────────────────────────────┘
//              │ 指向链表中的节点
//              ▼
//  DummyHead ←→ [1,1] ←→ [3,3] ←→ DummyTail
//  (最近使用)                    (最久未使用)
//
// 核心操作：
//   get(key):  HashMap找到节点 → moveToHead → 返回value
//   put(key,v): 已存在? 更新+moveToHead
//               不存在? 新建+addToHead, 超容量则 removeTail+从HashMap删
//
// 链表指针操作图解：
//
//   removeNode(node):
//     A ←→ node ←→ B   变成   A ←→ B
//     node->prev->next = node->next
//     node->next->prev = node->prev
//
//   addToHead(node):
//     Head ←→ first ←→ ...  变成  Head ←→ node ←→ first ←→ ...
//     四步: node->prev=head, node->next=head->next,
//           head->next->prev=node, head->next=node
//     注意: 先设置 node 的指针，再改周围节点，否则会丢失引用！
// ============================================================

struct DLinkedNode {
    int key, value;           // 必须存 key! 驱逐时需要从 HashMap 中删除
    DLinkedNode* prev;
    DLinkedNode* next;
    DLinkedNode() : key(0), value(0), prev(nullptr), next(nullptr) {}
    DLinkedNode(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

class LRUCache2 {
private:
    unordered_map<int, DLinkedNode*> cache;
    DLinkedNode* head;  // 哨兵头 — 避免空指针判断
    DLinkedNode* tail;  // 哨兵尾 — 避免空指针判断
    int capacity;
    int size;

    // 把 node 插到 DummyHead 之后（标记为"最近使用"）
    void addToHead(DLinkedNode* node) {
        // 为什么按这个顺序？先设置 node 自己的指针，
        // 再修改周围节点的指针，这样不会丢失任何引用
        node->prev = head;
        node->next = head->next;
        head->next->prev = node;  // 旧的第一个数据节点的 prev 指向 node
        head->next = node;        // head 的 next 指向 node
    }

    // 从链表中摘掉 node（不释放内存）
    void removeNode(DLinkedNode* node) {
        // 因为是双向链表，O(1) 就能完成
        // 如果是单链表，找 prev 需要 O(n)，这就是为什么要用双向链表
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    // 移到头部 = 摘掉 + 插到头部（分两步更清晰，不易出错）
    void moveToHead(DLinkedNode* node) {
        removeNode(node);
        addToHead(node);
    }

    // 删除尾部节点并返回（用于驱逐 + 从 HashMap 中删除）
    DLinkedNode* removeTail() {
        // 注意: tail 是哨兵，真正的最后一个数据节点是 tail->prev
        DLinkedNode* node = tail->prev;
        removeNode(node);
        return node;
    }

public:
    LRUCache2(int capacity) : capacity(capacity), size(0) {
        // 初始化哨兵节点，形成 head ←→ tail 的空链表
        head = new DLinkedNode();
        tail = new DLinkedNode();
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        auto it = cache.find(key);
        if (it == cache.end()) return -1;

        DLinkedNode* node = it->second;
        moveToHead(node);   // 被访问了 → 标记为最近使用
        return node->value;
    }

    void put(int key, int value) {
        auto it = cache.find(key);
        if (it != cache.end()) {
            // key 已存在：更新 value + 移到头部
            DLinkedNode* node = it->second;
            node->value = value;
            moveToHead(node);
        } else {
            // key 不存在：创建新节点
            DLinkedNode* newNode = new DLinkedNode(key, value);
            cache[key] = newNode;
            addToHead(newNode);
            ++size;

            if (size > capacity) {
                // 超容量：删除尾部（最久未使用的）
                DLinkedNode* removed = removeTail();
                cache.erase(removed->key);  // 这就是节点要存 key 的原因
                delete removed;             // 释放内存，防止泄漏
                --size;
            }
        }
    }

    // 析构函数：清理所有节点
    ~LRUCache2() {
        DLinkedNode* curr = head;
        while (curr) {
            DLinkedNode* next = curr->next;
            delete curr;
            curr = next;
        }
    }
};

// ============================================================
// 解法3: HashMap + STL std::list — 简洁写法
// 时间: O(1) per operation  空间: O(capacity)
//
// 【思路】
// 和解法2完全相同的策略，但用 std::list 替代手写双向链表。
// 关键 API:
//   list::splice(pos, list, it) — O(1) 把 it 指向的节点移到 pos 前面
//   list::emplace_front / pop_back — O(1) 头部插入 / 尾部删除
//
// 为什么 splice 是 O(1)？因为 std::list 是双向链表，
// splice 只是修改指针，不涉及拷贝或移动数据。
// 且 splice 不会使被移动节点的迭代器失效，这很关键。
//
// 面试时要先问面试官是否允许用 STL。
// 大多数面试官更希望看手写版（解法2），以考察链表操作基本功。
// ============================================================
class LRUCache3 {
private:
    int capacity;
    // list 存 {key, value}，front() = 最近使用，back() = 最久未使用
    list<pair<int, int>> lruList;
    // map 存 key → list 迭代器（相当于解法2中的 key → 节点指针）
    unordered_map<int, list<pair<int, int>>::iterator> cache;

public:
    LRUCache3(int capacity) : capacity(capacity) {}

    int get(int key) {
        auto it = cache.find(key);
        if (it == cache.end()) return -1;

        // splice: 把节点移到 list 头部，O(1)，迭代器不失效
        lruList.splice(lruList.begin(), lruList, it->second);
        return it->second->second;  // pair 的 second 是 value
    }

    void put(int key, int value) {
        auto it = cache.find(key);
        if (it != cache.end()) {
            // key 已存在：更新 value，移到头部
            it->second->second = value;
            lruList.splice(lruList.begin(), lruList, it->second);
        } else {
            // key 不存在
            if ((int)cache.size() == capacity) {
                // 驱逐尾部
                int lruKey = lruList.back().first;  // 拿到将被删除的 key
                lruList.pop_back();
                cache.erase(lruKey);
            }
            lruList.emplace_front(key, value);
            cache[key] = lruList.begin();  // 新节点在头部，begin() 就是它
        }
    }
};

// ============================================================
// 【解法对比】
//
// | 解法        | 时间     | 代码量 | 面试推荐度 |
// |------------|----------|--------|-----------|
// | 暴力       | O(n)/op  | 少     | 理解即可   |
// | 手写双向链表 | O(1)/op | ~60行  | ⭐ 必写    |
// | STL list   | O(1)/op  | ~30行  | 问面试官   |
//
// 解法2 vs 解法3:
// - 解法2 展示你对链表指针操作的掌握，面试更加分
// - 解法3 更简洁，适合工程中快速实现
// - 底层原理完全一致，只是封装程度不同
//
// ============================================================
// 【易错点】
//
// 1. 节点不存 key:
//    ✗ struct Node { int value; ... };
//    → removeTail 后拿不到 key，无法从 HashMap 中 erase
//    ✓ struct Node { int key, value; ... };
//
// 2. addToHead 指针修改顺序错:
//    ✗ head->next = node;              // 这行先执行
//      head->next->prev = node;         // 此时 head->next 已经是 node 了！
//    ✓ node->next = head->next;         // 先设置 node 自己的指针
//      head->next->prev = node;         // 再改周围节点
//      head->next = node;               // 最后改 head
//
// 3. removeTail 删了哨兵:
//    ✗ removeNode(tail);    // tail 是哨兵，不是数据节点
//    ✓ removeNode(tail->prev);
//
// 4. put 已存在 key 时漏了 moveToHead:
//    ✗ node->value = value; return;     // 顺序没更新
//    ✓ node->value = value; moveToHead(node);
//
// 5. 内存泄漏:
//    ✗ removeTail 后只 erase map，不 delete 节点
//    ✓ DLinkedNode* removed = removeTail();
//      cache.erase(removed->key);
//      delete removed;                  // 释放内存
//
// 6. STL 版本中 splice 参数写反:
//    ✗ lruList.splice(it->second, lruList, lruList.begin());  // 把 begin 移到 it 前
//    ✓ lruList.splice(lruList.begin(), lruList, it->second);  // 把 it 移到 begin 前
//
// ============================================================
// 【面试追问】
//
// Q1(基础理解): 为什么用双向链表而不是单向链表？
//   → 删除已知节点时需要修改前驱的 next 指针。单链表找前驱要 O(n) 遍历，
//     双向链表通过 node->prev 直接 O(1) 拿到。
//
// Q2(设计扩展): 如果要支持 TTL（过期时间）怎么改？
//   → 节点增加 expireTime 字段。get 时检查是否过期，过期就删除返回 -1。
//     主动清理可以用最小堆按过期时间排序，或用懒惰删除（访问时才检查）。
//
// Q3(并发): 如果要线程安全怎么做？
//   → 最简单: 全局互斥锁。优化: 分段锁（按 key 哈希分桶）或读写锁
//     (shared_mutex，允许多个 get 并发读)。高级: lock-free 方案。
//
// Q4(变体): LRU 有什么缺陷？有什么改进方案？
//   → 缓存污染问题（偶发大量访问挤掉热点数据）。
//     改进: LRU-K、2Q、ARC（自适应替换缓存）。
//
// ============================================================
// 【相关题型】
//
// - 460. LFU 缓存 — 驱逐"最不频繁使用"的。复用 HashMap+双向链表框架，
//   区别是需要按频率分组（每个频率一条链表），并维护最小频率变量。
//   代码多了一个 freq_map<int, list> 和 minFreq 变量。
//
// - 355. 设计推特 — 同样用 HashMap+链表组合。每个用户一条推文链表，
//   getNewsFeed 时合并多条链表取 top 10（复用"合并K个有序链表"的技巧）。
//
// - 206. 反转链表 / 21. 合并有序链表 — 如果双向链表指针操作不熟，
//   先练这些基础题建立指针操作的直觉。
// ============================================================
