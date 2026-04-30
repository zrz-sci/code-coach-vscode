# 545. 二叉树的边界

## 核心思路

这道题本质是把二叉树的"外壳"按特定顺序收集：**根 + 左边界（自顶向下，不含叶子）+ 所有叶子（从左到右）+ 右边界（自底向上，不含叶子）**。关键在于理解"左边界"和"右边界"的精确定义，以及如何避免节点重复收集。

## 思维链

1. **读完题第一反应**：边界 = 根 + 左边界 + 叶子 + 逆序右边界，四个部分拼接。能不能分别收集这四部分然后合并？
2. **分解问题**：
   - 左边界：从根的左子节点开始，优先走左，没左走右，直到遇到叶子停止
   - 叶子：用 DFS/前序遍历收集所有叶节点（从左到右的顺序天然保证）
   - 右边界：从根的右子节点开始，优先走右，没右走左，直到遇到叶子停止，最后逆序
3. **难点在哪**：各部分之间不能有重复节点！根节点不算叶子，左边界不包含最底部的叶子，右边界同理。
4. **怎么避免重复**：左边界收集时遇到叶子就停；叶子单独收集；右边界收集时遇到叶子就停。根节点单独加，三部分互不重叠。
5. **另一种思路**：能不能一次 DFS 搞定？给每个节点打标记（是左边界 / 右边界 / 叶子 / 内部节点），按前序遍历的顺序判断是否加入结果。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 分三部分收集 | 分别收集左边界、叶子、右边界 | O(n) | O(n) | ⭐ 必须写出 |
| 一次 DFS + 标记 | 前序遍历，用 flag 标记节点角色 | O(n) | O(n) | 加分项 |

## 关键提示

1. **左边界的定义要精读**：不是"最左边的路径"，而是"从根的左孩子开始，有左走左、无左走右"的路径，且叶子不算。

2. **想清楚四部分的拼接顺序和去重**：
   ```
   示例2的树:
              1           边界 = [1] + [2] + [4,7,8,9,10] + [6,3]
            /   \                  根   左边界    叶子(左→右)   右边界(逆序)
           2     3
          / \   /
         4   5 6
            / \ / \
           7  8 9  10
   
   左边界: 从2开始 → 2有左孩子4 → 4是叶子,停止 → 左边界=[2]
   右边界: 从3开始 → 3无右孩子,走左6 → 6有右孩子10 → 10是叶子,停止
           收集顺序[3,6], 逆序后=[6,3]
   叶子: 前序遍历收集 → [4,7,8,9,10]
   ```

3. **根节点只有一侧子树时**：如果根没有左子节点，左边界为空；如果根没有右子节点，右边界为空。不要把根本身当作左/右边界。

4. **叶子的左右顺序**：用前序遍历（或任何先左后右的DFS）自然保证从左到右的顺序。

5. **右边界需要逆序**：收集时是自顶向下的，输出需要自底向上。可以先收集再 reverse，也可以用栈。

## 解法详解

### 解法1: 分三部分收集 — O(n) / O(n) ⭐ 面试首选

**思考过程**: 题目明确说边界 = 根 + 左边界 + 叶子 + 逆序右边界，自然想到分别收集再拼接。关键是精确实现每部分的定义，避免重复。

```cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 */
class Solution {
public:
    vector<int> boundaryOfBinaryTree(TreeNode* root) {
        if (!root) return {};
        vector<int> res;
        
        // 1. 根节点（非叶子时单独加入）
        if (!isLeaf(root)) {
            res.push_back(root->val);
        }
        
        // 2. 左边界：从 root->left 出发，有左走左、无左走右，叶子停
        TreeNode* cur = root->left;
        while (cur && !isLeaf(cur)) {
            res.push_back(cur->val);
            cur = cur->left ? cur->left : cur->right;
        }
        
        // 3. 所有叶子（前序 DFS）
        addLeaves(root, res);
        
        // 4. 右边界（逆序）：从 root->right 出发，有右走右、无右走左
        vector<int> rightBoundary;
        cur = root->right;
        while (cur && !isLeaf(cur)) {
            rightBoundary.push_back(cur->val);
            cur = cur->right ? cur->right : cur->left;
        }
        // 逆序加入结果
        for (int i = rightBoundary.size() - 1; i >= 0; i--) {
            res.push_back(rightBoundary[i]);
        }
        
        return res;
    }
    
private:
    bool isLeaf(TreeNode* node) {
        return node && !node->left && !node->right;
    }
    
    void addLeaves(TreeNode* node, vector<int>& res) {
        if (!node) return;
        if (isLeaf(node)) {
            res.push_back(node->val);
            return;
        }
        addLeaves(node->left, res);
        addLeaves(node->right, res);
    }
};
```

**关键点**:
- 根节点如果是叶子（整棵树只有一个节点），直接在 `addLeaves` 中收集，不需要单独处理
- 左右边界遇到叶子就停止，叶子由 `addLeaves` 统一收集，这是避免重复的关键
- 右边界先收集再逆序，比递归处理更直观

### 解法2: 一次 DFS + 标记 — O(n) / O(n)

**从解法1优化**: 解法1要跑三次遍历（左边界、叶子、右边界），能不能一次前序遍历搞定？思路是给每个节点一个 flag 标记它的"角色"，在前序遍历中根据 flag 决定是否加入结果。

```
flag 定义:
  0 = 根节点
  1 = 左边界节点
  2 = 右边界节点
  3 = 内部节点（既不是边界也不是叶子）

对于左边界上的节点: 其左孩子继承 flag=1, 右孩子在无左孩子时继承 flag=1
对于右边界上的节点: 其右孩子继承 flag=2, 左孩子在无右孩子时继承 flag=2
叶子节点: 无论 flag 是什么都加入结果
```

```cpp
class Solution {
public:
    vector<int> boundaryOfBinaryTree(TreeNode* root) {
        if (!root) return {};
        
        vector<int> left_bound, leaves, right_bound;
        
        // flag: 0=root, 1=leftBoundary, 2=rightBoundary, 3=internal
        preorder(root, 0, left_bound, leaves, right_bound);
        
        // 拼接: 左边界 + 叶子 + 逆序右边界
        vector<int> res;
        res.insert(res.end(), left_bound.begin(), left_bound.end());
        res.insert(res.end(), leaves.begin(), leaves.end());
        res.insert(res.end(), right_bound.rbegin(), right_bound.rend());
        return res;
    }
    
private:
    bool isLeaf(TreeNode* node) {
        return node && !node->left && !node->right;
    }
    
    void preorder(TreeNode* node, int flag,
                  vector<int>& left_bound, vector<int>& leaves, vector<int>& right_bound) {
        if (!node) return;
        
        if (isLeaf(node)) {
            // 叶子统一收集（前序保证左到右顺序）
            leaves.push_back(node->val);
            return;
        }
        
        // 非叶子节点: 根据 flag 决定放入哪个集合
        if (flag == 0 || flag == 1) {
            // 根或左边界节点 → 加入 left_bound（根也放这里，保证顺序）
            left_bound.push_back(node->val);
        } else if (flag == 2) {
            // 右边界节点 → 加入 right_bound（最后逆序）
            right_bound.push_back(node->val);
        }
        // flag == 3 的内部节点不加入任何集合
        
        // 计算左右孩子的 flag
        int leftChildFlag = getLeftChildFlag(node, flag);
        int rightChildFlag = getRightChildFlag(node, flag);
        
        preorder(node->left, leftChildFlag, left_bound, leaves, right_bound);
        preorder(node->right, rightChildFlag, left_bound, leaves, right_bound);
    }
    
    int getLeftChildFlag(TreeNode* node, int flag) {
        if (flag == 0 || flag == 1) {
            // 根/左边界的左孩子: 继承左边界
            return 1;
        }
        if (flag == 2) {
            // 右边界节点无右孩子时，左孩子继承右边界
            return node->right ? 3 : 2;
        }
        return 3; // 内部节点的孩子还是内部节点
    }
    
    int getRightChildFlag(TreeNode* node, int flag) {
        if (flag == 0 || flag == 2) {
            // 根/右边界的右孩子: 继承右边界
            return 2;
        }
        if (flag == 1) {
            // 左边界节点无左孩子时，右孩子继承左边界
            return node->left ? 3 : 1;
        }
        return 3;
    }
};
```

**关键点**: 
- flag 的传递规则要和题目定义完全对应："有左走左、无左走右"
- 前序遍历保证左边界自顶向下，叶子从左到右
- 右边界收集后逆序，因为前序遍历是自顶向下的，但输出要自底向上

## 解法对比

| | 解法1: 分三部分 | 解法2: 一次DFS+标记 |
|---|---|---|
| **遍历次数** | 3 次（左边界 + 叶子 + 右边界） | 1 次前序遍历 |
| **代码直观性** | ⭐ 非常直观，每部分逻辑独立 | 较复杂，flag 传递规则需要仔细推敲 |
| **时间复杂度** | O(n)（常数大一点） | O(n) |
| **面试推荐** | ⭐ 首选，容易写对 | 展示思维深度，但容易出错 |

## 易错点

1. **✗ 根节点是叶子时重复收集**：
   ```cpp
   // 错误: 根节点总是先加入
   res.push_back(root->val);
   addLeaves(root, res);  // 如果 root 是叶子，又加了一次！
   ```
   **✓ 正确做法**：先判断根是否叶子，如果是叶子则只在 addLeaves 中加入一次（或提前返回 `{root->val}`）

2. **✗ 左边界没在叶子处停止**：
   ```cpp
   // 错误: 没检查是否叶子
   while (cur) {
       res.push_back(cur->val);  // 叶子也被收集了！
       cur = cur->left ? cur->left : cur->right;
   }
   ```
   **✓ 正确**：循环条件改为 `while (cur && !isLeaf(cur))`

3. **✗ 根没有左子节点时从根开始收集左边界**：
   ```cpp
   // 错误: cur 从 root 开始
   TreeNode* cur = root;
   ```
   **✓ 正确**：`cur = root->left`，左边界的定义是从根的左子节点开始

4. **✗ 右边界忘记逆序**：
   题目要求右边界是自底向上的顺序。直接收集是自顶向下的，必须 reverse。

5. **✗ 只有根节点没有子节点时返回空**：
   只有一个根节点时，根节点既是根也是叶子（但题目说根不算叶子），应返回 `[root->val]`。

## 面试追问

**Q1: 你怎么保证左边界、叶子、右边界之间没有重复节点？**
→ 左边界只收集非叶子节点（叶子前停止），右边界同理，叶子由 DFS 统一收集。三者互不重叠。

**Q2: 如果树极度不平衡（如退化成链表），你的解法还正确吗？**
→ 正确。例如只有右链的树 `1→2→3→4`：左边界为空，叶子是 [4]，右边界是 [2,3] 逆序为 [3,2]，结果 [1,4,3,2]。递归深度可能到 O(n)，可以考虑迭代避免栈溢出。

**Q3: 能否用纯迭代（不用递归）实现叶子的收集？**
→ 可以。用栈模拟前序遍历，遇到叶子就收集。注意入栈顺序：先右后左，保证左子树先处理。

**Q4: 这道题和层序遍历的"左视图/右视图"有什么关系？**
→ 左视图是每层最左节点，但左边界的定义不同——左边界沿着"有左走左、无左走右"的路径走，不一定是每层的最左节点。两者定义不同，不能混淆。

## 相关题型

- **199. 二叉树的右视图** — 每层最右节点，用 BFS 层序遍历即可。和本题的"右边界"定义不同（右视图是每层最右，右边界是沿右侧走到底的路径）
- **116. 填充每个节点的下一个右侧节点指针** — 同样涉及树的层次/边界概念
- **987. 二叉树的垂序遍历** — 另一种"按位置收集节点"的题，需要同时追踪行和列