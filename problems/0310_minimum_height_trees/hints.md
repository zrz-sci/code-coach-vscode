# 310. 最小高度树 (Minimum Height Trees)

**难度**: Medium | **标签**: Depth-First Search, Breadth-First Search, Graph, Topological Sort

## 核心思路

本题的本质是**找树的中心**。一棵无根树的最小高度树的根一定是树的**中心节点**——即距离所有叶子最远的节点。树的中心最多有 1 个或 2 个。

最优解法是**拓扑排序/逐层剥叶子**：从所有度为 1 的叶子节点开始，一层一层地向内剥离，最后剩下的 1 个或 2 个节点就是答案。这类似于 BFS 的反向操作——从外向内收缩。

## 思维链

1. **暴力思考**：以每个节点为根做 BFS/DFS 求树高，O(n^2) → 太慢
2. **观察**：树的"最中心"的节点作为根时，高度最小
3. **类比**：找树的中心就像"剥洋葱"——先去掉最外层的叶子
4. **算法**：将所有度为 1 的节点入队，BFS 逐层剥离
5. **终止条件**：当剩余节点数 <= 2 时停止，剩下的就是答案
6. **为什么最多 2 个**：树的直径的中点最多有 2 个（奇数长度 1 个，偶数长度 2 个）

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 | 说明 |
|------|-----------|-----------|--------|------|
| 拓扑排序（剥叶子） | O(n) | O(n) | ⭐⭐⭐⭐⭐ | 最优解，面试首选 |
| 两次 BFS 找直径 | O(n) | O(n) | ⭐⭐⭐⭐ | 先找直径再取中点 |
| 暴力 BFS | O(n^2) | O(n) | ⭐⭐ | 每个节点做一次 BFS |

## 关键提示

1. **特判 n=1**：只有一个节点，直接返回 `{0}`
2. **度数组**：`degree[i]` 记录节点 i 的度（邻居数量），初始度为 1 的是叶子
3. **逐层剥离**：每次将当前所有叶子（degree==1）移除，更新邻居的度
4. **终止条件**：`remaining_nodes <= 2` 时停止
5. **无需真的删除节点**：通过度的更新来模拟删除过程

## 解法详解

### 解法一：拓扑排序 — 逐层剥叶子（推荐）

```cpp
class Solution {
public:
    vector<int> findMinHeightTrees(int n, vector<vector<int>>& edges) {
        if (n == 1) return {0};
        
        // 建图 + 计算度
        vector<vector<int>> adj(n);
        vector<int> degree(n, 0);
        for (auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
            degree[e[0]]++;
            degree[e[1]]++;
        }
        
        // 初始叶子入队
        queue<int> q;
        for (int i = 0; i < n; i++)
            if (degree[i] == 1) q.push(i);
        
        int remaining = n;
        while (remaining > 2) {
            int sz = q.size();
            remaining -= sz;
            for (int i = 0; i < sz; i++) {
                int leaf = q.front(); q.pop();
                for (int neighbor : adj[leaf]) {
                    degree[neighbor]--;
                    if (degree[neighbor] == 1) q.push(neighbor);
                }
            }
        }
        
        // 剩下的节点就是答案
        vector<int> result;
        while (!q.empty()) {
            result.push_back(q.front());
            q.pop();
        }
        return result;
    }
};
```

### 解法二：两次 BFS 找直径中点

```cpp
class Solution {
public:
    vector<int> findMinHeightTrees(int n, vector<vector<int>>& edges) {
        if (n == 1) return {0};
        
        vector<vector<int>> adj(n);
        for (auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }
        
        // 第一次 BFS：从任意点出发找最远的端点 u
        auto [u, _] = bfs(adj, 0, n);
        // 第二次 BFS：从 u 出发找最远的端点 v，同时记录路径
        auto [v, parent] = bfs(adj, u, n);
        
        // 找到直径路径
        vector<int> path;
        for (int cur = v; cur != -1; cur = parent[cur])
            path.push_back(cur);
        
        // 取中间节点
        int len = path.size();
        if (len % 2 == 1) return {path[len / 2]};
        else return {path[len / 2 - 1], path[len / 2]};
    }
    
private:
    pair<int, vector<int>> bfs(vector<vector<int>>& adj, int start, int n) {
        vector<int> dist(n, -1);
        vector<int> parent(n, -1);
        queue<int> q;
        q.push(start);
        dist[start] = 0;
        int farthest = start;
        
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int v : adj[u]) {
                if (dist[v] == -1) {
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q.push(v);
                    if (dist[v] > dist[farthest]) farthest = v;
                }
            }
        }
        return {farthest, parent};
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 忘记特判 n==1 | ✓ `if (n == 1) return {0};` | n=1 时没有边，没有叶子 |
| ✗ `while (remaining > 1)` | ✓ `while (remaining > 2)` | 答案可能是 1 个或 2 个节点 |
| ✗ 不减 remaining | ✓ `remaining -= q.size()` | 必须正确跟踪剩余节点数 |
| ✗ 只返回队列中的一个 | ✓ 返回队列中所有剩余节点 | 答案可能有 2 个 |
| ✗ 邻接表只加单向边 | ✓ 两个方向都加 | 无向图必须双向 |

## 面试追问

**Q1: 为什么答案最多只有 2 个节点？**
→ 树的中心是直径的中点。如果直径长度为奇数，中点唯一（1 个）；如果直径长度为偶数，中间有两个相邻节点（2 个）。不可能有 3 个或更多。

**Q2: 拓扑排序剥叶子和找直径中点的关系？**
→ 本质等价。剥叶子是从外向内收缩到中心；找直径中点是先找最长路径再定位中间。两者都能在 O(n) 内解决。拓扑排序更直觉、代码更短。

**Q3: 如果是有权图（边有权重），如何找最小高度树的根？**
→ 不能直接用度来判断叶子。需要用两次 BFS/DFS 找带权直径（替换距离为权重和），然后在直径路径上找使最大距离最小的中间位置（可能不在节点上）。

## 相关题型

- [207. Course Schedule](https://leetcode.com/problems/course-schedule/) - 拓扑排序基础
- [210. Course Schedule II](https://leetcode.com/problems/course-schedule-ii/) - 拓扑排序输出顺序
- [834. Sum of Distances in Tree](https://leetcode.com/problems/sum-of-distances-in-tree/) - 树上距离问题
- [1245. Tree Diameter](https://leetcode.com/problems/tree-diameter/) - 直接求树的直径
