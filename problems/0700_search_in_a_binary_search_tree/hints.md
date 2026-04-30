# 700. 二叉搜索树中的搜索

## 核心思路

本题本质上就是**利用 BST 的有序性质做二分查找**：每次比较当前节点值和目标值，就能排除一半的搜索空间，和在有序数组上做二分查找是同一个思想。

## 思维链

1. **读完题第一反应**：最朴素的做法是什么？→ 遍历整棵树（BFS 或 DFS），挨个检查每个节点是不是目标值。这对任何二叉树都适用，时间 O(n)。
2. **暴力解的瓶颈**：遍历了所有节点，完全没有利用"二叉搜索树"这个条件。BST 的左子树所有值 < 根 < 右子树所有值，这个信息被浪费了。
3. **怎么突破？**：就像在有序数组里查找不需要遍历所有元素一样，BST 上也可以"每次排除一半"——如果目标 < 当前节点，只需要往左子树找；如果目标 > 当前节点，只需要往右子树找。
4. **递归 vs 迭代**：这个"选择方向往下走"的过程既可以用递归表达，也可以用 while 循环迭代。迭代版甚至不需要栈，因为每一步只走一个方向（不像普通 DFS 两边都要走）。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力 DFS/BFS | 遍历整棵树找目标 | O(n) | O(n) | 能说出即可 |
| 递归（利用 BST） | 根据大小关系选择左/右子树递归 | O(h) | O(h) | ⭐ 必须写出 |
| 迭代（利用 BST） | 用 while 循环代替递归 | O(h) | O(1) | ⭐ 必须写出 |

> h = 树的高度。平衡 BST 中 h = O(log n)，退化为链表时 h = O(n)。

## 关键提示

1. **BST 性质是关键**：左 < 根 < 右。如果 val < root->val，目标一定不在右子树中（右子树所有值都 > root->val > val）。你能用这个性质把搜索空间减半吗？

2. **类比有序数组二分查找**：BST 本质上就是二分查找的树形结构。在有序数组中你用 mid 把区间一分为二，在 BST 中根节点就是 "mid"。

3. **递归终止条件**：什么时候停？两种情况——找到了（`root->val == val`）或走到空了（`root == nullptr`）。

4. **迭代为什么不需要栈？**普通二叉树 DFS 需要栈记住"回头路"，但 BST 搜索每步只走一个方向，不需要回头。

5. 示意图——在下面的 BST 中搜索 val = 2：
```
        4          val=2 < 4, 往左走
       / \
      2   7        val=2 == 2, 找到！返回以2为根的子树
     / \
    1   3          返回结果: [2, 1, 3]
```

## 解法详解

### 解法1: 暴力 DFS — O(n) / O(n)

**思考过程**：如果我们不知道这是 BST，只知道这是普通二叉树怎么办？那就只能遍历每一个节点，逐一比对。这是不利用任何特殊性质的"通用解法"。

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
    TreeNode* searchBST(TreeNode* root, int val) {
        if (!root) return nullptr;           // 空节点，没找到
        if (root->val == val) return root;   // 找到了
        
        // 不利用 BST 性质，左右都搜
        TreeNode* left = searchBST(root->left, val);
        if (left) return left;               // 左子树找到了就返回
        return searchBST(root->right, val);  // 否则去右子树找
    }
};
```

**关键点**：这个解法对任何二叉树都有效，但浪费了 BST 的有序性。最坏情况下要遍历所有 n 个节点。

---

### 解法2: 递归（利用 BST 性质） — O(h) / O(h) ⭐ 面试首选

**从解法1优化**：解法1左右子树都搜，但 BST 的性质告诉我们——如果 val < root->val，答案一定在左子树，不需要搜右子树。这和二分查找的思想一模一样。

**递归三要素**：
- **返回值**：找到的节点（或 nullptr）
- **终止条件**：`root == nullptr`（没找到）或 `root->val == val`（找到了）
- **单层逻辑**：比较大小，选择方向递归

```
// 搜索过程示意 (val = 2):
//
//       4          Step1: val=2 < 4, 递归左子树
//      / \
//     2   7        Step2: val=2 == 2, 返回此节点
//    / \
//   1   3
//
// 搜索过程示意 (val = 5):
//
//       4          Step1: val=5 > 4, 递归右子树
//      / \
//     2   7        Step2: val=5 < 7, 递归左子树
//    / \
//   1   3          Step3: 左子树为空(nullptr), 返回 nullptr
```

```cpp
class Solution {
public:
    TreeNode* searchBST(TreeNode* root, int val) {
        // 终止条件：走到空 或 找到目标
        if (!root || root->val == val) return root;
        
        // 利用 BST 性质：只往一个方向递归
        if (val < root->val) {
            return searchBST(root->left, val);   // 目标更小，去左子树
        } else {
            return searchBST(root->right, val);  // 目标更大，去右子树
        }
    }
};
```

**关键点**：
- 每次递归只进入一个子树，不是两个，所以时间复杂度是 O(h) 而不是 O(n)。
- 空间复杂度 O(h) 来自递归调用栈。平衡 BST 时 h = O(log n)，退化链表时 h = O(n)。

---

### 解法3: 迭代（利用 BST 性质） — O(h) / O(1) ⭐ 面试首选

**从解法2优化**：递归版的空间开销是 O(h)（调用栈）。但注意——每次递归只是"移动到下一个节点"，没有"回头"操作。这意味着我们根本不需要栈来记录路径，一个 while 循环就够了。

```cpp
class Solution {
public:
    TreeNode* searchBST(TreeNode* root, int val) {
        // 迭代：沿着 BST 的路径往下走
        while (root && root->val != val) {
            // 就像二分查找里移动 left/right 指针一样
            if (val < root->val) {
                root = root->left;    // 目标更小，走左边
            } else {
                root = root->right;   // 目标更大，走右边
            }
        }
        // 循环结束：要么 root==nullptr(没找到)，要么 root->val==val(找到了)
        return root;
    }
};
```

**关键点**：
- 空间 O(1)，没有递归栈开销。
- 代码极其简洁，面试中推荐先写这个版本。
- while 条件同时处理了两种结束情况：找到了和找不到。

## 解法对比

| | 暴力 DFS | 递归 (BST) | 迭代 (BST) |
|---|---|---|---|
| 时间 | O(n) | O(h) | O(h) |
| 空间 | O(n) | O(h) 递归栈 | O(1) |
| 利用 BST? | ❌ | ✅ | ✅ |
| 代码量 | 中等 | 最少 | 最少 |
| 适用场景 | 任何二叉树 | BST | BST |
| 面试推荐 | 作为对比提及 | 两个都要会 | 两个都要会 |

**选择建议**：面试中先写迭代版（代码短、空间 O(1)），然后口述递归版展示你理解递归思想。

## 易错点

1. **递归终止条件漏掉 nullptr 检查**
   - ✗ `if (root->val == val) return root;` — 当 root 为空时直接崩溃
   - ✓ `if (!root || root->val == val) return root;` — 先判空，再判值

2. **暴力解中忘记检查左子树的返回值**
   - ✗ 
   ```cpp
   searchBST(root->left, val);
   return searchBST(root->right, val);  // 左子树的结果丢失了
   ```
   - ✓ 
   ```cpp
   TreeNode* left = searchBST(root->left, val);
   if (left) return left;
   return searchBST(root->right, val);
   ```

3. **在利用 BST 性质的解法中，搞混方向**
   - ✗ `if (val < root->val) return searchBST(root->right, val);` — 方向反了！
   - ✓ `if (val < root->val) return searchBST(root->left, val);` — val 更小，往左找

## 面试追问

**Q1: 这道题的时间复杂度是 O(log n) 吗？**
→ 不完全对。时间是 O(h)，其中 h 是树的高度。如果 BST 是平衡的（如 AVL 树、红黑树），h = O(log n)；如果退化成链表（如所有节点只有右孩子），h = O(n)。

**Q2: 如果要搜索多次（比如 1000 次查询），有没有优化方法？**
→ 如果 BST 不平衡，可以先将其转换为平衡 BST（AVL/红黑树），保证每次查询 O(log n)。或者直接将 BST 中序遍历得到有序数组，然后用二分查找。

**Q3: 如果不是搜索，而是要插入一个新值呢？（LeetCode 701）**
→ 思路完全一样！沿着 BST 路径往下走，走到 nullptr 的位置就是新节点应该插入的位置。搜索的代码改一行就能变成插入。

**Q4: 如果要删除一个节点呢？（LeetCode 450）**
→ 删除比搜索和插入复杂。先搜索到目标节点，然后分三种情况处理：叶子直接删、只有一个孩子则用孩子替代、有两个孩子则找后继节点（右子树最小值）替代。

## 相关题型

- **701. 二叉搜索树中的插入操作** — 复用本题"沿 BST 路径走"的框架，区别：搜索是走到 val 停下，插入是走到 nullptr 处创建新节点。
- **450. 删除二叉搜索树中的节点** — 复用本题的搜索逻辑找到目标节点，然后额外处理删除的三种情况。
- **98. 验证二叉搜索树** — 本题是"利用 BST 性质搜索"，98 题是"验证 BST 性质是否成立"，需要维护上下界。
- **270. 最接近的二叉搜索树值** — 复用本题的迭代框架，沿路径走的同时记录最接近 val 的节点值。