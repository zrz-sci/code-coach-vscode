# 623. 在二叉树中增加一行 (Add One Row to Tree)

## 核心思路

在二叉树的指定深度插入一行新节点。核心是**找到 depth-1 层的所有节点**，然后在它们和原子节点之间插入新节点。可以用 **DFS 递归** 或 **BFS 层序遍历** 到达目标层。特殊处理 `depth == 1` 的边界情况。

## 思维链

1. **边界处理**：如果 `depth == 1`，创建新根节点，原树挂在新根的左子树上
2. **到达目标层**：需要找到 depth-1 层的所有节点（因为要在它们下面插入）
3. **DFS 方案**：递归深入到当前深度 == depth-1 时执行插入操作
4. **BFS 方案**：层序遍历直到第 depth-1 层，然后对该层所有节点执行插入
5. **插入逻辑**：对每个目标节点 cur：
   - 创建新左节点 newLeft，newLeft->left = cur->left
   - 创建新右节点 newRight，newRight->right = cur->right
   - cur->left = newLeft, cur->right = newRight
6. **返回根**：根节点可能被替换（depth==1 的情况）

## 解法概览

| 解法 | 思路 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|------|-----------|-----------|------|
| DFS 递归 | 递归到 depth-1 层插入 | O(n) | O(h) | ⭐ |
| BFS 层序 | 逐层遍历到 depth-1 层 | O(n) | O(w) | ⭐ |

> n = 节点数, h = 树高, w = 最大层宽

## 关键提示

1. **depth 从 1 开始计数**：root 在第 1 层，不是第 0 层
2. **depth == 1 特殊处理**：没有 depth-1 层，新节点成为新根，原树挂在左子树
3. **插入方向不对称**：左子树挂到新节点的**左边**，右子树挂到新节点的**右边**
4. **不要修改原有节点的值**：只是在 depth-1 和 depth 层之间插入新节点
5. **DFS 递归传参**：传入当前深度 curDepth，到 curDepth == depth-1 时执行操作

## 解法详解

### 解法一：DFS 递归（推荐）

```cpp
class Solution {
public:
    TreeNode* addOneRow(TreeNode* root, int val, int depth) {
        // 特殊情况: depth==1, 新节点成为根
        if (depth == 1) {
            TreeNode* newRoot = new TreeNode(val);
            newRoot->left = root;
            return newRoot;
        }
        dfs(root, val, depth, 1);
        return root;
    }

    void dfs(TreeNode* node, int val, int depth, int curDepth) {
        if (!node) return;

        if (curDepth == depth - 1) {
            // 在当前节点下方插入新行
            TreeNode* newLeft = new TreeNode(val);
            TreeNode* newRight = new TreeNode(val);
            newLeft->left = node->left;    // 原左子树接到新左节点的左边
            newRight->right = node->right; // 原右子树接到新右节点的右边
            node->left = newLeft;
            node->right = newRight;
            return; // 无需继续深入
        }

        dfs(node->left, val, depth, curDepth + 1);
        dfs(node->right, val, depth, curDepth + 1);
    }
};
```

**复杂度**：时间 O(n)，空间 O(h)（递归栈深度）

### 解法二：BFS 层序遍历

```cpp
class Solution {
public:
    TreeNode* addOneRow(TreeNode* root, int val, int depth) {
        if (depth == 1) {
            TreeNode* newRoot = new TreeNode(val);
            newRoot->left = root;
            return newRoot;
        }

        queue<TreeNode*> q;
        q.push(root);
        int curDepth = 1;

        // 遍历到第 depth-1 层
        while (curDepth < depth - 1) {
            int size = q.size();
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front(); q.pop();
                if (node->left) q.push(node->left);
                if (node->right) q.push(node->right);
            }
            curDepth++;
        }

        // 对 depth-1 层的每个节点执行插入
        while (!q.empty()) {
            TreeNode* node = q.front(); q.pop();
            TreeNode* newLeft = new TreeNode(val);
            TreeNode* newRight = new TreeNode(val);
            newLeft->left = node->left;
            newRight->right = node->right;
            node->left = newLeft;
            node->right = newRight;
        }

        return root;
    }
};
```

**复杂度**：时间 O(n)，空间 O(w)（w 为最宽层的节点数，最坏 n/2）

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ `newLeft->right = node->left` | ✓ `newLeft->left = node->left` | 原左子树应接到新左节点的**左边** |
| ✗ `newRight->left = node->right` | ✓ `newRight->right = node->right` | 原右子树应接到新右节点的**右边** |
| ✗ 忽略 `depth == 1` | ✓ 单独处理 `depth == 1` | depth==1 时没有父节点层，需要创建新根 |
| ✗ `curDepth == depth` 时插入 | ✓ `curDepth == depth - 1` 时插入 | 要在 depth-1 层的节点上操作，不是在 depth 层 |
| ✗ 插入后继续递归 | ✓ 插入后 return | 到达 depth-1 层插入后无需继续深入 |

## 面试追问

**Q1: DFS 和 BFS 在这道题中的取舍是什么？**

A: DFS 递归代码更简洁，空间复杂度 O(h)（h 为树高），对于平衡树 h = log n，对于退化链表 h = n。BFS 空间复杂度 O(w)（w 为最宽层），对于完美二叉树 w 可达 n/2。如果目标 depth 很浅，BFS 可以提前停止更高效；如果树很深且宽度小（如偏斜树），DFS 更节省空间。实际面试中推荐 DFS 递归，代码更短且易于讲解。

**Q2: 如果 depth 等于树的深度+1 呢？也就是在最底层下面加一行？**

A: 完全可以处理。DFS 会递归到所有叶子节点（它们就是 depth-1 层），然后在叶子节点下方创建新的左右子节点。原来的 null 子树（叶子没有子节点）会被正确地挂到新节点上（即新节点的 left/right 也是 null）。约束条件已保证 `1 <= depth <= depth_of_tree + 1`。

**Q3: 这道题能否不用递归也不用队列，用 Morris 遍历的思想实现 O(1) 空间？**

A: 理论上 Morris 遍历可以实现 O(1) 空间的中序或前序遍历，但本题需要**按层**操作（找到 depth-1 层的所有节点），Morris 遍历无法直接获取层级信息。要实现 O(1) 空间，可以用迭代版 DFS 配合显式栈，但本质上仍需要 O(h) 空间。因此这道题的最优空间下界是 O(h) 或 O(w)，无法做到 O(1)。

## 相关题型

- [104. 二叉树的最大深度](../0104_maximum_depth_of_binary_tree/) - 树的深度遍历基础
- [102. 二叉树的层序遍历](../0102_binary_tree_level_order_traversal/) - BFS 层序遍历模板
- [116. 填充每个节点的下一个右侧节点指针](../0116_populating_next_right_pointers_in_each_node/) - 按层操作树结构
- [226. 翻转二叉树](../0226_invert_binary_tree/) - 修改树结构的递归操作
