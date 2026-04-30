# 450. 删除二叉搜索树中的节点

## 核心思路

本题本质上是：**在 BST 中找到目标节点后，根据它的子树情况（0/1/2个孩子），用合适的替代节点接上去，保持 BST 性质不变。**

核心难点不在"找"，而在"删了之后怎么补"——特别是当被删节点有两个孩子时，需要找到中序前驱或后继来替代。

## 思维链

1. **读完题第一反应**：BST 的删除是数据结构课的经典操作。先找到节点，然后删除它。找节点很简单——利用 BST 性质，比 root 小往左走，比 root 大往右走，O(h) 就能定位。

2. **难点在哪？** 找到之后怎么删？直接去掉这个节点，它的子树怎么办？需要分情况讨论：
   - 叶子节点（无孩子）：直接删
   - 只有一个孩子：让孩子顶上来
   - **有两个孩子**：这是最复杂的情况，不能简单地让某个孩子顶上来

3. **两个孩子的情况怎么处理？** 我们需要找一个"替身"——它必须能同时满足"比左子树所有节点大、比右子树所有节点小"。谁满足这个条件？
   - **右子树的最小节点**（中序后继）：它比当前节点大，但比右子树其他所有节点都小
   - **左子树的最大节点**（中序前驱）：同理

4. **具体操作**：找到右子树的最小节点（一路往左走到底），用它的值替换被删节点的值，然后递归地在右子树中删除这个最小节点（这个最小节点最多只有右孩子，退化为简单情况）。

5. **递归 vs 迭代**：递归写法最自然——`deleteNode` 返回修改后的子树根节点，利用递归返回值自动接好指针。迭代需要手动维护父节点指针，代码更复杂但空间 O(1)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 递归（返回值接指针） | 递归查找 + 分3种情况删除 | O(h) | O(h) 递归栈 | ⭐ 必须写出 |
| 迭代 | 迭代查找 + 手动维护父指针 | O(h) | O(1) | 加分项 |

## 关键提示

- **提示1**：BST 删除的核心是分三种情况——被删节点有 0 个、1 个、2 个孩子。先画图把三种情况都想清楚再写代码。

- **提示2**：递归写法的精髓：让 `deleteNode` 的返回值表示"处理完删除后，这棵子树的新根是谁"。这样父节点只需要 `root->left = deleteNode(root->left, key)` 就自动接好了。

- **提示3**：两个孩子的情况，找右子树最小节点（中序后继）时，就是从 `root->right` 开始一路 `->left` 到底。

- **提示4**：别忘了 key 不存在于树中的情况——递归到 `nullptr` 直接返回即可。

- **可视化：删除节点3的三种策略**：
```
    原始树:              删除3(两个孩子):
        5                 策略A: 用右子树最小值4替代
       / \                    5
      3   6                  / \
     / \   \                4   6
    2   4   7              /     \
                          2       7
                          
                          策略B: 用左子树最大值2替代
                              5
                             / \
                            2   6
                             \   \
                              4   7
```

## 解法详解

### 解法1: 递归（返回值重接指针）— O(h) / O(h) ⭐ 面试首选

**思考过程**：BST 的递归结构天然适合递归处理。关键洞察：`deleteNode(root, key)` 返回删除后的子树根节点。这样，搜索过程中：
- `key < root->val`：去左子树删，`root->left = deleteNode(root->left, key)`
- `key > root->val`：去右子树删，`root->right = deleteNode(root->right, key)`
- `key == root->val`：执行删除逻辑

找到目标节点后分三种情况：

```
// 情况1: 叶子节点 → 返回 nullptr
//    5            5
//   / \    删4   / \
//  3   6  →    3   6
//   \   \       \   \
//    4   7       X   7
//
// 情况2: 只有一个孩子 → 返回那个孩子
//    5            5
//   / \    删3   / \
//  3   6  →    2   6
// /     \           \
// 2      7           7
//
// 情况3: 两个孩子 → 找右子树最小值替代，再递归删那个最小值
//    5            5
//   / \    删3   / \
//  3   6  →    4   6      (4是3的右子树最小值)
// / \   \     /     \
// 2  4   7   2       7
```

```cpp
class Solution {
public:
    TreeNode* deleteNode(TreeNode* root, int key) {
        if (!root) return nullptr;  // key 不存在
        
        if (key < root->val) {
            // key 在左子树，递归删除后重新接上
            root->left = deleteNode(root->left, key);
        } else if (key > root->val) {
            // key 在右子树
            root->right = deleteNode(root->right, key);
        } else {
            // 找到了要删除的节点，分三种情况
            
            // 情况1 & 2: 无左孩子（包括叶子），让右孩子顶上
            if (!root->left) return root->right;
            // 情况2: 无右孩子，让左孩子顶上
            if (!root->right) return root->left;
            
            // 情况3: 左右孩子都有
            // 找右子树的最小节点（中序后继）
            TreeNode* successor = root->right;
            while (successor->left) {
                successor = successor->left;
            }
            // 用后继的值替换当前节点
            root->val = successor->val;
            // 在右子树中递归删除这个后继节点
            root->right = deleteNode(root->right, successor->val);
        }
        return root;
    }
};
```

**关键点**：
- 情况1（叶子）和情况2（一个孩子）其实可以合并：`if (!root->left) return root->right;` 当叶子节点时 `root->right` 也是 `nullptr`，正好。
- 情况3 中，后继节点一定没有左孩子（否则它就不是最小的了），所以递归删除后继时会退化为情况1或情况2，不会无限递归。

### 解法2: 迭代 — O(h) / O(1)

**从解法1优化**：递归栈深度为 O(h)，进阶要求 O(h) 时间 + O(1) 空间。迭代需要手动维护父节点指针，代码更长但空间更优。

**思路**：
1. 迭代找到目标节点及其父节点
2. 执行删除（同样分三种情况）
3. 需要特别处理删除的是根节点的情况

```cpp
class Solution {
public:
    TreeNode* deleteNode(TreeNode* root, int key) {
        // 第一步：找到目标节点和它的父节点
        TreeNode* parent = nullptr;
        TreeNode* curr = root;
        while (curr && curr->val != key) {
            parent = curr;
            if (key < curr->val) curr = curr->left;
            else curr = curr->right;
        }
        if (!curr) return root;  // key 不存在
        
        // 第二步：执行删除
        // 情况3: 两个孩子 → 先转化为情况1或2
        if (curr->left && curr->right) {
            // 找右子树最小节点（中序后继）及其父节点
            TreeNode* succParent = curr;
            TreeNode* succ = curr->right;
            while (succ->left) {
                succParent = succ;
                succ = succ->left;
            }
            // 值替换
            curr->val = succ->val;
            // 问题转化为：删除 succ 节点（它最多一个右孩子）
            curr = succ;
            parent = succParent;
        }
        
        // 此时 curr 最多一个孩子
        TreeNode* child = curr->left ? curr->left : curr->right;
        
        if (!parent) {
            // 删除的是根节点
            return child;
        }
        // 将 child 接到 parent 上
        if (parent->left == curr) {
            parent->left = child;
        } else {
            parent->right = child;
        }
        
        return root;
    }
};
```

**关键点**：
- 迭代写法要单独处理"删除根节点"的情况（`parent == nullptr`）
- 两个孩子的情况先做值替换，把问题转化成删除后继节点（最多一个孩子），统一后续处理

## 解法对比

| | 递归 | 迭代 |
|---|---|---|
| 时间 | O(h) | O(h) |
| 空间 | O(h) 递归栈 | O(1) |
| 代码量 | 短，逻辑清晰 | 长，需手动维护父指针 |
| 面试推荐 | ⭐ 首选，简洁易写 | 追问空间优化时给出 |
| 核心技巧 | 返回值自动重接指针 | 手动维护 parent 指针 |

**选择建议**：面试中先写递归版本，如果面试官追问空间优化再给迭代版。递归版不容易出 bug，迭代版容易在 parent 指针处理上犯错。

## 易错点

1. **✗ 忘记处理 key 不存在的情况**
   ```cpp
   // 错误：递归到 nullptr 没有返回
   TreeNode* deleteNode(TreeNode* root, int key) {
       if (key < root->val) ...  // root 可能是 nullptr!
   }
   ```
   **✓ 正确**：第一行就要 `if (!root) return nullptr;`

2. **✗ 情况3中直接删除后继节点而不是递归**
   ```cpp
   // 错误：找到后继后直接 delete，但后继可能还有右子树！
   successor->parent->left = nullptr;  // 后继的右子树丢了！
   ```
   **✓ 正确**：`root->right = deleteNode(root->right, successor->val);` 递归处理确保后继的右子树被正确接上。

3. **✗ 迭代版中找后继时 succParent 初始化错误**
   ```cpp
   TreeNode* succParent = nullptr;  // 错！如果 curr->right 没有左子树，
                                     // succ 就是 curr->right，parent 应该是 curr
   ```
   **✓ 正确**：`TreeNode* succParent = curr;`

4. **✗ 将情况1和情况2分开写导致遗漏**
   ```cpp
   if (!root->left && !root->right) return nullptr;      // 叶子
   if (!root->left) return root->right;                   // 只有右孩子
   if (!root->right) return root->left;                   // 只有左孩子
   // 其实前两行就包含了叶子的情况，第一行多余
   ```
   **✓ 简洁写法**：`if (!root->left) return root->right; if (!root->right) return root->left;` 两行即可覆盖情况1和2。

## 面试追问

**Q1: 基础理解** — 删除 BST 节点有几种情况？每种怎么处理？
> 三种：(1) 叶子直接删；(2) 一个孩子，孩子顶上；(3) 两个孩子，找中序后继（或前驱）的值替换，然后递归删除后继。

**Q2: 为什么用"值替换"而不是"指针操作"？**
> 值替换只需改一个值然后递归删除，代码简洁。纯指针操作需要处理后继节点的父子关系、被删节点的父子关系，容易出错。但值替换的缺点是如果节点中存储了额外数据（如指向该节点的外部指针），替换值会导致这些引用失效。

**Q3: 能否 O(1) 空间？**
> 可以，用迭代解法（解法2），手动维护 parent 指针，不需要递归栈。

**Q4: 如果要频繁删除怎么优化？**
> 普通 BST 在最坏情况下退化为链表，h = O(n)。如果需要频繁插入/删除并保持 O(log n)，应使用自平衡 BST（如 AVL 树、红黑树）。

**Q5: 用前驱替代 vs 用后继替代有区别吗？**
> 功能上等价。但如果总是用后继（或总是用前驱），在大量删除后树可能变得不平衡。一种优化是交替使用前驱和后继。

## 相关题型

- **700. 二叉搜索树中的搜索** — 本题的第一步"查找"就是 700 题，复用 BST 的二分搜索逻辑
- **701. 二叉搜索树中的插入操作** — 同样利用"递归返回值重接指针"的框架，区别：插入只需找到 nullptr 位置创建新节点，不需要分情况讨论
- **98. 验证二叉搜索树** — 理解 BST 性质的基础题，本题删除后必须保持这个性质
- **230. 二叉搜索树中第K小的元素** — 同样利用 BST 中序遍历有序的性质，本题找后继时也是在利用这一点
- **669. 修剪二叉搜索树** — 同样使用"递归返回值重接指针"的技巧，区别是裁剪范围外的所有节点而不是单个节点