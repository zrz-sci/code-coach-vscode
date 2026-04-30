# 226. 翻转二叉树

## 核心思路

翻转二叉树的本质就是：**对每个节点，交换它的左右子树**。这是一个天然的递归问题——如果你能翻转一个子树，那就能翻转整棵树。

## 思维链

1. **读完题第一反应**：翻转 = 镜像，每个节点的左右孩子互换。看示例验证：根节点4不动，2和7互换，1和3互换，6和9互换 —— 确实是对每一层都做了左右交换。

2. **这是一个什么结构的问题？** 树的操作 → 自然想到递归。对于根节点，我只要交换左右子树，然后分别翻转左右子树即可。

3. **递归三要素想清楚**：
   - 终止条件：节点为空，返回 nullptr
   - 单层逻辑：交换当前节点的 left 和 right
   - 返回值：返回当前节点（翻转后的子树根）

4. **递归用什么序？** 前序（先交换再递归）、后序（先递归再交换）都可以。中序要小心——交换后左右孩子身份变了，容易重复处理。

5. **能否迭代？** 当然可以。用栈(DFS)或队列(BFS)遍历每个节点，对每个节点交换左右孩子即可。

6. **总结**：这道题的关键洞察是"翻转 = 递归地交换每个节点的左右子树"，不需要任何复杂的数据结构。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 递归 (DFS) | 对每个节点递归交换左右子树 | O(n) | O(h) | ⭐ 必须写出 |
| 迭代 DFS (栈) | 用栈模拟递归，遍历每个节点并交换 | O(n) | O(n) | 能说出即可 |
| 迭代 BFS (队列) | 层序遍历每个节点并交换 | O(n) | O(n) | 能说出即可 |

> h 为树高，最坏 O(n)（退化链表），平均 O(log n)

## 关键提示

- **提示1**：翻转不是"移动节点到新位置"，而是对每个节点做同一个操作：swap(left, right)。
- **提示2**：画出翻转前后的树，观察每个节点发生了什么变化：
```
//  翻转前:              翻转后:
//       4                    4
//      / \                  / \
//     2   7                7   2
//    / \ / \              / \ / \
//   1  3 6  9            9  6 3  1
```
- **提示3**：递归翻转时，前序和后序都可以，但中序需要特别注意 —— 想想为什么？
- **提示4**：如果你能写出递归版，试试用栈模拟同样的过程。
- **提示5**：空树也是合法输入，别忘了处理。

## 解法详解

### 解法1: 递归 (DFS 前序) — O(n) / O(h) ⭐ 面试首选

**思考过程**：树的问题首先想递归。翻转一棵树 = 交换根的左右子树 + 翻转左子树 + 翻转右子树。这天然就是一个前序遍历的思路：先处理当前节点（交换），再递归处理子树。

```
// 递归过程演示 (前序：先交换再递归):
//
//  Step1: 处理节点4         Step2: 处理节点7(原右)    Step3: 处理节点9
//       4                        4                        4
//      / \  swap(2,7)           / \                      / \
//     7   2  ─────→            7   2                    7   2
//    / \ / \                  / \ / \  swap(9,6)       / \ / \
//   6  9 1  3                6  9 1  3  ────→         9  6 1  3
//                                                     ✓
//  Step4: 处理节点6         Step5: 处理节点2          Step6: 处理节点3,1
//  (叶子，无操作)           swap(1,3)                 (叶子，无操作)
//       4                        4                        4
//      / \                      / \                      / \
//     7   2                    7   2                    7   2
//    / \ / \                  / \ / \                  / \ / \
//   9  6 1  3               9  6 3  1                9  6 3  1 ✓
//      ✓                             
```

```cpp
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        // 终止条件：空节点无需翻转
        if (!root) return nullptr;
        
        // 交换当前节点的左右子树（前序：先处理当前节点）
        swap(root->left, root->right);
        
        // 递归翻转左右子树
        invertTree(root->left);
        invertTree(root->right);
        
        return root;
    }
};
```

**关键点**：
- 递归终止条件是 `!root`（空节点），不是 `!root->left && !root->right`（叶子节点）。叶子节点交换两个 nullptr 不会出错，但空指针解引用会崩溃。
- 前序和后序都可以。后序就是先递归再交换：

```cpp
// 后序版本：先翻转子树，再交换
TreeNode* invertTree(TreeNode* root) {
    if (!root) return nullptr;
    invertTree(root->left);
    invertTree(root->right);
    swap(root->left, root->right);  // 后序：最后交换
    return root;
}
```

### 解法2: 迭代 DFS (栈) — O(n) / O(n)

**从解法1出发**：递归本质上用了系统调用栈。我们可以用显式栈来模拟同样的过程：遍历每个节点，交换其左右子树。

```
// 栈模拟过程:
// 初始: stack = [4]
// Pop 4, swap(2,7), push 7,2  → stack = [7, 2]
// Pop 2, swap(1,3), push 3,1  → stack = [7, 3, 1]
// Pop 1, 叶子无操作           → stack = [7, 3]
// Pop 3, 叶子无操作           → stack = [7]
// Pop 7, swap(6,9), push 9,6  → stack = [9, 6]
// Pop 6, 叶子无操作           → stack = [9]
// Pop 9, 叶子无操作           → stack = []  → 结束
```

```cpp
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return nullptr;
        
        stack<TreeNode*> stk;
        stk.push(root);
        
        while (!stk.empty()) {
            TreeNode* node = stk.top();
            stk.pop();
            
            // 对当前节点交换左右子树
            swap(node->left, node->right);
            
            // 将非空子节点入栈，继续处理
            if (node->left)  stk.push(node->left);
            if (node->right) stk.push(node->right);
        }
        
        return root;
    }
};
```

**关键点**：入栈顺序无所谓（先左后右或先右后左都行），因为我们只需要保证每个节点都被访问并交换一次，不关心遍历顺序。

### 解法3: 迭代 BFS (队列) — O(n) / O(n)

**思路**：BFS 层序遍历，对每个出队的节点交换左右子树。和栈的版本几乎一样，只是换成了队列。

```
// BFS 层序过程:
// Level 0: queue = [4]
//   处理4: swap(2,7) → queue = [7, 2]
// Level 1: queue = [7, 2]
//   处理7: swap(6,9) → push 9,6
//   处理2: swap(1,3) → push 3,1
//   → queue = [9, 6, 3, 1]
// Level 2: 全是叶子，无子节点入队
// 结束
```

```cpp
class Solution {
public:
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return nullptr;
        
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            TreeNode* node = q.front();
            q.pop();
            
            // 交换当前节点的左右子树
            swap(node->left, node->right);
            
            if (node->left)  q.push(node->left);
            if (node->right) q.push(node->right);
        }
        
        return root;
    }
};
```

## 解法对比

| | 递归 DFS | 迭代 DFS (栈) | 迭代 BFS (队列) |
|---|---|---|---|
| 时间 | O(n) | O(n) | O(n) |
| 空间 | O(h) 递归栈 | O(n) 显式栈 | O(n) 队列 |
| 代码量 | 最少，5行 | 中等 | 中等 |
| 面试推荐 | ⭐ 首选 | 追问时展示 | 追问时展示 |
| 适用场景 | 几乎所有情况 | 树很深怕栈溢出 | 需要层序处理时 |

**选择建议**：面试默认写递归，如果面试官追问"能否不用递归"再给迭代版。三种解法的核心都是一样的：**遍历每个节点 + 交换左右子树**，区别只在遍历方式。

## 易错点

- **✗ 中序递归不小心重复翻转**：
  ```cpp
  invertTree(root->left);
  swap(root->left, root->right);
  invertTree(root->right);  // ✗ 此时 right 是原来的 left（已经翻转过了），又翻转一次！
  ```
  ✓ 如果用中序，第二次递归应该是 `invertTree(root->left)`（因为交换后原右子树到了左边）：
  ```cpp
  invertTree(root->left);
  swap(root->left, root->right);
  invertTree(root->left);  // ✓ 交换后，原来的右子树现在在左边
  ```
  **建议**：直接用前序或后序，避免中序的这个陷阱。

- **✗ 终止条件写成叶子节点判断**：
  ```cpp
  if (!root->left && !root->right) return root;  // ✗ 没判断 root 本身为空！
  ```
  ✓ 正确写法：
  ```cpp
  if (!root) return nullptr;  // ✓ 空节点直接返回
  ```

- **✗ 交换后还用原变量名**（逻辑混乱）：
  ```cpp
  swap(root->left, root->right);
  invertTree(root->left);   // 这里的 left 其实是原来的 right
  ```
  虽然代码是对的，但注释别写错。递归调用的是"交换后的"左右子树。

## 面试追问

**Q1: 递归用前序还是后序？中序行不行？**
> 前序和后序都可以，效果一样。中序需要特别处理（交换后左右身份变了），容易出bug，不推荐。

**Q2: 如果树非常深（百万层），递归会栈溢出，怎么办？**
> 用迭代版（栈或队列）。显式栈分配在堆上，不受系统调用栈大小限制。

**Q3: 能否原地翻转？需要额外空间吗？**
> 递归本身就是原地修改节点指针，没有创建新节点。但递归/迭代都需要 O(h) 或 O(n) 的遍历开销（栈/队列空间），这是不可避免的——你必须访问每个节点。

**Q4: 如果只翻转某一层的子树呢？（变体）**
> 用 BFS 层序遍历，只在目标层做交换操作。

## 相关题型

- **[100. 相同的树](https://leetcode.com/problems/same-tree/)** — 复用同样的"双指针递归"框架，区别：226 是单树操作（交换），100 是双树比较（判等）
- **[101. 对称二叉树](https://leetcode.com/problems/symmetric-binary-tree/)** — 对称 = 翻转后和自身相同。可以先翻转再比较，也可以直接交叉递归判断。复用 226 的翻转 + 100 的比较
- **[104. 二叉树的最大深度](https://leetcode.com/problems/maximum-depth-of-binary-tree/)** — 同样的递归框架（终止条件 + 递归左右 + 合并结果），只是单层逻辑不同：226 是交换，104 是取 max+1
- **[114. 二叉树展开为链表](https://leetcode.com/problems/flatten-binary-tree-to-linked-list/)** — 同样是修改树结构的递归题，但更复杂，需要后序处理