# 236. 二叉树的最近公共祖先

## 核心思路

本质上是在问：从根节点出发，**p 和 q 分别在哪个子树中**？如果它们分居左右两侧，当前节点就是 LCA；如果都在同一侧，LCA 在那一侧的更深处。

## 思维链

1. **读完题第一反应**：如果我能知道从根到 p 和从根到 q 的路径，找到两条路径的最后一个公共节点就是答案。→ 这就是暴力解：记录路径，比较路径。

2. **暴力解的瓶颈**：需要两次 DFS 找路径，还需要额外空间存储路径。虽然时间 O(n)，但代码复杂（要用回溯或者父指针记录路径）。

3. **换个角度想**：不记录路径，直接在递归中"汇报"信息。对于当前节点 root，我问左子树"你那边有 p 或 q 吗？"，再问右子树同样的问题。

4. **三种情况**：
   - 左边找到了，右边也找到了 → p 和 q 分居两侧，当前 root 就是 LCA
   - 只有左边找到了 → LCA 在左子树中（包括左子树根就是答案的情况）
   - 只有右边找到了 → LCA 在右子树中

5. **递归返回值的含义**：返回的不是"LCA"，而是"在以 root 为根的子树中，能找到的 p 或 q（或它们的 LCA）"。这个"多义返回值"是这道题最精妙的地方。

6. **终止条件**：root 为空返回 null；root 就是 p 或 q，直接返回 root（因为题目保证 p、q 都存在，自己就是自己的祖先）。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 记录路径 + 比较 | DFS 找两条路径，逐节点比较 | O(n) | O(n) | 能说出即可 |
| 父指针 + 集合 | 用哈希表存父节点，回溯找交点 | O(n) | O(n) | 能说出即可 |
| 递归（后序遍历） | 自底向上汇报，左右汇合即 LCA | O(n) | O(h) | ⭐ 必须写出 |
| 迭代（父指针） | 迭代 DFS 建父指针表，再回溯 | O(n) | O(n) | 加分项 |

## 关键提示

- **提示1**：想一想递归函数的返回值应该代表什么含义。它不需要只代表"LCA"，它可以代表"在这棵子树中找到的目标节点"。

- **提示2**：后序遍历（先递归左右，再处理当前节点）天然适合"自底向上汇总信息"的场景。

- **提示3**：当 root == p 时直接返回 root，不需要继续往下找 q。为什么？因为即使 q 在 p 的子树中，p 就是答案；如果 q 不在 p 的子树中，另一侧会找到 q。

- **提示4**：递归返回 null 表示"这边没找到任何目标"，返回非 null 表示"找到了某个目标或 LCA"。

- **ASCII 示意图**：
```
//         3          ← p=5, q=1
//        / \
//       5   1        左子树返回5, 右子树返回1
//      / \ / \       → 左右都非空 → LCA = 3
//     6  2 0  8
//       / \
//      7   4
//
//         3          ← p=5, q=4
//        / \
//       5   1        左子树返回5, 右子树返回null
//      / \ / \       → 只有左边非空 → 返回5 (即LCA=5)
//     6  2 0  8
//       / \
//      7   4
```

## 解法详解

### 解法1: 记录路径 + 比较 — O(n) / O(n)

**思考过程**：最直觉的想法——如果我知道从根到 p 的路径和从根到 q 的路径，只需要找到两条路径的"分叉点"，分叉前的最后一个公共节点就是 LCA。

```cpp
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        vector<TreeNode*> pathP, pathQ;
        // 分别找到从根到 p 和从根到 q 的路径
        findPath(root, p, pathP);
        findPath(root, q, pathQ);
        
        // 从头比较两条路径，找最后一个相同的节点
        TreeNode* ans = root;
        for (int i = 0; i < min(pathP.size(), pathQ.size()); i++) {
            if (pathP[i] == pathQ[i]) {
                ans = pathP[i]; // 持续更新，最后一个相同的就是LCA
            } else {
                break; // 分叉了，前一个就是答案
            }
        }
        return ans;
    }
    
private:
    // 回溯法找从 root 到 target 的路径
    bool findPath(TreeNode* root, TreeNode* target, vector<TreeNode*>& path) {
        if (!root) return false;
        path.push_back(root);       // 选择当前节点
        if (root == target) return true;
        // 在左子树或右子树中找到了 target
        if (findPath(root->left, target, path) || 
            findPath(root->right, target, path)) {
            return true;
        }
        path.pop_back();            // 撤销选择（回溯）
        return false;
    }
};
```

**关键点**：`findPath` 是典型的回溯——加入路径 → 递归探索 → 没找到就撤销。路径比较时注意 `break` 的位置，分叉后立即停止。

---

### 解法2: 递归（后序遍历）— O(n) / O(h) ⭐ 面试首选

**从解法1优化**：解法1需要两次遍历 + 额外路径存储。能否在一次遍历中同时搞定？关键洞察：用后序遍历自底向上汇报信息，当左右子树的"汇报"在某节点交汇时，该节点就是 LCA。

**递归三要素**：
- **返回值**：在以 root 为根的子树中，找到的 p 或 q（或它们的 LCA），找不到返回 null
- **终止条件**：root 为空，或 root 就是 p 或 q
- **单层逻辑**：递归左右子树，根据返回值判断 LCA 在哪

```
//  递归过程（p=5, q=1）:
//
//         3
//        / \
//       5   1
//      / \ / \
//     6  2 0  8
//       / \
//      7   4
//
//  自底向上汇报：
//  节点6: 左null 右null → 不是p也不是q → 返回null
//  节点7: 同理 → 返回null
//  节点4: 同理 → 返回null
//  节点2: 左null 右null → 返回null
//  节点5: root==p → 直接返回5（不再往下看）
//  节点0: → 返回null
//  节点8: → 返回null
//  节点1: root==q → 直接返回1
//  节点3: left=5(非null), right=1(非null)
//         → 左右都找到了！→ 返回3 → 这就是LCA！
```

```cpp
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        // 终止条件：空节点，或者找到了 p 或 q
        if (!root || root == p || root == q) return root;
        
        // 后序遍历：先递归左右子树
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);
        
        // 汇总：左右都非空 → p、q 分居两侧 → 当前节点是 LCA
        if (left && right) return root;
        
        // 只有一侧非空 → LCA 在那一侧（另一侧没有目标节点）
        return left ? left : right;
    }
};
```

**关键点**：
- 这段代码只有 6 行核心逻辑，但含义非常深。`root == p` 时直接返回，**不继续往下搜索**——因为即使 q 在 p 下面，p 也是 LCA；如果 q 不在 p 下面，q 会被另一个分支找到。
- 返回值是"多义的"：可能是 p 本身、q 本身、或者 LCA。调用者通过左右返回值的组合来判断。

---

### 解法3: 迭代（父指针 + 祖先集合）— O(n) / O(n)

**思考过程**：如果面试官要求不用递归怎么办？我们可以先用迭代 BFS/DFS 遍历整棵树，记录每个节点的父节点。然后从 p 往上走到根，记录沿途所有祖先。再从 q 往上走，第一个出现在 p 的祖先集合中的节点就是 LCA。

```cpp
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        // Step1: 迭代遍历，建立 "子→父" 的映射
        unordered_map<TreeNode*, TreeNode*> parent;
        parent[root] = nullptr;
        
        stack<TreeNode*> stk;
        stk.push(root);
        
        // 遍历直到 p 和 q 都找到为止（它们的父节点都已记录）
        while (!parent.count(p) || !parent.count(q)) {
            TreeNode* node = stk.top(); stk.pop();
            if (node->left) {
                parent[node->left] = node;
                stk.push(node->left);
            }
            if (node->right) {
                parent[node->right] = node;
                stk.push(node->right);
            }
        }
        
        // Step2: 从 p 往上走，收集 p 的所有祖先
        unordered_set<TreeNode*> ancestors;
        TreeNode* cur = p;
        while (cur) {
            ancestors.insert(cur);
            cur = parent[cur];
        }
        
        // Step3: 从 q 往上走，第一个在 p 祖先集合中的就是 LCA
        cur = q;
        while (!ancestors.count(cur)) {
            cur = parent[cur];
        }
        return cur;
    }
};
```

**关键点**：这个解法的思想和"链表找交点"（LeetCode 160）非常相似——两条链（从 p 到根、从 q 到根）找第一个公共节点。

---

## 解法对比

| | 解法1: 记录路径 | 解法2: 递归后序 ⭐ | 解法3: 迭代父指针 |
|---|---|---|---|
| 时间 | O(n) | O(n) | O(n) |
| 空间 | O(n) 路径 | O(h) 递归栈 | O(n) 哈希表 |
| 代码量 | 中等 | 极少 | 较多 |
| 核心思想 | 找路径→比较 | 后序汇报→交汇 | 建父指针→回溯 |
| 适用场景 | 需要路径信息时 | 面试首选 | 不能用递归时 |

**选择建议**：面试中优先写解法2（递归），代码简洁且体现对递归的深刻理解。如果面试官追问"不用递归"，切换到解法3。

## 易错点

1. **✗ 遗漏 `root == p || root == q` 的终止条件**
   ```cpp
   // 错误：只写了 if (!root) return nullptr;
   // 这样永远找不到 p 和 q
   ```
   **✓ 必须在发现 root 就是 p 或 q 时直接返回**，这既是终止条件，也处理了"p 是 q 的祖先"的情况。

2. **✗ 认为找到 p 后还要继续搜索 p 的子树找 q**
   ```cpp
   // 错误想法：root==p 时不能直接返回，万一 q 在 p 下面呢？
   // 正确理解：即使 q 在 p 下面，p 本身就是 LCA，直接返回 p 是对的
   ```

3. **✗ 返回值理解错误，以为返回的一定是 LCA**
   ```cpp
   // 递归返回的可能是 p、q、LCA、或 null，取决于子树中的情况
   // 只有在最终的调用者那里，返回值才一定是 LCA
   ```

4. **✗ 解法3中 while 循环条件写成 `&&` 而不是 `||`**
   ```cpp
   // 错误：while (!parent.count(p) && !parent.count(q))
   // 这样只要找到一个就停了，另一个可能还没找到
   // 正确：while (!parent.count(p) || !parent.count(q))
   ```

## 面试追问

**Q1: 这个递归函数的返回值到底代表什么？**
> 返回值含义：在以 root 为根的子树中，找到的 p 或 q（或它们的 LCA）。如果没找到任何目标，返回 null。这是一个"多义返回值"——同一个函数在不同场景下返回的含义不同，但调用者通过组合左右结果可以正确判断。

**Q2: 如果 p 或 q 可能不在树中呢？（题目不保证存在）**
> 当前代码会出错。需要修改：不能在遇到 p/q 时立即返回，而要完整遍历两个子树。用两个 bool 标记是否真的找到了 p 和 q。只有两个都找到，返回的 LCA 才有效，否则返回 null。

**Q3: 如果是 BST（二叉搜索树）而不是普通二叉树呢？**
> 可以利用 BST 的有序性优化：如果 p、q 的值都小于 root，LCA 在左子树；都大于 root，LCA 在右子树；一大一小，root 就是 LCA。这就是 LeetCode 235 的做法，时间仍为 O(h)，但不需要遍历整棵树。

**Q4: 如果有大量查询（多对 p、q），每次都从头遍历太慢，怎么优化？**
> 可以用 Euler Tour + Sparse Table 实现 O(n) 预处理 + O(1) 单次查询。或者用 Tarjan 离线 LCA 算法（基于并查集），将所有查询一次性处理。这属于竞赛级别的优化。

## 相关题型

- **235. 二叉搜索树的最近公共祖先** — 本题的 BST 特化版。递归框架相同，区别是：不需要遍历左右子树再汇总，而是利用 BST 性质直接判断往左还是往右。代码从 `if(left && right) return root` 变成 `if(p->val < root->val && q->val < root->val) 往左走`。

- **1644. 二叉树的最近公共祖先 II** — p 或 q 可能不在树中。复用本题递归框架，但不能遇到 p/q 就提前返回，需要完整遍历后用标记判断。

- **1650. 二叉树的最近公共祖先 III** — 每个节点有 parent 指针。直接变成"两条链表找交点"问题（LeetCode 160），不需要先建父指针表。

- **160. 相交链表** — 解法3 的核心思想：两条路径找第一个交点，完全复用同样的 set 回溯方法。