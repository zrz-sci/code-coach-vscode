# 2096. 从二叉树一个节点到另一个节点每一步的方向

## 核心思路

二叉树中两个节点之间的最短路径**一定经过它们的最近公共祖先 (LCA)**。找到 LCA 后，起点到 LCA 的部分全是 'U'，LCA 到终点的部分是从根到终点路径的后半段。

## 思维链

1. **读完题第一反应**：树中两点之间的路径是唯一的（树无环），所以"最短路径"就是"唯一路径"。我需要从 start 走到 dest，允许向上走（U）、向左走（L）、向右走（R）。

2. **朴素想法**：如果我能知道从根到 start 的路径和从根到 dest 的路径，是不是就能拼出答案？比如从根到 start 是 "LL"，从根到 dest 是 "LR"，那公共前缀是 "L"（即 LCA 的位置），start 需要先向上走回 LCA（1步 "U"），再从 LCA 向下走到 dest（"R"）。

3. **关键洞察**：两条从根出发的路径，**公共前缀**对应的终点就是 LCA！去掉公共前缀后，start 的剩余路径长度就是需要走的 'U' 数量，dest 的剩余路径就是需要走的 L/R 方向。

4. **另一种思路**：也可以先用经典算法找 LCA 节点，然后分别求 LCA→start 和 LCA→dest 的路径。LCA→start 的路径全部替换为 'U'，拼上 LCA→dest 的路径。

5. **优化思考**：两种方式都是 O(n) 时间，但"找路径+去公共前缀"的方式更直观，不需要单独实现 LCA。

```
示例1的树:
         5
        / \
       1   2
      /   / \
     3   6   4

root→start(3) 的路径: "LL"   (5→1→3)
root→dest(6) 的路径:  "RL"   (5→2→6)

公共前缀: "" (第一个字符就不同 'L' vs 'R')
start剩余 "LL" → 长度2 → "UU"
dest剩余  "RL" → 保持   → "RL"

答案 = "UU" + "RL" = "UURL"
```

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 找LCA + 两次DFS | 先找LCA节点，再分别找LCA到两个目标的路径 | O(n) | O(n) | ⭐ 必须写出 |
| 两条根路径 + 去公共前缀 | 找根到start和dest的路径，去掉公共前缀 | O(n) | O(n) | ⭐ 必须写出 |
| BFS建图 | 把树转成无向图，BFS求最短路径 | O(n) | O(n) | 能说出即可 |

## 关键提示

1. **树中两点路径唯一**：不需要"最短路径算法"，找到那条唯一路径即可。

2. **LCA 是关键**：从 start 到 dest 的路径一定是 start → LCA → dest。start 到 LCA 全是 'U'。

3. **公共前缀技巧**：如果你求出了 root→start 和 root→dest 两条路径字符串，公共前缀的长度就告诉你 LCA 的深度位置。

4. **DFS 找路径**：用回溯法从根 DFS，记录路径。到达目标节点时保存路径。

5. **易忽略点**：LCA 可能就是 start 或 dest 本身（一个是另一个的祖先）。

```
LCA 示意:
         5 ← LCA
        / \
       1   2
      /   / \
     3   6   4
     ↑       
  start     dest(6)

路径: 3 →(U) 1 →(U) 5 →(R) 2 →(L) 6
      "UURL"
```

## 解法详解

### 解法1: 找LCA + 两次DFS — O(n) / O(n) ⭐ 面试首选

**思考过程**: 树中两点的最短路径一定经过 LCA。所以问题拆成三步：
1. 找到 start 和 dest 的 LCA
2. 从 LCA 出发 DFS 找到 start 的路径 → 全部替换为 'U'
3. 从 LCA 出发 DFS 找到 dest 的路径 → 保持 L/R

```cpp
class Solution {
public:
    // 经典LCA算法：后序遍历
    TreeNode* findLCA(TreeNode* node, int s, int d) {
        if (!node || node->val == s || node->val == d) return node;
        TreeNode* left = findLCA(node->left, s, d);
        TreeNode* right = findLCA(node->right, s, d);
        if (left && right) return node;   // s和d分别在左右子树 → 当前节点是LCA
        return left ? left : right;        // 哪边找到了返回哪边
    }
    
    // 从node出发DFS找target，路径记录在path中
    bool findPath(TreeNode* node, int target, string& path) {
        if (!node) return false;
        if (node->val == target) return true;
        
        path.push_back('L');
        if (findPath(node->left, target, path)) return true;
        path.pop_back();  // 回溯
        
        path.push_back('R');
        if (findPath(node->right, target, path)) return true;
        path.pop_back();  // 回溯
        
        return false;
    }
    
    string getDirections(TreeNode* root, int startValue, int destValue) {
        // Step1: 找LCA
        TreeNode* lca = findLCA(root, startValue, destValue);
        
        // Step2: 从LCA到start的路径（全变U）
        string pathToStart;
        findPath(lca, startValue, pathToStart);
        
        // Step3: 从LCA到dest的路径（保持L/R）
        string pathToDest;
        findPath(lca, destValue, pathToDest);
        
        // start到LCA：每一步都是向上走
        string ups(pathToStart.size(), 'U');
        return ups + pathToDest;
    }
};
```

**关键点**: 
- LCA 算法是后序遍历，left 和 right 都非空说明 s、d 分居两侧，当前节点就是 LCA。
- findPath 用回溯：尝试左边走不通就撤销，再尝试右边。

---

### 解法2: 两条根路径 + 去公共前缀 — O(n) / O(n) ⭐ 同样推荐

**思考过程**: 不需要显式找 LCA。从根分别到 start 和 dest 各找一条路径，它们的公共前缀就对应"根到 LCA"的部分。去掉公共前缀后：
- start 的剩余路径长度 → 全变 'U'
- dest 的剩余路径 → 保持 L/R

```
root→start: "L L"       root→dest: "R L"
公共前缀: ""  (一开始就不同)
start剩余长度=2 → "UU"
dest剩余="RL"
结果 = "UURL"

另一个例子，如果:
root→start: "L L R"     root→dest: "L L L"
公共前缀: "L L"  (长度2)
start剩余长度=1 → "U"
dest剩余="L"
结果 = "UL"
```

```cpp
class Solution {
public:
    bool findPath(TreeNode* node, int target, string& path) {
        if (!node) return false;
        if (node->val == target) return true;
        
        path.push_back('L');
        if (findPath(node->left, target, path)) return true;
        path.pop_back();
        
        path.push_back('R');
        if (findPath(node->right, target, path)) return true;
        path.pop_back();
        
        return false;
    }
    
    string getDirections(TreeNode* root, int startValue, int destValue) {
        string pathToStart, pathToDest;
        findPath(root, startValue, pathToStart);
        findPath(root, destValue, pathToDest);
        
        // 找公共前缀长度（公共前缀 = 根到LCA的路径）
        int i = 0;
        while (i < pathToStart.size() && i < pathToDest.size() 
               && pathToStart[i] == pathToDest[i]) {
            i++;
        }
        
        // start的剩余部分全部变U（向上走到LCA）
        string ups(pathToStart.size() - i, 'U');
        // dest的剩余部分保持（从LCA向下走到dest）
        string downs = pathToDest.substr(i);
        
        return ups + downs;
    }
};
```

**关键点**: 公共前缀的长度 i 就是 LCA 在路径中的深度。这个方法避免了显式写 LCA 函数，代码更简洁。

---

### 解法3: BFS建无向图 — O(n) / O(n)

**思考过程**: 把二叉树看成无向图。用 BFS 从 start 出发找 dest，BFS 天然找最短路径。需要额外记录每条边的方向（L/R/U）。

```cpp
class Solution {
public:
    string getDirections(TreeNode* root, int startValue, int destValue) {
        // 建邻接表：node_val → [(neighbor_val, direction)]
        unordered_map<int, vector<pair<int, char>>> graph;
        
        // BFS建图
        queue<TreeNode*> q;
        q.push(root);
        while (!q.empty()) {
            TreeNode* node = q.front(); q.pop();
            if (node->left) {
                graph[node->val].push_back({node->left->val, 'L'});
                graph[node->left->val].push_back({node->val, 'U'});
                q.push(node->left);
            }
            if (node->right) {
                graph[node->val].push_back({node->right->val, 'R'});
                graph[node->right->val].push_back({node->val, 'U'});
                q.push(node->right);
            }
        }
        
        // BFS找最短路径
        queue<pair<int, string>> bfs;
        unordered_set<int> visited;
        bfs.push({startValue, ""});
        visited.insert(startValue);
        
        while (!bfs.empty()) {
            auto [curr, path] = bfs.front(); bfs.pop();
            if (curr == destValue) return path;
            for (auto& [next, dir] : graph[curr]) {
                if (!visited.count(next)) {
                    visited.insert(next);
                    bfs.push({next, path + dir});
                }
            }
        }
        return "";  // 不会到达这里
    }
};
```

**关键点**: 这种方法概念简单但空间开销大（建图 + BFS 队列中存储路径字符串）。面试中可以作为第一反应提出，然后优化到解法1或2。

## 解法对比

| | 解法1: LCA + 两次DFS | 解法2: 根路径去前缀 | 解法3: BFS建图 |
|---|---|---|---|
| **核心思想** | 先找LCA，再从LCA出发 | 用公共前缀隐式得到LCA | 当成图的最短路径 |
| **代码量** | 中等（需写LCA+findPath） | 最少（只需findPath） | 最多（建图+BFS） |
| **空间** | O(h) 递归栈 + O(h) 路径 | O(h) 递归栈 + O(h) 路径 | O(n) 邻接表 + O(n) 队列 |
| **面试推荐** | ⭐ 展示LCA功底 | ⭐ 代码最简洁 | 第一反应可提 |

> h 是树的高度，最坏 O(n)。解法1和2实际空间使用更优。

## 易错点

1. **findPath 中忘记回溯**
   - ✗ `path.push_back('L'); findPath(node->left, target, path);` 走不通后没 pop_back
   - ✓ 必须在递归返回 false 后 `path.pop_back()` 撤销选择

2. **公共前缀比较时越界**
   - ✗ `while (pathToStart[i] == pathToDest[i])` 没检查 i 是否越界
   - ✓ `while (i < pathToStart.size() && i < pathToDest.size() && ...)`

3. **LCA 返回条件写错**
   - ✗ 只写 `if (!node) return nullptr;` 忘记 `node->val == s || node->val == d` 的情况
   - ✓ 找到目标节点时应该直接返回，不需要继续往下搜

4. **start 到 LCA 的路径方向搞反**
   - ✗ 保持 findPath 返回的 L/R 方向（那是从 LCA 到 start 的方向）
   - ✓ start 到 LCA 全部是 'U'，不是把 L/R 反转

5. **BFS 解法中字符串拼接导致 TLE**
   - BFS 队列中存 `path + dir` 每次都复制字符串，O(n²) 总开销
   - 可以改用 parent 数组回溯路径来优化

## 面试追问

**Q1: 你能想到几种方法？各自的优缺点？**
→ 三种：BFS建图（最直觉但空间大）、LCA+DFS（经典树算法）、根路径去前缀（最简洁）。面试中推荐先说 BFS 想法，再优化到 LCA 方案。

**Q2: 如果树非常深（接近 n），递归会有什么问题？怎么处理？**
→ 递归深度 O(n) 可能栈溢出。可以用迭代 DFS（手动栈）来找路径，或者用 BFS 建图的方式（解法3）避免深递归。

**Q3: 如果需要多次查询不同的 (start, dest) 对，怎么优化？**
→ 预处理：用 Euler Tour + Sparse Table 实现 O(1) LCA 查询；预存每个节点到根的路径。这样每次查询只需 O(深度差) 拼接答案。或者用 Binary Lifting 实现 O(log n) LCA。

**Q4: 这题和 LeetCode 236 (LCA) 有什么关系？**
→ 本题的核心子问题就是 236。解法1 直接复用了 236 的 LCA 算法。解法2 用公共前缀巧妙绕过了显式 LCA。

## 相关题型

- **236. 二叉树的最近公共祖先** — 本题的核心子问题，解法1直接复用 LCA 模板
- **257. 二叉树的所有路径** — 复用 DFS + 回溯找路径的框架，区别是本题找特定目标而非叶子
- **1740. 找到二叉树中的距离** — 同样基于 LCA，但只需求距离不需要方向，LCA到两个节点的深度之和即为答案
- **865. 具有所有最深节点的最小子树** — 同样的 LCA 后序遍历思想的变体