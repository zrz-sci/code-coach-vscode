# 333. Largest BST Subtree - 最大二叉搜索子树

## 核心思路

在二叉树中找到节点数最多的 BST 子树。关键在于自底向上（后序遍历）收集每个子树的信息：是否为 BST、节点数、值域范围。每个节点只需访问一次，实现 O(n) 时间复杂度。

## 思维链

1. **暴力思路**：对每个节点检查以它为根的子树是否为 BST → O(n^2)
2. **优化方向**：能否一次遍历同时判断 BST + 统计节点数？
3. **信息传递**：后序遍历时，子节点向父节点汇报 (isBST, size, min, max)
4. **合并条件**：左子树是 BST 且 max < node->val，右子树是 BST 且 min > node->val
5. **全局记录**：用变量维护遍历过程中遇到的最大 BST 节点数

## 解法概览

| 解法 | 时间 | 空间 | 特点 | 推荐 |
|------|------|------|------|------|
| 后序遍历 + 信息汇总 | O(n) | O(h) | 自底向上一次遍历 | ⭐⭐⭐ |
| 暴力验证每个子树 | O(n^2) | O(h) | 对每个节点调用 isBST | |

## 关键提示

1. 后序遍历的返回值需要携带多个信息：`{isBST, size, minVal, maxVal}`
2. 空节点视为合法 BST，size=0，min=INT_MAX，max=INT_MIN（方便父节点比较）
3. 如果左/右子树不是 BST，当前节点也不可能是 BST，直接向上传递 isBST=false
4. BST 的定义是严格的：左子树所有值 < 根 < 右子树所有值（不允许等于）

## 解法详解

### 解法一：后序遍历 + 信息汇总（推荐）

**思路**：定义辅助函数返回 `{isBST, size, min, max}`，自底向上合并。

```cpp
class Solution {
public:
    int largestBSTSubtree(TreeNode* root) {
        int ans = 0;
        dfs(root, ans);
        return ans;
    }

private:
    // 返回 {isBST, size, minVal, maxVal}
    struct Info {
        bool isBST;
        int size;
        int minVal, maxVal;
    };

    Info dfs(TreeNode* node, int& ans) {
        if (!node) return {true, 0, INT_MAX, INT_MIN};

        auto left = dfs(node->left, ans);
        auto right = dfs(node->right, ans);

        if (left.isBST && right.isBST &&
            left.maxVal < node->val && node->val < right.minVal) {
            int size = left.size + right.size + 1;
            ans = max(ans, size);
            return {true, size,
                    min(left.minVal, node->val),
                    max(right.maxVal, node->val)};
        }

        return {false, 0, 0, 0};
    }
};
```

**复杂度**：时间 O(n)，空间 O(h)（递归栈）

### 解法二：暴力验证

```cpp
class Solution {
public:
    int largestBSTSubtree(TreeNode* root) {
        if (!root) return 0;
        if (isValidBST(root, LONG_MIN, LONG_MAX))
            return countNodes(root);
        return max(largestBSTSubtree(root->left),
                   largestBSTSubtree(root->right));
    }

private:
    bool isValidBST(TreeNode* node, long lo, long hi) {
        if (!node) return true;
        if (node->val <= lo || node->val >= hi) return false;
        return isValidBST(node->left, lo, node->val) &&
               isValidBST(node->right, node->val, hi);
    }

    int countNodes(TreeNode* node) {
        if (!node) return 0;
        return 1 + countNodes(node->left) + countNodes(node->right);
    }
};
```

## 易错点

- ✗ 空节点的 min 设为 INT_MIN → ✓ 空节点 min=INT_MAX, max=INT_MIN（保证父节点比较时总是成立）
- ✗ BST 允许等于 → ✓ 本题 BST 严格：左 < 根 < 右，不允许等号
- ✗ 忘记更新全局最大值 → ✓ 每次合并成功后立即 `ans = max(ans, size)`
- ✗ 非 BST 节点还传递 min/max → ✓ 一旦 isBST=false，后续信息无意义，直接返回

## 面试追问

**Q1: 为什么空节点的 min 要设为 INT_MAX 而不是 INT_MIN？**
→ 因为父节点需要 `left.maxVal < node->val`，空左子树的 maxVal 应为 INT_MIN（总小于任何值），而 minVal 应为 INT_MAX（不影响向上传递的 min 计算）。

**Q2: 如果 BST 定义改为允许等于（<=），代码怎么改？**
→ 把 `left.maxVal < node->val` 改为 `left.maxVal <= node->val`，同理右侧。

**Q3: 如果要求返回最大 BST 子树的根节点而不是大小呢？**
→ 在 `ans = max(ans, size)` 的同时记录对应的 `node` 指针即可。

## 相关题型

- [98. Validate Binary Search Tree](https://leetcode.com/problems/validate-binary-search-tree/) - BST 验证基础
- [235. Lowest Common Ancestor of a BST](https://leetcode.com/problems/lowest-common-ancestor-of-a-binary-search-tree/) - BST 性质应用
- [1373. Maximum Sum BST in Binary Tree](https://leetcode.com/problems/maximum-sum-bst-in-binary-tree/) - 同类后序遍历题
