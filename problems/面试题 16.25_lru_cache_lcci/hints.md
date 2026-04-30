# 面试题 16.25. LRU 缓存 (LRU Cache LCCI)

## 难度: Medium
## 标签: Design, Hash Table, Linked List, Doubly-Linked List

---

## 核心思路

设计一个 LRU (Least Recently Used) 缓存，需要同时支持 O(1) 时间复杂度的 **查找**（get）和 **插入/删除**（put）操作。

**关键洞察**：
- **快速查找** -> 哈希表 (unordered_map)：key -> 节点指针，O(1) 查找
- **维护使用顺序 + 快速增删** -> 双向链表：O(1) 时间在任意位置插入/删除节点
- **组合** = 哈希表 + 双向链表，这就是经典的 LRU 数据结构

核心规则：
1. `get(key)`: 如果 key 存在，将对应节点移到链表头部（表示最近使用），返回值；否则返回 -1
2. `put(key, value)`: 如果 key 已存在，更新值并移到头部；如果不存在，插入头部，若超容量则删除尾部节点（最久未使用）

---

## 思维链（6步推导）

### Step 1: 分析操作需求
- get: 需要 O(1) 查找 -> 哈希表
- put: 需要 O(1) 插入 -> 链表
- 淘汰: 需要快速找到最久未使用的 -> 链表尾部
- 更新顺序: 需要 O(1) 移动节点到头部 -> 双向链表

### Step 2: 为什么用双向链表而不是单向？
- 删除节点时，需要修改前驱节点的 next 指针
- 单向链表删除需要遍历找前驱 -> O(n)
- 双向链表直接通过 prev 指针找前驱 -> O(1)

### Step 3: 哨兵节点的妙用
- 使用 dummy head 和 dummy tail 作为哨兵
- 消除头部/尾部插入删除时的空指针判断
- 大幅减少 if-else 分支，降低 bug 概率

### Step 4: 链表操作抽象
```
链表结构: dummy_head <-> node1 <-> node2 <-> ... <-> nodeN <-> dummy_tail
                         ^最近使用                     ^最久未使用

操作封装:
  moveToHead(node): 将已有节点移到头部 = removeNode(node) + addToHead(node)
  removeTail(): 删除尾部节点(淘汰) = 取 dummy_tail.prev, removeNode(it)
  addToHead(node): 在 dummy_head 后插入
  removeNode(node): 从链表中摘除
```

### Step 5: get 和 put 的完整流程
```
get(key):
  if key in map:
    node = map[key]
    moveToHead(node)    // 标记为最近使用
    return node.value
  else:
    return -1

put(key, value):
  if key in map:
    node = map[key]
    node.value = value  // 更新值
    moveToHead(node)    // 标记为最近使用
  else:
    newNode = new Node(key, value)
    map[key] = newNode
    addToHead(newNode)
    if map.size > capacity:
      tail = removeTail()
      map.erase(tail.key)   // 注意: 节点需要存 key，才能从 map 中删除
      delete tail
```

### Step 6: 注意事项
- **节点必须存储 key**：淘汰尾部节点时，需要从 map 中删除对应的 key
- **put 时 key 已存在要更新值**：别忘了这个 case
- **先删再加 vs moveToHead**：拆成基本操作组合使用

---

## 数据结构图示（面试画图加分）

```
容量=2 的 LRU Cache 操作过程:

put(1,1): head <-> [1:1] <-> tail           map: {1->node1}
put(2,2): head <-> [2:2] <-> [1:1] <-> tail  map: {1->node1, 2->node2}
get(1):   head <-> [1:1] <-> [2:2] <-> tail  map: {1->node1, 2->node2}  (1移到头)
put(3,3): head <-> [3:3] <-> [1:1] <-> tail  map: {1->node1, 3->node3}  (淘汰2)
get(2):   return -1 (已被淘汰)
put(4,4): head <-> [4:4] <-> [3:3] <-> tail  map: {3->node3, 4->node4}  (淘汰1)
get(1):   return -1 (已被淘汰)
get(3):   return 3, head <-> [3:3] <-> [4:4] <-> tail
get(4):   return 4, head <-> [4:4] <-> [3:3] <-> tail
```

---

## 解法概览

| 解法 | 时间 | 空间 | 特点 | 推荐度 |
|------|------|------|------|--------|
| Solution1: 手写双向链表 + 哈希表 | O(1) get/put | O(capacity) | **面试标准答案** | ⭐⭐⭐⭐⭐ |
| Solution2: STL list + 哈希表 | O(1) get/put | O(capacity) | 代码简洁 | ⭐⭐⭐⭐ |
| Solution3: STL list (splice优化) | O(1) get/put | O(capacity) | 最简洁 | ⭐⭐⭐⭐ |

---

## 关键提示

1. **节点必须存 key**：淘汰尾部节点时需要用 key 从哈希表中删除
2. **哨兵节点消除边界判断**：dummy_head 和 dummy_tail 避免空指针判断
3. **put 时 key 已存在的处理**：别忘了更新值并移到头部
4. **内存管理**：如果用 new 分配节点，淘汰时要 delete，否则内存泄漏
5. **STL list 的 splice 操作**：可以 O(1) 将节点从一个位置移到另一个位置

---

## 解法详解

### Solution1: 手写双向链表 + 哈希表（面试标准答案 ⭐）

**思考过程**：
- 自定义双向链表节点，包含 key, value, prev, next
- 哈希表: key -> DLinkedNode*
- 封装四个基本操作: addToHead, removeNode, moveToHead, removeTail

```cpp
class LRUCache {
    struct DLinkedNode {
        int key, value;
        DLinkedNode* prev;
        DLinkedNode* next;
        DLinkedNode() : key(0), value(0), prev(nullptr), next(nullptr) {}
        DLinkedNode(int k, int v) : key(k), value(v), prev(nullptr), next(nullptr) {}
    };

    unordered_map<int, DLinkedNode*> cache;
    DLinkedNode* head;   // 哨兵头
    DLinkedNode* tail;   // 哨兵尾
    int size;
    int capacity;

    // 在头部后面插入节点
    void addToHead(DLinkedNode* node) {
        node->prev = head;
        node->next = head->next;
        head->next->prev = node;
        head->next = node;
    }

    // 摘除节点(不释放内存)
    void removeNode(DLinkedNode* node) {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    // 移到头部 = 摘除 + 插入头部
    void moveToHead(DLinkedNode* node) {
        removeNode(node);
        addToHead(node);
    }

    // 删除尾部节点(LRU淘汰)
    DLinkedNode* removeTail() {
        DLinkedNode* node = tail->prev;
        removeNode(node);
        return node;
    }

public:
    LRUCache(int cap) : capacity(cap), size(0) {
        head = new DLinkedNode();
        tail = new DLinkedNode();
        head->next = tail;
        tail->prev = head;
    }

    int get(int key) {
        if (cache.find(key) == cache.end()) return -1;
        DLinkedNode* node = cache[key];
        moveToHead(node);       // 标记最近使用
        return node->value;
    }

    void put(int key, int value) {
        if (cache.find(key) != cache.end()) {
            DLinkedNode* node = cache[key];
            node->value = value;    // 更新值!
            moveToHead(node);
        } else {
            DLinkedNode* newNode = new DLinkedNode(key, value);
            cache[key] = newNode;
            addToHead(newNode);
            size++;
            if (size > capacity) {
                DLinkedNode* removed = removeTail();
                cache.erase(removed->key);  // 用节点存的key来删除
                delete removed;
                size--;
            }
        }
    }
};
```

**关键点**：
- 哨兵节点使所有操作统一化，无空链表判断
- removeNode 不负责 delete，由调用方决定是否释放
- removeTail 返回节点指针，便于从 map 中删除和释放内存

---

### Solution2: STL list + 哈希表

**思考过程**：
- 利用 C++ STL list（内部就是双向链表），存储 pair<int,int>
- 哈希表: key -> list::iterator
- list 的 push_front, erase, splice 都是 O(1)

```cpp
class LRUCache {
    int cap;
    list<pair<int,int>> lru;  // front = 最近使用, back = 最久未使用
    unordered_map<int, list<pair<int,int>>::iterator> cache;

public:
    LRUCache(int capacity) : cap(capacity) {}

    int get(int key) {
        auto it = cache.find(key);
        if (it == cache.end()) return -1;
        // splice: O(1)移动节点到头部，迭代器不失效
        lru.splice(lru.begin(), lru, it->second);
        return it->second->second;
    }

    void put(int key, int value) {
        auto it = cache.find(key);
        if (it != cache.end()) {
            it->second->second = value;    // 更新值
            lru.splice(lru.begin(), lru, it->second);
        } else {
            if ((int)cache.size() >= cap) {
                int oldKey = lru.back().first;  // 取尾部key
                lru.pop_back();
                cache.erase(oldKey);
            }
            lru.push_front({key, value});
            cache[key] = lru.begin();
        }
    }
};
```

**关键点**：
- `splice` 是 STL list 的杀手锏，O(1) 移动节点，不涉及内存分配/释放
- list 的 iterator 在 splice 后依然有效

---

### Solution3: STL list + splice 精简版

**思考过程**：与 Solution2 相同，更紧凑的写法。

```cpp
class LRUCache {
    int cap;
    list<pair<int,int>> dll;
    unordered_map<int, list<pair<int,int>>::iterator> mp;

public:
    LRUCache(int capacity) : cap(capacity) {}

    int get(int key) {
        if (!mp.count(key)) return -1;
        dll.splice(dll.begin(), dll, mp[key]);
        return mp[key]->second;
    }

    void put(int key, int value) {
        if (mp.count(key)) {
            mp[key]->second = value;
            dll.splice(dll.begin(), dll, mp[key]);
            return;
        }
        if ((int)mp.size() >= cap) {
            mp.erase(dll.back().first);
            dll.pop_back();
        }
        dll.push_front({key, value});
        mp[key] = dll.begin();
    }
};
```

---

## 解法对比

| 维度 | Solution1 手写链表 | Solution2/3 STL list |
|------|-------------------|---------------------|
| 时间复杂度 | O(1) | O(1) |
| 空间复杂度 | O(capacity) | O(capacity) |
| 代码量 | 较多（~60行） | 较少（~30行） |
| 面试表现 | 展示底层理解 | 展示 STL 熟练度 |
| 内存管理 | 需要手动 delete | STL 自动管理 |
| 面试推荐 | **首选（展示功底）** | 追问时展示 |

**面试策略**：先用 Solution1 手写链表展示底层理解，写完后提及"也可以用 STL list + splice 简化代码"。

---

## 易错点（具体 Bug 示例）

### Bug 1: 节点没存 key，淘汰时无法从 map 中删除
```cpp
struct Node { int value; Node* prev; Node* next; }; // 缺少 key!
// 淘汰时: tail 节点不知道自己的 key，无法 cache.erase(???)
// 修复: 节点中必须存储 key
```

### Bug 2: put 时 key 已存在但忘记更新值
```cpp
void put(int key, int value) {
    if (cache.count(key)) {
        moveToHead(cache[key]);
        // 缺少: cache[key]->value = value;
        // 导致 put(1,10) 后 get(1) 仍返回旧值 1 而不是 10
    }
}
```

### Bug 3: 删除尾部节点后忘记从 map 中删除
```cpp
if (size > capacity) {
    DLinkedNode* removed = removeTail();
    delete removed;  // 从链表中删了，但 map 里还有这个 key!
    // 缺少: cache.erase(removed->key);
    // 下次 get 这个 key 会访问已释放的内存 -> 悬挂指针!
}
```

### Bug 4: 哨兵节点初始化时没有互相连接
```cpp
head = new DLinkedNode();
tail = new DLinkedNode();
// 缺少: head->next = tail; tail->prev = head;
// 第一次 addToHead 时 head->next 是 nullptr -> 崩溃
```

### Bug 5: addToHead 指针赋值顺序错误
```cpp
void addToHead(DLinkedNode* node) {
    head->next = node;         // 错!先改了 head->next
    node->prev = head;
    node->next = head->next;   // 此时 head->next 已经是 node 自己了!
    head->next->prev = node;
}
// 正确顺序: 先设置新节点的指针，再修改周围节点的指针
```

---

## 面试追问递进链

### Q1: "为什么用双向链表而不是单向链表？"
**答**：删除一个已知节点时，需要修改前驱的 next。单链表找前驱要 O(n)，双链表通过 prev 指针 O(1) 搞定。

### Q2: "如何让 LRU Cache 线程安全？"
**答**：
- 简单方案: 整体加互斥锁 (mutex)
- 进阶: 分段锁 (ConcurrentHashMap 思路)
- 读写锁: 读多写少场景用 shared_mutex
- 无锁: 使用 CAS 操作（极其复杂）

### Q3: "LRU vs LFU 有什么区别？如何实现 LFU？"
**答**：
- LRU 按访问时间淘汰（最久未使用）
- LFU 按访问频率淘汰（最少使用）
- LFU 需要: 频率计数 + 每个频率一个双向链表 + min_freq 追踪
- 参考 LeetCode 460. LFU Cache

### Q4: "实际系统中 Redis 的 LRU 是如何实现的？"
**答**：Redis 使用近似 LRU —— 随机采样 N 个 key，淘汰其中最久未使用的。这样避免了维护全局链表的开销，以近似换取性能。

---

## 相关题型

| 题目 | 关系 | 复用说明 |
|------|------|----------|
| [146. LRU Cache](../0146_lru_cache/) | 完全相同 | 本题是 LCCI 版，题意完全一样 |
| [460. LFU Cache](../0460_lfu_cache/) | 频率淘汰 | 双哈希表+频率链表，更复杂 |
| [432. All O'one Data Structure](../0432_all_oone_data_structure/) | 全 O(1) | 类似的链表+哈希表设计 |
| [355. Design Twitter](../0355_design_twitter/) | 设计题 | 链表合并+哈希表 |
| [706. Design HashMap](../0706_design_hashmap/) | 基础设计 | 哈希表本身的实现 |
