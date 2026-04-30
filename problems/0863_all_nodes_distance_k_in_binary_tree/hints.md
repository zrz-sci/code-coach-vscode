# 863. 二叉树中所有距离为 K 的结点

## 核心思路

**本质：树是特殊的无向图，"距离为 K"就是图上的 BFS 层级遍历问题。** 树本身只有从父到子的指针，没有从子到父的指针，所以核心难点是：如何让节点能"往上走"回到父节点。

## 思维链

1. **读完题第一反应**：从 target 出发，找距离为 k 的所有节点。如果这是一个图（每个节点能到达邻居），直接 BFS k 层就行了。但树只有从上往下的指针，无法"往上走"。

2. **暴力解的想法**：对树中每个节点，计算它到 target 的距离，收集距离恰好为 k 的。但计算两个节点距离需要找 LCA（最近公共祖先），每次 O(n)，总共 O(n²)。

3. **瓶颈在哪？** 树只能从父到子，不能从子到父。如果每个节点都能访问自己的父节点，那 target 就可以向三个方向扩展（左子、右子、父节点），问题退化为无向图的 BFS。

4. **突破：建立父指针映射**。用一次 DFS/BFS 遍历整棵树，用哈希表记录每个节点的父节点。这样每个节点就有了三个"邻居"：左子、右子、父节点。

5. **然后从 target 做 BFS**，扩展 k 层，第 k 层的所有节点就是答案。注意要用 visited 集合避免重复访问。

6. **另一种思路（纯 DFS）**：不建图，利用递归返回值来传递距离信息。先找到 target，然后分两类讨论：(1) target 子树内距离为 k 的节点；(2) 通过 target 的祖先绕到另一侧的节点。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 建父指针图 + BFS | 哈希表记录父节点，BFS k 层 | O(n) | O(n) | ⭐ 必须写出 |
| 纯 DFS（递归传距离） | 递归找 target，分祖先/子孙两类 | O(n) | O(n) | 加分项 |

## 关键提示

- **提示1**：如果树变成无向图（每个节点能访问左子、右子、父节点），这道题就是标准的"从某点出发 BFS 找第 k 层"。

- **提示2**：怎么给每个节点加上"父指针"？用一次遍历 + 哈希表 `map<TreeNode*, TreeNode*>`。

- **提示3**：BFS 时一定要防止"走回头路"。用 `set<TreeNode*>` 记录已访问节点。

- **提示4（纯 DFS 思路）**：从 target 往下找距离 k 的节点很简单。难的是往上走——如果 target 在 root 的左子树中，距离 target 为 d，那么在 root 的右子树中需要找距离为 k-d-1 的节点。

- **ASCII 示意图**：
```
//        3          target = 5, k = 2
//       / \
//      5   1        距离 target 的计算:
//     / \ / \       - 7: 在 target 子树内, 距离=2 ✓
//    6  2 0  8      - 4: 在 target 子树内, 距离=2 ✓
//      / \          - 1: 通过父节点3绕过去, 5→3→1, 距离=2 ✓
//     7   4         - 6: 在 target 子树内, 距离=1 ✗
//
//  BFS 从 target=5 出发:
//  第0层: [5]
//  第1层: [6, 2, 3]    ← 左子、右子、父节点
//  第2层: [7, 4, 1]    ← 答案！
//         (6无子节点)  (2的子) (3的右子，3的父=null已访问)
```

## 解法详解

### 解法1: 建父指针 + BFS — O(n) / O(n) ⭐ 面试首选

**思考过程**：树的"距离"问题本质就是图的最短路径。树是图的特例，唯一缺的就是反向边（子→父）。补上这条边后，直接 BFS。

**步骤**：
1. DFS 遍历整棵树，用哈希表记录 `child → parent` 的映射
2. 从 target 开始 BFS，每个节点的"邻居"有三个方向：left、right、parent
3. 用 visited 集合避免重复访问
4. BFS 到第 k 层时，收集该层所有节点值

```cpp
class Solution {
public:
    vector<int> distanceK(TreeNode* root, TreeNode* target, int k) {
        // Step1: 建立父指针映射
        unordered_map<TreeNode*, TreeNode*> parent;
        // DFS 遍历，记录每个节点的父节点
        function<void(TreeNode*, TreeNode*)> buildParent = [&](TreeNode* node, TreeNode* par) {
            if (!node) return;
            parent[node] = par;
            buildParent(node->left, node);
            buildParent(node->right, node);
        };
        buildParent(root, nullptr);
        
        // Step2: 从 target 开始 BFS
        queue<TreeNode*> q;
        unordered_set<TreeNode*> visited;
        q.push(target);
        visited.insert(target);
        
        int dist = 0;
        while (!q.empty()) {
            if (dist == k) {
                // 当前层就是距离为 k 的所有节点
                vector<int> res;
                while (!q.empty()) {
                    res.push_back(q.front()->val);
                    q.pop();
                }
                return res;
            }
            int size = q.size();
            for (int i = 0; i < size; i++) {
                TreeNode* cur = q.front(); q.pop();
                // 三个方向：左子、右子、父节点
                for (TreeNode* next : {cur->left, cur->right, parent[cur]}) {
                    if (next && !visited.count(next)) {
                        visited.insert(next);
                        q.push(next);
                    }
                }
            }
            dist++;
        }
        return {}; // k 超过了树的范围，没有节点
    }
};
```

**关键点**：
- BFS 的层数就是距离，到第 k 层时直接收割答案
- visited 必须在入队时就标记，不是出队时（否则会重复入队）

### 解法2: 纯 DFS（递归传距离）— O(n) / O(n)

**思考过程**：不想建图，能否纯用递归解决？关键观察：距离 target 为 k 的节点分两类——
1. **target 子树内**：直接从 target 向下 DFS，深度为 k 的节点
2. **target 子树外**：需要"绕路"通过 target 的某个祖先节点到达。如果某个祖先 A 到 target 的距离是 d，那么 A 的另一侧子树中距离 A 为 k-d 的节点就是答案。

**核心递归函数**：`dfs(node)` 返回 node 到 target 的距离（如果 target 在 node 子树中），否则返回 -1。

```
//  理解"绕路"的关键:
//
//        3 (d=1, 到target距离=1)
//       / \
//    ★5   1    ← 3到target距离是1, 所以在右子树找距离 k-1-1=0 的节点
//     / \ / \      即节点1本身 ✓
//    6  2 0  8
//      / \
//     7   4
//
//  对于 target=5, k=2:
//  - 在 target 子树内找深度=2 的: 7, 4
//  - 在 target 父节点(3)的另一侧找深度=k-1-1=0 的: 1
```

```cpp
class Solution {
public:
    vector<int> res;
    
    vector<int> distanceK(TreeNode* root, TreeNode* target, int k) {
        dfs(root, target, k);
        return res;
    }
    
    // 返回 node 到 target 的距离; 如果 target 不在 node 子树中, 返回 -1
    int dfs(TreeNode* node, TreeNode* target, int k) {
        if (!node) return -1;
        
        if (node == target) {
            // 找到 target，在其子树中收集距离为 k 的节点
            collect(node, k);
            return 0; // target 到自己距离为 0
        }
        
        // 在左子树中找 target
        int leftDist = dfs(node->left, target, k);
        if (leftDist != -1) {
            // target 在左子树中, node 到 target 距离是 leftDist + 1
            int d = leftDist + 1;
            if (d == k) res.push_back(node->val); // node 本身就是答案
            // 在右子树中找距离为 k - d - 1 的节点 (减1因为还要经过 node)
            else collect(node->right, k - d - 1);
            return d;
        }
        
        // 在右子树中找 target
        int rightDist = dfs(node->right, target, k);
        if (rightDist != -1) {
            int d = rightDist + 1;
            if (d == k) res.push_back(node->val);
            else collect(node->left, k - d - 1);
            return d;
        }
        
        return -1; // target 不在 node 子树中
    }
    
    // 从 node 出发, 收集子树中深度恰好为 depth 的所有节点
    void collect(TreeNode* node, int depth) {
        if (!node || depth < 0) return;
        if (depth == 0) {
            res.push_back(node->val);
            return;
        }
        collect(node->left, depth - 1);
        collect(node->right, depth - 1);
    }
};
```

**关键点**：
- `dfs` 返回值的含义明确：node 到 target 的距离，-1 表示 target 不在此子树
- `collect` 是简单的"子树中找深度为 d 的节点"
- `k - d - 1` 的减 1 是因为从 node 还要走一步到另一侧子树的根

## 解法对比

| | 解法1: 建图 + BFS | 解法2: 纯 DFS |
|---|---|---|
| **核心思想** | 树→无向图，标准 BFS | 递归分类讨论 |
| **代码复杂度** | 简单直观，不易出错 | 递归逻辑较复杂 |
| **时间** | O(n) | O(n) |
| **空间** | O(n)（哈希表+队列+visited） | O(n)（递归栈+结果数组） |
| **面试推荐** | ⭐ 首选，思路清晰好解释 | 展示递归功底的加分项 |

**什么时候选哪个？**
- 面试中优先写解法1：思路直白，代码短，不容易出 bug
- 如果面试官追问"不用额外空间建图"，再给解法2

## 易错点

1. **BFS 的 visited 标记时机错误**
   - ✗ 出队时才标记 visited → 同一节点可能被多次入队
   - ✓ 入队时立即标记 visited

2. **纯 DFS 解法中 collect 忘记判断 depth < 0**
   - ✗ `if (!node) return;` → 当 k-d-1 为负数时会继续深入
   - ✓ `if (!node || depth < 0) return;` → 提前终止

3. **parent 映射中 root 的父节点**
   - ✗ 忘记处理 root 的父节点为 nullptr → BFS 时访问 parent[root] 可能出错
   - ✓ 初始化时 `buildParent(root, nullptr)`，并在 BFS 中用 `if (next && ...)` 过滤

4. **纯 DFS 中两侧都搜到 target**
   - ✗ 左子树找到 target 后没有 return，又去搜右子树
   - ✓ `if (leftDist != -1) { ... return d; }` — 找到就立即返回，不再搜右子树

## 面试追问

**Q1：为什么不能只从 target 往下 DFS？**
往下只能找到 target 子树中的节点。距离为 k 的节点可能在 target 的祖先方向，需要"往上走"。树没有父指针，所以需要额外处理。

**Q2：如果树非常大，BFS 的空间怎么优化？**
BFS 的空间瓶颈是队列，最坏情况存 O(n) 个节点。可以用解法2（纯 DFS），不需要建图，递归栈最多 O(h)（h 为树高）。但实际上两者都是 O(n)（都需要存结果）。如果树是平衡的，BFS 最多存一层 O(n/2) 节点，DFS 栈深 O(log n)，DFS 更优。

**Q3：如果不是二叉树而是多叉树呢？**
思路完全一样。解法1中 BFS 扩展邻居时从 `{left, right, parent}` 变成 `children + parent`。解法2中 `collect` 遍历所有子节点，`dfs` 也要遍历所有子节点找 target。

**Q4：如果 target 节点不是给指针，而是给值，怎么办？**
先做一次 DFS 找到 target 对应的 TreeNode 指针，然后再按同样方法解。或者在建 parent 映射的同时找到 target 指针。

## 相关题型

- **199. 二叉树的右视图** — 复用 BFS 层序遍历框架，每层只取最后一个
- **236. 二叉树的最近公共祖先** — 解法2 的递归思路（在左/右子树中找目标）非常相似，都是递归返回值表示"是否找到目标"
- **742. 二叉树最近的叶节点** — 同样的"建父指针+BFS"套路，区别是 BFS 终止条件变成"遇到叶节点就停"
- **1740. 找到二叉树中的距离** — 复用"找两个节点距离"的思路，可以通过 LCA + 深度差计算