# 116. 填充每个节点的下一个右侧节点指针

## 核心思路

本质上是**层序连接问题**：把同一层的节点从左到右用 `next` 指针串成一个链表。利用"完美二叉树"的特性——每个父节点都有两个子节点、所有叶子在同一层——可以实现 O(1) 空间的优雅解法。

## 思维链

1. **读完题第一反应**：这不就是层序遍历吗？BFS 逐层扫描，同一层的节点从左到右依次连接 `next`。
2. **暴力解能不能过**：BFS 用队列，时间 O(n)，空间 O(n)（队列最多存一层节点 ≈ n/2）。可以过，但 Follow-up 要求 O(1) 空间。
3. **瓶颈在哪**：空间瓶颈在队列。我们能不能不用队列就知道"下一层有哪些节点"？
4. **突破口**：如果当前层的 `next` 已经连好了，那当前层本身就是一个"链表"！我们可以**沿着当前层的 next 指针遍历**，同时**把下一层的 next 连接好**。这样就不需要队列了。
5. **具体操作**：对于节点 `node`，`node->left->next = node->right`（同一父亲的左右孩子连接），`node->right->next = node->next->left`（跨父亲的连接，利用当前层已有的 next）。
6. **为什么完美二叉树特别适合**：每个节点都有两个孩子，不需要处理缺失节点的情况，上述两个连接规则总是成立。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| BFS 层序遍历 | 队列逐层扫描，同层串联 | O(n) | O(n) | 能说出即可 |
| 递归 DFS | 利用父节点连接两个孩子，递归处理 | O(n) | O(log n) 栈 | 能说出即可 |
| 利用已建 next 逐层推进 | 当前层当链表，连下一层 | O(n) | O(1) | ⭐ 必须写出 |

## 关键提示

- **提示1**：层序遍历的标准模板可以直接用，每层内部从左到右用 `next` 连接即可。

- **提示2**：关键观察——同一父亲下，`left->next = right` 是显然的。难点是**跨父亲**的连接：`node->right` 和 `node->next->left` 之间怎么连？

- **提示3**：如果上一层的 `next` 已经连好了，当前层的"跨父亲连接"就可以通过 `node->next` 来获取右邻居的左孩子。

- **提示4**：完美二叉树的层数只有 O(log n)，所以逐层处理的外循环只跑 O(log n) 次。

- **ASCII 示意图**：
```
连接前:                       连接后:
        1                           1 → NULL
       / \                         / \
      2   3                       2 → 3 → NULL
     / \ / \                     / \ / \
    4  5 6  7                   4→5→6→7 → NULL

以第2层为例，处理节点2时:
  node = 2
  ① node->left->next = node->right    →  4->next = 5  (同父连接)
  ② node->right->next = node->next->left → 5->next = 6 (跨父连接，利用 2->next=3)
  
处理节点3时:
  node = 3
  ① node->left->next = node->right    →  6->next = 7  (同父连接)
  ② node->next == NULL，跳过           →  7->next 保持 NULL
```

## 解法详解

### 解法1: BFS 层序遍历 — O(n) / O(n)

**思考过程**：最直觉的做法。层序遍历天然按层处理节点，同一层的节点在队列中是连续的，只需要把前一个的 `next` 指向后一个。

```cpp
/*
// Definition for a Node.
class Node {
public:
    int val;
    Node* left;
    Node* right;
    Node* next;
    Node() : val(0), left(NULL), right(NULL), next(NULL) {}
    Node(int _val) : val(_val), left(NULL), right(NULL), next(NULL) {}
    Node(int _val, Node* _left, Node* _right, Node* _next)
        : val(_val), left(_left), right(_right), next(_next) {}
};
*/
class Solution {
public:
    Node* connect(Node* root) {
        if (!root) return nullptr;
        
        queue<Node*> q;
        q.push(root);
        
        while (!q.empty()) {
            int size = q.size();  // 当前层的节点数
            
            for (int i = 0; i < size; i++) {
                Node* node = q.front();
                q.pop();
                
                // 不是本层最后一个节点，就连向队列里的下一个
                if (i < size - 1) {
                    node->next = q.front();
                }
                // 最后一个节点的 next 保持 NULL（默认值）
                
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
        }
        return root;
    }
};
```

**关键点**：
- `i < size - 1` 判断是否是本层最后一个节点。如果写成 `i < size` 会把不同层的节点错误连接。
- 队列空间最坏 O(n/2) ≈ O(n)，在完美二叉树最后一层有 n/2 个节点。

---

### 解法2: 递归 DFS — O(n) / O(log n)

**思考过程**：对于每个节点，我们需要做两件事：① 连接它的左右孩子，② 连接它的右孩子和它右邻居的左孩子。递归处理每个节点即可。

```cpp
class Solution {
public:
    Node* connect(Node* root) {
        if (!root || !root->left) return root;  // 叶子节点或空树
        
        // ① 同父连接：左孩子 → 右孩子
        root->left->next = root->right;
        
        // ② 跨父连接：右孩子 → 父节点右邻居的左孩子
        if (root->next) {
            root->right->next = root->next->left;
        }
        
        // 递归处理左右子树
        connect(root->left);
        connect(root->right);
        
        return root;
    }
};
```

**关键点**：
- 终止条件是 `!root->left` 而不是 `!root`，因为完美二叉树中如果没有左孩子说明是叶子节点。当然加上 `!root` 处理空树。
- 递归顺序很重要吗？这里先处理左还是右都行，因为跨父连接只需要 `root->next` 已经连好，而 `root->next` 是在**上一层递归**中就已经设置好的。
- 栈空间 O(log n)，题目说递归栈不算额外空间。

---

### 解法3: 利用已建 next 逐层推进 — O(n) / O(1) ⭐ 面试首选

**从解法1优化**：BFS 的瓶颈是队列占 O(n) 空间。关键洞察是：**如果当前层的 next 已经全部连好，当前层就是一个链表，我们可以沿着 next 遍历它，同时把下一层的 next 连好**。这样就不需要队列了。

```
// 逐层推进的过程:
//
// 层0: 处理 [1]，连接下一层
//   1.left->next = 1.right  →  2->next = 3
//   结果: 层1 = 2 → 3 → NULL
//
// 层1: 沿着 2→3 遍历，连接下一层  
//   处理2: 2.left->next = 2.right    → 4->next = 5
//          2.right->next = 2.next.left → 5->next = 6
//   处理3: 3.left->next = 3.right    → 6->next = 7
//          3.next == NULL，跳过
//   结果: 层2 = 4 → 5 → 6 → 7 → NULL
//
// 层2: 叶子层，无子节点，结束
```

```cpp
class Solution {
public:
    Node* connect(Node* root) {
        if (!root) return nullptr;
        
        Node* leftmost = root;  // 每一层的最左节点
        
        // 当 leftmost 还有下一层时（即有左孩子，完美二叉树有左必有右）
        while (leftmost->left) {
            Node* curr = leftmost;  // 用 curr 遍历当前层的链表
            
            while (curr) {
                // ① 同父连接
                curr->left->next = curr->right;
                
                // ② 跨父连接
                if (curr->next) {
                    curr->right->next = curr->next->left;
                }
                
                // 沿着 next 移动到同层下一个节点
                curr = curr->next;
            }
            
            // 下一层的最左节点 = 当前层最左节点的左孩子
            leftmost = leftmost->left;
        }
        
        return root;
    }
};
```

**关键点**：
- **外循环**：`leftmost` 从根节点开始，每次下降一层。循环条件是 `leftmost->left` 存在（即还有下一层要处理）。
- **内循环**：`curr` 沿着已建好的 `next` 指针遍历当前层，把下一层连好。
- **为什么 O(1) 空间**：没有用队列，只用了 `leftmost` 和 `curr` 两个指针。
- **为什么能正确工作**：当处理第 k 层时，第 k 层的 next 已经在处理第 k-1 层时全部连好了，所以 `curr = curr->next` 总能正确遍历。

---

## 解法对比

| | BFS 层序 | 递归 DFS | 逐层推进（最优）|
|---|---|---|---|
| 时间 | O(n) | O(n) | O(n) |
| 空间 | O(n) 队列 | O(log n) 栈 | **O(1)** |
| 代码量 | 中等 | 最简洁 | 中等 |
| 适用范围 | 任意二叉树 | 完美二叉树 | 完美二叉树 |
| 面试推荐 | 说思路即可 | 可以先写 | **最终答案** |

**选择建议**：面试时先说 BFS 思路展示理解，然后主动提出"Follow-up 可以 O(1) 空间"，写出解法3。如果面试官问递归，给出解法2。

## 易错点

1. **跨父连接忘记判空**：
   - ✗ `curr->right->next = curr->next->left;` — 当 `curr->next == NULL` 时空指针异常
   - ✓ `if (curr->next) { curr->right->next = curr->next->left; }` — 每层最右节点没有右邻居

2. **外循环条件写错**：
   - ✗ `while (leftmost)` — 最后一层（叶子层）leftmost 不为空但没有 left，会导致访问 `leftmost->left` 后的操作出问题
   - ✓ `while (leftmost->left)` — 叶子层不需要处理，因为没有下一层

3. **递归版终止条件漏掉空树**：
   - ✗ `if (!root->left) return root;` — 当 root 为 nullptr 时就崩了
   - ✓ `if (!root || !root->left) return root;` — 先判空再访问成员

4. **BFS 中把不同层节点连在一起**：
   - ✗ 直接 `node->next = q.front()` 对每个节点都做
   - ✓ 要判断 `i < size - 1`，本层最后一个节点的 next 应该是 NULL

## 面试追问

**Q1（基础理解）**：最直觉的做法是什么？时间和空间复杂度？
> BFS 层序遍历，同一层内串联。时间 O(n)，空间 O(n)。

**Q2（优化空间）**：能否做到 O(1) 额外空间？核心思想是什么？
> 利用已建好的 next 指针把当前层当作链表遍历，同时连接下一层。不需要队列。

**Q3（变体 → LeetCode 117）**：如果不是完美二叉树，而是任意二叉树呢？
> 解法3 的两条连接规则不再直接适用（节点可能缺少左/右孩子）。需要用一个 `dummy` 头节点来帮助构建下一层的链表，遍历当前层时，把下一层的所有非空子节点依次串到 dummy 链表上。详见 117 题。

**Q4（扩展）**：递归和迭代有什么区别？递归栈空间是 O(log n) 还是 O(n)？
> 因为是完美二叉树，高度恒为 log n，所以递归栈是 O(log n)。如果是普通二叉树可能退化为 O(n)（链状树）。

## 相关题型

- **117. 填充每个节点的下一个右侧节点指针 II** — 本题是完美二叉树，117 是**任意二叉树**。解法3 的逐层推进思路可以复用，但连接逻辑需要改为用 dummy 节点构建下一层链表（因为不能假设每个节点都有左右孩子）。
- **199. 二叉树的右视图** — 同样是层序遍历的应用，每层只取最后一个节点。BFS 模板可以直接复用。
- **102. 二叉树的层序遍历** — 本题的解法1 就是 102 的模板加上 next 连接操作。
- **429. N 叉树的层序遍历** — BFS 模板的推广，从二叉扩展到 N 叉。