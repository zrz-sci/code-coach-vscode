# 285. Inorder Successor in BST (二叉搜索树中的中序后继)

**难度**: Medium | **标签**: Tree, DFS, Binary Search Tree, Binary Tree

## 核心思路

本质是**利用 BST 性质进行二分搜索**。中序后继就是"大于 p 的最小节点"。从根开始，如果当前节点值大于 p，则它可能是后继（记录），然后向左子树寻找更小的候选；如果当前节点值小于等于 p，则向右子树寻找。

## 思维链

1. 中序遍历 BST 得到升序序列，后继就是 p 的下一个
2. 暴力：中序遍历找到 p，返回下一个 -> O(n)
3. 优化：利用 BST 的有序性质，像二分查找一样
4. 如果 `root->val > p->val`：root 可能是后继，但左子树可能有更小的 -> 记录 root，往左走
5. 如果 `root->val <= p->val`：root 不可能是后继 -> 往右走
6. 特殊情况：如果 p 有右子树，后继是右子树的最左节点

```
BST 示例:
          5
         / \
        3   6
       / \
      2   4
     /
    1

找 p=3 的中序后继:

root=5: 5 > 3 -> 可能是后继, 记录 ans=5, 向左
root=3: 3 <= 3 -> 不是后继, 向右
root=4: 4 > 3 -> 更好的候选, 记录 ans=4, 向左
root=null -> 返回 ans=4

中序序列: 1,2,3,[4],5,6 -> 确认 4 是 3 的后继
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 解法1: 迭代二分搜索 | 利用BST性质从根搜索 | O(h) | O(1) | ⭐⭐⭐ |
| 解法2: 中序遍历 | 遍历找p的下一个 | O(n) | O(h) | ⭐ |

## 关键提示

```
BST 中序后继的两种情况:

情况1: p 有右子树 -> 后继是右子树的最左节点
          5
         / \
       [3]  6      p=3, 右子树存在
       / \         -> 右子树 {4} 的最左 = 4
      2   4

情况2: p 没有右子树 -> 后继是最近的"左转祖先"
          5
         / \
        3   6      p=4, 无右子树
       / \         -> 沿路径向上, 第一个 > 4 的祖先 = 5
      2  [4]

迭代法统一处理: 从根向下搜索，每次 val > p.val 时记录候选
```

## 解法详解

### 解法1: 迭代二分搜索 (推荐)

```cpp
class Solution {
public:
    TreeNode* inorderSuccessor(TreeNode* root, TreeNode* p) {
        TreeNode* successor = nullptr;
        while (root) {
            if (root->val > p->val) {
                // 当前节点可能是后继，记录并向左找更小的
                successor = root;
                root = root->left;
            } else {
                // 当前节点 <= p，不可能是后继，向右走
                root = root->right;
            }
        }
        return successor;
    }
};
```

### 解法2: 中序遍历

```cpp
class Solution {
public:
    TreeNode* inorderSuccessor(TreeNode* root, TreeNode* p) {
        stack<TreeNode*> stk;
        TreeNode* cur = root;
        bool foundP = false;

        while (cur || !stk.empty()) {
            while (cur) {
                stk.push(cur);
                cur = cur->left;
            }
            cur = stk.top(); stk.pop();
            if (foundP) return cur;  // p 的下一个就是后继
            if (cur == p) foundP = true;
            cur = cur->right;
        }
        return nullptr;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|----------|----------|------|
| ✗ `root->val >= p->val` 时向左 | ✓ `root->val > p->val` 时向左 | 等于 p 时应向右找更大的 |
| ✗ 找到第一个 > p 就返回 | ✓ 继续向左找更小的候选 | 需要找"大于 p 的最小值" |
| ✗ 只考虑右子树最左节点 | ✓ 迭代法统一处理两种情况 | 没有右子树时需要回溯祖先 |

## 面试追问

**Q1: 如果是普通二叉树（非BST）怎么找中序后继？**
-> 只能中序遍历 O(n)，或者如果有 parent 指针，可以 O(h) 解决（参考 LC 510）。

**Q2: 如何找中序前驱(predecessor)?**
-> 对称操作：`root->val < p->val` 时记录候选并向右走，`root->val >= p->val` 时向左走。

**Q3: 如果 BST 中有重复值怎么办？**
-> 需要精确匹配节点（用指针比较而非值比较），或者定义"后继"为严格大于 p.val 的最小节点。

## 相关题型

- [510. Inorder Successor in BST II](https://leetcode.com/problems/inorder-successor-in-bst-ii/) - 有 parent 指针版本
- [270. Closest Binary Search Tree Value](https://leetcode.com/problems/closest-binary-search-tree-value/) - BST 中最近的值
- [272. Closest Binary Search Tree Value II](https://leetcode.com/problems/closest-binary-search-tree-value-ii/) - BST 中最近的 k 个值
- [230. Kth Smallest Element in a BST](https://leetcode.com/problems/kth-smallest-element-in-a-bst/) - BST 第 k 小
