# 716. 最大栈

## 核心思路

本质上是设计一个数据结构，既要支持栈的 LIFO 操作（push/pop/top），又要支持**按值查找和删除最大元素**（peekMax/popMax）。难点在于 `popMax` 需要从栈的"中间"删除元素，这打破了栈只能操作栈顶的限制。

## 思维链

1. **读完题第一反应**：栈操作好办，但怎么快速找到最大值？→ 最朴素的想法：用两个栈，一个正常栈，一个维护当前最大值（类似 155. Min Stack）。但 `popMax` 要从中间删元素，两个栈做不到高效删除。

2. **暴力解的瓶颈**：如果用双栈，`popMax` 需要把元素一个个弹出来找到最大值再放回去，这是 O(n) 的。题目 Follow-up 要求除 `top` 外都是 O(log n)。

3. **怎么突破瓶颈？** 需要两个能力：
   - **快速找到最大值** → 有序数据结构（平衡BST / TreeMap / `std::map`）
   - **快速定位并删除栈中任意位置的元素** → 双向链表（O(1) 删除已知节点）

4. **核心设计**：用**双向链表**模拟栈（尾部是栈顶），用**有序映射（`std::map`）** 维护值到链表节点的映射。push 时链表尾部插入 + map 中记录；pop 时链表尾部删除 + map 中移除；popMax 时在 map 中找最大 key 的最后一个节点（最靠近栈顶），从链表中删除。

5. **为什么 map 中存节点列表？** 因为可能有重复值，且 popMax 要删"最靠近栈顶"的那个，所以同一个值可能对应多个链表节点，我们需要取最后插入的那个。

6. **时间复杂度验证**：双向链表插入/删除 O(1)，map 的插入/删除/查最大 O(log n)，所以除了 top O(1) 外，其他都是 O(log n)。✓

## 解法概览

| 解法 | 思路 | push | pop | top | peekMax | popMax | 空间 | 面试 |
|------|------|------|-----|-----|---------|--------|------|------|
| 双栈 | 正常栈 + 最大值栈 | O(1) | O(1) | O(1) | O(1) | **O(n)** | O(n) | 能说出即可 |
| 双向链表 + 有序映射 | 链表维护顺序，map 维护值 | O(log n) | O(log n) | O(1) | O(log n) | O(log n) | O(n) | ⭐ 必须写出 |

## 关键提示

1. **从 Min Stack (155) 出发思考**：Min Stack 用辅助栈就能搞定，因为只有 `getMin` 没有 `popMin`。Max Stack 多了 `popMax`，这意味着要能从"栈的中间"删除元素——普通数组/栈做不到 O(1) 中间删除。

2. **双向链表的优势**：如果你已经知道要删除哪个节点（有指针），双向链表可以 O(1) 完成删除。关键是怎么"快速找到要删的节点"。

3. **map 的最大值**：`std::map` 是有序的，`rbegin()` 直接给你最大 key，O(log n)。

4. **重复值处理**：值相同时 popMax 要删最靠近栈顶的 → map 的 value 用 `list` 存储所有同值节点的迭代器，删最后一个（最后插入 = 最靠近栈顶）。

5. **ASCII 示意图**：
```
双向链表 (栈底 ←→ 栈顶):
  dummy ↔ [5] ↔ [1] ↔ [5]
                        ↑ 栈顶

有序映射 (map):
  1 → { 指向链表中 [1] 节点的迭代器 }
  5 → { 指向链表中第一个[5]的迭代器, 指向第二个[5]的迭代器 }
       ← 最旧                      最新(最靠近栈顶) →

popMax: map.rbegin() → key=5, 取 list 最后一个迭代器 → 指向链表尾部的 [5]
        从链表删除该节点, 从 map 的 list 中移除该迭代器
```

## 解法详解

### 解法1: 双栈法 — O(n) popMax

**思考过程**：类比 155. Min Stack，用一个辅助栈 `maxStk` 记录到当前位置为止的最大值。push/pop/top/peekMax 都是 O(1)。但 popMax 要从栈中间删除最大元素，只能逐个弹出到临时栈，删掉最大值后再放回来。

```cpp
class MaxStack {
    // 正常栈存所有元素
    stack<int> stk;
    // 辅助栈：maxStk[i] = 前 i 个元素中的最大值
    stack<int> maxStk;
public:
    MaxStack() {}
    
    void push(int x) {
        stk.push(x);
        // 如果 maxStk 为空或 x >= 当前最大值，更新最大值
        // 注意是 >= 不是 >，因为重复最大值也要记录
        int curMax = maxStk.empty() ? x : max(maxStk.top(), x);
        maxStk.push(curMax);
    }
    
    int pop() {
        int val = stk.top();
        stk.pop();
        maxStk.pop();
        return val;
    }
    
    int top() {
        return stk.top();
    }
    
    int peekMax() {
        return maxStk.top();
    }
    
    int popMax() {
        int maxVal = maxStk.top();
        // 把栈顶元素逐个弹出到临时栈，直到找到最大值
        stack<int> tmp;
        while (stk.top() != maxVal) {
            tmp.push(stk.top());
            stk.pop();
            maxStk.pop();
        }
        // 弹出最大值
        stk.pop();
        maxStk.pop();
        // 把临时栈的元素放回去（要重新计算 maxStk）
        while (!tmp.empty()) {
            push(tmp.top()); // 复用 push，自动维护 maxStk
            tmp.pop();
        }
        return maxVal;
    }
};
```

**关键点**：
- `popMax` 最坏 O(n)，因为最大值可能在栈底。
- 放回时必须复用 `push` 而不是直接压栈，否则 `maxStk` 不会被正确维护。

---

### 解法2: 双向链表 + 有序映射 — O(log n) ⭐ 面试首选

**从解法1优化**：解法1的瓶颈是 `popMax` 需要线性扫描找最大值并重建栈。我们需要：
- O(log n) 找到最大值 → 用 `std::map`（有序，最大 key 在 `rbegin()`）
- O(1) 从栈中间删除节点 → 用双向链表（已知节点指针即可 O(1) 删除）

**核心数据结构**：
- `list<int> dll`：双向链表模拟栈，`back()` 是栈顶
- `map<int, vector<list<int>::iterator>> mp`：值 → 该值在链表中所有节点的迭代器列表

```cpp
class MaxStack {
    // 双向链表：back() = 栈顶
    list<int> dll;
    // 有序映射：key = 值, value = 该值对应的所有链表迭代器（按插入顺序）
    map<int, vector<list<int>::iterator>> mp;
    
public:
    MaxStack() {}
    
    void push(int x) {
        dll.push_back(x);
        // prev(dll.end()) 就是刚插入的节点的迭代器
        mp[x].push_back(prev(dll.end()));
    }
    
    int pop() {
        int val = dll.back();
        dll.pop_back();
        // 从 map 中移除该迭代器（它是该值的最后一个，即最靠近栈顶的）
        mp[val].pop_back();
        if (mp[val].empty()) mp.erase(val);
        return val;
    }
    
    int top() {
        return dll.back();
    }
    
    int peekMax() {
        // map 有序，rbegin() 就是最大 key
        return mp.rbegin()->first;
    }
    
    int popMax() {
        int maxVal = mp.rbegin()->first;
        // 取最靠近栈顶的那个（vector 最后一个 = 最后插入的 = 最靠近栈顶）
        auto it = mp[maxVal].back();
        mp[maxVal].pop_back();
        if (mp[maxVal].empty()) mp.erase(maxVal);
        // 从链表中 O(1) 删除
        dll.erase(it);
        return maxVal;
    }
};
```

**关键点**：
- `list::erase(iterator)` 是 O(1)，这是链表相比数组的核心优势。
- `map` 中 vector 的最后一个元素对应最靠近栈顶的节点，因为 push 是按时间顺序追加的。
- `prev(dll.end())` 获取刚插入节点的迭代器——`end()` 是 past-the-end，前一个才是最后一个有效节点。

## 解法对比

| | 解法1 (双栈) | 解法2 (链表+map) |
|---|---|---|
| popMax | O(n) — 逐个弹出再放回 | O(log n) — map 找最大 + 链表 O(1) 删除 |
| 实现难度 | 简单，纯栈操作 | 中等，需要理解迭代器和多数据结构配合 |
| 适用场景 | 数据量小或面试暖场 | 数据量大，满足 Follow-up 要求 |
| 核心思想 | 辅助栈记录历史最大值 | 双向链表支持中间删除 + 有序容器快速查最值 |

## 易错点

1. **push 时 maxStk 的比较用 `>=` 不是 `>`**
   - ✗ `if (x > maxStk.top()) maxStk.push(x);` — 重复最大值只记录一次，pop 时 maxStk 提前被弹空
   - ✓ `maxStk.push(max(maxStk.top(), x));` — 每次 push 都同步更新 maxStk

2. **解法2 中忘记清理 map 的空 vector**
   - ✗ 只做 `mp[val].pop_back()` — map 中留下空 vector，`rbegin()` 可能指向一个没有节点的 key
   - ✓ pop_back 后检查 `if (mp[val].empty()) mp.erase(val);`

3. **解法2 获取刚插入节点的迭代器写错**
   - ✗ `mp[x].push_back(dll.end());` — `end()` 不是有效节点！
   - ✓ `mp[x].push_back(prev(dll.end()));` — 或者用 `push_back` 前保存 `--dll.end()`

4. **popMax 弹出的不是"最靠近栈顶"的最大值**
   - vector 中最后一个才是最后插入的（最靠近栈顶），取 `back()` 而不是 `front()`

## 面试追问

**Q1: 如果只需要 peekMax 不需要 popMax，怎么简化？**
→ 就是 155. Min Stack 的镜像版。用辅助栈记录到当前位置为止的最大值，所有操作 O(1)。

**Q2: 为什么普通数组/vector 不能代替双向链表？**
→ 数组中间删除是 O(n)（要移动后面所有元素），双向链表中间删除是 O(1)（修改前后指针即可）。这正是 popMax 需要的。

**Q3: 能否用 `multiset` 代替 `map<int, vector<iterator>>`？**
→ 可以，但有陷阱。`multiset` 能快速找最大值（`*rbegin()`），但 popMax 要删"最靠近栈顶"的最大值。`multiset` 中相同值的多个元素没有"栈中位置"的概念，需要额外存储位置信息。所以 `map<int, vector<list::iterator>>` 更自然：vector 的顺序天然反映插入顺序。

**Q4: 如果操作极其频繁（10^7 次），有什么进一步优化？**
→ 可以考虑用平衡 BST（如 `std::set` 加自定义比较）直接维护 (value, timestamp) 对，避免 map 中 vector 的动态扩容。每个节点同时在链表和 set 中，删除时两边同步操作。核心思想不变，只是实现细节的优化。

## 相关题型

- **155. Min Stack** — 本题的简化版，不需要 `popMin`，所以辅助栈就够了。学会 155 后理解本题的解法1 直接复用。
- **146. LRU Cache** — 同样使用"双向链表 + 哈希映射"的组合设计。区别：LRU 的 map 是 key→node，本题是 value→nodes（因为有重复值）。复用双向链表节点删除+重插入的技巧。
- **895. Maximum Frequency Stack** — 频率栈，popMax 变成 pop 频率最高的元素。同样需要多数据结构配合。复用"用额外数据结构跟踪极值"的设计思路。