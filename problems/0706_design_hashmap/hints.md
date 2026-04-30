# 706. 设计哈希映射

## 核心思路

这道题本质上是让你**从零实现哈希表的底层机制**：如何用一个哈希函数把任意 key 映射到有限的存储空间，并正确处理哈希冲突。

## 思维链

1. **读完题第一反应**：key 范围是 `[0, 10^6]`，最朴素的做法是开一个大小为 `10^6 + 1` 的数组，key 直接作为下标。这不需要哈希函数，空间换时间，极其简单。

2. **暴力解的问题**：虽然能过这道题，但开了 `10^6+1` 大小的数组非常浪费。如果 key 范围是 `10^9` 甚至更大呢？这个方案就不可行了。面试官一定会追问"能否用更少的空间？"

3. **哈希表的核心思想**：用一个哈希函数 `h(key) = key % N` 把 key 映射到 `[0, N-1]` 的桶里。N 远小于 key 的范围，所以节省空间。但不同的 key 可能映射到同一个桶——这就是**哈希冲突**。

4. **如何解决冲突？** 最常见的两种方式：
   - **拉链法（Chaining）**：每个桶维护一个链表，冲突的元素串在一起
   - **开放寻址法（Open Addressing）**：冲突时向后探测空位

5. **拉链法实现**：选择一个质数大小的桶数组（如 769），每个桶是一个链表。put/get/remove 都先算出桶号，再在链表中线性查找。

6. **复杂度分析**：假设 N 个桶、M 个元素，平均每个桶长度 M/N。当 M/N 较小时（负载因子低），操作接近 O(1)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 超大数组 | key 直接做下标 | O(1) | O(10^6) | 能说出即可 |
| 拉链法 | 取模+链表处理冲突 | 均摊 O(N/B) | O(B+N) | ⭐ 必须写出 |
| 开放寻址法 | 取模+线性探测 | 均摊 O(1) | O(B) | 加分项 |

> B = 桶数量，N = 已存元素数

## 关键提示

1. **为什么桶的数量选质数（如 769）？** 质数能让哈希值分布更均匀，减少冲突聚集。如果选 2 的幂次如 1024，所有偶数 key 都只落在偶数桶里，浪费一半空间。

2. **拉链法中 put 要注意**：先遍历链表看 key 是否已存在。如果存在要**更新**值，而不是再插入一个新节点。这是最容易忘的点。

3. **remove 的实现**：不是标记删除，而是真的从链表中移除节点。考虑使用链表的删除技巧。

4. **开放寻址法的 remove 陷阱**：不能直接置空，否则会打断探测链。需要用特殊标记（tombstone）。

5. 哈希表结构示意：
   ```
   buckets[]:
     [0] → (1001, v1) → (1770, v2) → null
     [1] → (1, v3) → null
     [2] → null
     ...
     [768] → (768, v4) → null
     
   hash(key) = key % 769
   hash(1001) = 1001 % 769 = 232... 
   实际上 key 落在哪个桶取决于取模结果
   ```

## 解法详解

### 解法1: 超大数组 — O(1) / O(10^6)

**思考过程**: key 的范围是 `[0, 10^6]`，直接开一个数组，下标就是 key。这完全不需要哈希函数，是最"笨"但最有效的方法。用 `-1` 表示"不存在"。

```cpp
class MyHashMap {
    vector<int> data;
public:
    MyHashMap() : data(1000001, -1) {}
    
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
```

**关键点**: 
- 这个解法能 AC，但面试官不会满意——它回避了哈希表设计的所有核心问题（哈希函数、冲突处理）。
- 如果 key 范围变成 `[0, 10^9]`，就需要 4GB 内存，完全不可行。

---

### 解法2: 拉链法（Separate Chaining）— 均摊 O(N/B) / O(B+N) ⭐ 面试首选

**从解法1优化**: 解法1的问题是空间浪费。我们用哈希函数把 key 压缩到小范围，冲突用链表解决。

**哈希表内部结构示意**:
```
put(1,1), put(2,2), put(770,7) 后 (桶大小 769):

buckets[]:
  [0] → null
  [1] → (1, 1) → (770, 7) → null    ← 1%769=1, 770%769=1, 冲突!
  [2] → (2, 2) → null                ← 2%769=2
  [3] → null
  ...
  [768] → null

get(1):  hash=1%769=1 → 遍历桶[1]的链表 → 找到key=1 → 返回1
get(770): hash=770%769=1 → 遍历桶[1]的链表 → 跳过key=1 → 找到key=770 → 返回7
remove(1): hash=1%769=1 → 遍历桶[1]的链表 → 找到key=1 → 删除该节点
```

```cpp
class MyHashMap {
    // 为什么选 769？质数能让取模结果分布更均匀，减少冲突
    static const int BUCKET_SIZE = 769;
    
    // 每个桶是一个链表，存储 (key, value) 对
    vector<list<pair<int, int>>> buckets;

public:
    MyHashMap() : buckets(BUCKET_SIZE) {}
    
    void put(int key, int value) {
        int idx = key % BUCKET_SIZE;
        // 先在桶里找 key 是否已经存在
        for (auto& [k, v] : buckets[idx]) {
            if (k == key) {
                v = value;  // 已存在：更新值
                return;
            }
        }
        // 不存在：追加新节点
        buckets[idx].emplace_back(key, value);
    }
    
    int get(int key) {
        int idx = key % BUCKET_SIZE;
        for (auto& [k, v] : buckets[idx]) {
            if (k == key) return v;
        }
        return -1;  // 没找到
    }
    
    void remove(int key) {
        int idx = key % BUCKET_SIZE;
        auto& bucket = buckets[idx];
        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->first == key) {
                bucket.erase(it);  // 链表删除 O(1)
                return;
            }
        }
    }
};
```

**关键点**:
- `list<pair<int,int>>` 是 STL 双向链表，erase 操作 O(1)
- 也可以用 `vector<pair<int,int>>`，erase 是 O(n) 但缓存友好，实际更快
- 负载因子 = 元素数 / 桶数。题目最多 10^4 次操作，769 个桶，平均每桶约 13 个元素，完全够用

---

### 解法3: 开放寻址法（Open Addressing, Linear Probing）— 均摊 O(1) / O(B)

**不同视角**: 拉链法用额外的链表存冲突元素。开放寻址法不用额外结构，冲突时在数组内部找下一个空位。

**探测过程示意**:
```
桶大小 = 10007 (大质数，保证装不满)

put(5, 'A'):  hash=5 → slot[5]空 → 放入(5,'A')
put(10012, 'B'): hash=10012%10007=5 → slot[5]被占 → 探测slot[6]空 → 放入(10012,'B')
get(10012): hash=5 → slot[5]是key=5≠10012 → 探测slot[6]是key=10012 → 返回'B'
remove(5): hash=5 → slot[5]是key=5 → 标记为DELETED(不能置空!)

为什么不能置空？如果slot[5]置空:
get(10012): hash=5 → slot[5]空 → 以为不存在 → 错误返回-1!
应该: slot[5]=DELETED → 继续探测slot[6] → 找到10012 → 正确!
```

```cpp
class MyHashMap {
    // 开放寻址需要保证装载因子 < 1，开大一些
    static const int BUCKET_SIZE = 20011;  // 大质数
    static const int EMPTY = -1;
    static const int DELETED = -2;
    
    // 每个槽位存 key 和 value
    vector<int> keys;
    vector<int> values;
    
    // 线性探测：找到 key 所在位置或第一个可插入位置
    int probe(int key) {
        int idx = key % BUCKET_SIZE;
        // 遇到 EMPTY 停止（后面不可能有该 key）
        // 遇到 DELETED 继续探测（该 key 可能在后面）
        while (keys[idx] != EMPTY && keys[idx] != key) {
            // 跳过已删除的和不匹配的
            idx = (idx + 1) % BUCKET_SIZE;
        }
        return idx;
    }

public:
    MyHashMap() : keys(BUCKET_SIZE, EMPTY), values(BUCKET_SIZE, 0) {}
    
    void put(int key, int value) {
        int idx = key % BUCKET_SIZE;
        int firstDeleted = -1;
        while (keys[idx] != EMPTY) {
            if (keys[idx] == key) {
                values[idx] = value;  // 已存在，更新
                return;
            }
            if (keys[idx] == DELETED && firstDeleted == -1) {
                firstDeleted = idx;  // 记录第一个删除位，备用
            }
            idx = (idx + 1) % BUCKET_SIZE;
        }
        // key 不存在，插入到 firstDeleted 或 当前空位
        int insertIdx = (firstDeleted != -1) ? firstDeleted : idx;
        keys[insertIdx] = key;
        values[insertIdx] = value;
    }
    
    int get(int key) {
        int idx = key % BUCKET_SIZE;
        while (keys[idx] != EMPTY) {
            if (keys[idx] == key) return values[idx];
            idx = (idx + 1) % BUCKET_SIZE;
        }
        return -1;
    }
    
    void remove(int key) {
        int idx = key % BUCKET_SIZE;
        while (keys[idx] != EMPTY) {
            if (keys[idx] == key) {
                keys[idx] = DELETED;  // 墓碑标记，不能用 EMPTY!
                return;
            }
            idx = (idx + 1) % BUCKET_SIZE;
        }
    }
};
```

**关键点**:
- 删除时用 `DELETED` 标记而非 `EMPTY`，否则会打断探测链
- 桶要开得比元素数大很多（负载因子 < 0.7），否则探测次数急剧增加
- `put` 中要同时处理 DELETED 位的复用，避免空间泄漏

## 解法对比

| | 超大数组 | 拉链法 | 开放寻址 |
|---|---|---|---|
| 核心思想 | key=下标，无哈希 | 取模+链表 | 取模+线性探测 |
| 时间 | O(1) 严格 | O(N/B) 均摊 | O(1) 均摊(低负载) |
| 空间 | O(key范围) | O(B+N) | O(B) |
| 冲突处理 | 无冲突 | 链表存所有冲突元素 | 在数组内部找下一个空位 |
| 删除 | 直接置-1 | 链表删除 | 需要墓碑标记 |
| 适用场景 | key范围小且确定 | 通用，面试首选 | 缓存友好，工业常用 |

**面试选哪个？** 拉链法。它最直观、最不容易写错、是面试官最期望看到的解法。

## 易错点

1. **put 时忘记检查 key 是否已存在**：
   - ✗ 直接 `bucket.push_back({key, value})` → 同一个 key 出现多个节点
   - ✓ 先遍历链表，找到已有 key 则更新 value，找不到才追加

2. **remove 用迭代器删除后继续遍历**：
   - ✗ `for (auto& p : bucket) { if (...) bucket.erase(找p的迭代器); }` → 范围 for 中修改容器是未定义行为
   - ✓ 用显式迭代器遍历，erase 后立即 return

3. **开放寻址法 remove 置空**：
   - ✗ `keys[idx] = EMPTY` → 打断探测链，导致后续 get 找不到元素
   - ✓ `keys[idx] = DELETED`（墓碑标记）

4. **哈希函数选择 2 的幂次作为桶大小**：
   - ✗ `key % 1024` → 只看 key 的低 10 位，高位信息丢失，冲突严重
   - ✓ `key % 769`（质数）→ 分布更均匀

## 面试追问

**Q1: 你这个拉链法，最坏情况时间复杂度是多少？什么时候会发生？**
→ 最坏 O(N)。当所有 key 都映射到同一个桶时（如 key 全是 769 的倍数），退化为在单个链表上线性查找。

**Q2: 如何避免最坏情况？实际工程中怎么处理？**
→ 两个方向：(1) **动态扩容（rehash）**：当负载因子超过阈值（如 0.75）时，把桶数翻倍并重新哈希所有元素；(2) **用红黑树替代链表**（Java 8 的 HashMap 在链表长度 > 8 时转红黑树）。

**Q3: 如果这个 HashMap 需要支持多线程并发读写怎么办？**
→ (1) **粗粒度锁**：整个表一把锁，简单但并发度低；(2) **分段锁（Segment Lock）**：不同的桶用不同的锁，Java ConcurrentHashMap 的早期实现；(3) **CAS + 无锁链表**：更高并发度但实现复杂。

## 相关题型

- **705. 设计哈希集合** — 本题的姊妹题，区别：HashSet 只存 key 不存 value，把 `pair<int,int>` 改成 `int` 即可，其余框架完全复用
- **146. LRU 缓存** — HashMap + 双向链表的组合设计，复用本题的哈希思想，增加链表维护访问顺序
- **380. O(1)时间插入、删除和获取随机元素** — HashMap + 动态数组的组合，复用哈希 O(1) 查找的思想