# 889. 根据前序和后序遍历构造二叉树

## 核心思路

本质上是一个**分治递归问题**：利用前序和后序遍历的结构特点，递归地划分左右子树范围。前序遍历的第一个元素是根，前序的第二个元素是左子树的根；在后序遍历中找到左子树根的位置，就能确定左子树和右子树的元素范围。注意：与前序+中序不同，前序+后序的构造结果可能不唯一（当某个节点只有一个孩子时，无法区分是左孩子还是右孩子）。

## 思维链

1. **读完题第一反应**：需要从两种遍历序列重建二叉树。之前做过"前序+中序"（LC 105）和"中序+后序"（LC 106），这次是"前序+后序"——没有中序遍历。

2. **分析遍历结构**：
   - 前序：`[根, [左子树前序], [右子树前序]]`
   - 后序：`[[左子树后序], [右子树后序], 根]`
   
3. **如何确定左右子树的分界？**
   - 前序的第一个元素 = 根
   - 前序的第二个元素 = 左子树的根（假设左子树非空）
   - 在后序中找到"左子树根"的位置 → 该位置之前（含自身）就是左子树的后序，之后到倒数第二个是右子树的后序
   - 由此确定左子树大小 L，就能在前序中也划分出左右子树

4. **递归构建**：确定了左右子树在前序和后序中的范围后，递归构建左右子树。

5. **优化查找**：在后序中查找左子树根的位置，暴力 O(n)，用 HashMap 预处理可以 O(1) 查找。

6. **注意非唯一性**：当前序第二个元素不存在（只有根节点）时无需划分。当某节点只有一个孩子时，前序+后序无法区分左右，题目允许返回任意合法结果。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 递归 + 线性查找 | 分治，后序中线性查找左子树根 | O(n^2) | O(n) | 能说出即可 |
| 递归 + HashMap | 分治，HashMap O(1) 查找 | O(n) | O(n) | ⭐ 面试首选 |
| 迭代（栈模拟） | 用栈模拟递归过程 | O(n) | O(n) | 加分项 |

> n = 节点数

## 关键提示

1. **前序第二个元素的含义**：`preorder[preStart+1]` 是左子树的根节点值（如果存在左子树）。这是本题的关键切入点——它让我们能在后序中定位左子树的范围。

2. **后序中的分界**：在后序遍历中找到左子树根 `preorder[preStart+1]` 的位置 idx，则：
   - 左子树的后序范围：`[postStart, idx]`，大小 L = idx - postStart + 1
   - 右子树的后序范围：`[idx+1, postEnd-1]`
   - 左子树的前序范围：`[preStart+1, preStart+L]`
   - 右子树的前序范围：`[preStart+L+1, preEnd]`

3. **结果不唯一**：当只有一个孩子时，前序+后序无法区分是左孩子还是右孩子。题目说"返回任意一个"。在代码中，我们默认将唯一的孩子作为左孩子处理。

4. **用 HashMap 加速**：预处理 `postorder[i] -> i` 的映射，将后序中的查找从 O(n) 降到 O(1)。

5. **递归边界**：当 `preStart > preEnd` 时返回 nullptr；当 `preStart == preEnd` 时返回叶子节点（无需再划分）。

## 解法详解

### 解法1: 递归 + 线性查找 — O(n^2) / O(n)

**思考过程**：最直觉的分治方式——创建根节点，在后序中线性查找左子树根以确定左子树大小，然后递归构建左右子树。

```cpp
class Solution {
public:
    TreeNode* constructFromPrePost(vector<int>& preorder, vector<int>& postorder) {
        return build(preorder, postorder, 0, preorder.size() - 1, 0, postorder.size() - 1);
    }

private:
    TreeNode* build(vector<int>& pre, vector<int>& post,
                    int preS, int preE, int postS, int postE) {
        if (preS > preE) return nullptr;
        
        TreeNode* root = new TreeNode(pre[preS]);
        if (preS == preE) return root; // 叶子节点
        
        // 前序第二个元素 = 左子树根
        int leftRootVal = pre[preS + 1];
        
        // 在后序中找到左子树根的位置
        int idx = postS;
        while (post[idx] != leftRootVal) idx++;
        
        int leftSize = idx - postS + 1;
        
        root->left = build(pre, post, preS + 1, preS + leftSize, postS, idx);
        root->right = build(pre, post, preS + leftSize + 1, preE, idx + 1, postE - 1);
        
        return root;
    }
};
```

**关键点**：每次递归都线性查找，最坏情况（链状树）时间 O(n^2)。但对于平衡树是 O(n log n)。

### 解法2: 递归 + HashMap — O(n) / O(n) ⭐ 面试首选

**从解法1优化**：用 HashMap 预处理后序遍历中每个值的位置，查找从 O(n) 降到 O(1)。

```cpp
class Solution {
public:
    TreeNode* constructFromPrePost(vector<int>& preorder, vector<int>& postorder) {
        unordered_map<int, int> postIdx;
        for (int i = 0; i < (int)postorder.size(); i++) {
            postIdx[postorder[i]] = i;
        }
        return build(preorder, postorder, postIdx, 0, preorder.size() - 1, 0, postorder.size() - 1);
    }

private:
    TreeNode* build(vector<int>& pre, vector<int>& post,
                    unordered_map<int, int>& postIdx,
                    int preS, int preE, int postS, int postE) {
        if (preS > preE) return nullptr;
        
        TreeNode* root = new TreeNode(pre[preS]);
        if (preS == preE) return root;
        
        int leftRootVal = pre[preS + 1];
        int idx = postIdx[leftRootVal];
        int leftSize = idx - postS + 1;
        
        root->left = build(pre, post, postIdx, preS + 1, preS + leftSize, postS, idx);
        root->right = build(pre, post, postIdx, preS + leftSize + 1, preE, idx + 1, postE - 1);
        
        return root;
    }
};
```

**关键点**：HashMap 使总时间降到 O(n)。注意 `leftSize = idx - postS + 1` 计算的是左子树的节点数。

### 解法3: 迭代（栈模拟） — O(n) / O(n)

**完全不同的思路**：用栈模拟前序遍历的构建过程。维护一个栈和后序遍历的指针。栈顶元素的子树在后序中全部处理完时（栈顶值等于后序当前值），弹出栈顶。

```cpp
class Solution {
public:
    TreeNode* constructFromPrePost(vector<int>& preorder, vector<int>& postorder) {
        stack<TreeNode*> stk;
        TreeNode* root = new TreeNode(preorder[0]);
        stk.push(root);
        int postIdx = 0;
        
        for (int i = 1; i < (int)preorder.size(); i++) {
            TreeNode* node = new TreeNode(preorder[i]);
            
            while (stk.top()->val == postorder[postIdx]) {
                stk.pop();
                postIdx++;
            }
            
            // 当前节点是栈顶的左孩子或右孩子
            TreeNode* parent = stk.top();
            if (parent->left == nullptr) {
                parent->left = node;
            } else {
                parent->right = node;
            }
            
            stk.push(node);
        }
        
        return root;
    }
};
```

**关键点**：利用后序遍历的特点——当一个节点的所有后代都处理完后，它会出现在后序中。用这个信号来弹栈。

## 解法对比

| | 递归+线性查找 | 递归+HashMap | 迭代(栈) |
|--|-------------|-------------|---------|
| 时间 | O(n^2) 最坏 | O(n) | O(n) |
| 空间 | O(n) 栈 | O(n) | O(n) |
| 思路难度 | 低 | 低 | 高 |
| 代码难度 | 低 | 低 | 中高 |
| 面试建议 | 说思路 | ⭐首选 | 加分项 |

**选择建议**：
- 面试首选递归 + HashMap：思路清晰，时间最优，容易解释
- 先说暴力递归，再优化到 HashMap，展示思考过程
- 迭代解法作为加分项，但需要能清晰解释栈的含义

## 易错点

1. **忘记处理叶子节点**
   - 错：不判断 `preS == preE` 直接访问 `pre[preS+1]` → 越界
   - 对：`preS == preE` 时直接返回叶子节点

2. **leftSize 计算错误**
   - 错：`leftSize = idx - postS` → 少算了一个节点
   - 对：`leftSize = idx - postS + 1`（包含左子树根本身）

3. **右子树后序范围错误**
   - 错：`[idx+1, postE]` → 包含了根节点
   - 对：`[idx+1, postE-1]`（postE 是根节点在后序中的位置）

4. **混淆前序+中序和前序+后序的方法**
   - 前序+中序：在中序中查找根节点位置来划分
   - 前序+后序：在后序中查找左子树根（前序第二个）来划分
   - 方法不同！不能直接套用 LC 105 的模板

5. **忘记结果不唯一的情况**
   - 当节点只有一个孩子时，前序+后序无法区分左右
   - 代码中默认处理为左孩子即可（题目接受任意合法答案）

## 面试追问

**Q1: 为什么前序+后序不能唯一确定二叉树？**
→ 当某节点只有一个孩子时，无法区分是左孩子还是右孩子。例如前序 [1,2]、后序 [2,1] 既可以是 2 为左孩子也可以是右孩子。前序+中序或中序+后序可以唯一确定，因为中序遍历能明确左右分界。

**Q2: 和 LC 105（前序+中序）比，哪个更难？**
→ 本质相同（分治），但切入角度不同。105 在中序中找根，本题在后序中找左子树根（前序第二个元素）。本题多了"结果不唯一"的分析。

**Q3: 迭代解法的栈代表什么？**
→ 栈中维护的是"从根到当前节点的路径上还未完成子树构建的祖先节点"。当后序指针指向的值等于栈顶时，说明栈顶节点的子树已全部构建完毕，可以弹出。

**Q4: 能否不用 HashMap 也做到 O(n)？**
→ 迭代栈解法不需要 HashMap 就是 O(n)。也可以在递归中用全局指针（类似 LC 106 的优化方式）避免 HashMap，但代码更复杂。

## 相关题型

- **LeetCode 105 从前序与中序遍历序列构造二叉树** — 同为树的重建问题，但前序+中序可以唯一确定。方法几乎相同：分治 + 在中序中查找根。
- **LeetCode 106 从中序与后序遍历序列构造二叉树** — 同为树的重建，从后序找根，在中序中划分左右。
- **LeetCode 297 二叉树的序列化与反序列化** — 更一般的重建问题，需要设计序列化格式使得能唯一重建。
- **LeetCode 1008 前序遍历构造二叉搜索树** — BST 的有序性使得仅前序就能唯一确定。
