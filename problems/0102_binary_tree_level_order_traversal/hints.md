# 102. 二叉树的层序遍历

## 核心思路

本质上就是 **BFS（广度优先搜索）** 的直接应用：逐层访问树的节点，每层从左到右收集。关键在于如何区分"哪些节点属于同一层"。

## 思维链

1. **读完题第一反应**：要逐层遍历，最自然的想法就是用队列——先进先出正好对应"先访问的节点，其子节点也先被访问"。
2. **核心难点**：普通 BFS 只是把所有节点按顺序取出，但题目要求按"层"分组。怎么知道一层结束了？
3. **突破口**：在处理每一层之前，先记录当前队列的长度 `size`，这个 `size` 就是当前层的节点数。只处理 `size` 个节点，剩下的都是下一层的。
4. **另一个角度**：也可以用 DFS/递归，只要给每个节点带上"深度"信息，就能把它放到对应层的数组里。
5. **总结**：BFS + 按层计数是最直觉的做法；DFS + 深度标记是另一种等价思路。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| BFS + 队列 | 逐层处理，用 size 划分层 | O(n) | O(n) | ⭐ 必须写出 |
| DFS 递归 | 带深度参数递归，按深度分层 | O(n) | O(n) | 加分项 |

## 关键提示

- 提示1：BFS 天然就是逐层扩展的，关键是怎么知道"一层处理完了"？→ 想想处理某一层时队列里都是谁。
- 提示2：处理第 k 层开始时，队列里恰好只包含第 k 层的所有节点。此时 `queue.size()` 就是这一层的节点数。
- 提示3：如果用 DFS，你需要什么额外信息来判断一个节点属于第几层？→ 深度/层号。
- 提示4：DFS 时，如果当前深度 `depth` 等于 `result.size()`，说明这是第一次访问这一层，需要先创建一个新的空数组。

```
// 示例树结构:
//        3          ← 第0层: [3]
//       / \
//      9   20       ← 第1层: [9, 20]
//         / \
//        15   7     ← 第2层: [15, 7]
//
// BFS 队列变化过程:
// 初始:    queue = [3]           → size=1, 处理1个
// 第0层后: queue = [9, 20]      → size=2, 处理2个
// 第1层后: queue = [15, 7]      → size=2, 处理2个
// 第2层后: queue = []           → 结束
//
// 输出: [[3], [9,20], [15,7]]
```

## 解法详解

### 解法1: BFS + 队列 — O(n) / O(n) ⭐ 面试首选

**思考过程**：层序遍历 = 一层一层地访问。BFS 本身就是逐层扩展的，用队列实现。关键技巧：每处理一层前，先用 `size = queue.size()` 锁定当前层的节点数，然后只弹出 `size` 个节点。这 `size` 个节点的子节点入队后，自然就是下一层的全部节点。

```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;
        if (!root) return result;
        
        queue<TreeNode*> q;
        q.push(root);
        
        while (!q.empty()) {
            int size = q.size();  // 关键：锁定当前层的节点数
            vector<int> level;
            
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front();
                q.pop();
                level.push_back(node->val);
                
                // 子节点入队（它们属于下一层）
                if (node->left)  q.push(node->left);
                if (node->right) q.push(node->right);
            }
            
            result.push_back(level);
        }
        
        return result;
    }
};
```

**关键点**：`size` 必须在 for 循环之前取，不能写 `i < q.size()`，因为循环体中队列长度在变化。

---

### 解法2: DFS 递归 — O(n) / O(n)

**从另一个角度思考**：不用队列也能做层序。DFS 遍历时，只要知道当前节点在第几层，就能把它放到 `result[depth]` 中。前序遍历（根→左→右）保证同一层的节点按从左到右的顺序被访问。

```
// DFS 递归过程 (前序遍历):
//        3  (depth=0)
//       / \
//      9   20  (depth=1)
//         / \
//        15   7  (depth=2)
//
// 调用顺序:
// dfs(3, 0)  → result[0].push(3)    → result = [[3]]
// dfs(9, 1)  → result[1].push(9)    → result = [[3],[9]]
// dfs(20, 1) → result[1].push(20)   → result = [[3],[9,20]]
// dfs(15, 2) → result[2].push(15)   → result = [[3],[9,20],[15]]
// dfs(7, 2)  → result[2].push(7)    → result = [[3],[9,20],[15,7]]
```

```cpp
class Solution {
public:
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> result;
        dfs(root, 0, result);
        return result;
    }
    
    void dfs(TreeNode* node, int depth, vector<vector<int>>& result) {
        if (!node) return;
        
        // 第一次到达新的一层，创建空数组
        if (depth == result.size()) {
            result.push_back({});
        }
        
        result[depth].push_back(node->val);
        
        // 前序遍历：先左后右，保证同层从左到右
        dfs(node->left, depth + 1, result);
        dfs(node->right, depth + 1, result);
    }
};
```

**关键点**：`depth == result.size()` 这个判断精确地在第一次到达新层时创建空数组。因为前序遍历先访问左子树，所以每层最左边的节点一定是最先到达该层的。

## 解法对比

| | BFS + 队列 | DFS 递归 |
|---|---|---|
| 核心思想 | 用队列逐层处理，size 划分层 | 带深度参数递归，按深度分组 |
| 时间复杂度 | O(n) | O(n) |
| 空间复杂度 | O(n)（队列最宽层） | O(n)（递归栈最深 = 树高） |
| 直觉性 | ⭐ 层序遍历的最自然实现 | 需要"DFS也能做BFS"的认知 |
| 面试推荐 | **首选**，BFS 与层序天然对应 | 追问"能否用DFS实现？"时展示 |
| 空间特点 | 宽树（完全二叉树）队列大 | 深树（链状树）递归栈深 |

**什么时候选哪个？**
- 面试默认写 BFS，因为层序遍历用 BFS 是最直觉的。
- 如果面试官追问"能不能不用队列？"，展示 DFS 递归解法。
- 对于极度不平衡的树（很深但不宽），BFS 空间更优；对于很宽但不深的树，DFS 空间更优。

## 易错点

1. **`size` 在循环前取 vs 循环中取**
   - ✗ `for (int i = 0; i < q.size(); i++)` — 队列长度随着子节点入队而变化，会把下一层的节点也处理掉
   - ✓ `int size = q.size(); for (int i = 0; i < size; i++)` — 锁定当前层的节点数

2. **忘记判空**
   - ✗ 直接 `q.push(root)` 不判空 → root 为 nullptr 时 crash
   - ✓ 开头 `if (!root) return result;`

3. **DFS 解法中忘记创建新层数组**
   - ✗ 直接 `result[depth].push_back(...)` → 越界
   - ✓ 先检查 `if (depth == result.size()) result.push_back({});`

4. **DFS 用中序/后序而非前序**
   - ✗ 中序或后序遍历 → 同一层的节点不按从左到右的顺序插入
   - ✓ 前序遍历（根→左→右）保证同层节点从左到右被访问

## 面试追问

**Q1: 能否说说 BFS 中队列的最大长度是多少？**
→ 最大长度等于树最宽的那一层的节点数。对于完全二叉树，最后一层节点数约 n/2，所以空间 O(n)。

**Q2: 能否用 DFS 实现层序遍历？**
→ 可以，用前序 DFS + 深度参数。`result[depth]` 收集对应层的节点。见解法2。

**Q3: 如果要求自底向上的层序遍历（LeetCode 107），怎么改？**
→ 方法一：正常 BFS 做完后 `reverse(result.begin(), result.end())`。
→ 方法二：DFS 解法中用 `result[result.size() - 1 - depth]` 存放（但需要先知道树的高度或者直接最后反转更简单）。
→ 代码只加一行 `reverse`。

**Q4: 如果要求锯齿形层序遍历（LeetCode 103），怎么改？**
→ 加一个布尔变量记录当前层是左到右还是右到左。偶数层正常 push_back，奇数层 reverse 或用 deque 从前面插入。

## 相关题型

- **107. 二叉树的层序遍历 II** — 复用本题 BFS 代码，最后加 `reverse(result)` 即可。
- **103. 二叉树的锯齿形层序遍历** — 复用本题 BFS 框架，奇数层反转当前层数组（或用 deque）。
- **199. 二叉树的右视图** — 复用本题 BFS 框架，每层只取最后一个节点（`i == size - 1`）。
- **637. 二叉树的层平均值** — 复用本题 BFS 框架，每层求和除以 size。
- **116/117. 填充每个节点的下一个右侧节点指针** — 复用本题 BFS 框架，处理时把同层节点串联起来。