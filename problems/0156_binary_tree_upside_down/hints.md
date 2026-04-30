# 156. 上下翻转二叉树

## 核心思路

这道题本质上是**沿着左子链（left spine）做一次类似链表反转的操作**：每个节点的左孩子变成新的父节点，原父节点变成右孩子，原右兄弟变成左孩子。

## 思维链

1. **读完题第一反应**：题目说"原左孩子变新根、原根变新右孩子、原右孩子变新左孩子"，而且逐层进行。观察示例，整棵树的新根就是原树最左下角的节点。

2. **画图找规律**：把示例画出来，标记每个节点翻转前后的连接关系：
   ```
   翻转前:            翻转后:
        1                  4
       / \                / \
      2   3              5   2
     / \                    / \
    4   5                  3   1
   ```
   观察到：节点 4 是新根；节点 2 的新左孩子是 3（原右兄弟），新右孩子是 1（原父亲）；以此类推。

3. **发现结构特征**：题目保证"右节点都有左兄弟且右节点无子节点"，这意味着树的形状是一条**左子链**，每个节点最多挂一个右孩子。这就像一条链表！

4. **类比链表反转**：沿着左子链从上到下走，就像链表 `1→2→4`。翻转就是把这条链反转成 `4→2→1`，同时把每个节点原来的右兄弟重新挂到正确位置。

5. **选择实现方式**：可以用递归（先递归到底再回溯时重连指针），也可以用迭代（像迭代反转链表一样用 prev/curr/next 三指针推进）。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 递归（自底向上） | 递归到最左叶子，回溯时重连指针 | O(n) | O(h) 递归栈 | ⭐ 必须写出 |
| 迭代（链表反转） | 沿左子链迭代，用 prev/curr/next 重连 | O(n) | O(1) | 加分项 |

## 关键提示

1. **画出左子链**：把树沿着左子节点方向展开，发现它就是一条 "链"：`1 → 2 → 4`，每个节点挂着一个右兄弟。

2. **类比链表反转**：如果你会反转链表（LeetCode 206），那这道题就是"每次反转时还要额外把右兄弟接上"。

3. **递归的关键**：递归到最左叶子后，那就是新根。回溯时，`left->left = root->right`（原右兄弟变左孩子），`left->right = root`（原父变右孩子），然后**断开 root 的左右指针**。

4. **迭代的关键**：需要保存三样东西——当前节点、父节点、父节点的右孩子（因为向下移动后会丢失这些信息）。

5. **树的形状示意**（关键前提！）：
   ```
   题目保证的树形状（左子链 + 右叶子）:
        1
       / \
      2   3      ← 3 是叶子
     / \
    4   5        ← 5 是叶子
   
   等价于一条链: 1(right=3) → 2(right=5) → 4
   ```

## 解法详解

### 解法1: 递归（自底向上） — O(n) / O(h) ⭐ 面试首选

**思考过程**：树的问题优先考虑递归。翻转后新根是最左下角的节点，所以先递归到底找到新根，然后在回溯过程中重新连接指针。

对于每个节点 `root`，它的左孩子 `left` 在翻转后会成为 `root` 的父节点：
- `left->left = root->right`（原右兄弟变成 left 的左孩子）
- `left->right = root`（原父节点变成 left 的右孩子）
- 断开 `root->left` 和 `root->right`（避免形成环）

```
// 递归回溯过程 (示例 [1,2,3,4,5]):
//
// 递归下沉: root=1 → root=2 → root=4 (无左孩子，返回4作为新根)
//
// 回溯 root=2, left=4:
//   left->left = root->right = 5    →  4的左孩子 = 5
//   left->right = root = 2          →  4的右孩子 = 2
//   root->left = root->right = NULL →  断开2的指针
//   此时:   4
//          / \
//         5   2
//
// 回溯 root=1, left=2:
//   left->left = root->right = 3    →  2的左孩子 = 3
//   left->right = root = 1          →  2的右孩子 = 1
//   root->left = root->right = NULL →  断开1的指针
//   最终:   4
//          / \
//         5   2
//            / \
//           3   1
```

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
    TreeNode* upsideDownBinaryTree(TreeNode* root) {
        // 终止条件：空节点 或 叶子节点（最左下角就是新根）
        if (!root || !root->left) return root;
        
        // 递归到最左下角，拿到新根
        TreeNode* newRoot = upsideDownBinaryTree(root->left);
        
        // 回溯时重连指针
        // root->left 就是当前节点的左孩子，翻转后它成为父节点
        root->left->left = root->right;  // 原右兄弟 → 左孩子的左孩子
        root->left->right = root;        // 原父节点 → 左孩子的右孩子
        
        // 断开当前节点的指针，避免形成环
        root->left = nullptr;
        root->right = nullptr;
        
        return newRoot;  // 新根始终是最左下角的节点
    }
};
```

**关键点**：
- `newRoot` 在递归过程中一路传递回来，始终是最左下角的节点。
- 必须在重连后断开 `root->left` 和 `root->right`，否则会形成环导致无限循环。

---

### 解法2: 迭代（类似链表反转） — O(n) / O(1)

**从解法1优化**：递归解法使用 O(h) 的栈空间。既然这个过程类似链表反转，我们可以用迭代的方式，只需 O(1) 额外空间。

**核心思路**：沿着左子链迭代，维护三个变量：
- `curr`：当前处理的节点
- `prev`：上一个节点（翻转后 curr 的右孩子）
- `prevRight`：上一个节点的右孩子（翻转后 curr 的左孩子）

```
// 迭代过程 (示例 [1,2,3,4,5]):
//
// 初始: prev=NULL, prevRight=NULL, curr=1
//
// 第1轮: curr=1
//   next = curr->left = 2        (保存下一个)
//   currRight = curr->right = 3  (保存当前右孩子)
//   curr->left = prevRight = NULL
//   curr->right = prev = NULL
//   prev = curr = 1, prevRight = currRight = 3
//   curr = next = 2
//   状态: 1(left=NULL, right=NULL), 下一步处理2
//
// 第2轮: curr=2
//   next = curr->left = 4
//   currRight = curr->right = 5
//   curr->left = prevRight = 3   ← 1的右兄弟3变成2的左孩子
//   curr->right = prev = 1       ← 1变成2的右孩子
//   prev = 2, prevRight = 5
//   curr = 4
//   状态:   2
//          / \
//         3   1
//
// 第3轮: curr=4
//   next = curr->left = NULL
//   currRight = curr->right = NULL
//   curr->left = prevRight = 5   ← 2的右兄弟5变成4的左孩子
//   curr->right = prev = 2       ← 2变成4的右孩子
//   prev = 4, prevRight = NULL
//   curr = NULL → 退出循环
//   最终:   4
//          / \
//         5   2
//            / \
//           3   1
```

```cpp
class Solution {
public:
    TreeNode* upsideDownBinaryTree(TreeNode* root) {
        TreeNode* curr = root;
        TreeNode* prev = nullptr;       // 翻转后当前节点的右孩子
        TreeNode* prevRight = nullptr;  // 翻转后当前节点的左孩子
        
        while (curr) {
            // 先保存，因为马上要覆盖
            TreeNode* next = curr->left;          // 沿左子链的下一个节点
            TreeNode* currRight = curr->right;    // 当前节点的右孩子
            
            // 重连指针
            curr->left = prevRight;   // 上一个节点的右孩子 → 当前的左孩子
            curr->right = prev;       // 上一个节点 → 当前的右孩子
            
            // 推进
            prev = curr;
            prevRight = currRight;
            curr = next;
        }
        
        return prev;  // 循环结束时 prev 指向最后一个非空节点（即新根）
    }
};
```

**关键点**：
- 和链表反转的 `prev/curr/next` 模式完全一致，只是多了一个 `prevRight` 来传递右兄弟信息。
- 循环结束后 `prev` 就是新根（最左下角的节点），因为 `curr` 已经走到 `nullptr`。

## 解法对比

| | 递归 | 迭代 |
|---|---|---|
| 时间 | O(n) | O(n) |
| 空间 | O(h) 递归栈 | O(1) |
| 可读性 | 更直观，先递归到底再重连 | 需要理解链表反转类比 |
| 面试推荐 | ⭐ 先写这个，更容易讲清楚 | 追问"能否O(1)空间"时给出 |

## 易错点

1. **忘记断开 root 的指针（递归版）**
   - ✗ 重连 `root->left->left` 和 `root->left->right` 后就完事了
   - ✓ 必须加 `root->left = nullptr; root->right = nullptr;`
   - **为什么**：不断开会形成环。例如节点 2 的 right 指向 1，但如果 1 的 left 还指向 2，就形成了 1↔2 的环。

2. **递归终止条件写错**
   - ✗ `if (!root) return root;`（漏掉叶子节点的情况）
   - ✓ `if (!root || !root->left) return root;`
   - **为什么**：当 `root` 没有左孩子时，它就是最左下角的节点，即新根，应该直接返回。

3. **迭代版保存顺序搞混**
   - ✗ 先修改 `curr->left` 再读取 `curr->left` 作为 next
   - ✓ 必须**先保存** `next = curr->left` 和 `currRight = curr->right`，**再修改**指针
   - **为什么**：修改后原始信息就丢失了，这和链表反转先存 next 再改指针是同一个道理。

## 面试追问

**Q1（基础理解）**：这棵树有什么特殊结构？为什么普通二叉树不能这样翻转？
> 题目保证右节点都是叶子且都有左兄弟，所以树退化成一条左子链+每个节点挂一个右叶子。这让翻转过程变成类似链表反转。如果是普通二叉树，右子树可能有复杂结构，无法用同样的方式处理。

**Q2（空间优化）**：递归解法空间 O(h)，能否优化到 O(1)？
> 可以，用迭代解法。沿左子链迭代，用 prev/prevRight/curr 三个变量就能完成，和链表反转的迭代版完全类比。

**Q3（变体）**：如果不保证右节点是叶子（即右子树也有复杂结构），这道题还能做吗？
> 不能简单用同样的方法。因为翻转后原右子树要整体变成某个节点的左子树，但新的连接位置不再是简单的一对一关系。需要重新定义翻转规则，或者只能对满足特定条件的子树做局部翻转。

## 相关题型

- **206. 反转链表** — 本题迭代解法直接复用链表反转的 prev/curr/next 模式，区别是多了一个 prevRight 变量来传递右兄弟信息。
- **226. 翻转二叉树** — 同样是"翻转"操作，但 226 是左右互换（每个节点交换左右孩子），本题是上下翻转（改变父子关系）。226 用简单递归即可，本题需要类似链表反转的思路。
- **114. 二叉树展开为链表** — 同样是改变树的结构/指针指向。114 是把树展开成右链，本题是沿左链翻转。都需要仔细管理指针修改顺序。