# 404. 左叶子之和

## 核心思路

这道题本质上是一个**带条件的树遍历**：遍历整棵树，但只累加满足"是左孩子且是叶子"这两个条件同时成立的节点值。关键难点在于——一个节点自己不知道自己是不是"左孩子"，必须由**父节点**来判断。

## 思维链

1. **读完题第一反应**：遍历所有节点，找到左叶子，把值加起来。问题是怎么判断"左叶子"？
2. **什么是左叶子？** 需要同时满足两个条件：①它是某个节点的**左孩子** ②它自身是**叶子节点**（无左右孩子）。
3. **关键观察**：一个节点自己无法知道它是左孩子还是右孩子，所以判断必须在**父节点处**完成——当我发现 `node->left` 存在且 `node->left` 是叶子时，就累加它的值。
4. **选择遍历方式**：DFS（递归/迭代）或 BFS 都能遍历所有节点，任选一种，在遍历过程中做上述判断即可。
5. **递归最自然**：对于树的遍历，递归代码最简洁；迭代用栈/队列也能轻松实现。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DFS 递归 | 递归遍历，父节点判断左孩子是否为叶子 | O(n) | O(h) | ⭐ 必须写出 |
| DFS 迭代（栈） | 用栈模拟递归，同样逻辑 | O(n) | O(h) | 能说出即可 |
| BFS 层序遍历 | 用队列逐层遍历，同样判断逻辑 | O(n) | O(w) | 能说出即可 |

> h = 树高，w = 树最大宽度。最坏情况 h = n（链状），w = n/2（满二叉树最后一层）

## 关键提示

1. **判断"左叶子"的责任在父节点**，不在节点自身。如果你试图给递归函数加一个 `isLeft` 参数，也能工作，但不如直接在父节点处判断简洁。

2. **叶子节点判断**：`node->left == nullptr && node->right == nullptr`。

3. 用 ASCII 图理解示例：
```
//         3
//        / \
//       9   20
//           / \
//          15   7
//
// 节点3: 左孩子9是叶子 → 累加9 ✓
// 节点3: 右孩子20不是叶子 → 跳过
// 节点20: 左孩子15是叶子 → 累加15 ✓
// 节点20: 右孩子7是叶子但不是左孩子 → 跳过
// 结果: 9 + 15 = 24
```

4. **边界条件**：根节点本身如果是叶子（整棵树只有一个节点），它不算左叶子，返回 0。

5. 递归的终止条件：节点为空返回 0。不需要特殊处理叶子——因为叶子的左右子树递归都返回 0。

## 解法详解

### 解法1: DFS 递归（父节点判断） — O(n) / O(h) ⭐ 面试首选

**思考过程**：
- 对于每个节点，我检查它的左孩子是不是叶子。如果是，加上左孩子的值。
- 然后递归处理左子树和右子树，把结果加起来。
- 这是一种**前序遍历**的变体——在访问当前节点时做判断。

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
    int sumOfLeftLeaves(TreeNode* root) {
        if (!root) return 0;
        
        int sum = 0;
        // 关键判断：当前节点的左孩子存在且是叶子
        if (root->left && !root->left->left && !root->left->right) {
            sum += root->left->val;  // 父节点处累加左叶子的值
        }
        
        // 递归处理左右子树，收集更深层的左叶子
        sum += sumOfLeftLeaves(root->left);
        sum += sumOfLeftLeaves(root->right);
        
        return sum;
    }
};
```

**关键点**：判断逻辑写在父节点，不是在子节点。即使 `root->left` 是叶子被累加了，我们仍然要递归进入 `root->left`（虽然它会直接返回 0），代码逻辑是统一的。

---

### 解法2: DFS 递归（传参标记） — O(n) / O(h)

**另一种思路**：给递归函数加一个布尔参数 `isLeft`，让每个节点知道自己是不是左孩子。如果自己是左孩子且是叶子，就贡献自己的值。

```cpp
class Solution {
public:
    int sumOfLeftLeaves(TreeNode* root) {
        return dfs(root, false);  // 根节点不是任何人的左孩子
    }
    
    int dfs(TreeNode* node, bool isLeft) {
        if (!node) return 0;
        
        // 如果是叶子且是左孩子，返回自己的值
        if (!node->left && !node->right) {
            return isLeft ? node->val : 0;
        }
        
        // 左孩子传 true，右孩子传 false
        return dfs(node->left, true) + dfs(node->right, false);
    }
};
```

**关键点**：根节点即使是叶子，`isLeft=false`，所以正确返回 0。

---

### 解法3: DFS 迭代（栈） — O(n) / O(h)

**从递归到迭代**：用显式栈模拟递归过程，判断逻辑完全一样——在处理每个节点时检查其左孩子。

```cpp
class Solution {
public:
    int sumOfLeftLeaves(TreeNode* root) {
        if (!root) return 0;
        
        int sum = 0;
        stack<TreeNode*> stk;
        stk.push(root);
        
        while (!stk.empty()) {
            TreeNode* node = stk.top();
            stk.pop();
            
            // 和递归版完全相同的判断逻辑
            if (node->left) {
                if (!node->left->left && !node->left->right) {
                    sum += node->left->val;  // 左孩子是叶子
                } else {
                    stk.push(node->left);    // 左孩子不是叶子，继续探索
                }
            }
            if (node->right) {
                stk.push(node->right);
            }
        }
        
        return sum;
    }
};
```

**注意**：当左孩子是叶子时，我们直接累加它的值，不需要再把它入栈（因为叶子没有子节点可探索）。当然入栈也不会出错，只是多余操作。

---

### 解法4: BFS 层序遍历（队列） — O(n) / O(w)

**思路**：用队列逐层遍历，对每个出队的节点检查其左孩子是否为叶子。

```cpp
class Solution {
public:
    int sumOfLeftLeaves(TreeNode* root) {
        if (!root) return 0;
        
        int sum = 0;
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            TreeNode* node = q.front();
            q.pop();
            
            if (node->left) {
                if (!node->left->left && !node->left->right) {
                    sum += node->left->val;
                } else {
                    q.push(node->left);
                }
            }
            if (node->right) {
                q.push(node->right);
            }
        }
        
        return sum;
    }
};
```

## 解法对比

| | 解法1 递归(父判断) | 解法2 递归(传参) | 解法3 迭代(栈) | 解法4 BFS(队列) |
|---|---|---|---|---|
| 代码简洁度 | ⭐⭐⭐ 最简洁 | ⭐⭐ 需要辅助函数 | ⭐⭐ 较多代码 | ⭐⭐ 较多代码 |
| 空间 | O(h) 递归栈 | O(h) 递归栈 | O(h) 显式栈 | O(w) 队列 |
| 面试推荐 | ✅ 首选 | ✅ 思路清晰 | 面试官要求迭代时 | 面试官要求BFS时 |

**核心区别只在遍历方式不同，判断逻辑完全一样。** 面试中写解法1即可，如果追问迭代版本，切换到解法3。

## 易错点

1. **把右叶子也算进去了**
   - ✗ `if (node->left && isLeaf(node->left)) sum += node->left->val; if (node->right && isLeaf(node->right)) sum += node->right->val;`
   - ✓ 只对 `node->left` 做叶子判断并累加，`node->right` 只需要递归进去，不累加。

2. **叶子判断少了一个条件**
   - ✗ `if (!node->left->left)` 只检查了左，没检查右
   - ✓ `if (!node->left->left && !node->left->right)` 左右都为空才是叶子

3. **根节点是唯一节点时返回了 root->val**
   - ✗ 根节点虽然是叶子但不是"左叶子"（它不是任何节点的左孩子）
   - ✓ 应返回 0。解法1 自然处理了这种情况（root->left 为空，跳过判断）

4. **迭代版中叶子节点入栈导致重复**
   - 不会出错但效率低。当左孩子是叶子时，可以直接累加而不入栈。

## 面试追问

**Q1: 如果要求返回所有"右叶子"之和呢？**
→ 只需把判断条件从 `node->left` 改成 `node->right`：检查 `node->right` 存在且为叶子，累加 `node->right->val`。

**Q2: 如果要求返回所有叶子节点之和（不区分左右）呢？**
→ 更简单了，不需要父节点判断。直接在递归中：如果当前节点是叶子 `(!node->left && !node->right)`，返回 `node->val`。

**Q3: 如果要求返回"第 k 层的左叶子之和"呢？**
→ BFS 层序遍历最直接，用 level 变量追踪当前层数，只在第 k 层做左叶子判断。或者 DFS 传入 depth 参数。

**Q4: 能否用 O(1) 额外空间？（Morris 遍历）**
→ 可以用 Morris 遍历实现 O(1) 空间，但需要在遍历过程中额外判断左叶子条件，实现复杂，面试中一般不要求。

## 相关题型

- **112. 路径总和 (Path Sum)** — 同样是树的遍历+条件判断，区别：112 判断的是根到叶子路径和，本题判断的是左叶子。复用同样的递归框架。
- **129. 求根节点到叶节点数字之和** — 也是遍历到叶子节点时做特殊处理，但需要在路径中累积数字。复用"到叶子节点时结算"的模式。
- **513. 找树左下角的值** — 同样涉及"左"和"叶子"的概念，但要求的是最底层最左边的值，用 BFS 层序遍历最直接。
- **257. 二叉树的所有路径** — 练习树的 DFS 遍历，在叶子节点处做特殊处理的通用模式。