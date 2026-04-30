# 987. 二叉树的垂序遍历

## 核心思路

这道题本质上是：**给二叉树的每个节点分配一个 (row, col) 坐标，然后按 col 分组、组内按 (row, val) 排序输出**。难点不在遍历本身，而在排序规则的正确实现。

## 思维链

1. **读完题第一反应**：每个节点有一个 (row, col) 位置，需要按列从左到右输出。→ 先遍历树，给每个节点标上坐标。
2. **坐标怎么分配？** 根节点 (0, 0)，左孩子 (row+1, col-1)，右孩子 (row+1, col+1)。用 DFS 或 BFS 都能给所有节点打标。
3. **坐标打完之后怎么办？** 需要按列 (col) 分组，同一列内按行 (row) 排序，同行同列按值 (val) 排序。→ 这就是一个多关键字排序问题。
4. **怎么实现多关键字排序？** 方法一：收集所有 (col, row, val) 三元组，全局排序后分组。方法二：用 `map<int, ...>` 按 col 自动排序，组内再排序。
5. **为什么这题是 Hard？** 因为排序规则容易搞错——注意和 LeetCode 314（普通垂序遍历）的区别：314 中同行同列按**左到右的遍历顺序**，本题 987 中同行同列按**值从小到大**。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DFS + 全局排序 | 收集三元组 (col, row, val)，排序后分组 | O(n log n) | O(n) | ⭐ 必须写出 |
| BFS + TreeMap | 层序遍历 + map<col, vector> 分组排序 | O(n log n) | O(n) | 能说出即可 |
| DFS + TreeMap 精细分组 | map<col, map<row, multiset<val>>> | O(n log n) | O(n) | 加分项 |

## 关键提示

1. **坐标系可视化**：先画出示例树上每个节点的 (row, col)，确保你理解坐标分配规则。

```
示例2: root = [1,2,3,4,5,6,7]

           1(0,0)
          /       \
      2(1,-1)    3(1,1)
      /    \     /    \
  4(2,-2) 5(2,0) 6(2,0) 7(2,2)

注意：5 和 6 坐标相同 (2,0)，按值排序 → 5 在 6 前面
```

2. **排序优先级**：第一关键字 col（列，从左到右），第二关键字 row（行，从上到下），第三关键字 val（值，从小到大）。把这三个关键字排对，题目就解了。

3. **和 314 题的区别**：314 题同行同列不按值排序，而是按遍历顺序。987 题明确要求同行同列按值排序。如果你做过 314，**不要**照搬代码。

4. **col 可能为负数**：根的 col = 0，左子树 col 递减。用 `map<int, ...>`（有序）或者收集后排序都能处理负数列号。

5. **最终分组**：排序完之后，相同 col 的节点要放到同一个子数组里。可以用 map 天然分组，也可以排序后线性扫描分组。

## 解法详解

### 解法1: DFS + 全局排序 — O(n log n) / O(n) ⭐ 面试首选

**思考过程**：既然每个节点需要一个 (col, row, val) 三元组，最简单的方式就是 DFS 遍历树把所有三元组收集起来，然后按 (col, row, val) 排序，最后按 col 分组输出。

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
    vector<vector<int>> verticalTraversal(TreeNode* root) {
        // nodes 存储 {col, row, val} 三元组
        vector<tuple<int,int,int>> nodes;
        
        // DFS 给每个节点标上坐标
        function<void(TreeNode*, int, int)> dfs = [&](TreeNode* node, int row, int col) {
            if (!node) return;
            nodes.push_back({col, row, node->val});
            dfs(node->left, row + 1, col - 1);
            dfs(node->right, row + 1, col + 1);
        };
        dfs(root, 0, 0);
        
        // 按 (col, row, val) 排序 — tuple 默认按字典序比较，正好满足需求
        sort(nodes.begin(), nodes.end());
        
        // 按 col 分组输出
        vector<vector<int>> result;
        int prevCol = INT_MIN;
        for (auto& [col, row, val] : nodes) {
            if (col != prevCol) {
                result.push_back({});  // 新的一列
                prevCol = col;
            }
            result.back().push_back(val);
        }
        return result;
    }
};
```

**关键点**：
- `tuple<int,int,int>` 的默认比较就是字典序，先比第一个（col），再比第二个（row），最后比第三个（val），完美匹配题目要求。
- 分组时只需要检测 col 是否变化，因为排序后同一列的节点一定相邻。

### 解法2: BFS + TreeMap — O(n log n) / O(n)

**从另一个角度**：用 BFS（层序遍历）天然按行从上到下访问，用 `map<int, vector<pair<int,int>>>` 按列分组（map 按 key 自动排序），组内存 (row, val) 再排序。

```cpp
class Solution {
public:
    vector<vector<int>> verticalTraversal(TreeNode* root) {
        // key = col, value = 该列所有节点的 (row, val)
        map<int, vector<pair<int,int>>> colMap;
        
        // BFS，队列存 {节点, row, col}
        queue<tuple<TreeNode*, int, int>> q;
        q.push({root, 0, 0});
        
        while (!q.empty()) {
            auto [node, row, col] = q.front(); q.pop();
            colMap[col].push_back({row, node->val});
            if (node->left)  q.push({node->left, row + 1, col - 1});
            if (node->right) q.push({node->right, row + 1, col + 1});
        }
        
        // 按列输出，每列内按 (row, val) 排序
        vector<vector<int>> result;
        for (auto& [col, vec] : colMap) {
            sort(vec.begin(), vec.end()); // pair 默认按 (row, val) 字典序
            vector<int> column;
            for (auto& [row, val] : vec) {
                column.push_back(val);
            }
            result.push_back(column);
        }
        return result;
    }
};
```

**关键点**：
- `map<int, ...>` 的 key 自动按升序排列，所以列从左到右不需要额外排序。
- 组内 `sort` 对 `pair<int,int>` 排序，先按 row 再按 val，也是字典序。

### 解法3: DFS + 嵌套 TreeMap — O(n log n) / O(n)

**精细分组**：用 `map<int, map<int, multiset<int>>>` 即 `col → row → values`。利用 map 和 multiset 的有序性，插入时就已经排好序了，最后直接遍历输出。

```cpp
class Solution {
public:
    vector<vector<int>> verticalTraversal(TreeNode* root) {
        // col → row → 该位置所有值（multiset 自动排序且允许重复）
        map<int, map<int, multiset<int>>> grid;
        
        function<void(TreeNode*, int, int)> dfs = [&](TreeNode* node, int row, int col) {
            if (!node) return;
            grid[col][row].insert(node->val);
            dfs(node->left, row + 1, col - 1);
            dfs(node->right, row + 1, col + 1);
        };
        dfs(root, 0, 0);
        
        // 直接遍历，三层都是有序的
        vector<vector<int>> result;
        for (auto& [col, rowMap] : grid) {
            vector<int> column;
            for (auto& [row, vals] : rowMap) {
                for (int v : vals) {
                    column.push_back(v);
                }
            }
            result.push_back(column);
        }
        return result;
    }
};
```

**关键点**：
- 三层嵌套的有序容器让插入即排序，但代码的可读性和空间开销稍大。
- `multiset` 而不是 `set`，因为同行同列可能有相同值的节点。

## 解法对比

| | 解法1: DFS+全局排序 | 解法2: BFS+TreeMap | 解法3: DFS+嵌套TreeMap |
|---|---|---|---|
| 核心思路 | 收集三元组全局排序 | BFS 遍历 + map 分组 | 嵌套有序容器 |
| 代码简洁度 | ⭐⭐⭐ 最简洁 | ⭐⭐ 中等 | ⭐ 略繁琐 |
| 面试推荐 | **首选** | 可以 | 如果面试官追问 |
| 排序方式 | 一次全局 sort | map 分组 + 组内 sort | 插入即有序 |

**面试建议**：首选解法1，代码量最少、思路最直接。解法3 适合展示你对 STL 有序容器的熟练运用。

## 易错点

1. **✗ 把 tuple 顺序写成 `(row, col, val)`** → ✓ 必须是 `(col, row, val)`。因为第一分组关键字是列，不是行。错误顺序会导致输出按行分组。

2. **✗ 同行同列按遍历顺序而非按值排序** → ✓ 987 题明确要求同行同列按值排序。如果你做过 314 题，注意这里的区别：
   ```
   // 314 题：同行同列按 BFS 遍历顺序（从左到右）
   // 987 题：同行同列按值从小到大 ← 本题！
   ```

3. **✗ 用 `set` 而不是 `multiset`** → ✓ 如果同行同列有两个相同值的节点，`set` 会去重丢失一个。

4. **✗ 忘记 col 可以为负数，用数组下标直接存** → ✓ 用 `map` 或者排序后动态分组，不要预分配数组。

## 面试追问

**Q1（基础理解）**：这道题的排序规则是什么？和普通的垂直遍历（314 题）有什么区别？
> A：排序三关键字：col 升序 → row 升序 → val 升序。314 题中同行同列按遍历顺序，987 题按值排序。

**Q2（优化方向）**：如果节点数非常大（如 10^6），你的解法瓶颈在哪？
> A：瓶颈在排序 O(n log n)。但这是不可避免的下界，因为同行同列需要按值排序。可以优化常数：用 DFS 收集 + 全局排序（解法1），避免多层 map 的额外开销。

**Q3（变体）**：如果题目改成"同行同列按左到右的遍历顺序"（即 314 题），代码怎么改？
> A：不需要 val 作为排序关键字了。可以用 BFS 层序遍历，同层从左到右自然有序。或者 DFS 收集 (col, row, **遍历序号**) 而非 (col, row, val)。

**Q4（拓展）**：能否不用全局排序，做到 O(n) 时间？
> A：一般情况下不行，因为同行同列需要按值排序。但如果值域有限（如本题 val ≤ 1000），理论上可以用计数排序做到 O(n + V)，但面试中意义不大。

## 相关题型

- **314. 二叉树的垂直遍历** — 本题的"弱化版"，同行同列不按值排序而按遍历顺序。复用本题的坐标分配框架，但排序规则不同。**关键区别：把三元组的第三关键字从 val 改成遍历序号**。
- **102. 二叉树的层序遍历** — 复用 BFS 逐层遍历的模板，本题在层序基础上增加了列的维度。
- **199. 二叉树的右视图** — 同样是按某种"视角"组织节点输出，复用 BFS/DFS + 坐标标记的思路。
- **1382. 将二叉搜索树变平衡** — 也涉及中序遍历后重新组织节点，是树遍历 + 后处理的另一类应用。