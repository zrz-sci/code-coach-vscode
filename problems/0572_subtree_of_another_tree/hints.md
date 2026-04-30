# 572. 另一棵树的子树 (Subtree of Another Tree)

## 核心思路
遍历主树的每个节点，检查以该节点为根的子树是否与 subRoot 完全相同。"完全相同"意味着结构和值都一致。

## 思维链
1. 遍历 root 的每个节点 → DFS/BFS
2. 对每个节点，调用 isSameTree() 检查是否匹配 → 递归比较
3. 优化思路：序列化两棵树后做字符串匹配 → KMP/Hash

## 解法概览表

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|------|-----------|-----------|------|
| DFS 暴力匹配 ⭐ | 遍历 + isSameTree | O(m * n) | O(h) | 首选 |
| 序列化 + 字符串匹配 | 先序遍历序列化后 KMP | O(m + n) | O(m + n) | 最优 |
| 树哈希 | 对每棵子树算 hash | O(m + n) | O(m + n) | 进阶 |

## 关键提示
1. isSameTree 是独立的辅助函数，判断两棵树是否完全一致
2. 注意 subRoot 为 null 的边界情况（空树是任何树的子树）
3. "子树"的定义：必须从某个节点往下包含所有后代，不能截断

## 解法详解

### 解法一：DFS 暴力匹配（推荐）⭐
```cpp
class Solution {
public:
    bool isSubtree(TreeNode* root, TreeNode* subRoot) {
        if (!root) return false;
        if (isSame(root, subRoot)) return true;
        return isSubtree(root->left, subRoot) || isSubtree(root->right, subRoot);
    }

    bool isSame(TreeNode* a, TreeNode* b) {
        if (!a && !b) return true;
        if (!a || !b) return false;
        return a->val == b->val && isSame(a->left, b->left) && isSame(a->right, b->right);
    }
};
```

### 解法二：序列化 + 字符串匹配
```cpp
class Solution {
public:
    bool isSubtree(TreeNode* root, TreeNode* subRoot) {
        string s1 = serialize(root);
        string s2 = serialize(subRoot);
        return s1.find(s2) != string::npos;
    }

    string serialize(TreeNode* node) {
        if (!node) return "#";
        // 用分隔符避免 "12" 匹配 "1" + "2" 的歧义
        return "," + to_string(node->val) + "," + serialize(node->left) + "," + serialize(node->right);
    }
};
```

### 解法三：树哈希
```cpp
class Solution {
public:
    bool isSubtree(TreeNode* root, TreeNode* subRoot) {
        size_t targetHash = treeHash(subRoot);
        return findHash(root, targetHash);
    }

    size_t treeHash(TreeNode* node) {
        if (!node) return hash<int>()(INT_MIN);
        size_t left = treeHash(node->left);
        size_t right = treeHash(node->right);
        return hash<size_t>()(left ^ (right * 31) ^ hash<int>()(node->val));
    }

    bool findHash(TreeNode* node, size_t target) {
        if (!node) return false;
        if (treeHash(node) == target) return true;  // 需要二次验证避免碰撞
        return findHash(node->left, target) || findHash(node->right, target);
    }
};
```

## 易错点
- ✗ isSame 中只比较值不比较结构 → ✓ 必须同时判断 null 情况
- ✗ 序列化不加分隔符，"12" 和 "1,2" 歧义 → ✓ 每个值前后加分隔符
- ✗ 认为子树可以不包含所有后代 → ✓ 子树必须包含某节点的全部后代

## 面试追问
**Q1: 时间复杂度分析？**
> DFS 暴力：最坏 O(m*n)，每个节点都可能触发 isSameTree。序列化+KMP：O(m+n)。

**Q2: 如何优化暴力解法？**
> (1) 先比较子树大小，大小不匹配直接跳过；(2) 用树哈希 O(m+n) 预处理；(3) 序列化后用 KMP 匹配。

**Q3: 与 "Same Tree" (100) 的关系？**
> 100 题是本题的子问题。本题 = 遍历主树 + 在每个节点调用 "Same Tree"。

## 相关题型
- [100. Same Tree](../0100_same_tree/) - 判断两棵树是否相同
- [101. Symmetric Tree](../0101_symmetric_tree/) - 对称树判断
- [236. Lowest Common Ancestor](../0236_lowest_common_ancestor_of_a_binary_tree/) - 树的最近公共祖先
