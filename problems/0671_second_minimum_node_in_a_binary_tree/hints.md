# 671. 二叉树中第二小的节点 (Second Minimum Node In a Binary Tree)

[Easy] | Tree, Depth-First Search, Binary Tree

## 核心思路

根节点一定是最小值（树的性质保证 `root.val = min(left.val, right.val)`）。所以问题转化为：找到树中最小的不等于 `root.val` 的值。DFS 遍历时，一旦遇到比 `root.val` 大的节点，它是候选答案，且不需要继续深入该子树（因为其子节点只会更大或相等）。

---

## 思维链

1. `root.val` 就是整棵树的最小值
2. 需要找的是所有节点中第二小的值（即最小的 `> root.val` 的值）
3. DFS 遍历，遇到 `node.val > root.val` 时，该值是候选，不需要深入
4. 遇到 `node.val == root.val` 时，需要继续深入子节点寻找
5. 所有候选值中取最小的就是答案

---

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 解法一 | DFS + 剪枝 | O(n) | O(h) | ⭐⭐⭐ 首选 |
| 解法二 | BFS 层序遍历 | O(n) | O(n) | ⭐⭐ 备选 |

---

## 关键提示

### 提示 1 - 树的特殊性质
每个内部节点的值等于其两个子节点中较小的那个。因此根节点就是全局最小值。

### 提示 2 - 剪枝优化
当某节点 `val > root.val` 时，该节点的所有后代只会 `>= val`（因为树的性质），所以无需继续深入。

### 提示 3 - 注意 INT 溢出
节点值可达 `2^31 - 1`，用 `long long` 存储候选答案或初始化为 -1 特判。

### 提示 4 - 全相同的情况
如果所有节点值都相同，返回 -1。

---

## 解法详解

### 解法一: DFS + 剪枝

```cpp
class Solution {
public:
    int findSecondMinimumValue(TreeNode* root) {
        long long ans = LLONG_MAX;
        int minVal = root->val;
        dfs(root, minVal, ans);
        return ans == LLONG_MAX ? -1 : (int)ans;
    }

    void dfs(TreeNode* node, int minVal, long long& ans) {
        if (!node) return;
        if (node->val > minVal) {
            ans = min(ans, (long long)node->val);
            return; // 剪枝：子节点只会更大
        }
        dfs(node->left, minVal, ans);
        dfs(node->right, minVal, ans);
    }
};
```

### 解法二: BFS

```cpp
class Solution {
public:
    int findSecondMinimumValue(TreeNode* root) {
        long long ans = LLONG_MAX;
        int minVal = root->val;
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode* node = q.front(); q.pop();
            if (node->val > minVal) {
                ans = min(ans, (long long)node->val);
                continue; // 不入队子节点
            }
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
        return ans == LLONG_MAX ? -1 : (int)ans;
    }
};
```

---

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 用 `INT_MAX` 初始化候选答案 | ✓ 用 `LLONG_MAX` 或特判 -1 | 节点值可达 `2^31-1 == INT_MAX` |
| ✗ 遍历所有节点不剪枝 | ✓ `node.val > minVal` 时不继续深入 | 不影响正确性但影响效率 |
| ✗ 只比较根和两个子节点 | ✓ 需要 DFS 整棵树 | `[2,2,5,null,null,5,7]` 答案是 5 不是 5 |

---

## 面试追问

**Q1: 为什么 `node.val > minVal` 时可以停止深入？**
→ 树的性质保证 `parent.val = min(left.val, right.val)`，所以子节点值 `>= parent.val`。如果 `parent.val > minVal`，则所有后代 `>= parent.val > minVal`，不可能比当前候选更小。

**Q2: 时间复杂度一定是 O(n) 吗？**
→ 最坏情况 O(n)（如所有节点值相同，必须遍历全部）。最好情况可以在很浅的层找到答案提前终止。剪枝使平均性能好于 O(n)。

**Q3: 如果改成"找第 k 小"怎么做？**
→ DFS 收集所有不同的值到集合中，排序后取第 k 个。或用大小为 k 的最大堆维护最小的 k 个不同值。树最多 25 个节点，直接收集排序即可。

---

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| 230 | Kth Smallest Element in a BST | 树中第 k 小 |
| 783 | Minimum Distance Between BST Nodes | BST 中最小差值 |
| 993 | Cousins in Binary Tree | 二叉树节点关系 |
