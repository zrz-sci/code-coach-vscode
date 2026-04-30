# 897. 递增顺序搜索树

## 核心思路

本质上就是一个**中序遍历 + 重建链式树**的问题：对 BST 做中序遍历得到递增序列，然后把每个节点串成一条只有右子节点的链。关键在于如何在遍历过程中"边走边接"，而不是先收集再重建。

## 思维链

1. **读完题第一反应** → BST 的中序遍历天然就是递增的，这是 BST 的核心性质。如果把中序遍历的结果存下来，然后按顺序建一条只有 right 指针的链，就完事了。
2. **暴力解的瓶颈在哪？** → 其实没有真正的"瓶颈"，因为 O(n) 就能做到。但"先收集后重建"这种两遍扫描的做法不够优雅，面试中更希望看到一遍搞定的写法。
3. **怎么优雅一点？** → 在中序遍历的过程中，维护一个"当前链尾"指针 `cur`，每访问一个节点就把它接到 `cur->right` 上，同时清空该节点的 `left`。这样一遍 DFS 就同时完成了遍历和重建。
4. **dummy 节点的作用** → 和链表题一样，用一个 dummy 头节点避免对"第一个节点"做特殊判断。最终返回 `dummy->right`。
5. **还有其他思路吗？** → 可以用栈做迭代中序遍历，逻辑一样但更适合不喜欢递归的人。Morris 遍历可以做到 O(1) 空间（不算输出），但面试中通常不要求。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 中序遍历收集 + 重建 | 先 DFS 收集节点到数组，再逐个串起来 | O(n) | O(n) | 能说出即可 |
| 中序遍历边走边接（递归） | DFS 过程中用 cur 指针直接接链 | O(n) | O(h) | ⭐ 面试首选 |
| 中序遍历边走边接（迭代栈） | 用显式栈模拟中序遍历，同时接链 | O(n) | O(h) | 等价写法 |

## 关键提示

1. **BST 中序遍历 = 递增序列**：这是 BST 的核心性质。任何 BST 问题都应该先想到中序遍历。
2. **每个节点的 left 必须清空**：如果你只设置了 `cur->right = node` 而忘记 `node->left = nullptr`，原始的 left 指针还在，会形成环或者多余子树，导致输出错误。
3. **dummy 节点简化代码**：创建一个 dummy 作为链头的前驱，`cur` 从 dummy 开始，最终返回 `dummy->right`。这和链表题里的 dummy head 技巧完全一样。
4. **递归栈空间**：递归 DFS 的空间复杂度是 O(h)，其中 h 是树高。最坏情况（退化链表）h = n，平衡树时 h = log n。面试要能说清楚。
5. **不要在递归中返回新树**：这道题的一个陷阱是试图用返回值传递重建后的子树，这会让逻辑变得非常复杂。直接用一个类成员变量 `cur` 做"当前链尾"最简洁。

## 解法详解

### 解法1: 中序遍历收集 + 重建 — O(n) / O(n)

**思考过程**: 最直觉的做法——先把所有节点按中序顺序收集到数组里，然后遍历数组把它们串成链。

```
示例: root = [5, 3, 6, 2, 4, null, 8, 1, null, null, null, 7, 9]

中序遍历收集: [1, 2, 3, 4, 5, 6, 7, 8, 9]

重建过程:
  1 → 2 → 3 → 4 → 5 → 6 → 7 → 8 → 9
  每个节点: left = null, right = 下一个节点
```

```cpp
class Solution {
public:
    TreeNode* increasingBST(TreeNode* root) {
        // 第一步：中序遍历收集所有节点
        vector<TreeNode*> nodes;
        inorder(root, nodes);
        
        // 第二步：串成只有 right 指针的链
        TreeNode dummy(0);
        TreeNode* cur = &dummy;
        for (TreeNode* node : nodes) {
            node->left = nullptr;   // 清空左子节点！
            cur->right = node;
            cur = node;
        }
        return dummy.right;
    }
    
    void inorder(TreeNode* node, vector<TreeNode*>& nodes) {
        if (!node) return;
        inorder(node->left, nodes);
        nodes.push_back(node);
        inorder(node->right, nodes);
    }
};
```

**关键点**:
- 两遍扫描：第一遍收集，第二遍重建。虽然总共是 O(n)，但需要额外 O(n) 存储所有节点指针。
- 千万不要忘记 `node->left = nullptr`，否则会保留原来的左子树指针。

---

### 解法2: 中序遍历边走边接（递归） — O(n) / O(h) ⭐ 面试首选

**从解法1优化**: 解法1的两遍扫描其实可以合并为一遍——在中序遍历的"访问"阶段直接做重建操作，不需要额外数组。

```
DFS 过程 (root = [5, 3, 6, 2, 4, null, 8, 1]):

                5
               / \
              3   6
             / \   \
            2   4   8
           /       /
          1       7

中序遍历: 先左 → 访问 → 再右
到达节点1时: cur->right = 1, 1->left = null, cur = 1
到达节点2时: cur->right = 2, 2->left = null, cur = 2
...依次类推...

最终链: dummy → 1 → 2 → 3 → 4 → 5 → 6 → 7 → 8 → 9
```

```cpp
class Solution {
public:
    TreeNode* cur;
    
    TreeNode* increasingBST(TreeNode* root) {
        TreeNode dummy(0);
        cur = &dummy;
        inorder(root);
        return dummy.right;
    }
    
    void inorder(TreeNode* node) {
        if (!node) return;
        inorder(node->left);
        
        // "访问"阶段：把当前节点接到链尾
        node->left = nullptr;    // 断开左子树
        cur->right = node;       // 接到链尾
        cur = node;              // 移动链尾指针
        
        inorder(node->right);
    }
};
```

**关键点**:
- `cur` 用类成员变量而非局部变量，因为递归过程中需要在不同层级共享这个状态。
- **必须先断 left 再接 right**：`node->left = nullptr` 要在 `inorder(node->right)` 之前执行，否则后续遍历不受影响但输出树会有多余的 left 指针。
- 空间仅 O(h)（递归栈），比解法1的 O(n) 更优。

---

### 解法3: 迭代栈中序遍历 — O(n) / O(h)

**不同视角**: 如果你不想用递归（比如担心栈溢出），可以用显式栈模拟中序遍历，边遍历边接链。

```cpp
class Solution {
public:
    TreeNode* increasingBST(TreeNode* root) {
        TreeNode dummy(0);
        TreeNode* cur = &dummy;
        
        stack<TreeNode*> stk;
        TreeNode* node = root;
        
        while (node || !stk.empty()) {
            // 一路向左，全部压栈
            while (node) {
                stk.push(node);
                node = node->left;
            }
            // 弹出栈顶 = 当前最小未处理节点
            node = stk.top();
            stk.pop();
            
            // 接到链尾
            node->left = nullptr;
            cur->right = node;
            cur = node;
            
            // 转向右子树
            node = node->right;
        }
        
        return dummy.right;
    }
};
```

**关键点**:
- 迭代中序遍历的模板：先一路向左压栈，弹出处理，然后转向右子树。
- 空间同样是 O(h)，但用显式栈代替了递归栈，在极端深度的树上不会栈溢出。

## 解法对比

|  | 收集+重建 | 递归边走边接 | 迭代栈 |
|--|----------|-------------|--------|
| 时间 | O(n) | O(n) | O(n) |
| 空间 | O(n) | **O(h)** | **O(h)** |
| 代码量 | 中等 | **最少** | 较多 |
| 适用场景 | 理解题意 | **面试首选** | 深树/不想递归 |

**面试选择**: 直接写递归版的"边走边接"。代码最短、逻辑最清晰。迭代版作为 follow-up 准备。

## 易错点

1. **忘记清空 left 指针**：
   - ✗ 只做 `cur->right = node` 而不做 `node->left = nullptr` → 原始左子树指针保留，输出有多余结构
   - ✓ 每次接链时同时 `node->left = nullptr`

2. **cur 指针初始化错误**：
   - ✗ `cur = root` → 第一个节点（中序第一个）没地方接
   - ✓ 创建 dummy 节点，`cur = &dummy`，最终返回 `dummy->right`

3. **试图用返回值传递重建结果**：
   - ✗ 让递归函数返回重建后的子树头 → 逻辑混乱，因为中序遍历中"左子树的最后一个节点"才是链尾
   - ✓ 用一个共享的 cur 指针，在"访问"阶段操作

4. **迭代版中忘记 `node = node->right`**：
   - ✗ 弹出栈顶后直接继续外层 while → 死循环或遗漏右子树
   - ✓ 处理完当前节点后 `node = node->right`，让下一轮循环去处理右子树

## 面试追问

**Q1: 为什么不直接把节点值收集到数组再建新树？**
> 可以，但题目要求的是**原地**重排原始树的指针，不是建新树。面试中原地操作更能展示你对指针的掌控力。当然，如果面试官允许，收集值再建新节点也可以，但空间开销更大。

**Q2: 这道题能不能用 Morris 遍历做到 O(1) 空间？**
> 理论上可以，但 Morris 遍历本身就是通过临时修改树的 left/right 指针来实现的，和本题的目标（修改指针结构）会冲突，实现起来非常容易出错。面试中不推荐，除非面试官特别要求 O(1) 空间。

**Q3: 如果输入不是 BST 而是普通二叉树，要求按前序/后序展开呢？**
> 同样的思路：对应遍历顺序 + dummy + cur 指针。前序展开就是 LeetCode 114 (Flatten Binary Tree to Linked List)。区别在于前序展开时需要先保存 right 子节点（因为你会覆盖它），而中序不需要（左边先处理完了才到当前节点）。

**Q4: 时间空间的最优解是什么？**
> 时间必须 O(n)（至少要访问每个节点一次）。空间上递归/迭代都是 O(h)，完全平衡时 O(log n)，退化时 O(n)。Morris 理论上 O(1) 但如前所述实现复杂。所以递归 O(h) 就是实践中的最优。

## 相关题型

- **94. 二叉树的中序遍历** — 本题的基础。先掌握标准中序遍历（递归和迭代），再来做这道"中序遍历的应用"。
- **114. 二叉树展开为链表** — 同类型但是前序遍历展开。区别：前序展开需要先保存右子节点，否则会被覆盖。而中序展开不需要。
- **426. 将二叉搜索树转化为排序的双向链表** — 进阶版：不仅要 right 指针，还要维护 left 指针作为"前驱"。核心思路相同，但需要额外处理首尾相连（循环链表）。
- **109. 有序链表转换二叉搜索树** — 本题的"逆过程"：给你一个升序链表，建出平衡 BST。用快慢指针找中点然后递归构建。
