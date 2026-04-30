# 776. 拆分二叉搜索树 (Split BST)

**难度**: Medium | **标签**: Tree, Binary Search Tree, Recursion, Binary Tree

## 核心思路

利用 BST 的性质进行**递归拆分**。对于当前节点 root：
- 若 `root->val <= target`，则 root 及其左子树都属于"小等于"组。但右子树中可能有部分节点 <= target，需要递归拆分右子树。
- 若 `root->val > target`，则 root 及其右子树都属于"大于"组。但左子树中可能有部分节点 > target，需要递归拆分左子树。

每次递归返回两棵子树 `[small, large]`，然后将递归结果"嫁接"到当前节点的对应位置。

## 思维链

1. **BST 性质利用**: 当前节点 val 与 target 的关系决定了当前节点归属哪棵子树。
2. **递归子问题**: 只需递归处理"可能跨界"的那一侧子树。
3. **嫁接操作**: 递归返回的两部分，一部分接到当前节点上，另一部分作为对面子树的根。
4. **base case**: 空节点返回 `[null, null]`。

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 适用场景 |
|------|-----------|-----------|---------|
| 递归拆分 | O(h) | O(h) | 标准最优解 |
| 迭代拆分 | O(h) | O(1) | 避免递归栈 |

> h = 树的高度，最坏 O(n)

## 关键提示

1. **两种情况清晰分类**: `root->val <= target` 时 root 归入左树(small)；`root->val > target` 时 root 归入右树(large)。
2. **递归只处理一侧**: 不需要两侧都递归。val <= target 只递归右子树，val > target 只递归左子树。
3. **嫁接是关键**: 递归返回 `[small, large]` 后，需要将其中一部分接到当前节点的子指针上。
4. **返回值约定**: `result[0]` 是所有 <= target 的节点构成的树，`result[1]` 是所有 > target 的节点构成的树。
5. **不需要创建新节点**: 只是修改指针（left/right），复用原有节点。

## 解法详解

### 解法一：递归拆分 (标准解法)

**思路**:
递归函数 `splitBST(root, target)` 返回 `[small, large]`：

**Case 1: root == nullptr**
- 返回 `[nullptr, nullptr]`。

**Case 2: root->val <= target**
- root 和其左子树全部 <= target，归入 small。
- 右子树中可能有 <= target 的节点，递归拆分：`[rightSmall, rightLarge] = splitBST(root->right, target)`。
- 将 root->right 指向 rightSmall（右子树中 <= target 的部分仍跟着 root）。
- 返回 `[root, rightLarge]`。

**Case 3: root->val > target**
- root 和其右子树全部 > target，归入 large。
- 左子树中可能有 > target 的节点，递归拆分：`[leftSmall, leftLarge] = splitBST(root->left, target)`。
- 将 root->left 指向 leftLarge（左子树中 > target 的部分仍跟着 root）。
- 返回 `[leftSmall, root]`。

**图解** (root->val <= target 的情况):
```
       root (<=T)                root (<=T)
      /     \          =>       /     \
   left    right             left   rightSmall
           / \
     rightSmall rightLarge   rightLarge (独立子树)
```

**复杂度**:
- 时间: O(h)。每次递归沿树的一条路径走一步，最多走到叶子。
- 空间: O(h)。递归调用栈深度。

### 解法二：迭代拆分

**思路**: 用两个指针分别追踪 small 树和 large 树的"接口位置"，沿 BST 的路径迭代。

在每一步中：
- 若当前节点 <= target，将其接入 small 树，并沿右子树继续。
- 若当前节点 > target，将其接入 large 树，并沿左子树继续。

需要维护两个"接口指针"来记录下一次嫁接的位置。实现较复杂，面试中不太推荐。

**复杂度**:
- 时间: O(h)。
- 空间: O(1)。无递归栈开销。

## 易错点

1. **指针修改不完整**: 忘记将 `root->right = rightSmall` 或 `root->left = leftLarge`，导致原有子树结构被保留，结果错误。
2. **返回值顺序搞混**: `result[0]` 是 <= target 的树，`result[1]` 是 > target 的树。搞混会导致错误。
3. **判断条件 <= vs <**: 题目要求 <= target 归入第一棵树，> target 归入第二棵树。注意等号的归属。
4. **忘记处理空节点**: base case `root == nullptr` 必须返回 `{nullptr, nullptr}`。
5. **以为需要深拷贝**: 不需要。直接修改原节点指针即可，不创建新节点。

## 面试追问

1. **Q: 能否不修改原树的结构？**
   A: 可以，需要深拷贝节点。每次需要修改指针时，先复制节点再修改副本。时间空间都变为 O(n)。

2. **Q: 如果要拆成三棵树（<target, ==target, >target）怎么办？**
   A: 先 splitBST(root, target) 得到 [<=T, >T]，再对 <=T 子树 splitBST(root, target-1) 得到 [<T, ==T]。两次拆分即可。

3. **Q: 这道题和二叉搜索树的删除操作有什么关系？**
   A: 删除操作本质上也是在 BST 中"拆"和"接"子树。Split 可以看作一种广义的 BST 结构变换。在 Treap/Splay 等平衡树中，split 和 merge 是基本操作。

4. **Q: 时间复杂度最坏为什么是 O(n)？**
   A: 当 BST 退化为链表时，高度 h = n，递归需要 O(n) 步。平衡 BST 则为 O(log n)。

5. **Q: 如何验证拆分结果的正确性？**
   A: 中序遍历两棵结果树，验证：(1) 第一棵所有值 <= target，(2) 第二棵所有值 > target，(3) 两者合并后的中序遍历等于原树的中序遍历。

## 相关题型

- [450. Delete Node in a BST](../0450_delete_node_in_a_bst/) - BST 删除节点（需要拆接子树）
- [669. Trim a Binary Search Tree](../0669_trim_a_binary_search_tree/) - BST 裁剪（类似拆分思想）
- [1008. Construct BST from Preorder](../1008_construct_binary_search_tree_from_preorder_traversal/) - BST 构建
- [98. Validate Binary Search Tree](../0098_validate_binary_search_tree/) - BST 验证
- [235. Lowest Common Ancestor of BST](../0235_lowest_common_ancestor_of_a_binary_search_tree/) - BST 的 LCA
