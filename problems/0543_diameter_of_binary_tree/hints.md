# 543. 二叉树的直径

## 核心思路

二叉树的直径 = 某个节点的「左子树深度 + 右子树深度」的最大值。本质上是在求每个节点的深度的过程中，顺便统计经过该节点的最长路径。

## 思维链

1. **读完题第一反应**：直径是任意两节点之间的最长路径。最朴素的想法 —— 对每个节点，算它的左子树深度和右子树深度，加起来就是"经过这个节点的最长路径"。遍历所有节点取最大值。

2. **暴力解的瓶颈**：如果对每个节点都单独调用 `depth()` 函数，每次 `depth()` 都要遍历其子树，总复杂度 O(n²)。大量子树被重复遍历。

3. **关键观察**：`depth(node)` 在递归时会先算 `depth(left)` 和 `depth(right)`，这时候我们已经拿到了左右深度 —— 何不在这一刻就顺手更新直径？这样只需一次 DFS 就够了。

4. **一次 DFS 解决**：定义一个递归函数求"以当前节点为根的子树深度"，在递归过程中用一个全局变量记录 `leftDepth + rightDepth` 的最大值，这就是答案。

5. **返回值 vs 更新值的区别**：递归函数返回的是「深度」（给父节点用），但同时在函数内部更新「直径」（全局答案）。这种"函数返回值和题目答案不是同一个东西"的模式是树题中非常经典的技巧。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力 | 每个节点分别算左右深度之和 | O(n²) | O(n) | 能说出即可 |
| 一次 DFS（递归） | 求深度的同时更新直径 | O(n) | O(n) | ⭐ 必须写出 |
| 一次 DFS（迭代） | 后序遍历 + 栈模拟 | O(n) | O(n) | 加分项 |

## 关键提示

- **提示 1**：直径不一定经过根节点！比如一棵左倾的树，最长路径可能完全在左子树中。所以不能只看 `depth(root.left) + depth(root.right)`。

- **提示 2**：对于任意一个节点，"经过它的最长路径" = 它的左子树深度 + 右子树深度。直径就是所有节点中这个值的最大值。

- **提示 3**：能不能在一次递归中同时完成"求深度"和"更新直径"两件事？想想递归函数的返回值和全局变量各自承担什么角色。

- **提示 4（ASCII 图解）**：
```
//       1            对于节点 1:
//      / \             leftDepth = 2 (1→2→4 或 1→2→5)
//     2   3            rightDepth = 1 (1→3)
//    / \               经过节点 1 的路径长度 = 2 + 1 = 3
//   4   5
//
// 对于节点 2:           对于节点 3:
//   leftDepth = 1       leftDepth = 0
//   rightDepth = 1      rightDepth = 0
//   路径长度 = 2         路径长度 = 0
//
// 全局最大值 = max(3, 2, 0, 0, 0) = 3
```

- **提示 5**：深度定义 —— 叶子节点深度为 0？还是为 1？注意这里"深度"指的是边数，所以 `depth(null) = 0`，`depth(叶子) = 0` 还是 `1`？其实两种都能做对，关键是定义一致。本题我们定义 `depth(node)` 返回"从 node 到其最远叶子的边数"，`depth(null) = 0`，`depth(叶子) = 0`。

## 解法详解

### 解法 1: 暴力法 — O(n²) / O(n)

**思考过程**：最直接的想法 —— 遍历每个节点，对每个节点分别求左右子树深度，取和的最大值。

```cpp
class Solution {
public:
    int diameterOfBinaryTree(TreeNode* root) {
        int result = 0;
        // 遍历每个节点
        traverse(root, result);
        return result;
    }
    
    void traverse(TreeNode* node, int& result) {
        if (!node) return;
        // 对当前节点，分别求左右子树深度
        int left = depth(node->left);
        int right = depth(node->right);
        // 经过当前节点的最长路径 = 左深度 + 右深度
        result = max(result, left + right);
        // 递归遍历左右子树（直径可能不经过当前节点）
        traverse(node->left, result);
        traverse(node->right, result);
    }
    
    int depth(TreeNode* node) {
        if (!node) return 0;
        return 1 + max(depth(node->left), depth(node->right));
    }
};
```

**关键点**：
- 这个解法是对的，但每个节点调 `depth()` 会导致子树被反复遍历
- 最坏情况（链状树）：O(n²)

---

### 解法 2: 一次 DFS（递归）— O(n) / O(n) ⭐ 面试首选

**从解法 1 优化**：解法 1 的瓶颈是"对每个节点单独求深度"，导致子树重复遍历。但其实 `depth()` 函数在递归时已经算出了左右子树的深度 —— 我们只需在计算深度的同时，"顺手"更新直径就行了。

```
//  递归过程（自底向上）:
//
//       1            depth(4) = 0, 更新 ans = max(0, 0+0) = 0
//      / \           depth(5) = 0, 更新 ans = max(0, 0+0) = 0
//     2   3          depth(2): left=1, right=1, 更新 ans = max(0, 1+1) = 2
//    / \                       返回 1 + max(1,1) = 2
//   4   5            depth(3) = 0, 更新 ans = max(2, 0+0) = 2
//                    depth(1): left=2, right=1, 更新 ans = max(2, 2+1) = 3
//                              返回 1 + max(2,1) = 3
//
//  最终 ans = 3
```

```cpp
class Solution {
public:
    int diameterOfBinaryTree(TreeNode* root) {
        int ans = 0;
        depth(root, ans);
        return ans;
    }

private:
    // 返回：以 node 为根的子树深度（边数）
    // 同时更新 ans：经过某节点的最长路径
    int depth(TreeNode* node, int& ans) {
        if (!node) return 0;
        
        int left = depth(node->left, ans);    // 左子树深度
        int right = depth(node->right, ans);  // 右子树深度
        
        // 关键：经过当前节点的路径长度 = left + right
        ans = max(ans, left + right);
        
        // 返回当前节点的深度（给父节点用）
        return 1 + max(left, right);
    }
};
```

**关键点**：
- 递归函数的**返回值**（深度）和**更新的全局变量**（直径）是两个不同的东西
- 这是树题中极其常见的模式：「返回给父节点的信息」和「维护全局答案」解耦

---

### 解法 3: 一次 DFS（迭代 — 后序遍历）— O(n) / O(n)

**思考过程**：面试官可能追问"不用递归怎么做？" 深度是自底向上计算的，对应后序遍历。我们用栈模拟后序遍历，用哈希表存每个节点的深度。

```cpp
class Solution {
public:
    int diameterOfBinaryTree(TreeNode* root) {
        if (!root) return 0;
        
        int ans = 0;
        unordered_map<TreeNode*, int> depthMap;
        depthMap[nullptr] = 0;  // null 节点深度为 0
        
        // 后序遍历：用两个栈实现
        // 栈1 用于遍历，栈2 存储后序序列
        stack<TreeNode*> stk1, stk2;
        stk1.push(root);
        while (!stk1.empty()) {
            TreeNode* node = stk1.top(); stk1.pop();
            stk2.push(node);
            if (node->left)  stk1.push(node->left);
            if (node->right) stk1.push(node->right);
        }
        
        // 按后序序列处理（保证处理节点时子节点已处理）
        while (!stk2.empty()) {
            TreeNode* node = stk2.top(); stk2.pop();
            int left  = depthMap[node->left];
            int right = depthMap[node->right];
            ans = max(ans, left + right);
            depthMap[node] = 1 + max(left, right);
        }
        
        return ans;
    }
};
```

**关键点**：
- 后序遍历保证处理父节点时，左右子节点的深度已经算好
- 用 `unordered_map` 存储每个节点深度，模拟递归中的返回值

## 解法对比

| | 解法 1 暴力 | 解法 2 递归 DFS | 解法 3 迭代 DFS |
|---|---|---|---|
| 时间 | O(n²) | O(n) | O(n) |
| 空间 | O(n) 递归栈 | O(n) 递归栈 | O(n) 栈 + 哈希表 |
| 代码量 | 多（两个递归函数） | 最少 | 最多 |
| 面试推荐 | 说思路即可 | ⭐ 首选 | 追问时给出 |

**核心区别**：解法 1 把"求深度"和"求直径"分成两步，解法 2/3 把它们合并为一步。

## 易错点

1. **✗ 只看根节点**：`return depth(root->left) + depth(root->right);`
   **✓ 需要考虑所有节点**：直径可能不经过根节点。比如：
   ```
   //        1
   //       /
   //      2        直径在左子树中: 4→2→5, 长度=2
   //     / \       如果只看根: depth(left)=2, depth(right)=0, 得到2
   //    4   5      碰巧对了，但如果左子树更深更复杂就会出错
   ```

2. **✗ 深度定义混乱**：`depth(null) = -1` 和 `depth(null) = 0` 混用导致答案差 1。
   **✓ 保持一致**：本题中 `depth(null) = 0`，叶子节点 `depth = 0`（指边数为 0），那么 `depth(node) = 1 + max(left, right)` 中的 `1` 代表当前节点到子节点的那条边。

3. **✗ 忘记更新 ans**：递归函数只返回了深度，没有在函数内部更新直径。
   **✓** `ans = max(ans, left + right);` 必须在每次递归中执行。

4. **✗ 返回值写成 `left + right`**：递归函数应该返回深度（单侧最长路径），不是直径。
   **✓ 返回 `1 + max(left, right)`**，`left + right` 只用于更新答案。

## 面试追问

**Q1：直径一定经过根节点吗？为什么？**
→ 不一定。考虑一棵左倾的树，最长路径可能完全在左子树内部。所以必须对每个节点都检查 `leftDepth + rightDepth`。

**Q2：你的递归函数返回的是什么？它和最终答案是什么关系？**
→ 返回"以当前节点为根的子树深度"（边数），这是给父节点用的。最终答案（直径）是在递归过程中通过 `ans = max(ans, left + right)` 维护的全局变量。返回值 ≠ 答案。

**Q3：能否不用全局变量/引用参数？**
→ 可以。让递归函数返回一个 pair<int,int>，分别是（子树深度, 子树内的最大直径）。这样不需要全局变量，但代码稍复杂：
```cpp
pair<int,int> dfs(TreeNode* node) {
    if (!node) return {0, 0};
    auto [ld, ldia] = dfs(node->left);
    auto [rd, rdia] = dfs(node->right);
    int depth = 1 + max(ld, rd);
    int diameter = max({ld + rd, ldia, rdia});
    return {depth, diameter};
}
```

**Q4：如果要返回直径路径上的所有节点（而不只是长度），怎么改？**
→ 需要在递归中记录路径。每个节点返回「从该节点到最远叶子的路径节点列表」，在更新最大直径时拼接左右路径。

## 相关题型

- **124. 二叉树中的最大路径和** — 复用完全相同的「递归返回值 ≠ 全局答案」模式。区别：543 求的是路径长度（边数），124 求的是路径上节点值的最大和，且需要考虑负值的剪枝（`max(0, child)`）。
- **104. 二叉树的最大深度** — 本题的子问题。543 的递归函数本质就是在求 104 的答案，只是多了一步"更新直径"。
- **687. 最长同值路径** — 复用相同框架，区别：只有当子节点值等于当前节点值时，路径才能延伸。把 `depth` 函数中的 `1 + max(left, right)` 改成条件判断。
- **110. 平衡二叉树** — 同样在求深度的递归中"顺手"判断平衡性，技巧完全一致。