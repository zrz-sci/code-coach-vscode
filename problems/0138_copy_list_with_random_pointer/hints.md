# 138. 随机链表的复制

## 核心思路

这道题本质上是**带有"跨节点引用"的深拷贝问题**：`next` 指针好处理（顺序复制即可），难点在于 `random` 指针——当你复制节点 A 时，A.random 指向的节点 B 可能还没被创建，你无法直接设置新节点的 random。核心问题是：**如何建立"原节点 → 新节点"的映射关系**。

## 思维链

1. **读完题第一反应**：如果没有 `random` 指针，就是普通链表复制——遍历一遍，逐个 `new` 就行。难点完全在 `random` 上。

2. **为什么 random 难处理？** 当我遍历到节点 A 想设置 `A_copy.random` 时，`A.random` 指向的节点 B 对应的副本 `B_copy` 可能还不存在（如果 B 在 A 后面的话）。即使 B_copy 存在了，我也无法从 `B` 快速找到 `B_copy`。

3. **暴力想法**：先把所有节点复制一遍（不管 random），然后对每个节点，从头遍历原链表找到 random 指向的是第几个节点，再到新链表中找到对应位置。这需要 O(n) 找位置，总共 O(n²)。

4. **瓶颈在哪？** "从原节点找到对应的新节点"这个查找操作是 O(n)。如果能 O(1) 完成这个映射，就能降到 O(n)。

5. **用哈希表突破**：用 `HashMap<原节点, 新节点>` 存映射。第一遍遍历创建所有新节点并建立映射；第二遍遍历设置 next 和 random。

6. **能否不用额外空间？** 把新节点"编织"到原链表中（插在每个原节点后面），这样原节点的 next 就天然指向它的副本——不需要哈希表就能 O(1) 找到映射！

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力两遍遍历 | 先复制 next，再对每个 random 从头找位置 | O(n²) | O(1)* | 能说出即可 |
| 哈希表映射 | HashMap 存原→新的映射，两遍搞定 | O(n) | O(n) | ⭐ 必须写出 |
| 节点编织(交错插入) | 新节点插原节点后面，利用位置关系设 random | O(n) | O(1) | 加分项 |

> *暴力解的 O(1) 空间不算输出链表本身

## 关键提示

1. **映射是核心**：这道题的一切难度都来自"给定一个原节点，如何快速找到它对应的新节点"。想清楚这个映射怎么建，问题就解决了。

2. **哈希表解法分两步想**：
   - 第一步：只管创建新节点，不管指针连接
   - 第二步：利用映射表，设置每个新节点的 next 和 random

3. **编织法的关键直觉**：如果新节点 A' 紧跟在原节点 A 后面，那 `A.random.next` 就是 `A'.random` 应该指向的节点！

4. **ASCII 图示——编织法三步骤**：
```
原始链表:
  A → B → C → null
  |       ↑
  +-------+  (A.random → C)

Step1: 编织插入
  A → A' → B → B' → C → C' → null

Step2: 设置 random
  A.random = C, 所以 A'.random = C.next = C'

Step3: 拆分
  A → B → C → null          (原链表恢复)
  A' → B' → C' → null       (新链表)
```

5. **别忘了 null**：`random` 可以为 null，设置 random 指针时必须先判空。

## 解法详解

### 解法1: 暴力 — O(n²) / O(1)

**思考过程**: 最直觉的做法：先按 next 顺序复制一遍链表（忽略 random），然后对每个节点，用"数位置"的方式处理 random——原链表中 A.random 是第 k 个节点，那新链表中 A_copy.random 也应该是第 k 个新节点。

```cpp
class Solution {
public:
    Node* copyRandomList(Node* head) {
        if (!head) return nullptr;
        
        // 第一步：复制所有节点，只连 next
        Node* newHead = new Node(head->val);
        Node* oldCur = head->next;
        Node* newCur = newHead;
        while (oldCur) {
            newCur->next = new Node(oldCur->val);
            newCur = newCur->next;
            oldCur = oldCur->next;
        }
        
        // 第二步：对每个节点，找 random 的位置
        oldCur = head;
        newCur = newHead;
        while (oldCur) {
            if (oldCur->random) {
                // 数一下 oldCur->random 是原链表的第几个节点
                int idx = 0;
                Node* tmp = head;
                while (tmp != oldCur->random) {
                    tmp = tmp->next;
                    idx++;
                }
                // 在新链表中找第 idx 个节点
                Node* target = newHead;
                for (int i = 0; i < idx; i++) {
                    target = target->next;
                }
                newCur->random = target;
            }
            oldCur = oldCur->next;
            newCur = newCur->next;
        }
        return newHead;
    }
};
```

**关键点**: 每个节点的 random 都要从头遍历找位置，所以是 O(n²)。这个解法说明你理解了问题，但效率太低。

---

### 解法2: 哈希表 — O(n) / O(n) ⭐ 面试首选

**从解法1优化**: 解法1的瓶颈是"找原节点对应的新节点"需要 O(n)。用哈希表 `map<原节点指针, 新节点指针>` 存储映射，查找降为 O(1)。

```cpp
class Solution {
public:
    Node* copyRandomList(Node* head) {
        if (!head) return nullptr;
        
        // 第一遍：创建所有新节点，建立 原→新 的映射
        unordered_map<Node*, Node*> oldToNew;
        Node* cur = head;
        while (cur) {
            oldToNew[cur] = new Node(cur->val);
            cur = cur->next;
        }
        
        // 第二遍：利用映射设置 next 和 random
        cur = head;
        while (cur) {
            // cur 的副本是 oldToNew[cur]
            // cur->next 的副本是 oldToNew[cur->next]
            oldToNew[cur]->next = oldToNew[cur->next];       // cur->next 为 null 时，map[null] 默认为 nullptr
            oldToNew[cur]->random = oldToNew[cur->random];   // 同理
            cur = cur->next;
        }
        
        return oldToNew[head];
    }
};
```

**关键点**: 
- `unordered_map` 的 `operator[]` 对不存在的 key 会插入默认值（指针默认 nullptr），所以 `oldToNew[nullptr]` 自动返回 nullptr，正好处理了 random/next 为 null 的情况。
- 为什么分两遍？第一遍必须先把所有新节点都创建出来，第二遍设置 random 时才能保证目标节点已存在。

---

### 解法3: 节点编织（交错插入）— O(n) / O(1) 

**核心直觉**: 如果不想用哈希表，需要另一种方式建立"原→新"的映射。巧妙的做法是把新节点直接插到原节点后面——这样 `原节点.next` 就是它的副本，天然完成了映射！

```
// 编织后的链表结构:
//   原A → 新A' → 原B → 新B' → 原C → 新C' → null
//   
//   对于任意原节点 old:
//     old.next = old 的副本
//     old.random.next = old.random 的副本（这就是我们要设置的 random!）
```

```cpp
class Solution {
public:
    Node* copyRandomList(Node* head) {
        if (!head) return nullptr;
        
        // ===== Step 1: 在每个原节点后面插入它的副本 =====
        // A → B → C  变成  A → A' → B → B' → C → C'
        Node* cur = head;
        while (cur) {
            Node* copy = new Node(cur->val);
            copy->next = cur->next;   // A' 指向 B
            cur->next = copy;          // A 指向 A'
            cur = copy->next;          // 跳到 B（下一个原节点）
        }
        
        // ===== Step 2: 设置每个副本节点的 random =====
        // 关键: A.random 的副本就是 A.random.next
        cur = head;
        while (cur) {
            Node* copy = cur->next;    // cur 的副本紧跟在 cur 后面
            // 如果 cur.random 不为 null，它的副本是 cur.random.next
            copy->random = (cur->random) ? cur->random->next : nullptr;
            cur = copy->next;          // 跳到下一个原节点
        }
        
        // ===== Step 3: 拆分——恢复原链表 + 提取新链表 =====
        Node* newHead = head->next;
        cur = head;
        while (cur) {
            Node* copy = cur->next;
            cur->next = copy->next;                       // 恢复原链表: A→B
            copy->next = (copy->next) ? copy->next->next : nullptr;  // 新链表: A'→B'
            cur = cur->next;                              // 移到下一个原节点
        }
        
        return newHead;
    }
};
```

**关键点**: 
- Step 3 中必须同时恢复原链表和构建新链表，否则原链表被破坏了（面试中改变输入通常不被允许）
- `copy->next->next` 之前要检查 `copy->next` 是否为 null（最后一个节点的情况）

## 解法对比

| | 解法1(暴力) | 解法2(哈希表) | 解法3(编织) |
|---|---|---|---|
| 时间 | O(n²) | O(n) | O(n) |
| 空间 | O(1) | O(n) | O(1) |
| 代码复杂度 | 简单但慢 | 简洁易懂 | 需要仔细处理指针 |
| 面试推荐 | 用于说明理解 | ⭐ 首选 | 追问"能否O(1)空间"时给出 |
| 是否修改原链表 | 否 | 否 | 临时修改，最后恢复 |

**面试策略**：先说解法2（哈希表），简洁正确不出错。面试官追问空间优化时再给解法3。

## 易错点

1. **哈希表解法中忘记处理 null**
   - ✗ `oldToNew[cur]->random = oldToNew[cur->random];` 在某些 map 实现中对 null key 可能有问题
   - ✓ C++ 的 `unordered_map` 对 `nullptr` 键会返回默认值 `nullptr`，恰好可以利用；但如果你不确定，显式判断更安全：`copy->random = cur->random ? oldToNew[cur->random] : nullptr;`

2. **编织法 Step3 拆分时漏了边界**
   - ✗ `copy->next = copy->next->next;` —— 当 copy 是最后一个新节点时，copy->next 是 null，访问 null->next 崩溃
   - ✓ `copy->next = copy->next ? copy->next->next : nullptr;`

3. **编织法忘记恢复原链表**
   - 如果只提取新链表而不恢复原链表的 next 指针，原链表就被永久破坏了。面试中这是扣分点。

4. **遗漏空链表检查**
   - ✗ 直接 `Node* newHead = head->next;` —— 如果 head 为 null 就崩溃
   - ✓ 开头加 `if (!head) return nullptr;`

## 面试追问

**Q1: 你能用最简单的方式深拷贝这个链表吗？**
→ 哈希表解法。两遍遍历，第一遍创建节点建映射，第二遍设置 next 和 random。时间 O(n)，空间 O(n)。

**Q2: 能否在 O(1) 额外空间内完成？**
→ 编织法（交错插入）。利用"新节点紧跟原节点"的位置关系替代哈希表。三步：插入→设 random→拆分。注意最后要恢复原链表。

**Q3: 如果这不是链表而是一个任意图的深拷贝（节点有多个邻居）呢？**
→ 同样用哈希表 `map<原节点, 新节点>`，但需要 BFS 或 DFS 遍历图。遍历过程中：遇到新节点先创建副本放入 map，再递归/迭代处理邻居。这就是 LeetCode 133（克隆图）。编织法不适用于图，因为图没有线性 next 结构可以利用。

**Q4: 如果用递归写哈希表解法呢？**
→ 递归 + 记忆化：`copy(node)` 函数先检查 map 中是否已有副本，没有就创建并递归设置 next 和 random。本质是 DFS。

## 相关题型

- **133. 克隆图** — 复用同样的"哈希表映射原→新"思路，区别：图用 BFS/DFS 遍历，链表用线性遍历
- **141/142. 环形链表 I/II** — 复用快慢指针的链表技巧，虽然问题不同，但都是链表指针操作的经典训练
- **深拷贝类题目通用模式**：任何带有"跨节点引用"的数据结构深拷贝，核心都是建立原→新的映射。链表用编织法可以省空间，图/树只能用哈希表。