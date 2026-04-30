# 250. Count Univalue Subtrees (统计同值子树)

## 核心思路

统计二叉树中所有节点值相同的子树个数。本质是**后序遍历 + 自底向上判断**: 一个子树是同值子树当且仅当左右子树都是同值子树，且左右子节点的值都等于当前节点的值。叶子节点天然是同值子树。

## 思维链

1. **定义同值子树**: 子树中所有节点的值都相同
2. **递归方向**: 自底向上（后序遍历），先判断左右子树，再判断当前节点
3. **叶子节点**: 叶子节点一定是同值子树，直接计数+1
4. **递归返回值**: 返回 bool 表示"以当前节点为根的子树是否为同值子树"
5. **判断条件**: 左子树是同值 && 右子树是同值 && 左值==当前值 && 右值==当前值
6. **注意空节点**: 空子节点应返回 true（不影响父节点的同值判断）

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| 后序遍历 + bool 返回值 | O(n) | O(h) | ⭐⭐⭐⭐⭐ |
| 后序遍历 + 传父值参数 | O(n) | O(h) | ⭐⭐⭐⭐ |

## 关键提示

- 后序遍历确保先处理子树再处理当前节点
- 空节点返回 true 是关键，表示空子树不影响同值判断
- 即使左子树不是同值，也**必须继续递归右子树**（不能短路），因为要统计完整的计数
- 使用成员变量 `count` 或引用参数累加计数

## 解法详解

### 解法1: 后序遍历 + bool 返回值 (推荐)

递归函数返回当前子树是否为同值子树，用成员变量累加计数。

```cpp
class Solution {
public:
    int count = 0;
    
    int countUnivalSubtrees(TreeNode* root) {
        isUnival(root);
        return count;
    }
    
    bool isUnival(TreeNode* node) {
        if (!node) return true;
        
        // 后序: 先递归左右子树
        bool left = isUnival(node->left);
        bool right = isUnival(node->right);
        
        // 如果左右都是同值子树, 检查值是否匹配
        if (left && right) {
            if (node->left && node->left->val != node->val) return false;
            if (node->right && node->right->val != node->val) return false;
            count++;
            return true;
        }
        return false;
    }
};
```

**复杂度**: 时间 O(n), 空间 O(h), h 为树高

### 解法2: 后序遍历 + 传父值参数

递归时传入父节点的值，简化判断逻辑。

```cpp
class Solution {
public:
    int countUnivalSubtrees(TreeNode* root) {
        int count = 0;
        isUnival(root, 0, count);
        return count;
    }
    
    bool isUnival(TreeNode* node, int parentVal, int& count) {
        if (!node) return true;
        
        // 后序遍历: 不能短路! 两侧都要递归
        bool left = isUnival(node->left, node->val, count);
        bool right = isUnival(node->right, node->val, count);
        
        // 如果左或右子树不是同值, 当前也不是
        if (!left || !right) return false;
        
        // 当前子树是同值子树
        count++;
        // 返回当前值是否等于父值 (给上层判断用)
        return node->val == parentVal;
    }
};
```

**复杂度**: 时间 O(n), 空间 O(h), h 为树高

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `bool left = isUnival(left) && isUnival(right)` | ✓ 分别赋值再组合 | 短路求值会跳过右子树, 导致计数不完整 |
| ✗ 空节点返回 false | ✓ 空节点返回 true | 空子树不应影响父节点的同值判断 |
| ✗ 只检查 `node->left->val == node->val` | ✓ 先检查 `node->left` 是否存在 | 空指针访问导致崩溃 |
| ✗ 先序遍历判断 | ✓ 后序遍历 | 必须先知道子树状态才能判断当前节点 |

## 面试追问

**Q1: 能否不用全局变量/引用参数来统计?**
> 可以让递归返回 pair<bool, int>，分别表示是否同值和子树中同值子树的个数。

**Q2: 如果要求返回最大的同值子树的大小而非个数?**
> 修改返回值为子树大小，同值子树返回其节点数，非同值返回 -1 或特殊标记。

**Q3: 时间复杂度能否优于 O(n)?**
> 不能。每个节点都必须访问至少一次才能判断其子树是否同值。

## 相关题型

- [572. Subtree of Another Tree](https://leetcode.com/problems/subtree-of-another-tree/) - 子树匹配
- [687. Longest Univalue Path](https://leetcode.com/problems/longest-univalue-path/) - 最长同值路径
- [100. Same Tree](https://leetcode.com/problems/same-tree/) - 判断两棵树相同
- [965. Univalued Binary Tree](https://leetcode.com/problems/univalued-binary-tree/) - 整棵树是否同值
