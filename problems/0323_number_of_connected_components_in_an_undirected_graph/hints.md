# 323. 无向图中连通分量的数目 (Number of Connected Components in an Undirected Graph)

**难度**: Medium | **标签**: Depth-First Search, Breadth-First Search, Union Find, Graph

## 核心思路

求无向图的连通分量数，是图论最基础的问题。三种经典方法：**DFS/BFS** 遍历计数连通块，或用 **并查集 (Union-Find)** 合并连通节点后统计独立集合数。

## 思维链

1. **建图**: 用邻接表存储无向图
2. **遍历标记**: 从每个未访问节点出发做 DFS/BFS，每次启动遍历即发现一个新连通分量
3. **并查集替代**: 初始每个节点自成一个集合 (n 个分量)，每条边合并两个集合，最终统计根节点数
4. **结果**: DFS/BFS 启动次数 = 并查集中不同根的个数 = 连通分量数

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 | 说明 |
|------|-----------|-----------|--------|------|
| DFS | O(V + E) | O(V + E) | ⭐⭐⭐⭐⭐ | 最直观 |
| BFS | O(V + E) | O(V + E) | ⭐⭐⭐⭐ | 与 DFS 等价 |
| Union-Find | O(V + E * alpha(V)) | O(V) | ⭐⭐⭐⭐⭐ | 面试高频，适用于动态连通性 |

## 关键提示

1. **邻接表建图**: 无向图每条边加两次 (a->b, b->a)
2. **visited 数组**: 防止重复访问
3. **Union-Find 优化**: 路径压缩 + 按秩合并使 union/find 近 O(1)
4. **分量计数**: 初始 count = n，每次成功 union 时 count--

## 解法详解

### 解法一：DFS

```cpp
class Solution {
public:
    int countComponents(int n, vector<vector<int>>& edges) {
        vector<vector<int>> adj(n);
        for (auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }
        
        vector<bool> visited(n, false);
        int count = 0;
        
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                dfs(adj, visited, i);
                count++;
            }
        }
        
        return count;
    }
    
private:
    void dfs(vector<vector<int>>& adj, vector<bool>& visited, int node) {
        visited[node] = true;
        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                dfs(adj, visited, neighbor);
            }
        }
    }
};
```

### 解法二：BFS

```cpp
class Solution {
public:
    int countComponents(int n, vector<vector<int>>& edges) {
        vector<vector<int>> adj(n);
        for (auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }
        
        vector<bool> visited(n, false);
        int count = 0;
        
        for (int i = 0; i < n; i++) {
            if (!visited[i]) {
                queue<int> q;
                q.push(i);
                visited[i] = true;
                while (!q.empty()) {
                    int node = q.front(); q.pop();
                    for (int nb : adj[node]) {
                        if (!visited[nb]) {
                            visited[nb] = true;
                            q.push(nb);
                        }
                    }
                }
                count++;
            }
        }
        
        return count;
    }
};
```

### 解法三：Union-Find（并查集）

```cpp
class Solution {
public:
    int countComponents(int n, vector<vector<int>>& edges) {
        vector<int> parent(n), rank_(n, 0);
        iota(parent.begin(), parent.end(), 0);  // parent[i] = i
        int count = n;
        
        for (auto& e : edges) {
            if (unite(parent, rank_, e[0], e[1])) {
                count--;
            }
        }
        
        return count;
    }
    
private:
    int find(vector<int>& parent, int x) {
        if (parent[x] != x) parent[x] = find(parent, parent[x]);  // 路径压缩
        return parent[x];
    }
    
    bool unite(vector<int>& parent, vector<int>& rank_, int x, int y) {
        int px = find(parent, x), py = find(parent, y);
        if (px == py) return false;  // 已连通
        if (rank_[px] < rank_[py]) swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
        return true;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 只加单向边 | ✓ 无向图每条边加两次 | `adj[a].push_back(b); adj[b].push_back(a)` |
| ✗ DFS 忘记标记 visited | ✓ 进入节点立即标记 | 否则会无限循环 |
| ✗ Union-Find 没有路径压缩 | ✓ `parent[x] = find(parent, parent[x])` | 不优化会退化为 O(n) |
| ✗ count 初始化为 0 然后 ++ | ✓ Union-Find 中 count 初始为 n 然后 -- | 两种计数方式都行，但别混用 |

## 面试追问

**Q1: DFS vs BFS vs Union-Find 如何选择？**
> 静态图用 DFS/BFS 最简单。如果边是动态增加的（在线查询），Union-Find 更优因为不需要重建图。

**Q2: 如果还需要输出每个连通分量包含哪些节点？**
> DFS/BFS 中收集遍历到的节点即可。Union-Find 则需要最后按根节点分组。

**Q3: 如何处理有向图的强连通分量？**
> 用 Tarjan 算法或 Kosaraju 算法，复杂度同为 O(V+E)，但逻辑更复杂。

## 相关题型

- [200. 岛屿数量](https://leetcode.com/problems/number-of-islands/) - 网格版连通分量
- [547. 省份数量](https://leetcode.com/problems/number-of-provinces/) - 邻接矩阵版
- [261. 以图判树](https://leetcode.com/problems/graph-valid-tree/) - 连通 + 无环
- [305. 岛屿数量 II](https://leetcode.com/problems/number-of-islands-ii/) - 动态连通性 (Union-Find)
- [684. 冗余连接](https://leetcode.com/problems/redundant-connection/) - Union-Find 找环
