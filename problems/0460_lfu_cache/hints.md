# 460. LFU 缓存

## 核心思路

本题本质上是设计一个数据结构，在 O(1) 时间内完成三件事：**通过 key 查找值**、**更新某个 key 的访问频率**、**淘汰频率最低且最久未使用的 key**。这要求我们同时维护"频率"和"时间顺序"两个维度的信息。

## 思维链

1. **读完题第一反应**：每个 key 有一个使用频率计数器，淘汰时找频率最小的，频率相同找最久未用的。最朴素的做法？遍历所有 key 找最小频率的那个，O(n) 淘汰。

2. **暴力解的瓶颈**：`get` 可以用 HashMap 做到 O(1)，但淘汰时需要找"频率最小"的 key，遍历一遍是 O(n)。瓶颈在于**快速定位最小频率对应的 key**。

3. **怎么突破瓶颈？** 回忆 LRU 缓存（第 146 题）：LRU 用 HashMap + 双向链表做到 O(1)。LFU 比 LRU 多了一个"频率"维度。能不能**按频率分组**，每个频率维护一条 LRU 链表？

4. **核心设计**：
   - **HashMap1** `keyMap`: key → {value, freq, 链表迭代器}，O(1) 查找
   - **HashMap2** `freqMap`: freq → 双向链表（该频率下所有 key，按时间排列）
   - **变量** `minFreq`: 记录当前最小频率，O(1) 定位淘汰目标

5. **频率变化时怎么维护？** 当一个 key 被访问，它的频率从 f 变成 f+1：
   - 从 `freqMap[f]` 的链表中移除
   - 插入到 `freqMap[f+1]` 的链表尾部（最近使用的在尾部）
   - 如果 `freqMap[f]` 变空了且 f == minFreq，则 minFreq++

6. **淘汰怎么做？** 直接去 `freqMap[minFreq]` 的链表头部（最久未使用的），删除它。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力 | HashMap + 线性扫描找最小频率 | get O(1) / put O(n) | O(n) | 能说出即可 |
| 双 HashMap + 双向链表 | 频率分桶 + 每桶 LRU | get O(1) / put O(1) | O(n) | ⭐ 必须写出 |
| 双 HashMap + LinkedHashSet | 用语言内置有序集合简化 | get O(1) / put O(1) | O(n) | 加分项（代码更简洁） |

## 关键提示

1. **与 LRU 的关系**：LRU 是"一条链表维护时间顺序"，LFU 是"多条链表按频率分组，每条内部维护时间顺序"。先回忆 146. LRU Cache 的设计。

2. **minFreq 的维护**：新插入的 key 频率一定是 1，所以 `put` 新 key 时 `minFreq = 1`。频率增加时，只有当旧频率等于 `minFreq` 且该频率的链表变空时，`minFreq` 才加 1。

3. **链表方向约定**：链表头部 = 最久未使用（淘汰候选），链表尾部 = 最近使用。每次访问/更新都移到尾部。

4. **put 已存在的 key**：等价于先更新 value，再做一次"频率增加"操作（和 get 的频率更新逻辑完全相同）。

5. **capacity = 0 的边界**：题目说 capacity ≥ 1，但有些测试用例可能给 0，此时所有 put 都不生效。

```
// 数据结构整体架构:
//
// keyMap:  key → (value, freq, iter)    ← O(1) 查找
//
// freqMap: freq=1 → [A] ↔ [B] ↔ [C]   ← A是最久未用(淘汰它)
//          freq=2 → [D] ↔ [E]              C是最近使用
//          freq=3 → [F]
//
// minFreq = 1  ← 淘汰时直接看 freqMap[minFreq] 的头部
//
// 访问 B (freq 1→2):
// freqMap: freq=1 → [A] ↔ [C]          ← B从freq=1链表移除
//          freq=2 → [D] ↔ [E] ↔ [B]   ← B加到freq=2链表尾部
//          freq=3 → [F]
```

## 解法详解

### 解法1: 暴力 — O(1) get / O(n) put

**思考过程**：最直觉的做法——用 HashMap 存 key→{value, freq}，get 是 O(1)。淘汰时遍历所有 entry 找频率最小的（频率相同比时间戳），O(n)。

```cpp
class LFUCache {
    // 暴力思路伪代码（不推荐实现，仅展示思路）:
    // unordered_map<int, tuple<int,int,int>> cache; // key → (value, freq, timestamp)
    // int ts = 0; // 全局时间戳
    //
    // get(key): 找到 → freq++, 更新 timestamp, 返回 value
    // put(key, value):
    //   如果存在 → 更新 value, freq++, 更新 timestamp
    //   如果不存在:
    //     如果满了 → 遍历找 (freq最小, timestamp最小) 的 key, 删除
    //     插入 (value, freq=1, timestamp=ts++)
    //
    // 瓶颈: 淘汰时遍历 O(n)，不满足题目 O(1) 要求
};
```

**关键点**：暴力解帮我们理清了需求，但淘汰操作 O(n) 是不可接受的。

---

### 解法2: 双 HashMap + 双向链表 — O(1) / O(n) ⭐ 面试首选

**从暴力优化**：暴力的瓶颈是"找最小频率的最久未使用 key"。我们通过**按频率分桶** + **维护 minFreq 变量**来把这个操作降到 O(1)。

**核心数据结构**：
- `unordered_map<int, list<int>> freqMap`：频率 → 该频率下所有 key 的双向链表
- `unordered_map<int, {val, freq, iter}> keyMap`：key → 值、频率、在链表中的迭代器
- `int minFreq`：当前最小频率

```cpp
class LFUCache {
    // key → (value, frequency, 在freqMap对应链表中的迭代器)
    struct Node {
        int val, freq;
        list<int>::iterator iter;
    };
    
    int cap, minFreq;
    unordered_map<int, Node> keyMap;              // key → Node
    unordered_map<int, list<int>> freqMap;        // freq → 双向链表(存key)
    
    // 核心操作: 将 key 的频率从 f 提升到 f+1
    void increaseFreq(int key) {
        auto& node = keyMap[key];
        int oldFreq = node.freq;
        node.freq++;
        
        // 从旧频率链表中移除
        freqMap[oldFreq].erase(node.iter);
        
        // 如果旧频率链表空了，清理 + 更新 minFreq
        if (freqMap[oldFreq].empty()) {
            freqMap.erase(oldFreq);
            if (minFreq == oldFreq) {
                minFreq++;  // 关键: minFreq 只可能 +1，不会跳跃
            }
        }
        
        // 加入新频率链表尾部(尾部 = 最近使用)
        freqMap[node.freq].push_back(key);
        node.iter = prev(freqMap[node.freq].end());
    }
    
public:
    LFUCache(int capacity) : cap(capacity), minFreq(0) {}
    
    int get(int key) {
        if (keyMap.find(key) == keyMap.end()) return -1;
        increaseFreq(key);    // 访问一次，频率+1
        return keyMap[key].val;
    }
    
    void put(int key, int value) {
        if (cap <= 0) return;  // 边界: 容量为0
        
        // Case 1: key 已存在 → 更新值 + 增加频率
        if (keyMap.find(key) != keyMap.end()) {
            keyMap[key].val = value;
            increaseFreq(key);
            return;
        }
        
        // Case 2: key 不存在，需要插入
        // 如果满了，先淘汰
        if ((int)keyMap.size() >= cap) {
            // 淘汰 minFreq 链表的头部(最久未使用)
            int evictKey = freqMap[minFreq].front();
            freqMap[minFreq].pop_front();
            if (freqMap[minFreq].empty()) {
                freqMap.erase(minFreq);
            }
            keyMap.erase(evictKey);
        }
        
        // 插入新 key，频率为 1
        minFreq = 1;  // 新 key 的频率一定是最小的
        freqMap[1].push_back(key);
        keyMap[key] = {value, 1, prev(freqMap[1].end())};
    }
};
```

**关键点**：
- `minFreq` 只会在两种情况下改变：(1) 插入新 key 时重置为 1；(2) `increaseFreq` 中旧频率链表空了且等于 minFreq 时加 1。
- **minFreq 只可能 +1，不会跳跃**。因为如果 minFreq 对应的链表变空了，说明里面的 key 刚被提升到 minFreq+1，所以新的最小频率就是 minFreq+1。

---

### 解法3: 使用 STL list 作为有序集合（代码更简洁的变体）

**思路**：和解法2相同的架构，但利用 C++ `list` 的 `splice` 操作可以在 O(1) 内将节点从一个 list 移到另一个 list，避免手动 erase + push_back。

```cpp
class LFUCache {
    int cap, minFreq;
    // 链表节点: (key, value, freq)
    struct KVF { int key, val, freq; };
    
    unordered_map<int, list<KVF>> freqMap;                     // freq → list of KVF
    unordered_map<int, list<KVF>::iterator> keyMap;             // key → iterator in some freq list
    
public:
    LFUCache(int capacity) : cap(capacity), minFreq(0) {}
    
    int get(int key) {
        auto it = keyMap.find(key);
        if (it == keyMap.end()) return -1;
        
        auto listIter = it->second;
        int val = listIter->val;
        int freq = listIter->freq;
        
        // 从旧频率链表移除，加入新频率链表
        freqMap[freq + 1].splice(freqMap[freq + 1].end(), freqMap[freq], listIter);
        listIter->freq++;
        
        // 清理旧频率
        if (freqMap[freq].empty()) {
            freqMap.erase(freq);
            if (minFreq == freq) minFreq++;
        }
        
        // 迭代器在 splice 后仍然有效，不需要更新 keyMap
        return val;
    }
    
    void put(int key, int value) {
        if (cap <= 0) return;
        
        auto it = keyMap.find(key);
        if (it != keyMap.end()) {
            it->second->val = value;
            get(key);  // 复用 get 的频率更新逻辑
            return;
        }
        
        if ((int)keyMap.size() >= cap) {
            auto& minList = freqMap[minFreq];
            keyMap.erase(minList.front().key);
            minList.pop_front();
            if (minList.empty()) freqMap.erase(minFreq);
        }
        
        minFreq = 1;
        freqMap[1].push_back({key, value, 1});
        keyMap[key] = prev(freqMap[1].end());
    }
};
```

**关键点**：`list::splice` 在 O(1) 时间内将节点从一个 list 转移到另一个 list，且**迭代器不失效**。这让我们不需要更新 keyMap 中的迭代器。

## 解法对比

| | 解法1 暴力 | 解法2 双HashMap+链表 | 解法3 splice优化 |
|---|---|---|---|
| get 时间 | O(1) | O(1) | O(1) |
| put 时间 | O(n) 淘汰时 | O(1) | O(1) |
| 代码复杂度 | 简单 | 中等 | 稍简洁 |
| 面试推荐 | 仅讲思路 | ⭐ 首选 | 加分项 |
| 核心区别 | 线性查找淘汰 | 分桶+minFreq定位 | splice避免迭代器更新 |

解法2和3思路完全一致，只是代码实现细节不同。面试中写出解法2即可，如果面试官问"能不能更简洁"再提 splice。

## 易错点

1. **minFreq 更新遗漏**
   - ✗ 只在淘汰时更新 minFreq
   - ✓ 在 `increaseFreq` 中也要检查：如果旧频率链表空了且 `oldFreq == minFreq`，则 `minFreq++`

2. **put 已存在 key 时忘记增加频率**
   - ✗ 只更新 value，不调用 `increaseFreq`
   - ✓ put 已存在的 key 等价于一次"访问"，必须增加频率

3. **新插入 key 后 minFreq 不重置**
   - ✗ 插入新 key 后 minFreq 还是旧值
   - ✓ 新 key 的频率是 1，所以 `minFreq = 1`（因为 1 一定 ≤ 任何已存在 key 的频率）

4. **淘汰时从链表尾部删除**
   - ✗ `pop_back()`（删除了最近使用的）
   - ✓ `pop_front()`（删除最久未使用的）
   - 约定：头部 = 最久未使用，尾部 = 最近使用

5. **capacity 为 0 时没有提前返回**
   - ✗ 直接走正常逻辑导致 size < 0 的比较出错
   - ✓ `if (cap <= 0) return;`

6. **迭代器存储错误**
   - ✗ 存入 `freqMap[freq].end()` 作为迭代器
   - ✓ 存入 `prev(freqMap[freq].end())`（`end()` 指向的是 past-the-end，不是最后一个元素）

## 面试追问

**Q1: 你能描述一下 LFU 和 LRU 的区别吗？各自适合什么场景？**
- LRU 按"最近使用时间"淘汰，适合访问模式有时间局部性的场景（最近访问过的数据可能很快再次被访问）。
- LFU 按"使用频率"淘汰，适合某些数据长期高频访问的场景（热点数据不会因为一段时间没访问就被淘汰）。
- LFU 的缺点：曾经高频但已不再需要的数据("缓存污染")很难被淘汰。

**Q2: 你的 minFreq 为什么只会 +1，不会跳跃增长？请证明。**
- 当 `freqMap[minFreq]` 变空时，说明该链表中的所有 key 都刚被提升了频率。但在本次操作中我们只提升了一个 key（从 minFreq 到 minFreq+1），所以 minFreq+1 对应的链表一定非空。因此新的最小频率就是 minFreq+1。

**Q3: 如果要支持 `decreaseFreq` 操作（某个 key 的频率降低），你的设计需要怎么改？**
- `minFreq` 不能简单维护了，因为频率降低可能导致 minFreq 需要减小。
- 解决方案1: 用一个 `min-heap` 维护所有存在的频率，但这会让操作变成 O(log n)。
- 解决方案2: 维护一个有序集合(如 `std::set`)存所有非空频率，O(log n) 找最小值。
- O(1) 在这种情况下很难保证。

**Q4: 如何解决 LFU 的"缓存污染"问题？**
- 可以引入"频率衰减"机制：随时间推移，所有 key 的频率定期减半。
- 或使用"窗口 LFU"(W-LFU)：只统计最近一段时间窗口内的频率。
- 实际工业界常用 LRU-K (考虑最近 K 次访问时间) 或 ARC (自适应替换缓存)。

## 相关题型

- **146. LRU 缓存** — LFU 的前置题。复用 "HashMap + 双向链表" 的核心框架。区别：LRU 只需一条链表维护时间顺序，LFU 需要按频率分桶，每桶内维护时间顺序。学会 LRU 后做 LFU，只需加一个频率维度。

- **895. 最大频率栈** — 同样需要按频率分组，但操作相反（要弹出频率最高的元素）。复用"freqMap 按频率分桶"的思路，区别：LFU 追踪 minFreq，895 追踪 maxFreq。

- **432. 全 O(1) 的数据结构** — 同样需要 O(1) 维护最大/最小值。复用"频率分桶 + 维护极值"的思路，但用双向链表将所有频率桶串起来，可以 O(1) 访问两端极值。