# 366. 寻找二叉树的叶子节点

## 核心思路

这道题本质上是按节点的"高度"（从叶子算起的距离）对二叉树节点进行分组——叶子节点高度为0，其父节点高度为1，以此类推。每一轮"剥叶子"操作恰好剥掉同一高度层的所有节点。

## 思维链

1. **读完题第一反应**：模拟题意——每轮找出所有叶子，收集并删除，重复直到树空。这就是最朴素的"多轮剥叶子"做法。
2. **暴力的瓶颈**：每一轮都要遍历整棵树才能找到叶子，剥掉后再遍历一遍……如果树退化成链表（高度 h=n），要做 O(n) 轮，每轮 O(n)，总共 O(n²)。
3. **关键观察**：一个节点在第几轮被剥掉，完全取决于它离最远叶子的距离（即它的"高度"）。叶子高度=0，叶子的父亲高度=1，根的高度=树高。`height(node) = 1 + max(height(left), height(right))`，叶子为0。
4. **突破**：只需要一次后序 DFS，算出每个节点的高度，高度相同的节点放进同一组。高度值正好就是结果数组的下标。
5. **实现**：后序遍历（先左右再自己），递归返回高度，按高度把节点值放入 `result[height]`。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力模拟 | 多轮遍历，每轮找叶子并删除 | O(n²) | O(n) | 能说出即可 |
| 后序DFS算高度 | 一次遍历，按高度分组 | O(n) | O(n) | ⭐ 必须写出 |
| BFS拓扑排序 | 从叶子向内逐层剥离（类似拓扑） | O(n) | O(n) | 加分项 |

## 关键提示

1. **高度 vs 深度**：深度是从根往下数（根=0），高度是从叶子往上数（叶子=0）。这道题需要的是"高度"——叶子被第0轮剥掉，叶子的父亲第1轮……

2. **后序遍历天然适合**：因为要先知道左右子树的高度，才能算自己的高度——这就是"自底向上"，即后序遍历的处理顺序。

3. **示例推演**：
```
//       1  (高度2)
//      / \
//     2   3  (高度1, 高度0)
//    / \
//   4   5  (高度0, 高度0)
//
// 高度0: [4, 5, 3]  ← 第1轮叶子
// 高度1: [2]        ← 第2轮叶子
// 高度2: [1]        ← 第3轮叶子
```

4. **result 数组动态增长**：当计算出的高度 >= result.size() 时，需要 push_back 一个新的空 vector。

5. **对比"剥洋葱"**：类似 BFS 中"层序遍历"但方向相反——层序从根向外，这里从叶子向内。

## 解法详解

### 解法1: 暴力模拟（多轮剥叶子）— O(n²) / O(n)

**思考过程**：直接按题意模拟。每一轮遍历树，找到所有叶子节点（左右孩子都为空），收集它们的值，然后把它们从树上"删掉"（将父节点对应的指针设为 null）。重复直到根也被删掉。

```cpp
class Solution {
public:
    vector<vector<int>> findLeaves(TreeNode* root) {
        vector<vector<int>> result;
        // 每一轮收集并删除所有叶子
        while (root) {
            vector<int> leaves;
            root = removeLeaves(root, leaves);
            result.push_back(leaves);
        }
        return result;
    }
    
private:
    // 返回修改后的子树根节点（叶子被删除后返回nullptr）
    TreeNode* removeLeaves(TreeNode* node, vector<int>& leaves) {
        if (!node) return nullptr;
        // 当前节点是叶子：收集并"删除"（返回nullptr给父节点）
        if (!node->left && !node->right) {
            leaves.push_back(node->val);
            return nullptr;
        }
        // 递归处理左右子树，更新指针
        node->left = removeLeaves(node->left, leaves);
        node->right = removeLeaves(node->right, leaves);
        return node;
    }
};
```

**关键点**：
- 每轮需要完整遍历剩余的树，树退化成链表时总遍历量为 n + (n-1) + ... + 1 = O(n²)
- 修改了原树结构（破坏性操作），面试中可能被追问"能否不修改原树？"

### 解法2: 后序DFS按高度分组 — O(n) / O(n) ⭐ 面试首选

**从解法1优化**：解法1多轮遍历的本质是——它不知道每个节点"该在第几轮被剥掉"。如果我们能一次性算出每个节点的"剥离轮次"，就只需一次遍历。

**关键洞察**：节点的"剥离轮次" = 节点的高度（叶子高度为0）。

```
// 高度的定义:
// height(null) = -1
// height(leaf) = 0
// height(node) = 1 + max(height(left), height(right))
//
// 示例:
//       1  h=2 → result[2]
//      / \
//     2   3  h=1, h=0 → result[1], result[0]
//    / \
//   4   5  h=0, h=0 → result[0], result[0]
//
// 后序遍历顺序: 4→5→2→3→1
// 计算过程:
//   visit 4: h = 1+max(-1,-1) = 0 → result[0].push(4)
//   visit 5: h = 1+max(-1,-1) = 0 → result[0].push(5)
//   visit 2: h = 1+max(0,0) = 1   → result[1].push(2)
//   visit 3: h = 1+max(-1,-1) = 0 → result[0].push(3)
//   visit 1: h = 1+max(1,0) = 2   → result[2].push(1)
//
// 结果: [[4,5,3], [2], [1]]
```

```cpp
class Solution {
public:
    vector<vector<int>> findLeaves(TreeNode* root) {
        vector<vector<int>> result;
        getHeight(root, result);
        return result;
    }
    
private:
    // 返回当前节点的高度（叶子=0），并把节点值放入对应的组
    int getHeight(TreeNode* node, vector<vector<int>>& result) {
        if (!node) return -1;  // 空节点高度为-1，叶子就能算出0
        
        int leftH = getHeight(node->left, result);
        int rightH = getHeight(node->right, result);
        int h = 1 + max(leftH, rightH);
        
        // 高度h正好是result的下标，按需扩容
        if (h >= (int)result.size()) {
            result.push_back({});
        }
        result[h].push_back(node->val);
        
        return h;
    }
};
```

**关键点**：
- 空节点返回 -1 而不是 0，这样叶子节点 `1 + max(-1, -1) = 0`，正好对应 result[0]
- 不修改原树结构
- 一次 DFS 就完成所有工作

### 解法3: BFS拓扑排序思路 — O(n) / O(n)

**不同角度**：把树看成图，叶子节点入度为0（或度为1，只连接父节点）。类似拓扑排序，从叶子开始逐层剥离。

```cpp
class Solution {
public:
    vector<vector<int>> findLeaves(TreeNode* root) {
        // 预处理：建邻接表 + 统计度数
        unordered_map<TreeNode*, int> degree;
        unordered_map<TreeNode*, TreeNode*> parent;
        queue<TreeNode*> q;
        
        // BFS建图
        queue<TreeNode*> bfs;
        bfs.push(root);
        degree[root] = 0;
        while (!bfs.empty()) {
            TreeNode* node = bfs.front(); bfs.pop();
            if (node->left) {
                degree[node]++;
                parent[node->left] = node;
                degree[node->left] = 0;
                bfs.push(node->left);
            }
            if (node->right) {
                degree[node]++;
                parent[node->right] = node;
                degree[node->right] = 0;
                bfs.push(node->right);
            }
        }
        
        // 叶子入队
        for (auto& [node, deg] : degree) {
            if (deg == 0) q.push(node);
        }
        
        vector<vector<int>> result;
        while (!q.empty()) {
            int sz = q.size();
            vector<int> layer;
            for (int i = 0; i < sz; i++) {
                TreeNode* node = q.front(); q.pop();
                layer.push_back(node->val);
                if (parent.count(node)) {
                    degree[parent[node]]--;
                    if (degree[parent[node]] == 0) {
                        q.push(parent[node]);
                    }
                }
            }
            result.push_back(layer);
        }
        return result;
    }
};
```

**关键点**：这种方法代码较长，面试中不推荐首选，但展示了"拓扑排序"的思维方式——从外向内逐层剥离，和课程表问题(207/210)异曲同工。

## 解法对比

| | 暴力模拟 | 后序DFS高度法 | BFS拓扑 |
|---|---|---|---|
| 时间 | O(n²) 最坏 | O(n) | O(n) |
| 空间 | O(n) 递归栈 | O(n) 递归栈+结果 | O(n) 哈希表+队列 |
| 修改原树 | 是 | 否 | 否 |
| 代码量 | 中等 | 最少 | 最多 |
| 面试推荐 | 引入话题 | ⭐ 首选 | 了解即可 |

**核心区别**：解法1"模拟"题意，每轮重新找叶子；解法2用"高度=剥离轮次"的洞察将多轮操作压缩成一次遍历。

## 易错点

1. **空节点高度返回值错误**：
   - ✗ `if (!node) return 0;` → 叶子高度变成1，所有结果偏移一位
   - ✓ `if (!node) return -1;` → 叶子 `1 + max(-1,-1) = 0`，正确对应 result[0]

2. **result 数组越界**：
   - ✗ 直接 `result[h].push_back(val)` 而没有检查 h 是否超出 size
   - ✓ 先检查 `if (h >= result.size()) result.push_back({})` 再 push

3. **暴力解中忘记更新指针**：
   - ✗ `removeLeaves(node->left, leaves)` 不赋值回 `node->left`
   - ✓ `node->left = removeLeaves(node->left, leaves)` — 必须接收返回值来"删除"叶子

## 面试追问

**Q1: 暴力模拟的时间复杂度是多少？瓶颈在哪？**
→ O(n²) 最坏（链表形态的树）。瓶颈在每一轮都要重新遍历整棵树找叶子。

**Q2: 能否一次遍历解决？关键观察是什么？**
→ 关键观察：节点被剥离的轮次 = 节点的高度。叶子高度0，第0轮剥掉；高度1的节点第1轮……用后序 DFS 自底向上算高度，按高度分组即可。

**Q3: 如果不能用递归（栈溢出风险），怎么迭代实现？**
→ 用显式栈模拟后序遍历。需要额外记录每个节点的高度。或者用解法3的拓扑排序方法，完全用队列实现，天然迭代。

**Q4: 这个"按高度分层"的思路还能用在哪些题？**
→ 类似的"从外向内剥离"思路见于拓扑排序(207/210)、树的直径计算(543)、以及 N-ary 树的类似问题。

## 相关题型

- **104. 二叉树的最大深度** — 本题的 `getHeight` 函数本质上就是在算高度，区别在于104只返回最终高度，本题额外把节点按高度分组
- **207/210. 课程表** — 复用"拓扑排序从度为0的节点开始逐层剥离"的框架，区别在于207是有向图，本题是树
- **543. 二叉树的直径** — 同样用后序DFS自底向上计算高度，区别在于543利用 `leftH + rightH` 更新全局最大值，本题利用高度做分组