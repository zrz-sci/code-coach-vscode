# 146. LRU 缓存

## 核心思路

本质上在问：**如何设计一个数据结构，同时支持 O(1) 的查找、O(1) 的插入/更新、O(1) 的删除最久未使用元素？** 单一数据结构无法同时满足这三个需求，因此需要**组合两种数据结构**——哈希表负责 O(1) 查找，双向链表负责 O(1) 维护使用顺序。

## 思维链

1. **读完题第一反应**：需要一个 key-value 存储，同时跟踪"最近使用"的顺序。最朴素的做法——用数组/vector 存 `(key, value, timestamp)`，get 时线性查找，put 时线性查找 + 驱逐时找最小 timestamp。
2. **暴力解的瓶颈在哪？** get 需要 O(n) 遍历找 key，驱逐需要 O(n) 找最久未使用的。题目要求 O(1)。
3. **O(1) 查找 → 哈希表**。用 `unordered_map<key, value>` 可以 O(1) 找到 key 对应的 value。但问题是：怎么 O(1) 知道"谁是最久未使用的"？怎么 O(1) 更新某个元素的使用时间？
4. **维护顺序 → 链表**。把所有元素按使用时间排成一条链，最近使用的放头部，最久未使用的在尾部。每次 get/put 时把对应节点**移到头部**。满了就**删尾部**。
5. **O(1) 移动节点 → 双向链表**。单链表删除一个节点需要找前驱 O(n)，双向链表可以 O(1) 删除任意已知节点。
6. **组合**：`unordered_map<key, ListNode*>` — 哈希表存 key → 链表节点指针，链表维护使用顺序。这样查找 O(1)（哈希表），移动/删除 O(1)（双向链表），完美！

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力（数组+时间戳） | 数组存 KV + timestamp，get/put 线性扫描 | O(n) | O(n) | 能说出即可 |
| 哈希表 + 双向链表（手写） | HashMap 定位节点，双向链表维护顺序 | O(1) | O(n) | ⭐ 必须写出 |
| 利用 STL list（简化版） | 用 `std::list` 替代手写双向链表 | O(1) | O(n) | 加分项 |

## 关键提示

1. **为什么是双向链表而不是单向链表？** 想想删除一个节点需要什么——需要修改前驱的 next 指针。单链表拿不到前驱（除非再遍历一次），双向链表直接 `node->prev` 拿到。

2. **Dummy Head + Dummy Tail 技巧**：在链表头尾各加一个哨兵节点，这样"在头部插入"和"删除尾部"永远不需要判空，代码大幅简化。
   ```
   // 带哨兵的双向链表结构：
   // dummy_head <-> node1 <-> node2 <-> ... <-> nodeN <-> dummy_tail
   //   ↑ 最近使用                                  ↑ 最久未使用
   ```

3. **"移到头部" = 删除 + 插到头部**：不要试图写一个"移动"函数，拆成两步更清晰：先从原位置摘掉（修改前驱后继的指针），再插到 dummy_head 后面。

4. **put 操作有两种情况**：key 已存在（更新 value + 移到头部）vs key 不存在（新建节点 + 插到头部 + 检查容量是否超限）。

5. **面试中画这张图再写代码**：
   ```
   HashMap: { key1: Node*, key2: Node*, ... }
                      |           |
                      v           v
   DummyHead <-> [k1,v1] <-> [k2,v2] <-> ... <-> DummyTail
   (MRU端)                                        (LRU端)
   ```

## 解法详解

### 解法1: 暴力（数组 + 线性查找） — O(n) / O(n)

**思考过程**：最直觉的做法——用一个数组存所有 key-value 对，再用一个计数器模拟时间戳。get 时遍历数组找 key，put 时遍历找 key 更新或插入，满了就遍历找时间戳最小的删除。

```cpp
class LRUCache {
public:
    int cap;
    int timestamp;
    // 每个元素: {key, value, last_used_time}
    vector<tuple<int, int, int>> cache;

    LRUCache(int capacity) : cap(capacity), timestamp(0) {}

    int get(int key) {
        for (auto& [k, v, t] : cache) {
            if (k == key) {
                t = ++timestamp;  // 更新使用时间
                return v;
            }
        }
        return -1;
    }

    void put(int key, int value) {
        // 先看 key 是否已存在
        for (auto& [k, v, t] : cache) {
            if (k == key) {
                v = value;
                t = ++timestamp;
                return;
            }
        }
        // 不存在，检查容量
        if ((int)cache.size() == cap) {
            // 找最久未使用的（时间戳最小）
            int minIdx = 0;
            for (int i = 1; i < (int)cache.size(); i++) {
                if (get<2>(cache[i]) < get<2>(cache[minIdx]))
                    minIdx = i;
            }
            cache.erase(cache.begin() + minIdx);
        }
        cache.push_back({key, value, ++timestamp});
    }
};
```

**关键点**：这个解法帮助理解 LRU 的语义，但每个操作都是 O(n)，面试中说出思路即可，不需要写完整代码。

---

### 解法2: 哈希表 + 手写双向链表 — O(1) / O(n) ⭐ 面试首选

**从解法1优化**：暴力解有两个 O(n) 瓶颈——查找 key 和找最久未使用的。哈希表解决查找 O(1)，双向链表解决排序维护 O(1)。

**数据结构设计**：
```
// 整体结构示意图:
//
//  unordered_map<int, DLinkedNode*>
//  ┌─────────────────────────────┐
//  │ key=1 → Node*(k=1,v=1)     │
//  │ key=3 → Node*(k=3,v=3)     │
//  │ key=4 → Node*(k=4,v=4)     │
//  └─────────────────────────────┘
//                 │ 指向链表中的节点
//                 ▼
//  DummyHead ←→ [4,4] ←→ [3,3] ←→ [1,1] ←→ DummyTail
//  (最近使用)                            (最久未使用)
//
//  get(3): HashMap O(1)找到 Node(3,3) → 摘出 → 插到Head后 → 返回3
//  put(5,5) 且满了: 删 DummyTail.prev (即key=1) → HashMap删key=1
//                    新建Node(5,5) → 插Head后 → HashMap加key=5
```

**核心操作图解**：
```
// "移到头部" = removeNode + addToHead
//
// removeNode(node):        在任意位置摘掉一个节点
//   before:  A ←→ node ←→ B
//   after:   A ←→ B          (node 被摘出)
//   操作: node->prev->next = node->next
//         node->next->prev = node->prev
//
// addToHead(node):         插入到 DummyHead 之后
//   before:  DummyHead ←→ first ←→ ...
//   after:   DummyHead ←→ node ←→ first ←→ ...
//   操作: node->prev = head
//         node->next = head->next
//         head->next->prev = node
//         head->next = node
```

```cpp
struct DLinkedNode {
    int key, value;
    DLinkedNode* prev;
    DLinkedNode* next;
    DLinkedNode() : key(0), value(0), prev(nullptr), next(nullptr) {}
    DLinkedNode(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

class LRUCache {
private:
    unordered_map<int, DLinkedNode*> cache;
    DLinkedNode* head;  // 哨兵头
    DLinkedNode* tail;  // 哨兵尾
    int capacity;
    int size;

    void addToHead(DLinkedNode* node) {
        // 把 node 插入到 head 和 head->next 之间
        node->prev = head;
        node->next = head->next;
        head->next->prev = node;
        head->next = node;
    }

    void removeNode(DLinkedNode* node) {
        // 把 node 从链表中摘掉
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    void moveToHead(DLinkedNode* node) {
        // "最近使用" = 摘掉 + 放到头部
        removeNode(node);
        addToHead(node);
    }

    DLinkedNode* removeTail() {
        // 删除尾部节点（最久未使用），返回它以便从 HashMap 中删除
        DLinkedNode* node = tail->prev;  // 不是 tail 本身，tail 是哨兵
        removeNode(node);
        return node;
    }

public:
    LRUCache(int capacity) : capacity(capacity), size(0) {
        head = new DLinkedNode();
        tail = new DLinkedNode();
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if (cache.find(key) == cache.end()) return -1;
        DLinkedNode* node = cache[key];
        moveToHead(node);  // 标记为"最近使用"
        return node->value;
    }

    void put(int key, int value) {
        if (cache.find(key) != cache.end()) {
            // key 已存在: 更新 value + 移到头部
            DLinkedNode* node = cache[key];
            node->value = value;
            moveToHead(node);
        } else {
            // key 不存在: 新建节点
            DLinkedNode* newNode = new DLinkedNode(key, value);
            cache[key] = newNode;
            addToHead(newNode);
            ++size;
            if (size > capacity) {
                // 超容量: 删除尾部（最久未使用）
                DLinkedNode* removed = removeTail();
                cache.erase(removed->key);  // 这就是为什么节点要存 key
                delete removed;
                --size;
            }
        }
    }
};
```

**关键点**：
- 节点里**必须存 key**！因为删除尾部节点时，需要从 HashMap 中删除对应的 key，而此时你手里只有节点指针，不存 key 就没法从 map 里删。
- 哨兵节点让代码不需要任何空指针判断，是链表题的常见简化技巧。

---

### 解法3: 利用 STL `std::list`（简化版）— O(1) / O(n)

**思路**：C++ 的 `std::list` 本身就是双向链表，`splice` 操作可以 O(1) 把一个节点移到链表头部，省去手写链表的麻烦。

```cpp
class LRUCache {
private:
    int capacity;
    // list 存 {key, value}，front 是最近使用，back 是最久未使用
    list<pair<int, int>> lruList;
    // map 存 key → list 中对应的迭代器
    unordered_map<int, list<pair<int, int>>::iterator> cache;

public:
    LRUCache(int capacity) : capacity(capacity) {}

    int get(int key) {
        auto it = cache.find(key);
        if (it == cache.end()) return -1;
        // splice: 把 it->second 指向的节点移到 lruList 的 begin 位置
        // 这是 O(1) 操作，且不会使迭代器失效
        lruList.splice(lruList.begin(), lruList, it->second);
        return it->second->second;
    }

    void put(int key, int value) {
        auto it = cache.find(key);
        if (it != cache.end()) {
            it->second->second = value;  // 更新 value
            lruList.splice(lruList.begin(), lruList, it->second);
        } else {
            if ((int)cache.size() == capacity) {
                // 删除尾部（最久未使用）
                int lruKey = lruList.back().first;
                lruList.pop_back();
                cache.erase(lruKey);
            }
            lruList.emplace_front(key, value);
            cache[key] = lruList.begin();
        }
    }
};
```

**关键点**：`std::list::splice` 是 O(1) 的节点移动操作，且不会使被移动节点的迭代器失效（迭代器仍指向同一个节点），这是 `list` 比 `vector/deque` 适合本题的核心原因。

---

## 解法对比

| | 解法1 暴力 | 解法2 手写双向链表 | 解法3 STL list |
|---|---|---|---|
| 时间 | O(n) per op | O(1) per op | O(1) per op |
| 空间 | O(n) | O(n) | O(n) |
| 代码量 | 最少 | 最多（~60行） | 中等（~30行） |
| 面试推荐 | 说思路即可 | ⭐ **必须掌握** | 看面试官是否允许 STL |
| 适用场景 | 理解题意 | 展示数据结构功底 | 快速实现，工程场景 |

**面试中的选择**：大多数面试官期望你手写双向链表（解法2），因为这考察的是**对链表操作的熟练度**。如果面试官允许用 STL，解法3更简洁。建议先问面试官。

## 易错点

1. **节点不存 key，导致驱逐时无法从 HashMap 删除**
   - ✗ `struct Node { int value; ... };` — 删尾部时拿不到 key
   - ✓ `struct Node { int key, value; ... };` — 删尾部时用 `node->key` 从 map 中 erase

2. **addToHead 时指针修改顺序错误**
   - ✗ 先改 `head->next = node`，再改 `head->next->prev = node` — 此时 `head->next` 已经变了！
   - ✓ 先设置 `node` 的指针（`node->prev = head; node->next = head->next`），再修改周围节点（`head->next->prev = node; head->next = node`）

3. **removeTail 删的是哨兵而不是最后一个数据节点**
   - ✗ `removeNode(tail)` — tail 是哨兵，不是数据
   - ✓ `removeNode(tail->prev)` — tail->prev 才是最后一个数据节点

4. **put 已存在的 key 时忘记 moveToHead**
   - ✗ 只更新 value，不移到头部 — 使用顺序没更新，后续驱逐会出错
   - ✓ 更新 value **并且** moveToHead

5. **忘记 delete 被删除的节点（内存泄漏）**
   - 在 removeTail 后，拿到返回的指针，erase 完 map 后要 `delete removed`

## 面试追问

**Q1: 为什么用双向链表而不是单向链表？**
→ 删除一个已知节点时，需要修改前驱的 next 指针。单链表找前驱要 O(n) 遍历，双向链表通过 `node->prev` 直接 O(1) 拿到。

**Q2: 如果要支持过期时间（TTL）怎么改？**
→ 节点中增加 `expireTime` 字段。get 时检查是否过期，过期则删除并返回 -1。put 时设置过期时间。如果要主动清理过期 key，可以用一个额外的最小堆按过期时间排序，或者用懒惰删除策略（访问时才检查）。

**Q3: 如果要线程安全怎么做？**
→ 最简单的方法是整个 get/put 加互斥锁（`std::mutex`），但并发性差。优化方案：分段锁（对 key 哈希到不同的桶，每个桶一把锁），或者用读写锁（`shared_mutex`）允许多个 get 并发。更进一步可以用 lock-free 数据结构，但实现复杂。

**Q4: 实际系统中 LRU 有什么问题？怎么改进？**
→ LRU 在"偶发性大量访问"场景下会把热点数据挤出缓存（缓存污染）。改进方案：LRU-K（访问 K 次才进入缓存）、2Q（两个队列区分首次访问和多次访问）、ARC（自适应替换缓存，同时维护 LRU 和 LFU 的信息）。

## 相关题型

- **460. LFU 缓存** — LRU 的升级版，驱逐"最不频繁使用"的。复用同样的 HashMap + 双向链表框架，区别是需要按频率分组（每个频率维护一条链表），并跟踪最小频率。
- **355. 设计推特** — 同样用 HashMap + 链表组合，复用"合并多个有序链表"的技巧。
- **linked list 系列题（206 反转链表、21 合并两个有序链表）** — 如果双向链表指针操作不熟练，先练这些基础题。