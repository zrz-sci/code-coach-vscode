# 111. 二叉树的最小深度

## 核心思路

本质上是求从根节点到**最近的叶子节点**的路径长度。关键陷阱：**只有一个子节点为空时，该节点不是叶子节点**，不能简单地取左右子树深度的最小值。

## 思维链

1. **读完题第一反应**：和"最大深度"（104题）很像，递归取左右子树的最小值？→ 但这样会出错！
2. **发现陷阱**：如果一个节点只有右子树没有左子树，`min(0, rightDepth)` 会返回 0，但该节点不是叶子节点，最小深度应该走右子树那边。
3. **DFS 修正**：递归时必须特判——当左子树为空时走右子树，当右子树为空时走左子树，都不为空时才取 min。
4. **换个角度——BFS**：层序遍历，**第一个遇到的叶子节点所在层就是最小深度**。BFS 天然适合求"最短"问题，且可以提前终止。
5. **对比选择**：DFS 必须遍历整棵树，BFS 可以在找到第一个叶子时就停下来，对于极不平衡的树 BFS 更优。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DFS 递归 | 递归求左右子树最小深度，特判单子树情况 | O(n) | O(h) | ⭐ 必须写出 |
| BFS 层序 | 逐层遍历，第一个叶子所在层即答案 | O(n) | O(w) | ⭐ 必须写出 |
| DFS 迭代 | 用栈模拟 DFS，维护全局最小深度 | O(n) | O(h) | 能说出即可 |

> h = 树高，w = 树的最大宽度。最坏情况 h = n（退化链表），w = n/2（完美二叉树）。

## 关键提示

- **提示1**：叶子节点的定义是**左右子节点都为空**。当一个节点只有一个孩子时，它不是叶子！这是本题和"最大深度"最关键的区别。

- **提示2**：画一下示例2的树，思考为什么答案是5而不是1：
```
//       2
//        \
//         3
//          \
//           4
//            \
//             5
//              \
//               6    ← 唯一的叶子节点在第5层
```
如果简单 `min(left, right)`，根节点左子树为空返回0，最终答案变成1，大错特错。

- **提示3**：BFS 的优势在于"最短路径"问题可以提前终止——找到第一个叶子就不用继续了。

- **提示4**：思考递归终止条件：`root == nullptr` 返回什么？如果 root 是叶子返回什么？如果只有一侧子树呢？

- **提示5**：对比 104 题"最大深度"，核心代码只改了一处逻辑——处理单侧子树为空的情况。

## 解法详解

### 解法1: DFS 递归 — O(n) / O(h) ⭐ 面试首选

**思考过程**：

自底向上递归。对于每个节点，我们想知道"从这个节点到最近叶子的距离"。
- 如果是空节点，返回 0
- 如果是叶子节点（无左无右），返回 1
- 如果**只有右子树**，最小深度只能走右边：`1 + minDepth(right)`
- 如果**只有左子树**，最小深度只能走左边：`1 + minDepth(left)`
- 如果**两边都有**，取较小的：`1 + min(minDepth(left), minDepth(right))`

```
//  示例1:                    示例2:
//       3                         2
//      / \                         \
//     9   20                        3
//        /  \                        \
//       15   7                        4
//                                      \
//  节点9: 叶子→返回1                     5
//  节点15: 叶子→返回1                      \
//  节点7: 叶子→返回1                        6
//  节点20: min(1,1)+1=2
//  节点3: min(1,2)+1=2       节点2: 左空→只走右→1+4=5
//  答案: 2                   答案: 5
```

```cpp
class Solution {
public:
    int minDepth(TreeNode* root) {
        if (!root) return 0;
        
        int left = minDepth(root->left);
        int right = minDepth(root->right);
        
        // 关键：如果某一侧为空，不能取 min（那一侧返回0不代表叶子）
        // 必须走有子树的那一侧
        if (!root->left)  return 1 + right;   // 左空，只能走右
        if (!root->right) return 1 + left;    // 右空，只能走左
        
        // 两边都有子树，取较小值
        return 1 + min(left, right);
    }
};
```

**关键点**：两个 `if` 判断是本题的灵魂。没有这两行，就会把"单侧子树为空"误判为深度0。


### 解法2: BFS 层序遍历 — O(n) / O(w) ⭐ 面试首选

**从解法1的特点出发**：DFS 必须遍历完整棵树才能确定最小深度。而 BFS 逐层扫描，**第一个遇到的叶子节点所在的层数就是答案**，可以提前返回。

对于极度不平衡的树（如示例2的链状结构），DFS 和 BFS 都要遍历所有节点。但如果树比较"宽"且最浅的叶子在左侧，BFS 能很快找到答案。

```
//  BFS 逐层遍历示例1:
//  
//  Level 1: [3]          → 3 不是叶子（有两个孩子）
//  Level 2: [9, 20]      → 9 是叶子！→ 返回 depth = 2
//  （不需要遍历 Level 3）
```

```cpp
class Solution {
public:
    int minDepth(TreeNode* root) {
        if (!root) return 0;
        
        queue<TreeNode*> q;
        q.push(root);
        int depth = 0;
        
        while (!q.empty()) {
            depth++;
            int size = q.size();  // 当前层的节点数
            
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front();
                q.pop();
                
                // 第一个叶子节点 → 当前层就是最小深度
                if (!node->left && !node->right) {
                    return depth;
                }
                
                if (node->left)  q.push(node->left);
                if (node->right) q.push(node->right);
            }
        }
        
        return depth;  // 理论上不会到这里（root非空时）
    }
};
```

**关键点**：BFS 找到的第一个叶子就是最浅的叶子，无需再继续搜索。


### 解法3: DFS 迭代（栈模拟）— O(n) / O(h)

**思考过程**：用显式栈模拟 DFS，对每个节点记录其深度。遇到叶子节点时更新全局最小值。

```cpp
class Solution {
public:
    int minDepth(TreeNode* root) {
        if (!root) return 0;
        
        stack<pair<TreeNode*, int>> stk;  // {节点, 深度}
        stk.push({root, 1});
        int minDep = INT_MAX;
        
        while (!stk.empty()) {
            auto [node, depth] = stk.top();
            stk.pop();
            
            // 叶子节点：更新最小深度
            if (!node->left && !node->right) {
                minDep = min(minDep, depth);
                continue;  // 剪枝：叶子节点无需继续
            }
            
            // 只在 depth < minDep 时才继续探索（剪枝）
            if (node->right && depth + 1 < minDep) {
                stk.push({node->right, depth + 1});
            }
            if (node->left && depth + 1 < minDep) {
                stk.push({node->left, depth + 1});
            }
        }
        
        return minDep;
    }
};
```

**关键点**：添加 `depth + 1 < minDep` 剪枝，避免探索比当前已知最小深度更深的分支。


## 解法对比

| | DFS 递归 | BFS 层序 | DFS 迭代 |
|---|---|---|---|
| **时间** | O(n) 遍历全树 | O(n) 最坏全树，但可提前终止 | O(n) 遍历全树 |
| **空间** | O(h) 递归栈 | O(w) 队列宽度 | O(h) 显式栈 |
| **提前终止** | ✗ 必须遍历完 | ✓ 第一个叶子即停 | 有限剪枝 |
| **代码简洁度** | 最简洁 | 中等 | 较复杂 |
| **适合场景** | 通用 | 树较宽、最浅叶子浅 | 需要迭代且要剪枝 |

**面试建议**：先写 DFS 递归（简洁、体现递归思维），然后主动提出 BFS 的优势（提前终止）。

## 易错点

- **✗ 直接取 min(left, right) + 1**
  ```cpp
  // 错误写法：
  return 1 + min(minDepth(root->left), minDepth(root->right));
  ```
  **→ ✓ 必须特判单侧子树为空的情况**
  ```cpp
  // 正确写法：
  if (!root->left)  return 1 + minDepth(root->right);
  if (!root->right) return 1 + minDepth(root->left);
  return 1 + min(minDepth(root->left), minDepth(root->right));
  ```
  **原因**：`root->left` 为空时 `minDepth(root->left)` 返回 0，但根节点不是叶子，0 不代表"有一条到叶子的路径"。

- **✗ 忘记处理空树**
  ```cpp
  // 错误：root 为 nullptr 时直接访问 root->left 段错误
  ```
  **→ ✓ 函数开头 `if (!root) return 0;`**

- **✗ BFS 中判断叶子时搞混条件**
  ```cpp
  // 错误：只判断 node->left == nullptr（那只是没有左子树，不是叶子）
  if (!node->left) return depth;
  ```
  **→ ✓ 叶子 = 左右都为空**
  ```cpp
  if (!node->left && !node->right) return depth;
  ```

## 面试追问

**Q1: 这题和 104（最大深度）有什么区别？代码改了哪里？**
> 最大深度可以直接 `max(left, right) + 1`，因为取 max 时空子树的 0 不影响结果。最小深度必须特判空子树，因为 `min(0, x)` 总是 0，但空子树不代表叶子路径。

**Q2: DFS 必须遍历整棵树，BFS 能提前终止。还有什么方法能让 DFS 也提前终止？**
> DFS 迭代版本可以添加剪枝：维护一个 `minDep` 变量，当当前深度已经 >= `minDep` 时不再继续深入。但这只是局部剪枝，整体上 BFS 的提前终止效果更好。

**Q3: 如果这棵树不是存在内存中，而是以层序方式流式输入（每次给一层的节点），怎么求最小深度？**
> 天然适合 BFS 思路：逐层接收，每层检查是否有叶子节点，找到第一个叶子所在的层即可停止接收后续数据。

## 相关题型

- **104. 二叉树的最大深度** — 同样的递归框架，区别在于 max 不需要特判空子树，而本题的 min 需要特判。**核心差异就是两行 `if` 判断**。
- **110. 平衡二叉树** — 复用自底向上求高度的递归框架，额外检查左右高度差 ≤ 1。
- **559. N 叉树的最大深度** — 从二叉树推广到 N 叉树，递归结构一样，只是遍历所有 children 而非 left/right。
- **102. 二叉树的层序遍历** — BFS 模板完全一致，本题只是在层序遍历中加了一个叶子判断提前返回。