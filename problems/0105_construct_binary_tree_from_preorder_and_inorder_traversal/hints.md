# 105. 从前序与中序遍历序列构造二叉树

## 核心思路（1句话）
前序的第一个元素是根，在中序中定位根的位置就能分割出左右子树的范围，递归构建即可；用 HashMap 将中序查找从 O(n) 优化到 O(1)。

## 思维链（4-6步）

1. **第一反应**：前序遍历的第一个元素一定是根节点。但光知道根不够，还需要知道哪些节点属于左子树、哪些属于右子树。
2. **关键洞察**：中序遍历的特性——根节点左边的所有元素属于左子树，右边的所有元素属于右子树。所以在中序数组中找到根的位置，就能分割出左右子树。
3. **子问题分解**：知道了左子树的节点数 `leftSize`，就能在前序数组中确定左子树范围 `preorder[1..leftSize]` 和右子树范围 `preorder[leftSize+1..end]`。递归处理即可。
4. **效率瓶颈**：每次在中序数组中线性查找根的位置是 O(n)，总复杂度 O(n^2)。用 HashMap 预处理中序数组（值 -> 下标），查找变 O(1)，总复杂度降到 O(n)。
5. **迭代方案**：也可以用栈模拟递归过程，但实现复杂且面试中很少要求。
6. **最终决策**：HashMap + 递归是面试最佳方案，兼顾效率和可读性。

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| 解法1: 递归+HashMap ⭐面试首选 | O(n) | O(n) | HashMap 加速中序查找 |
| 解法2: 递归+线性查找 | O(n^2) | O(n) | 无优化，面试不推荐 |
| 解法3: 迭代（栈模拟） | O(n) | O(n) | 最优但实现复杂 |

## 关键提示

1. **前序和中序的对应关系（ASCII 图）**：
   ```
   preorder = [3, 9, 20, 15, 7]
   inorder  = [9, 3, 15, 20, 7]

   第一步：preorder[0] = 3 是根
   在 inorder 中找到 3 的位置 (index=1)

   inorder:  [9, | 3 | , 15, 20, 7]
              左    根     右
              
   leftSize = 1 (index - inLeft)

   preorder: [3, | 9 | , 20, 15, 7]
              根   左      右

   构建的树：
          3
         / \
        9   20
           /  \
          15   7
   ```

2. **递归子问题的下标范围**：
   ```
   设 inorder 中根的位置为 inIdx, 左子树大小 leftSize = inIdx - inLeft
   
   左子树:
     preorder: [preStart+1, preStart+leftSize]
     inorder:  [inLeft, inIdx-1]
   
   右子树:
     preorder: [preStart+leftSize+1, preEnd]
     inorder:  [inIdx+1, inRight]
   ```

3. **用全局 preIdx 简化代码**：解法1 的核心技巧——不传递 preorder 的范围参数，而是用一个全局递增的 `preIdx`。因为前序遍历的访问顺序就是"根->左->右"，所以每次取 `preorder[preIdx++]` 就自然是当前子树的根。但必须**先递归左子树再递归右子树**，否则 preIdx 的顺序就乱了。

4. **为什么题目保证"无重复值"**：如果有重复值，在中序数组中找根的位置可能有多个匹配，无法唯一确定树的结构。这也是 HashMap 方案的前提条件。

5. **递归树的可视化**：
   ```
   build(pre=[3,9,20,15,7], in=[9,3,15,20,7])
   ├── root = 3, inIdx = 1
   ├── build_left(in=[9])
   │   └── root = 9, 无子树 -> 叶子节点
   └── build_right(in=[15,20,7])
       ├── root = 20, inIdx = 3
       ├── build_left(in=[15])
       │   └── root = 15 -> 叶子节点
       └── build_right(in=[7])
           └── root = 7 -> 叶子节点
   ```

## 解法详解

### 解法1: 递归 + HashMap — O(n)/O(n) ⭐面试首选

**思考过程**: 前序第一个是根 -> 在中序找到根 -> 分出左右子树 -> 递归。用 HashMap 把中序查找从 O(n) 优化到 O(1)。用全局 `preIdx` 避免传递复杂的下标参数。

```cpp
class Solution {
public:
    unordered_map<int, int> inMap;  // 中序值 -> 下标，O(1) 查找
    int preIdx = 0;                 // 全局前序指针，每次取根后自增

    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        // 预处理：构建中序数组的 HashMap
        for (int i = 0; i < (int)inorder.size(); i++) {
            inMap[inorder[i]] = i;
        }
        return build(preorder, 0, inorder.size() - 1);
    }

private:
    // 构建 inorder[inLeft..inRight] 对应的子树
    TreeNode* build(vector<int>& preorder, int inLeft, int inRight) {
        // base case: 区间为空
        if (inLeft > inRight) return nullptr;

        // 前序的下一个元素就是当前子树的根
        int rootVal = preorder[preIdx++];
        TreeNode* root = new TreeNode(rootVal);

        // 在中序中定位根，分割左右子树
        int inIdx = inMap[rootVal];

        // 关键：必须先建左子树，再建右子树！
        // 因为 preIdx 是按前序遍历递增的，前序 = 根->左->右
        root->left = build(preorder, inLeft, inIdx - 1);
        root->right = build(preorder, inIdx + 1, inRight);

        return root;
    }
};
```

**关键点**: 
- `preIdx` 是成员变量全局递增，不是函数参数
- **必须先递归左子树再递归右子树**，否则 `preIdx` 对应关系就乱了
- HashMap 的键是节点值（题目保证无重复），值是中序下标

### 解法2: 递归 + 线性查找 — O(n^2)/O(n)

**从解法1简化**: 如果不使用 HashMap，直接在中序数组中线性查找根的位置。代码更直观但效率较低。需要同时传递前序和中序的下标范围。

```cpp
class Solution {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        return build(preorder, 0, preorder.size() - 1,
                     inorder, 0, inorder.size() - 1);
    }

private:
    TreeNode* build(vector<int>& pre, int pL, int pR,
                    vector<int>& in, int iL, int iR) {
        if (pL > pR) return nullptr;

        // 前序第一个是根
        TreeNode* root = new TreeNode(pre[pL]);

        // 线性查找根在中序中的位置
        int idx = iL;
        while (in[idx] != pre[pL]) idx++;

        // 左子树大小
        int leftSize = idx - iL;

        // 递归构建
        // 左子树: pre[pL+1..pL+leftSize], in[iL..idx-1]
        root->left = build(pre, pL + 1, pL + leftSize,
                           in, iL, idx - 1);
        // 右子树: pre[pL+leftSize+1..pR], in[idx+1..iR]
        root->right = build(pre, pL + leftSize + 1, pR,
                            in, idx + 1, iR);

        return root;
    }
};
```

**关键点**: 
- 需要计算 `leftSize` 来确定前序数组中左右子树的分界点
- 下标公式容易出错，面试时建议用解法1 的 `preIdx` 方式

### 解法3: 迭代（栈模拟） — O(n)/O(n)

**从解法1优化**: 用栈模拟递归过程，适合追问"不用递归怎么做"的场景。

```cpp
class Solution {
public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        if (preorder.empty()) return nullptr;

        TreeNode* root = new TreeNode(preorder[0]);
        stack<TreeNode*> stk;
        stk.push(root);
        int inIdx = 0;  // 中序数组指针

        for (int i = 1; i < (int)preorder.size(); i++) {
            TreeNode* node = new TreeNode(preorder[i]);
            
            if (stk.top()->val != inorder[inIdx]) {
                // 当前栈顶不是中序的下一个 -> 新节点是栈顶的左孩子
                stk.top()->left = node;
            } else {
                // 栈顶等于中序的下一个 -> 回溯找到正确的父节点
                TreeNode* parent = nullptr;
                while (!stk.empty() && stk.top()->val == inorder[inIdx]) {
                    parent = stk.top();
                    stk.pop();
                    inIdx++;
                }
                // 新节点是最后弹出节点的右孩子
                parent->right = node;
            }
            stk.push(node);
        }
        return root;
    }
};
```

**关键点**: 
- 利用前序遍历"根左右"和中序遍历"左根右"的关系
- 栈维护从根到当前节点的路径
- 当栈顶 == inorder[inIdx] 时，说明左子树结束，需要回溯

## 解法对比

| 维度 | 解法1: 递归+HashMap | 解法2: 递归+线性查找 | 解法3: 迭代 |
|------|-------------------|---------------------|------------|
| 时间 | O(n) | O(n^2) | O(n) |
| 空间 | O(n) HashMap+栈 | O(n) 递归栈 | O(n) 显式栈 |
| 代码量 | 中等 | 中等 | 较长 |
| 面试推荐 | 首选 | 不推荐 | 追问时给出 |
| 下标管理 | 简单(preIdx) | 复杂(6个参数) | 隐式(栈管理) |

## 易错点

1. **先右后左导致 preIdx 错位**
   ```cpp
   // ✗ 错误：先建右子树，preIdx 已经跳到右子树的前序区间
   root->right = build(preorder, inIdx + 1, inRight);  // preIdx 指向右子树
   root->left = build(preorder, inLeft, inIdx - 1);    // preIdx 已经错了！

   // ✓ 正确：必须先左后右，与前序遍历顺序一致
   root->left = build(preorder, inLeft, inIdx - 1);    // preIdx 先消费左子树
   root->right = build(preorder, inIdx + 1, inRight);  // preIdx 再消费右子树
   ```
   **原因**：`preIdx` 按前序顺序（根->左->右）递增，所以递归也必须先左后右。

2. **解法2 下标计算错误**
   ```cpp
   // ✗ 错误：右子树的前序起点算错
   root->right = build(pre, pL + leftSize, pR, in, idx + 1, iR);
   // 应该是 pL + leftSize + 1（跳过根节点 pL 和左子树 leftSize 个）

   // ✓ 正确
   root->right = build(pre, pL + leftSize + 1, pR, in, idx + 1, iR);
   ```
   **原因**：前序中根在 pL，左子树占 pL+1 到 pL+leftSize，右子树从 pL+leftSize+1 开始。

3. **HashMap 用值做 key 但有重复值**
   ```cpp
   // ✗ 如果输入有重复值，HashMap 后建的会覆盖先建的，导致构建错误
   for (int i = 0; i < n; i++) inMap[inorder[i]] = i;
   // 重复值只保留最后一个的下标

   // ✓ 题目保证无重复值，所以 HashMap 方案可行
   // 如果有重复值，只能用解法2的线性查找，且无法唯一确定树结构
   ```

4. **preIdx 定义为局部变量导致递归时不共享**
   ```cpp
   // ✗ 错误：preIdx 作为参数传递，每层递归有自己的副本
   TreeNode* build(vector<int>& preorder, int preIdx, int inLeft, int inRight) {
       int rootVal = preorder[preIdx];  // OK
       root->left = build(preorder, preIdx + 1, inLeft, inIdx - 1);
       // 左子树消费了多少个 preIdx？不确定！右子树的 preIdx 怎么算？
   }

   // ✓ 正确：preIdx 作为成员变量，所有递归层共享同一个
   int preIdx = 0;  // 成员变量
   ```

## 面试追问

### Q1: 如果给的是后序 + 中序，怎么构建？（LC 106）
**回答**：
思路完全类似，只有两点变化：
1. **根在后序数组的最后一个**（而非第一个）
2. **必须先建右子树再建左子树**（因为后序是"左右根"，从后往前取是"根右左"）

```cpp
int postIdx;  // 从后序数组末尾开始
TreeNode* build(vector<int>& postorder, int inLeft, int inRight) {
    if (inLeft > inRight) return nullptr;
    int rootVal = postorder[postIdx--];  // 从后往前取
    TreeNode* root = new TreeNode(rootVal);
    int inIdx = inMap[rootVal];
    root->right = build(postorder, inIdx + 1, inRight);  // 先右！
    root->left = build(postorder, inLeft, inIdx - 1);    // 后左！
    return root;
}
```

### Q2: 只给前序和后序能唯一确定二叉树吗？（LC 889）
**回答**：
不能唯一确定。前序+后序无法区分"只有左孩子"和"只有右孩子"的情况。例如：
```
    1          1
   /    和      \
  2              2
```
两棵树的前序都是 [1,2]，后序都是 [2,1]。
但如果每个节点要么是叶子要么有两个孩子（满二叉树），则前序+后序可以唯一确定。

### Q3: 时间复杂度为什么是 O(n)？递归不是 O(n) 层吗？
**回答**：
虽然递归可能有 O(n) 层（退化为链状树时），但每层递归只做 O(1) 的工作（取根、查 HashMap、创建节点）。关键观察：每个节点恰好被创建一次，HashMap 查找一次，所以总工作量是 O(n)。

换个角度：`preIdx` 从 0 递增到 n-1，每次递归调用消耗一个 `preIdx`，总共 n 次递归调用，每次 O(1)，总计 O(n)。

## 相关题型

| 题目 | 关系 | 复用技巧 |
|------|------|---------|
| [106. 从中序与后序遍历序列构造二叉树](https://leetcode.com/problems/construct-binary-tree-from-inorder-and-postorder-traversal/) | 镜像 | 同样的分治思路，改为后序从后取根+先右后左 |
| [889. 根据前序和后序遍历构造二叉树](https://leetcode.com/problems/construct-binary-tree-from-preorder-and-postorder-traversal/) | 变体 | 前序+后序不唯一，需要额外约束 |
| [297. 二叉树的序列化与反序列化](https://leetcode.com/problems/serialize-and-deserialize-binary-tree/) | 扩展 | 用前序+null 标记可以唯一确定树 |
| [144/94/145. 前序/中序/后序遍历](https://leetcode.com/problems/binary-tree-preorder-traversal/) | 基础 | 理解三种遍历顺序是本题的前提 |
| [654. 最大二叉树](https://leetcode.com/problems/maximum-binary-tree/) | 同思路 | 同样的"找根+分割左右+递归"模式 |
