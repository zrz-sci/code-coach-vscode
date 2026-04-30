# 337. 打家劫舍 III

## 核心思路

这道题本质上是在**树形结构上做"选或不选"的决策**：对每个节点，要么偷它（则不能偷其直接子节点），要么不偷它（则子节点可偷可不偷），求全局最大收益。这是 **树形 DP** 的经典题目。

## 系列题定位

| 题目 | 结构 | 核心区别 |
|------|------|----------|
| 198. House Robber I | 线性数组 | dp[i] = max(dp[i-1], dp[i-2] + nums[i]) |
| 213. House Robber II | 环形数组 | 拆成两次线性 DP（去掉首 or 去掉尾） |
| **337. House Robber III** | **二叉树** | **从线性 DP 变成树形 DP，每个节点返回"偷/不偷"两个状态** |

和 I、II 的关键区别：数据结构从数组变成了树，所以不能用下标递推，必须用后序遍历（自底向上）收集子树信息。

## 思维链

1. **读完题第一反应**：这和 House Robber I 很像，只是从数组变成了树。暴力做法：对每个节点，递归计算"偷它"和"不偷它"两种情况，取较大值。

2. **暴力解的问题**：如果"偷当前节点"，需要跳到孙子节点去递归，这导致同一个节点会被反复计算。比如计算 root 时要算 grandchildren，计算 root.left 时也要算那些 grandchildren → **大量重叠子问题**。

3. **第一次优化 — 记忆化**：既然有重叠子问题，用 HashMap 缓存每个节点的结果。这就是从暴力递归到记忆化搜索的标准套路。

4. **终极优化 — 树形 DP**：换一个角度，让每个节点返回一对值 `(偷自己的最大值, 不偷自己的最大值)`。这样一次后序遍历就搞定，不需要 HashMap，因为信息是自底向上传递的，每个节点只访问一次。

5. **状态转移**：
   - 偷当前节点：`rob = node.val + left.notRob + right.notRob`
   - 不偷当前节点：`notRob = max(left.rob, left.notRob) + max(right.rob, right.notRob)`

6. **这就是树形 DP 的核心模式**：每个节点维护多个状态，后序遍历时从子节点收集信息，合并得到当前节点的状态。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力递归 | 偷/不偷递归到孙子节点 | O(2^n) 指数级 | O(h) 栈深 | 能说出即可 |
| 记忆化搜索 | 暴力 + HashMap 缓存 | O(n) | O(n) | 能说出即可 |
| 树形 DP (pair) | 每个节点返回 (偷, 不偷) | O(n) | O(h) | ⭐ 必须写出 |

## 关键提示

- **提示1**：和 House Robber I 类比——数组上 `dp[i]` 只依赖 `dp[i-1]` 和 `dp[i-2]`；树上每个节点只依赖其左右子节点的结果。能否让每个节点"汇报"两种状态？

- **提示2**：暴力解里 `rob(node) = node.val + rob(node.left.left) + rob(node.left.right) + rob(node.right.left) + rob(node.right.right)` 这个"跳孙子"的写法，本质上是在重复计算子节点。如果子节点直接告诉你"偷我"和"不偷我"的结果，就不需要跳了。

- **提示3**：树的示意图与状态转移：
```
//  示例1:
//         3          ← 偷 root(3) + 孙子(3+1) = 7
//        / \            不偷 root → max子 = max(2,3)+max(3,1) = 3+3 = 6
//       2   3        所以答案 = max(7, 6) = 7
//        \   \
//         3   1
//
//  示例2:
//         3          ← 偷 root(3) + 孙子(1+3+0+1) = 8
//        / \            不偷 root → max子 = max(4,?)+max(5,?) = 4+5 = 9
//       4   5        所以答案 = max(8, 9) = 9
//      / \   \
//     1   3   1
```

- **提示4**：后序遍历（左→右→根）天然适合这种"从子节点收集信息"的场景。

- **提示5**：用 `pair<int,int>` 或 `array<int,2>` 返回两个状态，比用全局 HashMap 更优雅、更省空间。

## 解法详解

### 解法1: 暴力递归 — O(2^n) / O(h)

**思考过程**：最直接的想法——对每个节点，考虑偷和不偷：
- **偷当前节点**：不能偷子节点，只能偷孙子节点
- **不偷当前节点**：可以偷子节点

```cpp
class Solution {
public:
    int rob(TreeNode* root) {
        if (!root) return 0;
        
        // 选择1: 偷当前节点 → 跳到孙子
        int robCurr = root->val;
        if (root->left) {
            robCurr += rob(root->left->left) + rob(root->left->right);
        }
        if (root->right) {
            robCurr += rob(root->right->left) + rob(root->right->right);
        }
        
        // 选择2: 不偷当前节点 → 偷子节点
        int skipCurr = rob(root->left) + rob(root->right);
        
        return max(robCurr, skipCurr);
    }
};
```

**关键点**：这个解法的问题在于 `rob(root->left)` 内部又会递归调用 `rob(root->left->left)`，而外层的 `robCurr` 也直接调用了 `rob(root->left->left)`，导致同一个节点被重复计算。

```
// 重复计算示意:
// rob(root) 调用了:
//   rob(root->left)          ← 这里面会算 rob(root->left->left)
//   rob(root->left->left)    ← 这里又算了一遍！
//   rob(root->left->right)   ← 同理
```

### 解法2: 记忆化搜索 — O(n) / O(n)

**从解法1优化**：解法1的瓶颈是重复计算，用 HashMap 缓存每个节点的结果。

```cpp
class Solution {
public:
    unordered_map<TreeNode*, int> memo;
    
    int rob(TreeNode* root) {
        if (!root) return 0;
        if (memo.count(root)) return memo[root]; // 命中缓存
        
        // 偷当前节点
        int robCurr = root->val;
        if (root->left) {
            robCurr += rob(root->left->left) + rob(root->left->right);
        }
        if (root->right) {
            robCurr += rob(root->right->left) + rob(root->right->right);
        }
        
        // 不偷当前节点
        int skipCurr = rob(root->left) + rob(root->right);
        
        memo[root] = max(robCurr, skipCurr);
        return memo[root];
    }
};
```

**关键点**：这里用 `TreeNode*` 作为 key 是合法的，因为每个节点地址唯一。但 HashMap 有额外空间开销，而且哈希操作有常数因子。

### 解法3: 树形 DP (每个节点返回一对值) — O(n) / O(h) ⭐ 面试首选

**核心洞察**：与其"跳孙子"，不如让每个节点直接返回两种状态：
- `rob`：偷这个节点能获得的最大值
- `notRob`：不偷这个节点能获得的最大值

这样父节点就能直接从子节点的两个状态中做决策，不需要跳级。

```
// 树形 DP 状态转移:
//
//     node [rob_node, notRob_node]
//      / \
//   left   right
//  [r_l, nr_l]  [r_r, nr_r]
//
// rob_node    = node.val + nr_l + nr_r      (偷node, 则子不能偷)
// notRob_node = max(r_l, nr_l) + max(r_r, nr_r)  (不偷node, 子可偷可不偷)
```

**后序遍历过程（示例1）**：
```
//         3
//        / \
//       2   3
//        \   \
//         3   1
//
// 后序遍历: 叶子先算, 根最后算
//
// 节点3(左孙): 叶子 → [3, 0]
// 节点1(右孙): 叶子 → [1, 0]
// 节点2:
//   rob_2 = 2 + 0(左null的notRob) + 0(右3的notRob) = 2
//   notRob_2 = max(0,0) + max(3,0) = 3
//   → [2, 3]
// 节点3(右子):
//   rob_3 = 3 + 0(左null的notRob) + 0(右1的notRob) = 3
//   notRob_3 = max(0,0) + max(1,0) = 1
//   → [3, 1]
// 节点3(根):
//   rob_root = 3 + 3(左2的notRob) + 1(右3的notRob) = 7
//   notRob_root = max(2,3) + max(3,1) = 3 + 3 = 6
//   → [7, 6]
// 答案 = max(7, 6) = 7 ✓
```

```cpp
class Solution {
public:
    int rob(TreeNode* root) {
        auto [robRoot, notRobRoot] = dfs(root);
        return max(robRoot, notRobRoot);
    }
    
private:
    // 返回 {偷当前节点的最大值, 不偷当前节点的最大值}
    pair<int, int> dfs(TreeNode* node) {
        if (!node) return {0, 0};
        
        auto [robLeft, notRobLeft] = dfs(node->left);
        auto [robRight, notRobRight] = dfs(node->right);
        
        // 偷当前节点: 子节点不能偷
        int robCurr = node->val + notRobLeft + notRobRight;
        // 不偷当前节点: 子节点各自取最优
        int notRobCurr = max(robLeft, notRobLeft) + max(robRight, notRobRight);
        
        return {robCurr, notRobCurr};
    }
};
```

**关键点**：
- 后序遍历保证了计算当前节点时，左右子节点的结果已经就绪
- 不需要额外的 HashMap，因为每个节点的状态通过返回值"传上去"，用完即丢
- 空间只有递归栈 O(h)，其中 h 是树的高度

## 解法对比

| | 暴力递归 | 记忆化搜索 | 树形 DP (pair) |
|---|---|---|---|
| **时间** | O(2^n) 指数级 | O(n) | O(n) |
| **空间** | O(h) 递归栈 | O(n) HashMap + O(h) 栈 | O(h) 递归栈 |
| **核心思想** | 偷/不偷 + 跳孙子 | 加缓存消除重复 | 每个节点返回两种状态 |
| **和 HR I 的关系** | 类比暴力递归 | 类比记忆化 | 类比滚动变量优化 |

**选择建议**：面试直接写解法3，简洁高效。如果面试官追问思考过程，可以从解法1的"跳孙子"出发，说明为什么解法3更好。

## 易错点

1. **✗ 不偷当前节点时，错误地强制偷子节点**：
   ```cpp
   // 错误: 不偷node就一定要偷left和right
   int notRobCurr = robLeft + robRight;
   ```
   **✓ 正确：不偷当前节点时，子节点可以偷也可以不偷，各自取最优**：
   ```cpp
   int notRobCurr = max(robLeft, notRobLeft) + max(robRight, notRobRight);
   ```
   为什么？因为即使不偷 node，如果 node->left 的值很小，不偷 left 反而能让 left 的子节点贡献更多。

2. **✗ 忘记处理 null 节点**：
   ```cpp
   // 错误: 没有 base case
   pair<int,int> dfs(TreeNode* node) {
       auto [rl, nrl] = dfs(node->left); // node 为 null 时崩溃!
   ```
   **✓ 正确：null 节点返回 {0, 0}**。

3. **✗ 暴力解中访问孙子时不检查子节点是否为 null**：
   ```cpp
   // 错误: root->left 可能为 null
   robCurr += rob(root->left->left);
   ```
   **✓ 正确：先检查 `if (root->left)` 再访问其子节点**。

## 面试追问

**Q1（基础理解）**：这道题和 House Robber I 有什么关系？
> House Robber I 是线性数组上的 DP，相邻不能选；本题是树结构上的 DP，父子不能同时选。核心的"选或不选"决策框架一样，只是数据结构从数组变成了树，递推变成了后序遍历。

**Q2（优化思路）**：为什么解法3比解法2好？
> 解法2用 HashMap 缓存，额外 O(n) 空间且有哈希常数因子。解法3通过函数返回值传递状态，空间只有 O(h) 的递归栈。本质上，解法3把"缓存"嵌入了返回值，信息用完即丢，不需要全局存储。这和 House Robber I 中用滚动变量代替 dp 数组是同一个思想。

**Q3（变体追问）**：如果树变成一般的图（有环），怎么办？
> 有环的话不能简单后序遍历了。需要：(1) 如果是树+一条额外边形成一个环，可以类比 House Robber II 的思路，删掉环上一条边拆成两棵树分别做。(2) 如果是一般图，这变成了最大权独立集问题（NP-hard），需要回溯/状态压缩 DP。

**Q4（代码变体）**：能否用迭代（BFS 层序遍历）实现树形 DP？
> 可以。用 BFS 获得层序遍历顺序，然后逆序处理（从叶子到根），用 HashMap 存储每个节点的 (rob, notRob) 状态。但代码不如递归简洁，面试中递归版本是首选。

## 相关题型

- **198. House Robber I** — 本题的线性版本。复用"选或不选"的 DP 框架，区别：数组用下标递推 `dp[i] = max(dp[i-1], dp[i-2]+nums[i])`，树用后序遍历返回 pair。
- **213. House Robber II** — 环形数组版本。复用 HR I 的线性 DP，技巧是拆成两次线性 DP。
- **968. Binary Tree Cameras** — 同样是树形 DP + 每个节点多状态。区别：968 需要三个状态（被覆盖/有摄像头/未覆盖），本题只需两个状态（偷/不偷）。
- **124. Binary Tree Maximum Path Sum** — 同样是后序遍历 + 从子节点收集信息的模式。区别：124 的路径可以"拐弯"，需要区分"经过当前节点的路径"和"以当前节点为端点的路径"。