# 160. 相交链表

## 核心思路

本质上是在问：两条长度可能不同的链表，如何**消除长度差**，让两个指针同时到达交点（或同时到达 null）。

## 思维链

1. **读完题第一反应** → 判断两个节点是否相同，不是比较值，而是比较**指针（内存地址）**是否相同。最朴素的做法：对 A 的每个节点，遍历整个 B 看有没有相同节点。
2. **暴力解的瓶颈** → 对 A 的每个节点都要遍历 B 一遍，O(m×n)。瓶颈在"查找某个节点是否在 B 中"这一步。
3. **用哈希表优化查找** → 先把 A 的所有节点存入 HashSet，然后遍历 B，第一个在 Set 中的节点就是交点。查找从 O(n) 降到 O(1)，总体 O(m+n)，但要 O(m) 额外空间。
4. **能否 O(1) 空间？** → 关键观察：两条链表从交点开始**共享后半段**。如果 A 长度为 a+c，B 长度为 b+c（c 是公共部分），那么 a+c+b = b+c+a。让指针 A 走完后跳到 B 头部，指针 B 走完后跳到 A 头部，它们一定会在走了 a+b+c 步后同时到达交点！
5. **如果不相交呢？** → 两个指针都会在走了 a+b 步后同时到达 null，循环自然结束。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力嵌套 | A 每个节点在 B 中线性查找 | O(m×n) | O(1) | 能说出即可 |
| 哈希表 | 存 A 所有节点，遍历 B 查找 | O(m+n) | O(m) | 能说出即可 |
| 双指针（拼接法）| A→B 与 B→A 等长，同步走 | O(m+n) | O(1) | ⭐ 必须写出 |
| 差值对齐法 | 先算长度差，长的先走差值步 | O(m+n) | O(1) | 加分项 |

## 关键提示

- **提示1**：相交的判断是**指针相等**（同一个节点对象），不是**值相等**。值为 1 的两个不同节点不算相交。
- **提示2**：画图！两条链表形成的结构像一个"Y"字形：

```
A:  a1 → a2 ↘
              c1 → c2 → c3 → null
B:  b1 → b2 → b3 ↗
```

- **提示3**：两条链表长度不同的"本质困难"是什么？指针不能同时到达交点。如果能让两个指针走的**总长度一样**，问题就解决了。
- **提示4**：`a + c + b = b + c + a`，这就是双指针法的数学基础。
- **提示5**：不相交的情况下，两个指针最终都走了 `a + b` 步到达 null，`null == null` 返回 null，无需特殊处理。

## 解法详解

### 解法1: 暴力嵌套 — O(m×n) / O(1)

**思考过程**: 最直觉的做法——对于 A 中的每一个节点，检查它是否出现在 B 中。

```cpp
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        // 对 A 的每个节点，遍历 B 查找是否为同一节点
        for (ListNode* a = headA; a != nullptr; a = a->next) {
            for (ListNode* b = headB; b != nullptr; b = b->next) {
                if (a == b) return a;  // 指针相同 = 同一个节点
            }
        }
        return nullptr;
    }
};
```

**关键点**: 这里比较的是 `a == b`（指针地址），不是 `a->val == b->val`（值）。

---

### 解法2: 哈希表 — O(m+n) / O(m)

**从解法1优化**: 暴力解的瓶颈是"对 A 的每个节点，都要线性扫描 B"。用哈希表把 B 的节点存起来，查找就从 O(n) 变成 O(1)。

```cpp
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        // 先把 A 的所有节点指针存入 set
        unordered_set<ListNode*> visited;
        for (ListNode* a = headA; a != nullptr; a = a->next) {
            visited.insert(a);
        }
        // 遍历 B，第一个在 set 中出现的就是交点
        for (ListNode* b = headB; b != nullptr; b = b->next) {
            if (visited.count(b)) return b;
        }
        return nullptr;
    }
};
```

**关键点**: 存的是节点指针（地址），不是节点值。

---

### 解法3: 双指针拼接法 — O(m+n) / O(1) ⭐ 面试首选

**从解法2优化**: 哈希表需要 O(m) 空间。能否 O(1) 空间？

**核心观察**: 两条链表长度不同导致指针无法同时到达交点。但如果让：
- 指针 pA 走完 A 之后接着走 B
- 指针 pB 走完 B 之后接着走 A

那么两个指针各走了 `a + c + b` 和 `b + c + a` 步，**总步数相同**，必然同时到达交点！

```
// 示意图 (A 长度 = a+c, B 长度 = b+c):
//
// pA 的路径:  a1 → a2 → [c1 → c2 → c3] → b1 → b2 → b3 → [c1] ← 在这里相遇
//                         ^公共部分^                          ^交点^
//
// pB 的路径:  b1 → b2 → b3 → [c1 → c2 → c3] → a1 → a2 → [c1] ← 在这里相遇
//                              ^公共部分^                    ^交点^
//
// pA 走的总步数 = a + c + b
// pB 走的总步数 = b + c + a
// 相等! 所以它们同时到达 c1
//
// 如果不相交 (c=0):
// pA: a1 → a2 → ... → null → b1 → b2 → ... → null  (共 a+b 步到 null)
// pB: b1 → b2 → ... → null → a1 → a2 → ... → null  (共 b+a 步到 null)
// 同时到达 null，返回 null
```

```cpp
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        ListNode* pA = headA;
        ListNode* pB = headB;
        
        // 当 pA == pB 时停止：要么同时到达交点，要么同时到达 null
        while (pA != pB) {
            // 走到末尾就跳到另一条链表的头部
            pA = (pA != nullptr) ? pA->next : headB;
            pB = (pB != nullptr) ? pB->next : headA;
        }
        
        return pA;  // 交点 or null
    }
};
```

**关键点**: 
- 判断的是 `pA != nullptr`，不是 `pA->next != nullptr`。必须让指针走到 null 之后才跳转，否则会跳过尾节点。
- 这个算法最多走 2(m+n) 步，不会死循环——即使不相交，两个指针也会在各走 m+n 步后同时到达 null。

---

### 解法4: 差值对齐法 — O(m+n) / O(1)

**另一个视角**: 与其拼接链表，不如直接计算长度差，让长的链表先走几步"对齐"。

```
// 示意图:
// A:  a1 → a2 → c1 → c2 → c3       (长度 5)
// B:  b1 → b2 → b3 → c1 → c2 → c3  (长度 6)
//                     ^交点^
//
// B 比 A 长 1，让 B 先走 1 步:
// A:  a1 → a2 → c1 → c2 → c3
//     ↑
// B:       b2 → b3 → c1 → c2 → c3   (先走了1步)
//          ↑
// 现在两个指针同时走，就会在 c1 相遇
```

```cpp
class Solution {
public:
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        // 第一步：计算两条链表的长度
        int lenA = 0, lenB = 0;
        for (ListNode* p = headA; p; p = p->next) lenA++;
        for (ListNode* p = headB; p; p = p->next) lenB++;
        
        // 第二步：长的链表先走 |lenA - lenB| 步
        ListNode* pA = headA;
        ListNode* pB = headB;
        while (lenA > lenB) { pA = pA->next; lenA--; }
        while (lenB > lenA) { pB = pB->next; lenB--; }
        
        // 第三步：同时走，直到相遇或同时到 null
        while (pA != pB) {
            pA = pA->next;
            pB = pB->next;
        }
        
        return pA;
    }
};
```

**关键点**: 这个解法更直觉，分三步清晰明了，面试时容易讲清楚。

## 解法对比

| | 暴力 | 哈希表 | 双指针拼接 | 差值对齐 |
|---|---|---|---|---|
| 时间 | O(m×n) | O(m+n) | O(m+n) | O(m+n) |
| 空间 | O(1) | O(m) | O(1) | O(1) |
| 代码量 | 最少 | 简单 | 最简洁 | 稍长 |
| 理解难度 | 简单 | 简单 | 需要理解数学 | 直觉 |
| 面试推荐 | ✗ | 作为过渡 | ⭐首选 | 也很好 |

**双指针拼接 vs 差值对齐**: 本质上是同一个思想——消除长度差。拼接法更优雅（代码更短），差值对齐法更直觉（容易向面试官解释）。两种都能拿满分。

## 易错点

1. **比较值而不是指针**
   - ✗ `if (a->val == b->val) return a;`
   - ✓ `if (a == b) return a;`
   - 两个不同节点可以有相同的值，相交判定必须是同一个节点对象

2. **双指针法跳转时机搞错**
   - ✗ `pA = (pA->next != nullptr) ? pA->next : headB;` — 永远跳不到 null，不相交时死循环
   - ✓ `pA = (pA != nullptr) ? pA->next : headB;` — 走到 null 再跳转
   - 原因：必须让指针经历 null 这一步，这样不相交时两指针才能同时为 null 退出循环

3. **差值对齐法算错长度差**
   - ✗ 用 `lenA - lenB` 可能得到负数（unsigned 溢出更危险）
   - ✓ 用两个 while 分别处理 `lenA > lenB` 和 `lenB > lenA`

## 面试追问

**Q1: 暴力法能做吗？时间复杂度是多少？**
A: 可以，对 A 的每个节点遍历 B，O(m×n) 时间，O(1) 空间。

**Q2: 能优化到 O(m+n) 吗？用什么数据结构？**
A: 哈希表。把 A 所有节点存入 set，遍历 B 查找。O(m+n) 时间，O(m) 空间。

**Q3: Follow-up — 能否 O(1) 空间 + O(m+n) 时间？**
A: 双指针法。核心思想：让两个指针走的总路程相同，从而同时到达交点。pA 走完 A 接着走 B，pB 走完 B 接着走 A，总步数都是 a+b+c。

**Q4: 如果链表可能有环呢？（进阶变体）**
A: 先用快慢指针检测环。如果一个有环一个无环，不可能相交。如果都有环，找到各自的入环点，分情况讨论（同一入环点 vs 不同入环点）。

**Q5: 为什么双指针法不会死循环？**
A: 不相交时，两指针各走 m+n 步后同时到达 null（`null == null` 退出）。相交时，各走 a+b+c 步后同时到达交点。最多 2(m+n) 步一定结束。

## 相关题型

- **141. 环形链表** — 复用快慢指针思想，但目的是检测环而不是找交点。双指针法同样是 O(1) 空间。
- **142. 环形链表 II** — 找环的入口点，和本题一样需要巧妙消除"距离差"，用数学推导证明指针相遇位置。
- **21. 合并两个有序链表** — 同样是双指针遍历两条链表，但操作不同（合并 vs 比较）。
- **2. 两数相加** — 同时遍历两条链表，需要处理长度不同的对齐问题。