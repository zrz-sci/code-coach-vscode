# 432. 全 O(1) 的数据结构

## 核心思路

本题本质上要求设计一个数据结构，支持**增减字符串计数**并能**O(1) 获取最大/最小计数对应的字符串**。核心挑战是：如何同时维护"键→计数"的映射和"按计数排序"的结构，且所有操作都是 O(1)。

## 思维链

1. **读完题第一反应**：用一个 `HashMap<string, int>` 存每个 key 的计数，inc/dec 都是 O(1)。但 getMax/getMin 需要遍历所有 key 找极值 → O(n)，不满足要求。

2. **瓶颈在哪？** getMax/getMin 要 O(1)，意味着最大值和最小值必须随时可以直接拿到。能不能用 `std::map`（有序红黑树）维护？map 的 begin/rbegin 是 O(1)，但 inc/dec 时需要删除旧计数、插入新计数，map 的插入删除是 O(log n)，不是 O(1)。

3. **怎么突破？** 需要一个**有序结构**，但插入删除是 O(1)。链表！双向链表的插入删除是 O(1)（只要你持有目标节点的指针）。如果链表节点按计数排序，头尾就是最小/最大。

4. **具体方案：** 每个链表节点代表一个**计数值**（而不是一个 key），节点内用一个 set 存所有具有该计数的 key。链表按计数从小到大排序。再用 HashMap 存 `key → 所在链表节点`。
   - inc(key)：找到 key 当前节点（计数为 c），把 key 移到计数 c+1 的节点（如果不存在就新建，插在当前节点后面）
   - dec(key)：类似，移到 c-1 的节点
   - getMax：链表尾节点的任意一个 key
   - getMin：链表头节点的任意一个 key

5. **为什么是 O(1)？** HashMap 查找 O(1)，链表节点的插入/删除 O(1)，set 中插入/删除一个元素 O(1)（用 unordered_set），取头尾节点 O(1)。关键洞察：inc/dec 只让计数变化 1，所以目标节点要么是当前节点的**相邻节点**，要么需要**新建一个节点插在相邻位置**——永远不需要搜索。

6. **哨兵节点简化边界**：链表头尾加虚拟哨兵节点（计数为 0 和 INT_MAX），避免处理空链表等边界情况。

## 解法概览

| 解法 | 思路 | 时间（所有操作） | 空间 | 面试 |
|------|------|------|------|------|
| HashMap + 遍历 | HashMap 存计数，getMax/getMin 遍历 | inc/dec O(1), get O(n) | O(n) | 能说出即可 |
| HashMap + 有序 Map | 额外用 map<int, set<string>> | inc/dec O(log n), get O(1) | O(n) | 能说出即可 |
| HashMap + 双向链表 | 链表节点按计数排序，HashMap 指向节点 | 全部 O(1) | O(n) | ⭐ 必须写出 |

## 关键提示

1. **不要让每个链表节点存一个 key**——那样 inc/dec 后需要在链表中移动节点到正确位置（O(n) 搜索）。让每个节点存一个**计数值**和该计数下的**所有 key 的集合**。

2. **inc/dec 只改变计数 ±1**，这意味着目标位置一定是当前节点的邻居，不需要搜索——这是 O(1) 的关键保证。

3. **节点为空时要删除**：当一个计数节点的 key 集合变空，需要从链表中移除该节点，否则 getMin/getMax 会返回已经不存在的计数。

4. **哨兵节点**大幅简化代码：头哨兵（count=0）和尾哨兵（count=INT_MAX），这样链表永远非空，不用特判。

5. **数据结构关系示意图**：
```
HashMap: key → Node*
  "hello" → [Node: count=2]
  "leet"  → [Node: count=1]

DoubleLinkedList (按count升序):
  [Sentinel:0] ↔ [Node:1 {"leet"}] ↔ [Node:2 {"hello"}] ↔ [Sentinel:INF]
       head                                                      tail
  getMinKey → head->next 的任意 key = "leet"
  getMaxKey → tail->prev 的任意 key = "hello"
```

## 解法详解

### 解法1: HashMap + 遍历 — O(n) / O(n)

**思考过程**：最直接的想法——用 map 存计数，需要最大最小时遍历。

```cpp
class AllOne {
    unordered_map<string, int> count;
public:
    AllOne() {}
    
    void inc(string key) {
        count[key]++;
    }
    
    void dec(string key) {
        if (--count[key] == 0) count.erase(key);
    }
    
    string getMaxKey() {
        string res = "";
        int maxVal = 0;
        for (auto& [k, v] : count) {
            if (v > maxVal) { maxVal = v; res = k; }
        }
        return res;
    }
    
    string getMinKey() {
        string res = "";
        int minVal = INT_MAX;
        for (auto& [k, v] : count) {
            if (v < minVal) { minVal = v; res = k; }
        }
        return res;
    }
};
```

**关键点**：getMaxKey/getMinKey 是 O(n)，不满足题目 O(1) 要求，但作为面试第一步展示理解是足够的。

---

### 解法2: HashMap + 双向链表 — O(1) / O(n) ⭐ 面试首选

**从解法1优化**：解法1 的瓶颈是 getMax/getMin 需要遍历找极值。我们需要一个**时刻保持有序**的结构，且增删都是 O(1)。

**核心设计**：
- 双向链表，每个节点存一个 `count` 值和该计数下的所有 key（用 `unordered_set`）
- 链表按 count **从小到大**排序
- HashMap 存 `key → 该 key 所在的链表节点指针`
- 头哨兵 count=0，尾哨兵 count=INT_MAX

**inc(key) 操作步骤图**：
```
inc("hello"), 当前 hello 在 count=2 的节点

Before:
  [S:0] ↔ [1:{"a"}] ↔ [2:{"hello","b"}] ↔ [S:INF]
                         ↑ key在此节点

Step1: 检查 next 节点 count 是否 == 3
       next 是 [S:INF] (count=INF) ≠ 3
       → 在当前节点和 next 之间新建 [3:{}]

  [S:0] ↔ [1:{"a"}] ↔ [2:{"hello","b"}] ↔ [3:{}] ↔ [S:INF]

Step2: 把 "hello" 从 [2] 的集合移到 [3] 的集合

  [S:0] ↔ [1:{"a"}] ↔ [2:{"b"}] ↔ [3:{"hello"}] ↔ [S:INF]

Step3: 更新 HashMap: "hello" → [3]节点
Step4: [2] 的集合非空，不删除

如果 [2] 的集合变空了，则从链表中删除 [2] 节点
```

```cpp
class AllOne {
    // 链表节点：存计数值和该计数下所有key
    struct Node {
        int count;
        unordered_set<string> keys;
        Node* prev;
        Node* next;
        Node(int c) : count(c), prev(nullptr), next(nullptr) {}
    };
    
    Node* head; // 哨兵头 count=0
    Node* tail; // 哨兵尾 count=INT_MAX
    unordered_map<string, Node*> keyNode; // key → 所在节点
    
    // 在 prev 和 next 之间插入新节点
    Node* insertAfter(Node* prevNode, int count) {
        Node* newNode = new Node(count);
        newNode->prev = prevNode;
        newNode->next = prevNode->next;
        prevNode->next->prev = newNode;
        prevNode->next = newNode;
        return newNode;
    }
    
    // 删除节点（当其 keys 为空时）
    void removeNode(Node* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
        delete node;
    }
    
public:
    AllOne() {
        head = new Node(0);
        tail = new Node(INT_MAX);
        head->next = tail;
        tail->prev = head;
    }
    
    void inc(string key) {
        if (keyNode.find(key) == keyNode.end()) {
            // key 不存在，计数从 0→1
            // 目标节点应该在 head 之后，count=1
            if (head->next->count != 1) {
                insertAfter(head, 1);
            }
            head->next->keys.insert(key);
            keyNode[key] = head->next;
        } else {
            // key 已存在，从 count → count+1
            Node* cur = keyNode[key];
            int newCount = cur->count + 1;
            // 目标是 cur 的下一个节点（如果 count 匹配的话）
            if (cur->next->count != newCount) {
                insertAfter(cur, newCount);
            }
            cur->next->keys.insert(key);
            keyNode[key] = cur->next;
            // 从旧节点删除
            cur->keys.erase(key);
            if (cur->keys.empty()) removeNode(cur);
        }
    }
    
    void dec(string key) {
        Node* cur = keyNode[key];
        int newCount = cur->count - 1;
        if (newCount == 0) {
            // 计数降为0，彻底删除
            keyNode.erase(key);
        } else {
            // 移到 count-1 的节点（应在 cur 的前一个位置）
            if (cur->prev->count != newCount) {
                insertAfter(cur->prev, newCount);
            }
            cur->prev->keys.insert(key);
            keyNode[key] = cur->prev;
        }
        cur->keys.erase(key);
        if (cur->keys.empty()) removeNode(cur);
    }
    
    string getMaxKey() {
        if (tail->prev == head) return "";
        return *(tail->prev->keys.begin());
    }
    
    string getMinKey() {
        if (head->next == tail) return "";
        return *(head->next->keys.begin());
    }
};
```

**关键点**：
- inc 时目标节点在当前节点的 **next** 方向（计数增大）
- dec 时目标节点在当前节点的 **prev** 方向（计数减小）
- 新 key（首次 inc）从 head 之后找 count=1 的节点

## 解法对比

| | 解法1: HashMap+遍历 | 解法2: HashMap+双向链表 |
|---|---|---|
| inc/dec | O(1) | O(1) |
| getMax/getMin | O(n) 遍历 | O(1) 取头尾 |
| 代码复杂度 | 极简 | 较复杂 |
| 面试场景 | 作为起点，展示理解 | 必须最终写出 |

核心区别：解法2 用**按计数排序的双向链表**替代了遍历查找极值的过程。由于计数只会 ±1 变化，目标位置一定是相邻节点，保证了 O(1)。

## 易错点

1. **inc 时新 key 的处理忘记特判**：
   - ✗ 直接去 `keyNode[key]` 找当前节点 → key 不存在时空指针
   - ✓ 先判断 key 是否在 keyNode 中，不存在时从 count=0 开始（即从 head 出发）

2. **insertAfter 方向搞反**：
   - ✗ inc 时在 cur->prev 后面插新节点（count 变大应该往后走）
   - ✓ inc 在 cur 后面插，dec 在 cur->prev 后面插

3. **忘记删除空节点**：
   - ✗ 把 key 从旧节点的 set 中移除后，没检查 set 是否为空
   - ✓ `if (cur->keys.empty()) removeNode(cur);` 必须有

4. **getMax/getMin 没判空**：
   - ✗ 直接返回 `tail->prev->keys.begin()` → 如果链表只有哨兵，解引用非法
   - ✓ 先检查 `tail->prev == head`（或 `head->next == tail`）

5. **内存泄漏**：removeNode 时忘记 `delete node`。面试时可以不管，但生产代码中必须处理。

## 面试追问

**Q1：为什么每个链表节点存一组 key 而不是每个 key 一个节点？**
→ 如果每个 key 一个节点，inc/dec 后需要把节点移动到正确位置，链表中查找正确位置是 O(n)。按计数分组后，由于计数只变化 ±1，目标位置一定是相邻节点。

**Q2：能不能用 `std::list` 代替手写双向链表？**
→ 可以，`std::list` 的 `insert`/`erase` 都是 O(1)（给定 iterator）。但需要配合 `unordered_map<string, list::iterator>` 来快速定位。代码更简洁但需要注意 iterator 失效问题。

**Q3：如果还要支持 `getMedianKey()`（返回计数中位数对应的 key），怎么改？**
→ 在链表上维护一个"中间指针"，类似"数据流中位数"（LeetCode 295）的思路。每次 inc/dec 后根据总 key 数和当前中间指针的位置，左移或右移一步。仍然是 O(1)。

**Q4：这个设计的并发安全问题？**
→ HashMap 和链表都不是线程安全的。需要加锁（粗粒度：全局锁；细粒度：读写锁分离 HashMap 和链表）。或使用分段锁/ConcurrentHashMap 类似思路。

## 相关题型

- **146. LRU 缓存** — 复用同样的 **HashMap + 双向链表** 框架。区别：LRU 的链表节点是单个 key，按访问时间排序；本题节点是一组 key，按计数排序。
- **460. LFU 缓存** — 本题的近亲。LFU 也用"按频率分桶的双向链表"，区别：LFU 还要在同一频率内按访问时间排序（桶内再用一个链表）。**学会 432 后做 460 可以直接复用桶链表结构，只需加上桶内的时间排序逻辑。**
- **295. 数据流的中位数** — 如果面试追问 getMedianKey，需要这道题的双堆/双指针思路。
- **380. O(1) 时间插入、删除和获取随机元素** — 另一道经典 O(1) 设计题，用 HashMap + 数组（swap-to-end 删除技巧）。