# 1650. 二叉树的最近公共祖先 III

## 核心思路

这道题本质上是：**两个单链表求交点**。每个节点有 `parent` 指针，所以从 p 和 q 沿着 parent 往上走，就像两条链表，LCA 就是它们的交点。

## 思维链

1. **读完题第一反应**：每个节点有 parent 指针！这和经典 LCA（236题）不同，236题只给了 root，需要从上往下递归。这里可以从节点**往上**走到 root，形成两条"到根的路径"。
2. **最朴素的做法**：把 p 到 root 的路径上所有节点存到集合里，然后让 q 往上走，第一个在集合中出现的节点就是 LCA。这需要 O(h) 空间。
3. **空间能否优化？**：两条路径最终都汇聚到 root，像两条链表最终汇合——这不就是 **"两个链表的交点"** 问题吗！（LeetCode 160）
4. **LeetCode 160 的经典技巧**：两个指针分别从两个起点走，走到尾部后跳到另一条链的头部继续走，最终会在交点相遇。原因：两个指针走的总路程相同（路径A + 路径B），所以一定同时到达交点。
5. **套用到本题**：指针 a 从 p 开始沿 parent 上走，到 root 后跳到 q 继续上走；指针 b 从 q 开始沿 parent 上走，到 root 后跳到 p 继续上走。两指针相遇处就是 LCA。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 哈希集合 | 存 p 的祖先路径，q 往上找第一个匹配 | O(h) | O(h) | 能说出即可 |
| 双指针（链表交点） | 两指针交替走两条路径，相遇即 LCA | O(h) | O(1) | ⭐ 必须写出 |
| 先求深度再对齐 | 先让深的节点走到同一层，再同步上走 | O(h) | O(1) | 加分项 |

> h = 树的高度，最坏 O(n)

## 关键提示

- **提示1**：从 p 沿 parent 走到 root 是一条路径，从 q 沿 parent 走到 root 也是一条路径。这两条路径有什么几何关系？

```
//       3          p=5 到根的路径: 5 → 3
//      / \         q=4 到根的路径: 4 → 2 → 5 → 3
//     5   1                        ↑交点=LCA=5
//    / \
//   6   2
//      / \
//     7   4
```

- **提示2**：把每条"到根路径"看作一个单链表，LCA 就是两条链表的交点。你做过 LeetCode 160 吗？
- **提示3**：两条链表求交点的 O(1) 空间做法：指针走完自己的链后跳到对方链的头，最终会在交点相遇。
- **提示4**：如果不记得 160 的技巧，还有另一种 O(1) 空间做法——先算出两条路径的长度差，让长的先走差值步，然后同步走。

## 解法详解

### 解法1: 哈希集合 — O(h) / O(h)

**思考过程**：最直觉的做法——把 p 的所有祖先记下来，然后让 q 往上找，第一个在集合里出现的就是 LCA。

```cpp
class Solution {
public:
    Node* lowestCommonAncestor(Node* p, Node* q) {
        unordered_set<Node*> ancestors;
        // 把 p 到 root 的整条路径存入集合
        Node* curr = p;
        while (curr) {
            ancestors.insert(curr);
            curr = curr->parent;
        }
        // q 往上走，第一个在集合中的节点就是 LCA
        curr = q;
        while (curr) {
            if (ancestors.count(curr)) return curr;
            curr = curr->parent;
        }
        return nullptr; // 题目保证 p、q 都在树中，不会走到这里
    }
};
```

**关键点**：这个解法简单直接，但使用了 O(h) 额外空间。面试中说完这个后应该主动提出优化。

---

### 解法2: 双指针（链表交点法） — O(h) / O(1) ⭐ 面试首选

**从解法1优化**：解法1的瓶颈是 O(h) 空间用于存储祖先集合。观察到从任何节点沿 parent 走到 root 形成的路径就像一条单链表，LCA 就是两条链表的交点——直接套用 LeetCode 160 的双指针技巧！

**为什么双指针能 work？**

```
// p=5 的路径: 5 → 3 → null          (长度 = 2)
// q=4 的路径: 4 → 2 → 5 → 3 → null  (长度 = 4)
//
// 指针a从p出发，指针b从q出发:
//   a: 5 → 3 → null → 跳到q → 4 → 2 → [5] ← 相遇!
//   b: 4 → 2 → 5 → 3 → null → 跳到p → [5] ← 相遇!
//
// a 走了: 2(p路径) + 4(q路径前半) = 到达节点5时走了 2+3=5 步
// b 走了: 4(q路径) + 2(p路径前半) = 到达节点5时走了 4+1=5 步
//
// 关键原理: a走"路径p+路径q"，b走"路径q+路径p"
// 总路程相同，所以一定在交点（LCA）处相遇!
```

```cpp
class Solution {
public:
    Node* lowestCommonAncestor(Node* p, Node* q) {
        Node* a = p;
        Node* b = q;
        // 两指针同步走，走到 null 后跳到对方起点
        // 最终在 LCA 处相遇（或同时到达 null 表示无交点，但题目保证有）
        while (a != b) {
            a = (a != nullptr) ? a->parent : q;  // a 走完 p 的路径后跳到 q
            b = (b != nullptr) ? b->parent : p;  // b 走完 q 的路径后跳到 p
        }
        return a;
    }
};
```

**关键点**：
- 当 `a` 走到 `nullptr`（即 root 的 parent）时，跳到 `q` 重新开始走
- 当 `b` 走到 `nullptr` 时，跳到 `p` 重新开始走
- 两者走的总步数相等：`depth(p) + depth(q)`，所以一定在 LCA 相遇

---

### 解法3: 先求深度再对齐 — O(h) / O(1)

**思考过程**：另一种 O(1) 空间的思路——先分别求出 p 和 q 的深度，让深的先走几步追平，然后两指针同步上走，相遇点就是 LCA。这和链表题"先量长度再对齐"的思路一致。

```cpp
class Solution {
public:
    Node* lowestCommonAncestor(Node* p, Node* q) {
        // 第一步：求 p 和 q 的深度
        int depthP = getDepth(p);
        int depthQ = getDepth(q);
        
        // 第二步：让深的节点先往上走，对齐到同一层
        while (depthP > depthQ) {
            p = p->parent;
            depthP--;
        }
        while (depthQ > depthP) {
            q = q->parent;
            depthQ--;
        }
        
        // 第三步：同步上走，相遇即 LCA
        while (p != q) {
            p = p->parent;
            q = q->parent;
        }
        return p;
    }
    
private:
    int getDepth(Node* node) {
        int depth = 0;
        while (node) {
            node = node->parent;
            depth++;
        }
        return depth;
    }
};
```

**关键点**：这个解法逻辑更直观——"先对齐，再同步走"，但代码稍长。解法2的双指针技巧本质上是把"对齐"和"同步走"合并成一个循环。

## 解法对比

| | 解法1 哈希集合 | 解法2 双指针 | 解法3 深度对齐 |
|---|---|---|---|
| 时间 | O(h) | O(h) | O(h)（常数稍大，走了3遍） |
| 空间 | O(h) | **O(1)** | **O(1)** |
| 代码量 | 最简单 | 最短 | 中等 |
| 思维难度 | 低 | 需要知道链表交点技巧 | 中等 |
| 面试推荐 | 先说这个展示思路 | **首选** | 作为替代方案 |

**关键区别**：解法2和解法3都是 O(1) 空间，但解法2只需一个 while 循环，代码更优雅。解法3逻辑更清晰易解释。

## 易错点

1. **双指针跳转时机错误**：
   - ✗ `a = (a->parent != nullptr) ? a->parent : q;` — 这样 a 永远不会跳到 q，因为到 root 时 root->parent == nullptr 但 a 本身不是 nullptr
   - ✓ `a = (a != nullptr) ? a->parent : q;` — 先检查 a 本身是否为 nullptr（即已经走过了 root）

2. **跳转目标搞反**：
   - ✗ a 走完跳到 p（又回自己的路径了）
   - ✓ a 从 p 出发走完后跳到 **q**，b 从 q 出发走完后跳到 **p**

3. **解法3 中忘记更新深度**：
   - ✗ `while (depthP > depthQ) { p = p->parent; }` — 死循环！depthP 永远大于 depthQ
   - ✓ `while (depthP > depthQ) { p = p->parent; depthP--; }`

## 面试追问

**Q1：这道题和 LeetCode 236（没有 parent 指针的 LCA）有什么区别？**
> 236 只给了 root，需要从上往下 DFS 递归查找 p 和 q 的位置。本题每个节点有 parent 指针，可以从下往上走，本质变成了"两条链表求交点"。

**Q2：如果 p 和 q 可能不在树中怎么办？**
> 这就变成了 LeetCode 1644 的变体。用哈希集合法可以轻松处理：p 走到 root 存完集合后，q 如果走到 root 都没找到匹配，说明 q 不在树中。双指针法则需要额外标记两个指针是否都完成了"跳转"，如果同时到 nullptr 说明无交点。

**Q3：如果需要频繁查询不同的 (p, q) 对，怎么优化？**
> 预处理：可以用 Euler Tour + Sparse Table 做到 O(n) 预处理、O(1) 查询。或者用 Binary Lifting（倍增法），O(n log n) 预处理、O(log n) 查询。本题的单次查询场景下，双指针 O(h) 已经是最优。

## 相关题型

- **LeetCode 160 相交链表** — 本题的双指针解法**完全复用**160的技巧，区别只是"链表的 next"变成了"树的 parent"
- **LeetCode 236 二叉树的最近公共祖先** — 没有 parent 指针版本，必须用自顶向下递归，复用"后序遍历收集信息"的框架
- **LeetCode 1644 二叉树的最近公共祖先 II** — 变体：p 或 q 可能不在树中，需要额外验证节点存在性
- **LeetCode 235 BST 的最近公共祖先** — BST 可以利用有序性：比两者都大往左走，比两者都小往右走