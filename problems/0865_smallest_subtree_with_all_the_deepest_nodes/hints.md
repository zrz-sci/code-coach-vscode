# 865. 具有所有最深节点的最小子树

## 核心思路

本题本质上是在问：**找到所有最深叶子节点的最近公共祖先（LCA）**。如果所有最深节点都在某个节点的子树里，且该节点是满足条件的最深的那个，那它就是答案。

## 思维链

1. **读完题第一反应**：先找出所有最深的节点，然后找包含它们的最小子树。最小子树的根 = 这些节点的最近公共祖先。
2. **朴素做法**：先 BFS/DFS 找到最大深度，再收集所有最深节点，最后用 LCA 算法找它们的公共祖先。但这需要多次遍历。
3. **能否一次遍历？** 关键观察：对于任意节点，如果它的左子树最深深度 == 右子树最深深度，那么最深节点同时存在于左右两侧，**当前节点就是答案**。如果左边更深，答案在左子树；右边更深，答案在右子树。
4. **自底向上递归**：后序遍历，每个节点返回"子树深度"和"子树中包含所有最深节点的最小子树根"。这样一次 DFS 就搞定了。
5. **核心递推逻辑**：
   - `leftDepth > rightDepth` → 答案在左子树
   - `leftDepth < rightDepth` → 答案在右子树
   - `leftDepth == rightDepth` → 当前节点就是答案
6. **直觉验证**：当左右深度相等时，说明最深叶子分布在两侧，当前节点是它们的 LCA，恰好是最小包含子树。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 两次遍历 | 先求深度，再找 LCA | O(n) | O(n) | 能说出即可 |
| 一次 DFS（返回 pair） | 后序遍历同时返回深度和答案节点 | O(n) | O(h) | ⭐ 必须写出 |
| BFS 逐层淘汰 | 从最深层往上收缩，直到只剩一个节点 | O(n) | O(n) | 加分项 |

## 关键提示

1. **想想左右子树深度的关系**：如果左子树比右子树深，所有最深节点一定都在左子树里，答案也一定在左子树里。
2. **什么时候当前节点就是答案？** 当左右子树"一样深"时——最深节点两边都有，当前节点就是它们的 LCA。
3. **递归返回什么？** 光返回节点不够，还需要深度信息来做判断。用 `pair<int, TreeNode*>` 同时返回深度和答案。
4. **ASCII 示例帮助理解**：
```
         3
        / \
       5   1
      / \ / \
     6  2 0  8
       / \
      7   4      ← 最深层（深度3）

节点2: leftDepth=3(来自7), rightDepth=3(来自4) → 相等 → 节点2是答案
节点5: leftDepth=2(来自6), rightDepth=3(来自2→7/4) → 右更深 → 答案在右=节点2
节点3: leftDepth=3(来自5→2→7/4), rightDepth=2(来自1→0/8) → 左更深 → 答案在左=节点2
```

5. **和 LeetCode 1123 完全相同**：本题就是"最深叶节点的最近公共祖先"。

## 解法详解

### 解法1: 两次遍历 — O(n) / O(n)

**思考过程**：最直觉的想法——先求出每个节点的深度，找到最大深度，然后自底向上找这些最深节点的 LCA。

```cpp
class Solution {
public:
    // 第一步：用 DFS 求每个节点的深度（存到哈希表）
    unordered_map<TreeNode*, int> depth;
    int maxDepth = 0;
    
    void computeDepth(TreeNode* node, int d) {
        if (!node) return;
        depth[node] = d;
        maxDepth = max(maxDepth, d);
        computeDepth(node->left, d + 1);
        computeDepth(node->right, d + 1);
    }
    
    // 第二步：找包含所有最深节点的最小子树
    // 如果当前节点就是最深节点且是叶子 → 返回自己
    // 如果左右子树都有结果 → 当前节点是 LCA
    // 只有一边有结果 → 返回那一边
    TreeNode* findLCA(TreeNode* node) {
        if (!node) return nullptr;
        if (depth[node] == maxDepth) return node; // 最深节点本身
        
        TreeNode* left = findLCA(node->left);
        TreeNode* right = findLCA(node->right);
        
        if (left && right) return node;  // 两边都有最深节点 → 当前节点是LCA
        return left ? left : right;       // 只有一边有
    }
    
    TreeNode* subtreeWithAllDeepest(TreeNode* root) {
        computeDepth(root, 0);
        return findLCA(root);
    }
};
```

**关键点**：这个解法需要两次遍历（一次求深度，一次找 LCA），且需要额外哈希表存储深度。逻辑清晰但不够优雅。

---

### 解法2: 一次 DFS（返回 pair）— O(n) / O(h) ⭐ 面试首选

**从解法1优化**：解法1 需要两次遍历，能否合二为一？关键洞察——在后序遍历时，我们可以**同时**得到子树深度和答案节点。

**核心逻辑**：
- 递归函数返回 `pair<int, TreeNode*>`：(子树最大深度, 包含所有最深节点的最小子树根)
- 比较左右子树深度来决定答案在哪边

```cpp
class Solution {
public:
    TreeNode* subtreeWithAllDeepest(TreeNode* root) {
        return dfs(root).second;
    }
    
    // 返回 {当前子树最大深度, 包含所有最深节点的最小子树根}
    pair<int, TreeNode*> dfs(TreeNode* node) {
        if (!node) return {0, nullptr};
        
        auto [leftDepth, leftNode] = dfs(node->left);
        auto [rightDepth, rightNode] = dfs(node->right);
        
        if (leftDepth > rightDepth) {
            // 最深节点全在左子树 → 答案来自左边
            return {leftDepth + 1, leftNode};
        } else if (leftDepth < rightDepth) {
            // 最深节点全在右子树 → 答案来自右边
            return {rightDepth + 1, rightNode};
        } else {
            // 左右一样深 → 当前节点是最深节点们的 LCA
            return {leftDepth + 1, node};
        }
    }
};
```

**关键点**：
- `pair<int, TreeNode*>` 同时携带深度信息和答案节点，一次遍历搞定
- 空间复杂度只有 O(h)（递归栈深度），h 为树高
- 这是面试中最推荐的写法：简洁、高效、思路清晰

---

### 解法3: BFS 逐层淘汰 — O(n) / O(n)

**不同角度思考**：从最底层开始，如果最深层有多个节点，它们的父节点中"公共"的那个就是答案。可以用 BFS 逐层向上收缩：每层把节点替换为它们的父节点，去重后如果只剩一个，就是答案。

```cpp
class Solution {
public:
    TreeNode* subtreeWithAllDeepest(TreeNode* root) {
        // BFS 找最深层所有节点，同时记录每个节点的父节点
        unordered_map<TreeNode*, TreeNode*> parent;
        queue<TreeNode*> q;
        q.push(root);
        parent[root] = nullptr;
        
        vector<TreeNode*> deepest;  // 最后一层的节点
        
        while (!q.empty()) {
            deepest.clear();
            int size = q.size();
            for (int i = 0; i < size; i++) {
                TreeNode* node = q.front(); q.pop();
                deepest.push_back(node);
                if (node->left) {
                    parent[node->left] = node;
                    q.push(node->left);
                }
                if (node->right) {
                    parent[node->right] = node;
                    q.push(node->right);
                }
            }
        }
        
        // 从最深层向上收缩，直到只剩一个节点
        // 用 set 去重：每个节点替换为父节点
        while (deepest.size() > 1) {
            unordered_set<TreeNode*> nextLevel;
            for (TreeNode* node : deepest) {
                nextLevel.insert(parent[node]);
            }
            deepest.assign(nextLevel.begin(), nextLevel.end());
        }
        
        return deepest[0];
    }
};
```

**关键点**：这个解法的思路更"直观"——像气泡一样从底部往上收拢。但代码较长，面试中不如解法2简洁。

## 解法对比

| | 解法1（两次遍历） | 解法2（一次 DFS） | 解法3（BFS 逐层淘汰） |
|---|---|---|---|
| 遍历次数 | 2 | 1 | 1+收缩 |
| 空间 | O(n) 哈希表 | O(h) 递归栈 | O(n) 哈希表+队列 |
| 代码量 | 中等 | 最少 | 最多 |
| 面试推荐 | ✗ | ⭐ 首选 | 作为思路展示 |

**核心区别**：解法2 把"求深度"和"找答案"合并到一次后序遍历中，通过返回 pair 携带两份信息，是最优雅的写法。

## 易错点

1. **✗ 递归返回深度时忘记 +1**
   ```cpp
   return {leftDepth, leftNode};     // ✗ 忘了 +1，深度没有累加
   return {leftDepth + 1, leftNode}; // ✓ 每向上一层深度 +1
   ```

2. **✗ 空节点返回深度 -1 或 -∞ 导致逻辑混乱**
   ```cpp
   if (!node) return {-1, nullptr};   // ✗ 后续比较逻辑变复杂
   if (!node) return {0, nullptr};    // ✓ 空节点深度为0，叶子深度为1
   ```

3. **✗ 左右深度相等时返回左节点或右节点而不是当前节点**
   ```cpp
   if (leftDepth == rightDepth) return {leftDepth + 1, leftNode};  // ✗ 应该返回当前节点
   if (leftDepth == rightDepth) return {leftDepth + 1, node};      // ✓ 当前节点才是LCA
   ```

4. **✗ 混淆"深度"和"高度"概念**：本题中递归返回的其实是子树高度（从底向上计），但用来比较左右是否一样深是等价的。

## 面试追问

**Q1: 这道题和"最近公共祖先 (LCA)"有什么关系？**
→ 本题等价于"找所有最深叶节点的 LCA"。经典 LCA 是给定两个目标节点，本题的目标节点是所有最深叶子。解法2 的核心思想和 LCA 的后序遍历解法完全一致。

**Q2: 如果树会动态更新（增删节点），怎么高效维护答案？**
→ 需要维护每个节点的子树高度。更新一个节点时，沿路径向上更新祖先的高度信息，然后重新判断。可以用 Euler Tour + 线段树维护，或者用 Link-Cut Tree，但面试中说出思路即可。

**Q3: 如果不是要最深的节点，而是给定深度 k 的所有节点，找它们的最小公共子树？**
→ 思路相同：递归时记录子树中是否存在深度为 k 的节点，用类似的"左右都有 → 当前节点是 LCA"逻辑。本质是把"最大深度"的判断改为"指定深度"的判断。

## 相关题型

- **236. 二叉树的最近公共祖先** — 复用"后序遍历 + 左右汇报结果"的框架。本题是 236 的变体，目标节点从"给定两个"变成"所有最深叶子"。
- **1123. 最深叶节点的最近公共祖先** — 和本题 100% 相同，完全复用解法2的代码。
- **104. 二叉树的最大深度** — 本题的子问题。解法2 中每个递归调用本质上就是在求子树最大深度。
- **110. 平衡二叉树** — 同样是"后序遍历返回高度并同时做判断"的模式，复用"返回 pair/额外信息"的技巧。