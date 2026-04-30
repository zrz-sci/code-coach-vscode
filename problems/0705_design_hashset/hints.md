# 705. 设计哈希集合

## 核心思路

不使用内置哈希表库，从零设计一个 HashSet，支持 `add`、`remove`、`contains` 三种操作。这道题考察的是**哈希表底层原理**的理解程度。

两种典型实现：
1. **大数组直接映射**（利用 key 范围有限 0~10^6）—— 最简方案，但不是面试想看到的
2. **链地址法**（数组 + 链表）—— 面试推荐，体现哈希冲突处理机制

## 数据结构示意图

```
链地址法 (Separate Chaining):

buckets (大小 = 1009，选质数减少冲突)
┌─────┐
│  0  │ → [1009] → [2018] → null
├─────┤
│  1  │ → [1] → [1010] → null
├─────┤
│  2  │ → [2] → null
├─────┤
│  3  │ → null  (空桶)
├─────┤
│ ... │
├─────┤
│1008 │ → [1008] → null
└─────┘

hash(key) = key % 1009

示例操作:
  add(1):      hash(1)=1,   桶1: [] → [1]
  add(2):      hash(2)=2,   桶2: [] → [2]
  add(1010):   hash(1010)=1, 桶1: [1] → [1,1010]  (哈希冲突，追加到链表)
  contains(1): hash(1)=1,   遍历桶1找到1 → true
  remove(1):   hash(1)=1,   从桶1中删除1 → 桶1: [1010]
  contains(1): hash(1)=1,   遍历桶1找不到1 → false
```

```
布尔数组直接映射法:

data[0..1000000]
  ┌───┬───┬───┬───┬───┬───┬─────┬───┐
  │ F │ T │ T │ F │ F │ F │ ... │ F │
  └───┴───┴───┴───┴───┴───┴─────┴───┘
    0   1   2   3   4   5        10^6

add(1):      data[1] = true
add(2):      data[2] = true
contains(1): return data[1]   → true
remove(2):   data[2] = false
contains(2): return data[2]   → false

优点: 每个操作 O(1)，实现极简
缺点: 空间 O(范围)，如果 key 范围很大就不适用
```

## 思维链

1. **读完题第一反应** → HashSet 需要支持增删查。最暴力的做法：用一个 `vector` 或 `list` 存所有元素，每次操作线性扫描。但这样每个操作都是 O(n)，太慢了。
2. **想到直接映射** → key 范围是 0~10^6，直接开一个 `bool` 数组 `data[1000001]`，用下标做"哈希"。`add(key)` 就是 `data[key] = true`，`contains(key)` 就是 `return data[key]`。O(1) 操作，O(10^6) 空间。
3. **面试官会怎么想？** → 直接映射虽然能过 OJ，但本质上没有用到任何哈希的知识。面试中应该展示**哈希冲突处理**的能力。
4. **链地址法** → 选一个质数 `BUCKET`（如 1009），用 `hash = key % BUCKET` 把 key 映射到桶。每个桶用链表（`list<int>`）存放所有映射到该桶的 key。这样平均每个操作 O(1)，最坏 O(n/BUCKET)。
5. **为什么选质数？** → 质数作为桶数可以减少哈希冲突的规律性。如果用 1000（偶数），所有偶数 key 都映射到偶数桶，奇数桶永远为空，冲突加倍。质数让 key 更均匀地分散到各个桶。
6. **桶数选择** → 题目最多 10^4 次调用，选 1009 个桶，平均每桶约 10 个元素，链表遍历代价很小。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试推荐 |
|------|------|------|------|----------|
| 布尔数组 | 直接下标映射 | O(1) | O(范围) = O(10^6) | 能说出，但不够 |
| 链地址法 | 数组+链表处理冲突 | 平均 O(1)，最坏 O(n/B) | O(n + B) | 推荐 |
| 开放寻址法 | 冲突时探测下一个空位 | 平均 O(1)，最坏 O(n) | O(B) | 进阶加分 |

## 关键提示

1. **提示1 — 最简方案**: `vector<bool> data(1000001, false)` 直接下标映射，add/remove/contains 都是一行代码。面试中先说这个方案表明你理解题意。
2. **提示2 — 哈希函数**: `hash = key % BUCKET`，BUCKET 选质数（如 769、1009、10007），减少冲突。
3. **提示3 — 冲突处理**: 每个桶用 `list<int>` 存放冲突元素。`add` 时先查是否已存在（Set 不允许重复）；`remove` 直接删除；`contains` 遍历链表查找。
4. **提示4 — add 去重**: HashSet 的 add 操作如果 key 已存在，不应重复插入。所以 `add` 必须先 `contains` 检查。
5. **提示5 — 开放寻址法思路**: 不用链表，冲突时在数组中线性探测下一个空位。删除时不能直接置空（会断开探测链），需要用特殊标记（tombstone）。面试中提到即可。

## 解法详解

### 解法1: 布尔数组直接映射 -- O(1) / O(10^6)

**思考过程**: key 范围 0~10^6，直接用数组下标表示 key 存在与否。

```cpp
class MyHashSet {
    vector<bool> data;
public:
    MyHashSet() : data(1000001, false) {}
    void add(int key)      { data[key] = true; }
    void remove(int key)   { data[key] = false; }
    bool contains(int key) { return data[key]; }
};
```

**关键点**:
- `vector<bool>` 在很多实现中会做 bit 压缩，实际内存约 125KB，非常小。
- 这个解法虽然高效，但没有展示哈希表的设计能力。面试中提一句即可，然后转到链地址法。

---

### 解法2: 链地址法 -- 平均 O(1) / O(n + BUCKET) -- 面试推荐

**从解法1优化**: 如果 key 范围是 0~10^9 甚至更大，直接映射不可行。链地址法通过**哈希函数 + 链表**，用有限的桶数处理任意范围的 key。

```
add(1), add(1010) 的过程 (BUCKET=1009):

hash(1) = 1 % 1009 = 1
hash(1010) = 1010 % 1009 = 1   ← 冲突！

桶1: [1] --add(1010)--> [1] → [1010]
       链表追加，两个不同的key共享一个桶
```

```cpp
class MyHashSet {
    static const int BUCKET = 1009; // 质数桶数
    vector<list<int>> data;         // 每个桶一条链表

    int hash(int key) { return key % BUCKET; }

public:
    MyHashSet() : data(BUCKET) {}

    void add(int key) {
        int h = hash(key);
        // 先检查是否已存在 (Set 不允许重复)
        for (int x : data[h]) {
            if (x == key) return; // 已存在，直接返回
        }
        data[h].push_back(key);
    }

    void remove(int key) {
        int h = hash(key);
        data[h].remove(key); // list::remove 删除所有值等于key的节点
    }

    bool contains(int key) {
        int h = hash(key);
        for (int x : data[h]) {
            if (x == key) return true;
        }
        return false;
    }
};
```

**关键点**:
- `list<int>` 的 `remove(val)` 方法会删除链表中所有值等于 `val` 的节点，时间 O(链表长度)。
- `add` 中必须先遍历检查是否已存在，否则同一个 key 会被多次插入。
- 桶数 1009 是质数，面试中解释选质数的理由可以加分。

## 易错点

1. **add 忘记去重**:
   - 错误: 直接 `data[h].push_back(key)` 不检查是否已存在 → 同一个 key 被插入多次
   - 正确: 先遍历检查，已存在则 return
   
2. **桶数选偶数或 10 的幂**:
   - 错误: `BUCKET = 1000` → 所有 1000 的倍数都映射到桶 0，冲突严重
   - 正确: `BUCKET = 1009`（质数），分布更均匀

3. **开放寻址法的删除问题**:
   - 错误: 删除时直接将槽位置空 → 后续探测链断裂，`contains` 误报不存在
   - 正确: 使用 tombstone 标记（如 -1 表示"曾有元素但已删除"），探测时遇到 tombstone 继续往后找

4. **混淆 HashSet 和 HashMap**:
   - HashSet 存储**值**，不允许重复
   - HashMap 存储**键值对**，key 不允许重复但 value 可以更新
   - 本题是 HashSet，add 时检查 key 已存在直接跳过；如果是 HashMap 的 put，应该更新 value

## 面试追问

**Q1: 为什么选质数作为桶数？**
> 如果桶数是合数（比如 1000），key 如果有规律（如都是偶数），会大量映射到同一批桶。质数让取模后的结果分布更均匀。例如，key = 2, 4, 6, 8...，桶数=1000 时只用偶数桶；桶数=1009 时分布到各个桶。

**Q2: 链地址法 vs 开放寻址法怎么选？**
> 链地址法：实现简单，删除容易，适合冲突多或负载因子高的场景。Java HashMap 用的就是链地址法。
> 开放寻址法：缓存友好（内存连续），适合负载因子低的场景。Python dict 用的就是开放寻址法。
> 面试中推荐链地址法，更容易写对。

**Q3: 如果数据量增大，如何动态扩容？**
> 当负载因子（元素数 / 桶数）超过阈值（如 0.75）时，将桶数翻倍（选下一个质数），然后 rehash 所有元素到新桶。这就是 Java HashMap 的 resize 机制。时间 O(n) 但均摊后仍是 O(1)。

**Q4: 这道题和 LC 706 (Design HashMap) 有什么区别？**
> 706 需要存 key-value 对，链表节点从 `int` 变成 `pair<int,int>`。add 变成 put(key, value)，如果 key 已存在要**更新 value** 而非跳过。contains 变成 get(key)，返回 value 或 -1。

## 相关题型

- **706. 设计哈希映射 (Design HashMap)** — 姊妹题，从 Set 扩展到 Map，链表节点存 `pair<key, value>`，put 时如果 key 已存在需更新 value。
- **1206. 设计跳表 (Design Skiplist)** — 更高级的数据结构设计题，支持有序操作。
- **380. O(1) 时间插入、删除和获取随机元素** — 需要 O(1) getRandom，用哈希表 + 数组配合实现。
- **146. LRU 缓存** — 哈希表 + 双向链表的经典设计题，哈希表提供 O(1) 查找，链表维护访问顺序。
