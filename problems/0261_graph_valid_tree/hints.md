# 261. Graph Valid Tree (以图判树)

## 核心思路

判断一个无向图是否构成有效树，需要同时满足两个条件：
1. **无环** - 图中不存在回路
2. **连通** - 所有节点都连通（只有一个连通分量）

等价条件：**边数 = n-1 且 图连通**（或边数 = n-1 且无环）。

## 思维链

1. **理解树的定义**: 树是无环的连通无向图，n 个节点恰好有 n-1 条边
2. **快速排除**: 如果 `edges.size() != n - 1`，直接返回 false
3. **选择检测手段**: Union Find / DFS / BFS 均可验证连通性或检测环
4. **Union Find 思路**: 遍历边，合并两端节点；若两端已在同一集合则有环
5. **DFS/BFS 思路**: 建邻接表，从节点 0 出发遍历，检查是否访问了所有 n 个节点
6. **返回结果**: 边数 = n-1 且无环（或全连通）即为有效树

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| Union Find | O(E * α(N)) | O(N) | ⭐ 最优，面试首选 |
| DFS 遍历 | O(N + E) | O(N + E) | 直观好理解 |
| BFS 遍历 | O(N + E) | O(N + E) | 层序遍历 |

## 关键提示

- 树的充要条件：**边数 = n-1** 且 **连通**
- Union Find 中若 find(u) == find(v)，说明 u-v 这条边会形成环
- DFS/BFS 要用 visited 数组防止重复访问
- 注意 n=1, edges=[] 的边界情况（单节点也是树）

## 解法详解

### 解法1: Union Find（并查集）⭐

```cpp
class Solution {
public:
    // 并查集：路径压缩 + 按秩合并
    vector<int> parent, rank_;
    
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    
    bool unite(int x, int y) {
        int px = find(x), py = find(y);
        if (px == py) return false; // 已在同一集合，形成环
        if (rank_[px] < rank_[py]) swap(px, py);
        parent[py] = px;
        if (rank_[px] == rank_[py]) rank_[px]++;
        return true;
    }
    
    bool validTree(int n, vector<vector<int>>& edges) {
        // 树的必要条件：边数 = n - 1
        if ((int)edges.size() != n - 1) return false;
        
        parent.resize(n);
        rank_.resize(n, 0);
        iota(parent.begin(), parent.end(), 0);
        
        for (auto& e : edges) {
            if (!unite(e[0], e[1])) return false; // 有环
        }
        return true; // 边数 = n-1 且无环 => 必定连通
    }
};
```

### 解法2: DFS 遍历

```cpp
class Solution {
public:
    bool validTree(int n, vector<vector<int>>& edges) {
        if ((int)edges.size() != n - 1) return false;
        
        // 建邻接表
        vector<vector<int>> adj(n);
        for (auto& e : edges) {
            adj[e[0]].push_back(e[1]);
            adj[e[1]].push_back(e[0]);
        }
        
        // DFS 检查连通性
        vector<bool> visited(n, false);
        stack<int> stk;
        stk.push(0);
        visited[0] = true;
        int count = 0;
        
        while (!stk.empty()) {
            int node = stk.top(); stk.pop();
            count++;
            for (int nei : adj[node]) {
                if (!visited[nei]) {
                    visited[nei] = true;
                    stk.push(nei);
                }
            }
        }
        
        return count == n; // 所有节点都访问到 => 连通
    }
};
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| 只判断边数 = n-1 | 边数 = n-1 且连通/无环 | 边数对但可能有多个连通分量 |
| 忘记处理 n=1 | n=1 时 edges 为空直接返回 true | 单节点是合法树 |
| DFS 不标记 visited | 入栈/入队时就标记 | 无向图会反复访问导致死循环 |

## 面试追问

- **Q1**: 如何判断无向图中有几个连通分量？
  → 用 Union Find 最后统计不同的 root 数量
- **Q2**: 如果图是有向的，如何判断是否是有效树（有根树）？
  → 需要额外检查入度：根节点入度 0，其余入度 1，且从根可达所有节点
- **Q3**: 如果边是动态添加的，如何在线判断？
  → Union Find 天然支持在线添加边，每次 unite 检测环

## 相关题型

- [323. Number of Connected Components in an Undirected Graph](https://leetcode.com/problems/number-of-connected-components-in-an-undirected-graph/) - 连通分量数
- [684. Redundant Connection](https://leetcode.com/problems/redundant-connection/) - 找多余的边
- [685. Redundant Connection II](https://leetcode.com/problems/redundant-connection-ii/) - 有向图版本
- [547. Number of Provinces](https://leetcode.com/problems/number-of-provinces/) - 并查集基础
