# 886. 可能的二分法

## 核心思路

本质上是一个**图的二分性检测问题**：把每个人看作图的节点，把"不喜欢"关系看作边，问题转化为"能否对图做二着色（2-Coloring）"——相邻节点颜色不同。这等价于判断图是否为**二分图（Bipartite Graph）**。

## 思维链

1. **读完题第一反应**：n 个人分成两组，互相不喜欢的人不能在同一组。每个人只能属于两组之一 → 这就是图的二着色问题。

2. **建模**：把 dislikes 关系构建成邻接表形式的无向图。注意节点编号从 1 开始。

3. **选择算法**：二分图检测有三种经典方法：
   - **BFS 染色**：从未染色节点出发做 BFS，交替染两种颜色，发现冲突则不是二分图。
   - **DFS 染色**：同理，用 DFS 递归交替染色。
   - **Union-Find（并查集）**：对每个节点，把它的所有邻居合并到同一集合（它们应该在同一组）。如果某个邻居和节点本身在同一集合中，则冲突。

4. **图可能不连通**：dislikes 关系可能形成多个连通分量，必须对每个连通分量分别检测。

5. **复杂度分析**：无论 BFS/DFS 还是 Union-Find，时间都是 O(V+E)，其中 V=n, E=|dislikes|。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| BFS 染色 | 逐层交替染色，检测冲突 | O(V+E) | O(V+E) | ⭐ 面试首选 |
| DFS 染色 | 递归交替染色，检测冲突 | O(V+E) | O(V+E) | ⭐ 面试首选 |
| Union-Find | 邻居合并到同一组，检测冲突 | O(V+E·α(V)) | O(V+E) | 加分项 |

> V = n（节点数），E = |dislikes|（边数），α 为反阿克曼函数

## 关键提示

1. **染色方案只有两种颜色**：用 0/1 或 1/-1 表示两种颜色，未染色用特殊值（如 0 或 -1）表示。推荐用 `color[i] = 0` 表示未染色，`1` 和 `-1` 表示两种颜色——这样染对立颜色只需 `-color[node]`。

2. **图可能不连通**：不能只从节点 1 开始搜索一次。必须遍历所有节点 1~n，对每个未染色的节点启动一次 BFS/DFS。

3. **Union-Find 的技巧**：对于节点 u 和它的所有邻居 v1, v2, ..., vk，将 v1~vk 全部合并（它们应该在同一组）。如果在合并过程中发现某个 vi 和 u 已经在同一集合 → 冲突，返回 false。

4. **边是无向的**：dislikes[i] = [a, b] 表示 a 不喜欢 b，同时 b 不喜欢 a。建图时需要双向添加边。

5. **BFS 染色的检测时机**：当从队列取出节点 u 要处理邻居 v 时——如果 v 未染色，染上 u 的对立颜色并入队；如果 v 已染色且和 u 颜色相同，则冲突。

## 解法详解

### 解法1: BFS 染色 — O(V+E) / O(V+E) ⭐ 面试首选

**思考过程**：最直觉的做法——把图看作需要着色的问题，从每个未着色的节点开始 BFS，交替染上两种颜色。每次扩展到邻居时检查是否冲突。

```cpp
class Solution {
public:
    bool possibleBipartition(int n, vector<vector<int>>& dislikes) {
        // 建邻接表（1-indexed）
        vector<vector<int>> graph(n + 1);
        for (auto& d : dislikes) {
            graph[d[0]].push_back(d[1]);
            graph[d[1]].push_back(d[0]);
        }
        
        // color: 0=未染色, 1=颜色A, -1=颜色B
        vector<int> color(n + 1, 0);
        
        for (int i = 1; i <= n; i++) {
            if (color[i] != 0) continue; // 已染色，跳过
            
            // BFS 从节点 i 开始
            queue<int> q;
            q.push(i);
            color[i] = 1;
            
            while (!q.empty()) {
                int u = q.front(); q.pop();
                for (int v : graph[u]) {
                    if (color[v] == 0) {
                        // 未染色：染上对立颜色
                        color[v] = -color[u];
                        q.push(v);
                    } else if (color[v] == color[u]) {
                        // 已染色且颜色冲突：不是二分图
                        return false;
                    }
                }
            }
        }
        return true;
    }
};
```

**关键点**：外层 for 循环确保处理所有连通分量；内层 BFS 做标准的二着色检测。

### 解法2: DFS 染色 — O(V+E) / O(V+E)

**从 BFS 切换到 DFS**：逻辑完全相同，只是遍历方式不同。DFS 用递归实现代码更简洁。

```cpp
class Solution {
public:
    bool possibleBipartition(int n, vector<vector<int>>& dislikes) {
        vector<vector<int>> graph(n + 1);
        for (auto& d : dislikes) {
            graph[d[0]].push_back(d[1]);
            graph[d[1]].push_back(d[0]);
        }
        
        vector<int> color(n + 1, 0);
        
        for (int i = 1; i <= n; i++) {
            if (color[i] == 0) {
                color[i] = 1;
                if (!dfs(graph, color, i)) return false;
            }
        }
        return true;
    }

private:
    bool dfs(vector<vector<int>>& graph, vector<int>& color, int u) {
        for (int v : graph[u]) {
            if (color[v] == 0) {
                color[v] = -color[u];
                if (!dfs(graph, color, v)) return false;
            } else if (color[v] == color[u]) {
                return false;
            }
        }
        return true;
    }
};
```

**关键点**：递归前先染色再递归进入，这样在递归中遇到已染色节点可以直接检测冲突。

### 解法3: Union-Find（并查集） — O(V+E·α(V)) / O(V+E)

**完全不同的思路**：不做染色，而是利用并查集的"分组"特性。核心观察：对于节点 u 的所有邻居，它们必须在同一组（与 u 对立的那一组）。因此把 u 的所有邻居合并到一起，同时检测 u 是否和它的某个邻居在同一组。

```cpp
class Solution {
public:
    bool possibleBipartition(int n, vector<vector<int>>& dislikes) {
        vector<vector<int>> graph(n + 1);
        for (auto& d : dislikes) {
            graph[d[0]].push_back(d[1]);
            graph[d[1]].push_back(d[0]);
        }
        
        vector<int> parent(n + 1);
        vector<int> rank_(n + 1, 0);
        for (int i = 0; i <= n; i++) parent[i] = i;
        
        for (int u = 1; u <= n; u++) {
            if (graph[u].empty()) continue;
            int firstNeighbor = graph[u][0];
            for (int v : graph[u]) {
                if (find(parent, u) == find(parent, v)) {
                    return false; // u 和邻居 v 在同一组 → 冲突
                }
                // 把所有邻居合并到一起
                unite(parent, rank_, firstNeighbor, v);
            }
        }
        return true;
    }

private:
    int find(vector<int>& parent, int x) {
        if (parent[x] != x) parent[x] = find(parent, parent[x]);
        return parent[x];
    }
    
    void unite(vector<int>& parent, vector<int>& rank_, int a, int b) {
        int ra = find(parent, a), rb = find(parent, b);
        if (ra == rb) return;
        if (rank_[ra] < rank_[rb]) swap(ra, rb);
        parent[rb] = ra;
        if (rank_[ra] == rank_[rb]) rank_[ra]++;
    }
};
```

**关键点**：`unite(firstNeighbor, v)` 把 u 的所有邻居合并到同一集合——它们应该站在 u 的对立面。如果 u 和某个邻居 v 已经在同一集合 → 矛盾。

## 解法对比

| | BFS 染色 | DFS 染色 | Union-Find |
|--|---------|---------|------------|
| 时间 | O(V+E) | O(V+E) | O(V+E·α(V)) |
| 空间 | O(V+E) | O(V+E) | O(V+E) |
| 思路难度 | 低 | 低 | 中 |
| 代码难度 | 低 | 低 | 中 |
| 面试建议 | ⭐首选 | ⭐首选 | 加分项 |

**选择建议**：
- 面试首选 BFS 或 DFS 染色：思路直观，代码简短，不容易出错
- Union-Find 作为加分项展示——特别是面试官问"还有其他方法吗"时
- BFS vs DFS 看个人偏好，BFS 不会有栈溢出问题

## 易错点

1. **忘记处理不连通的图**
   - 错：只从节点 1 开始一次 BFS/DFS → 忽略了其他连通分量中的冲突
   - 对：遍历所有 1~n，对每个未染色节点启动搜索

2. **建图时只加单向边**
   - 错：`graph[a].push_back(b)` 忘记 `graph[b].push_back(a)`
   - 对：dislikes 是双向关系，必须双向建边

3. **节点编号从 1 开始**
   - 错：数组大小开 n → 访问 graph[n] 越界
   - 对：数组大小开 n+1，使用 1-indexed

4. **DFS 栈溢出**
   - 在 n 很大且图是链状时，DFS 递归深度可能达到 n → 栈溢出
   - 可以改用 BFS 或显式栈避免

5. **Union-Find 忘记检测冲突时机**
   - 错：先合并再检测 → 已经合并了，检测不到冲突
   - 对：先检测 find(u) == find(v)，再 unite(firstNeighbor, v)

## 面试追问

**Q1: 这道题的本质是什么？**
→ 二分图检测（2-Coloring / Bipartite Check）。将"分两组"建模为图的二着色问题——相邻节点（互相不喜欢的人）必须颜色不同。

**Q2: BFS 和 DFS 在这个问题上有什么区别？**
→ 逻辑完全相同，只是遍历顺序不同。BFS 逐层扩展适合找最短路，DFS 深入优先适合找连通分量。对于二分检测，两者效果一样。BFS 更安全（不会栈溢出）。

**Q3: Union-Find 解法的核心思想是什么？**
→ "敌人的敌人是朋友"。对于节点 u 的所有邻居（和 u 对立），它们应该在同一组 → 合并。如果 u 和某个邻居在同一集合 → 冲突。不需要显式染色，只需检查分组关系。

**Q4: 如果要分成 k 组呢？（k-Coloring）**
→ k=2 时可以多项式时间解决（本题）。k>=3 的图着色是 NP-完全问题，没有已知的多项式时间算法。面试中提到这个区别是加分项。

## 相关题型

- **LeetCode 785 判断二分图** — 几乎相同的问题，直接给邻接表，判断是否为二分图。本题多了一步建图。
- **LeetCode 207 课程表** — 同样是图遍历（BFS/DFS），但检测的是有向图中的环，而非二分性。
- **LeetCode 547 省份数量** — Union-Find 的经典应用，训练并查集的基本操作。
- **LeetCode 990 等式方程的可满足性** — 也是 Union-Find + 冲突检测的思路。
