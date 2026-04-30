# 106. 从中序与后序遍历序列构造二叉树

## 核心思路

后序遍历的**最后一个元素**一定是当前子树的根节点，而中序遍历中根节点**左边是左子树、右边是右子树**。利用这两个性质，递归地"切分"两个数组，就能重建整棵二叉树。

## 思维链

1. **读完题第一反应**：中序遍历 = 左-根-右，后序遍历 = 左-右-根。后序的最后一个元素肯定是根！找到根之后，能不能把左子树和右子树分开？

2. **怎么分出左右子树？** 在中序数组中找到根的位置，根左边的全是左子树节点，根右边的全是右子树节点。这样就知道了左子树有多少个节点（设为 `leftSize`）。

3. **后序数组怎么对应切分？** 后序遍历是"左-右-根"，所以前 `leftSize` 个元素属于左子树，接下来的元素属于右子树，最后一个是根。

4. **暴力解的瓶颈**：每次在中序数组中"查找根的位置"要线性扫描 O(n)。对于一棵平衡树递归 O(log n) 层，总体 O(n log n)；但如果树是链状的，会退化成 O(n²)。

5. **用哈希表优化查找**：预先把中序数组的 `值→索引` 存到哈希表中，查找从 O(n) 变成 O(1)，总体降为 O(n)。

6. **进一步优化**：用索引传参代替真正切数组（避免 copy 开销），这是面试中的标准写法。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力递归 | 每次线性查找根在中序中的位置 + 切分数组 | O(n²) 最坏 | O(n²) 切数组 | 能说出即可 |
| 哈希表 + 索引分治 | 哈希表 O(1) 查根位置 + 索引传参避免拷贝 | O(n) | O(n) | ⭐ 必须写出 |
| 迭代法（栈） | 利用后序逆序遍历 + 栈匹配中序 | O(n) | O(n) | 加分项 |

## 关键提示

- **提示1**：后序数组的**最后一个元素**就是整棵树的根。拿到根后，你需要在中序数组中找到它，从而确定左右子树的大小。

- **提示2**：切分后序数组的关键是 `leftSize`——左子树的节点数。在后序数组中，前 `leftSize` 个元素对应左子树，紧接着的元素对应右子树。

- **提示3（优化方向）**：线性查找根的位置是瓶颈，用什么数据结构能把"值→位置"的查询变成 O(1)？

- **提示4**：递归时传索引 `[inLeft, inRight]` 和 `[postLeft, postRight]` 比真正切割数组高效得多。

- **提示5（可视化）**：
```
中序: [9, 3, 15, 20, 7]
       ↑  ↑  ←-------→
      左  根    右子树

后序: [9, 15, 7, 20, 3]
       ↑  ←-------→  ↑
      左    右子树    根

根 = postorder 最后一个 = 3
在 inorder 中找到 3 的位置 = 1
→ 左子树大小 = 1, 右子树大小 = 3
```

## 解法详解

### 解法1: 暴力递归（切分数组）— O(n²) / O(n²)

**思考过程**：最直觉的做法——每次取后序最后一个元素作为根，在中序中线性查找它的位置，然后真正地把两个数组切成左右子树的子数组，递归构建。

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
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        if (inorder.empty()) return nullptr;
        
        // 后序最后一个元素 = 当前子树的根
        int rootVal = postorder.back();
        TreeNode* root = new TreeNode(rootVal);
        
        // 在中序中找根的位置（线性扫描，这是瓶颈）
        int rootIdx = 0;
        for (int i = 0; i < inorder.size(); i++) {
            if (inorder[i] == rootVal) {
                rootIdx = i;
                break;
            }
        }
        
        int leftSize = rootIdx; // 左子树节点数
        
        // 切分中序数组: [0, rootIdx) 是左子树, (rootIdx, end) 是右子树
        vector<int> inLeft(inorder.begin(), inorder.begin() + rootIdx);
        vector<int> inRight(inorder.begin() + rootIdx + 1, inorder.end());
        
        // 切分后序数组: [0, leftSize) 是左子树, [leftSize, end-1) 是右子树
        vector<int> postLeft(postorder.begin(), postorder.begin() + leftSize);
        vector<int> postRight(postorder.begin() + leftSize, postorder.end() - 1);
        
        root->left = buildTree(inLeft, postLeft);
        root->right = buildTree(inRight, postRight);
        
        return root;
    }
};
```

**关键点**：这个解法能 AC 但效率低——每次切数组需要 O(n) 拷贝，每次查找根位置也是 O(n)。

---

### 解法2: 哈希表 + 索引分治 — O(n) / O(n) ⭐ 面试首选

**从解法1优化**：解法1有两个瓶颈：① 线性查找根在中序中的位置 → 用哈希表优化到 O(1)；② 每次拷贝子数组 → 改成传索引范围。

```
// 递归过程可视化（以示例为例）:
//
// inorder:   [9, 3, 15, 20, 7]    索引: 0  1  2   3  4
// postorder: [9, 15, 7, 20, 3]    索引: 0  1  2   3  4
//
// 第1层: root=3 (post[4]), 在inorder中位置=1
//        leftSize=1, rightSize=3
//        左子树: in[0,0], post[0,0]
//        右子树: in[2,4], post[1,3]
//
//           3
//          / \
//    递归左   递归右
//
// 第2层左: root=9 (post[0]), 在inorder中位置=0
//          leftSize=0, rightSize=0 → 叶子节点
//
// 第2层右: root=20 (post[3]), 在inorder中位置=3
//          leftSize=1, rightSize=1
//          左子树: in[2,2], post[1,1]
//          右子树: in[4,4], post[2,2]
//
//           3
//          / \
//         9   20
//            /  \
//          15    7
//
// 最终构建出的树:
//           3
//          / \
//         9   20
//            /  \
//          15    7
```

```cpp
class Solution {
public:
    unordered_map<int, int> inorderMap; // 值 → 中序索引，O(1) 查找
    
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        // 预建哈希表：把"查找根在中序中的位置"从 O(n) 优化到 O(1)
        for (int i = 0; i < inorder.size(); i++) {
            inorderMap[inorder[i]] = i;
        }
        return build(inorder, postorder, 
                     0, inorder.size() - 1, 
                     0, postorder.size() - 1);
    }
    
private:
    TreeNode* build(vector<int>& inorder, vector<int>& postorder,
                    int inLeft, int inRight, 
                    int postLeft, int postRight) {
        // 终止条件：区间为空，说明没有节点
        if (inLeft > inRight || postLeft > postRight) return nullptr;
        
        // 后序数组的最后一个元素 = 当前子树的根
        int rootVal = postorder[postRight];
        TreeNode* root = new TreeNode(rootVal);
        
        // O(1) 查找根在中序中的位置
        int rootIdx = inorderMap[rootVal];
        
        // 左子树节点个数——这是切分后序数组的关键
        int leftSize = rootIdx - inLeft;
        
        // 递归构建左子树:
        //   中序范围: [inLeft, rootIdx - 1]
        //   后序范围: [postLeft, postLeft + leftSize - 1]
        root->left = build(inorder, postorder,
                          inLeft, rootIdx - 1,
                          postLeft, postLeft + leftSize - 1);
        
        // 递归构建右子树:
        //   中序范围: [rootIdx + 1, inRight]
        //   后序范围: [postLeft + leftSize, postRight - 1]（去掉最后的根）
        root->right = build(inorder, postorder,
                           rootIdx + 1, inRight,
                           postLeft + leftSize, postRight - 1);
        
        return root;
    }
};
```

**关键点**：索引计算是这道题最容易出错的地方。记住后序数组 `[postLeft, postLeft+leftSize-1]` 是左子树，`[postLeft+leftSize, postRight-1]` 是右子树，`postRight` 是根。

---

### 解法3: 迭代法（栈）— O(n) / O(n)

**思考过程**：后序遍历逆序是"根-右-左"，跟前序遍历（根-左-右）是镜像关系。我们可以从后序数组**从右往左**遍历（先处理根，再右，再左），用栈来追踪父节点，用中序数组来判断"何时该从右子树转到左子树"。

```cpp
class Solution {
public:
    TreeNode* buildTree(vector<int>& inorder, vector<int>& postorder) {
        if (postorder.empty()) return nullptr;
        
        int n = postorder.size();
        TreeNode* root = new TreeNode(postorder[n - 1]);
        stack<TreeNode*> stk;
        stk.push(root);
        
        int inIdx = n - 1; // 中序数组从右往左的指针
        
        // 后序从倒数第二个开始（倒数第一个已经是 root）
        for (int i = n - 2; i >= 0; i--) {
            TreeNode* node = new TreeNode(postorder[i]);
            
            if (stk.top()->val != inorder[inIdx]) {
                // 当前栈顶还没到中序的边界 → 新节点是栈顶的右孩子
                stk.top()->right = node;
            } else {
                // 栈顶和中序匹配 → 需要回溯找到正确的父节点
                TreeNode* parent = nullptr;
                while (!stk.empty() && stk.top()->val == inorder[inIdx]) {
                    parent = stk.top();
                    stk.pop();
                    inIdx--;
                }
                // 新节点是回溯到的那个节点的左孩子
                parent->left = node;
            }
            stk.push(node);
        }
        
        return root;
    }
};
```

**关键点**：这个解法理解难度较高。核心思想是——后序逆序 = "根右左"，所以我们先建根，再建右子树，最后建左子树。中序数组从右往左走，用来判断"右子树是否结束，该转到左子树了"。

## 解法对比

| | 解法1: 暴力切数组 | 解法2: 哈希表+索引 | 解法3: 迭代栈 |
|---|---|---|---|
| 时间 | O(n²) 最坏 | O(n) | O(n) |
| 空间 | O(n²) 拷贝数组 | O(n) 哈希表+递归栈 | O(n) 栈 |
| 代码难度 | 简单 | 中等 | 较高 |
| 面试推荐 | 不推荐 | ⭐ 首选 | 追问时展示 |
| 核心区别 | 真实切割数组 | 用索引范围代替切割 | 无递归，用栈模拟 |

**面试中推荐解法2**：思路清晰、效率最优、代码不长。解法3除非面试官明确要求迭代写法，否则不需要主动写。

## 易错点

1. **后序数组的切分索引算错**：
   - ✗ `postLeft + leftSize` 到 `postRight`（忘记去掉根）
   - ✓ `postLeft + leftSize` 到 `postRight - 1`（后序最后一个是根，要排除）

2. **leftSize 的计算**：
   - ✗ `leftSize = rootIdx`（没有减去 inLeft 偏移量）
   - ✓ `leftSize = rootIdx - inLeft`（左子树节点数 = 根在中序中的位置 - 中序区间左端点）

3. **递归终止条件写反**：
   - ✗ `if (inLeft == inRight)` 返回 nullptr（单节点情况被漏掉了！）
   - ✓ `if (inLeft > inRight)` 返回 nullptr（等于的时候还有一个节点要创建）

4. **哈希表忘记建或者用错参数**：
   - 哈希表存的是 `inorder[i] → i`，不是 postorder 的索引

## 面试追问

**Q1（基础理解）**：为什么后序的最后一个元素一定是根？
> 后序遍历顺序是"左-右-根"，所以整个遍历结果的最后一个访问的是根节点。

**Q2（优化）**：暴力解每次要线性查找根，怎么优化？
> 用哈希表预存中序数组的 `值→索引` 映射，把每次查找从 O(n) 降到 O(1)。

**Q3（变体）**：如果给的是前序 + 中序呢？（LeetCode 105）
> 思路完全一样，唯一区别：前序的**第一个**元素是根（而不是后序的最后一个）。递归时前序数组从左边取根，后序从右边取根——代码改动只在索引计算处。

**Q4（深入）**：能不能不用递归，用迭代实现？
> 可以（解法3）。利用"后序逆序 = 根右左"的性质，用栈来维护父子关系，中序数组辅助判断转向时机。

**Q5（扩展）**：如果节点值有重复怎么办？
> 题目保证值不重复。如果有重复，仅靠中序+后序**无法唯一确定**一棵二叉树（因为在中序中找到的根位置可能不唯一）。

## 相关题型

- **105. 从前序与中序遍历序列构造二叉树** — 直接复用本题的分治框架，区别：前序取第一个元素为根（`preorder[preLeft]`），本题后序取最后一个（`postorder[postRight]`）。索引切分逻辑对称调整即可。
- **889. 根据前序和后序遍历构造二叉树** — 没有中序，无法唯一确定（可能有多种结果）。需要用前序第二个元素来区分左右子树。
- **654. 最大二叉树** — 同样的递归"在数组中找根 → 分左右 → 递归构建"框架，只是根的选取规则不同（最大值 vs 后序最后一个）。