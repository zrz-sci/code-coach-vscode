## 核心思路

利用 BST 中序遍历的性质，通过 parent 指针找到中序后继：有右子树则取右子树最左节点；无右子树则向上找第一个"作为左孩子"的祖先。

## 思维链

1. **中序后继定义**：比当前节点值大的最小节点
2. **情况一**：节点有右子树 → 后继是右子树的最左节点
3. **情况二**：节点无右子树 → 后继是从当前节点往上走，第一个"当前节点在其左子树中"的祖先
4. **特殊情况**：如果一路向上都是右孩子，说明没有后继，返回 null

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 推荐 |
|------|------|------|------|------|
| 分情况讨论 | 右子树最左 / 向上找左祖先 | O(h) | O(1) | ⭐ |

## 关键提示

- 本题节点有 parent 指针，但**没有 root**
- 不需要比较值，纯粹利用树的结构关系就能解决（Follow-up）
- 中序遍历顺序：左 → 根 → 右

## 解法详解

### 解法一：分情况讨论（推荐）

```cpp
class Solution {
public:
    Node* inorderSuccessor(Node* node) {
        // 情况1：有右子树 → 右子树的最左节点
        if (node->right) {
            Node* cur = node->right;
            while (cur->left) cur = cur->left;
            return cur;
        }
        
        // 情况2：无右子树 → 向上找，直到当前节点是某祖先的左孩子
        while (node->parent && node == node->parent->right) {
            node = node->parent;
        }
        return node->parent;  // 可能为 null（没有后继）
    }
};
```

**复杂度分析：**
- 时间：O(h)，h 为树高
- 空间：O(1)

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| `while (node->parent && node == node->parent->left)` | `while (node->parent && node == node->parent->right)` | 向上找时应跳过"作为右孩子"的祖先 |
| 忘记处理 node->parent 为 null | `return node->parent;` 自然处理 | 最大节点的后继为 null |
| 通过值比较找后继 | 利用结构关系找后继 | Follow-up 要求不访问值 |

## 面试追问

**Q1: 如果要找中序前驱呢？**
> 对称操作：有左子树取左子树最右节点；无左子树向上找第一个作为右孩子的祖先。

**Q2: 如果没有 parent 指针，只有 root 呢？（LC 285）**
> 从 root 出发利用 BST 性质：当 root->val > node->val 时记录 root 为候选并左走，否则右走。

**Q3: 如何不使用节点值来解决？**
> 本解法已经不依赖值比较，纯粹利用左右孩子和 parent 指针的结构关系。

## 相关题型

- [285. Inorder Successor in BST](https://leetcode.com/problems/inorder-successor-in-bst/) - 有 root 版本
- [510. Inorder Successor in BST II](https://leetcode.com/problems/inorder-successor-in-bst-ii/) - 本题
- [173. Binary Search Tree Iterator](https://leetcode.com/problems/binary-search-tree-iterator/) - BST 迭代器
