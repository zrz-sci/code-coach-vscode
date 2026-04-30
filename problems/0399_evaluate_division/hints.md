# 399. 除法求值

## 核心思路

**这道题本质上是一个带权有向图的路径查询问题**：每个变量是图中的节点，`a / b = 2.0` 意味着从 `a` 到 `b` 有一条权重为 2.0 的边（反向边权重为 1/2.0）。求 `c / d` 就是在图中找从 `c` 到 `d` 的路径，路径上所有边权的乘积就是答案。

## 思维链

1. **读完题第一反应**：`a/b = 2, b/c = 3`，求 `a/c`？很简单，`a/c = (a/b) × (b/c) = 6`。这不就是沿着一条"链"把比率乘起来吗？

2. **抽象成什么模型？** 变量之间的除法关系像一个网络——`a` 可以通过 `b` 间接算出与 `c` 的比率。这就是**图**！变量是节点，除法关系是带权重的边。

3. **查询怎么做？** 求 `c/d` = 从 `c` 走到 `d`，路径上权重相乘。用什么遍历？**BFS 或 DFS** 都行——就是图的路径搜索。

4. **有没有其他角度？** 除法有传递性：如果 `a/b` 和 `b/c` 已知，可以推出 `a/c`。这像什么？**并查集**——同一个连通分量内的变量可以互相推导，用带权并查集维护每个节点到根的比率。

5. **还有一种暴力思路**：Floyd 算法——预处理所有变量对之间的比率，查询时 O(1)。数据量小（≤40 个变量），Floyd O(n³) 完全够用。

6. **总结**：三种主流做法——DFS/BFS 逐次查询、Floyd 预处理全部、带权并查集在线合并查询。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| BFS/DFS | 建图 + 每次查询做图搜索 | O(Q × (V+E)) | O(V+E) | ⭐ 必须写出 |
| Floyd | 预处理所有节点对间的比率 | O(V³ + Q) | O(V²) | 能说出即可 |
| 带权并查集 | 维护到根的比率，路径压缩 | O((E+Q) × α(V)) | O(V) | 加分项 |

> V = 变量数（≤40），E = 等式数（≤20），Q = 查询数（≤20）

## 关键提示

1. **建图是关键第一步**：`a/b = 2.0` 要建**双向边**——`a→b` 权重 2.0，`b→a` 权重 0.5。漏掉反向边是最常见的 bug。

2. **图的表示**：变量是字符串不是整数，用 `unordered_map<string, vector<pair<string, double>>>` 做邻接表，或者先把字符串映射成整数编号。

3. **画出示例的图**：

```
示例1: a/b=2.0, b/c=3.0

    2.0       3.0
a ------→ b ------→ c
  ←------   ←------
    0.5       1/3

查询 a/c: 沿路径 a→b→c，权重乘积 = 2.0 × 3.0 = 6.0
查询 b/a: 沿路径 b→a，权重 = 0.5
查询 a/e: e 不在图中 → -1.0
查询 a/a: 起点=终点 → 1.0 （但 a 必须在图中！）
查询 x/x: x 不在图中 → -1.0
```

4. **易忽略的边界**：`a/a` 不是无脑返回 1.0——必须先检查 `a` 是否在图中。`x/x` 中 `x` 未定义，返回 -1.0。

5. **带权并查集的难点**：路径压缩时要同时更新权重。`find(x)` 不仅返回根，还要把 `weight[x]` 更新为 `x` 到根的累积比率。

## 解法详解

### 解法1: BFS 图搜索 — O(Q × (V+E)) / O(V+E) ⭐ 面试首选

**思考过程**：既然是图上找路径，最直觉的就是 BFS/DFS。BFS 的好处是代码结构清晰，不容易写出 bug。

对每个查询 `[src, dst]`：
- 如果 src 或 dst 不在图中 → 返回 -1.0
- 如果 src == dst → 返回 1.0
- 从 src 出发 BFS，维护"从 src 到当前节点的累积乘积"
- 到达 dst 时，累积乘积就是答案

```
BFS 过程示意 (求 a/c):
队列: [(a, 1.0)]
  → 弹出 (a, 1.0)，邻居 b，累积 1.0×2.0=2.0 → 入队 (b, 2.0)
  → 弹出 (b, 2.0)，邻居 c，累积 2.0×3.0=6.0 → 到达 c！答案=6.0
```

```cpp
class Solution {
public:
    vector<double> calcEquation(vector<vector<string>>& equations, 
                                vector<double>& values,
                                vector<vector<string>>& queries) {
        // 1. 建带权有向图（邻接表）
        unordered_map<string, vector<pair<string, double>>> graph;
        for (int i = 0; i < equations.size(); i++) {
            const string& a = equations[i][0];
            const string& b = equations[i][1];
            graph[a].push_back({b, values[i]});
            graph[b].push_back({a, 1.0 / values[i]});
        }
        
        vector<double> results;
        for (auto& q : queries) {
            results.push_back(bfs(q[0], q[1], graph));
        }
        return results;
    }
    
private:
    double bfs(const string& src, const string& dst,
               unordered_map<string, vector<pair<string, double>>>& graph) {
        // 边界：变量不存在于图中
        if (!graph.count(src) || !graph.count(dst)) return -1.0;
        if (src == dst) return 1.0;
        
        // BFS: 队列中存 (当前节点, 从src到此节点的累积乘积)
        queue<pair<string, double>> q;
        unordered_set<string> visited;
        q.push({src, 1.0});
        visited.insert(src);
        
        while (!q.empty()) {
            auto [node, product] = q.front();
            q.pop();
            
            for (auto& [neighbor, weight] : graph[node]) {
                if (neighbor == dst) return product * weight;
                if (!visited.count(neighbor)) {
                    visited.insert(neighbor);
                    q.push({neighbor, product * weight});
                }
            }
        }
        // src 和 dst 不连通
        return -1.0;
    }
};
```

**关键点**：
- 必须用 visited 集合防止重复访问（图可能有环：a→b 和 b→a）
- 找到 dst 立即返回，不需要继续搜索

### 解法2: Floyd 预处理 — O(V³ + Q) / O(V²)

**从解法1的角度看**：解法1 每次查询都做一次 BFS，如果查询很多且图很大，重复搜索浪费。Floyd 的思路是：**先把所有变量对之间的比率都算好，查询时直接查表**。

Floyd 的核心：`dist[i][j] = dist[i][k] × dist[k][j]`，即 `i/j = (i/k) × (k/j)`。

```
Floyd 填表过程 (a/b=2, b/c=3):
初始:       k=a后:      k=b后:        k=c后:
    a    b    c       a    b    c       a     b    c        (最终)
a [1.0  2.0   - ]  [1.0  2.0   - ]  [1.0  2.0  6.0]    a/c=2×3=6
b [0.5  1.0  3.0]  [0.5  1.0  3.0]  [0.5  1.0  3.0]    
c [ -   1/3  1.0]  [ -   1/3  1.0]  [1/6  1/3  1.0]    c/a=1/3×0.5
```

```cpp
class Solution {
public:
    vector<double> calcEquation(vector<vector<string>>& equations, 
                                vector<double>& values,
                                vector<vector<string>>& queries) {
        // 1. 给变量编号
        unordered_map<string, int> id;
        int idx = 0;
        for (auto& eq : equations) {
            if (!id.count(eq[0])) id[eq[0]] = idx++;
            if (!id.count(eq[1])) id[eq[1]] = idx++;
        }
        int n = idx;
        
        // 2. 初始化邻接矩阵，-1.0 表示不可达
        vector<vector<double>> dist(n, vector<double>(n, -1.0));
        for (int i = 0; i < n; i++) dist[i][i] = 1.0; // 自己除以自己 = 1
        
        for (int i = 0; i < equations.size(); i++) {
            int u = id[equations[i][0]], v = id[equations[i][1]];
            dist[u][v] = values[i];
            dist[v][u] = 1.0 / values[i];
        }
        
        // 3. Floyd: 尝试用每个中间节点 k 来连接 i 和 j
        for (int k = 0; k < n; k++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    // 只在 i→k 和 k→j 都可达时才更新
                    if (dist[i][k] > 0 && dist[k][j] > 0) {
                        dist[i][j] = dist[i][k] * dist[k][j];
                    }
                }
            }
        }
        
        // 4. 回答查询
        vector<double> results;
        for (auto& q : queries) {
            if (!id.count(q[0]) || !id.count(q[1])) {
                results.push_back(-1.0);
            } else {
                results.push_back(dist[id[q[0]]][id[q[1]]]);
            }
        }
        return results;
    }
};
```

**关键点**：Floyd 是在乘法意义下做的——普通 Floyd 是 `min(dist[i][k] + dist[k][j])`，这里是 `dist[i][k] * dist[k][j]`。

### 解法3: 带权并查集 — O((E+Q) × α(V)) / O(V) 加分项

**为什么想到并查集？** 同一个连通分量内的变量可以互相推导比率。并查集天然处理"连通性"问题。关键是：在并查集上额外维护一个权重 `weight[x]`，表示 `x / root(x)` 的值。

```
带权并查集示意:
初始: a/b=2.0, b/c=3.0

Step1: union(a,b,2.0)
  b 是根，weight[a]=2.0 (a/b=2.0), weight[b]=1.0

       b (weight=1.0)
       |
       a (weight=2.0，即 a/b=2.0)

Step2: union(b,c,3.0)
  c 是根，weight[b]=3.0 (b/c=3.0)
  
       c (weight=1.0)
       |
       b (weight=3.0，即 b/c=3.0)
       |
       a (weight=2.0，即 a/b=2.0)

查询 a/c:
  find(a): 路径压缩，a→b→c，weight[a] 更新为 2.0×3.0=6.0
  find(c): 根就是 c，weight[c]=1.0
  答案 = weight[a] / weight[c] = 6.0 / 1.0 = 6.0
```

```cpp
class Solution {
public:
    unordered_map<string, string> parent;
    unordered_map<string, double> weight; // weight[x] = x / root(x)
    
    string find(const string& x) {
        if (parent[x] != x) {
            string root = find(parent[x]);
            // 路径压缩时，权重要乘上父节点到根的权重
            // 原来 weight[x] = x/parent[x]
            // parent[x] 到根的权重已经在递归中更新为 parent[x]/root
            // 所以 x/root = (x/parent[x]) × (parent[x]/root)
            weight[x] *= weight[parent[x]];
            parent[x] = root;
        }
        return parent[x];
    }
    
    void unite(const string& a, const string& b, double val) {
        // 确保两个变量都在并查集中
        if (!parent.count(a)) { parent[a] = a; weight[a] = 1.0; }
        if (!parent.count(b)) { parent[b] = b; weight[b] = 1.0; }
        
        string rootA = find(a), rootB = find(b);
        if (rootA == rootB) return;
        
        // 把 rootA 挂到 rootB 下面
        // 需要: weight[rootA] = rootA / rootB
        // 已知: a/b = val, weight[a] = a/rootA, weight[b] = b/rootB
        // rootA/rootB = (a/rootA)^(-1) × (a/b) × (b/rootB)
        //             = val × weight[b] / weight[a]
        parent[rootA] = rootB;
        weight[rootA] = val * weight[b] / weight[a];
    }
    
    vector<double> calcEquation(vector<vector<string>>& equations, 
                                vector<double>& values,
                                vector<vector<string>>& queries) {
        // 建并查集
        for (int i = 0; i < equations.size(); i++) {
            unite(equations[i][0], equations[i][1], values[i]);
        }
        
        vector<double> results;
        for (auto& q : queries) {
            const string& c = q[0], & d = q[1];
            if (!parent.count(c) || !parent.count(d)) {
                results.push_back(-1.0);
            } else if (find(c) != find(d)) {
                // 不在同一个连通分量，无法推导
                results.push_back(-1.0);
            } else {
                // c/d = (c/root) / (d/root) = weight[c] / weight[d]
                results.push_back(weight[c] / weight[d]);
            }
        }
        return results;
    }
};
```

**关键点**：
- `find()` 中路径压缩时 **必须先递归再更新权重**，否则 `weight[parent[x]]` 还没压缩到根
- `unite()` 中推导 `weight[rootA]` 的公式是核心，画图推导最清晰

## 解法对比

| | BFS | Floyd | 带权并查集 |
|---|---|---|---|
| 建模方式 | 邻接表 + 逐次搜索 | 邻接矩阵 + 全量预处理 | 树形结构 + 路径压缩 |
| 查询时间 | O(V+E) 每次 | O(1) 查表 | 近 O(1) (α(V)) |
| 预处理时间 | O(E) 建图 | O(V³) Floyd | O(E × α(V)) 建树 |
| 适用场景 | 查询少，图大 | 变量少（V≤40），查询多 | 在线场景，动态添加等式 |
| 代码难度 | ⭐ 最简单 | ⭐⭐ 中等 | ⭐⭐⭐ 最难 |
| 面试推荐 | **首选**——思路清晰好解释 | 提一下即可 | 如果追问"能否更高效"再给 |

## 易错点

1. **漏建反向边**
   - ✗ 只建 `graph[a] → (b, val)`
   - ✓ 同时建 `graph[b] → (a, 1.0/val)`
   - 否则 `b/a` 查不到

2. **`a/a` 的特殊处理**
   - ✗ 无脑返回 1.0（对所有 `src==dst` 都返回 1.0）
   - ✓ 先检查 `a` 是否在图中，不在返回 -1.0
   - 题目明确说 `x/x` 中 `x` 未定义时返回 -1.0

3. **BFS 忘记 visited**
   - ✗ 不标记已访问节点 → 双向边导致 a→b→a→b... 无限循环
   - ✓ 入队时立即标记 visited

4. **带权并查集路径压缩顺序**
   - ✗ 先更新 weight 再递归 find → parent 还没压缩，weight[parent[x]] 不正确
   - ✓ 先递归 `find(parent[x])`（此时 weight[parent[x]] 已更新到根），再 `weight[x] *= weight[parent[x]]`

5. **Floyd 用 -1.0 表示不可达时的判断**
   - ✗ `if (dist[i][k] >= 0 && dist[k][j] >= 0)` — 题目说 values > 0，但初始 dist[i][i] = 1.0
   - ✓ 用 `> 0` 或改用 `dist[i][k] != -1.0`，只要一致就行。注意不要把 0 和 -1 搞混

## 面试追问

**Q1（基础理解）**：这道题和图有什么关系？
> 每个变量是节点，a/b=k 表示 a→b 边权 k 和 b→a 边权 1/k。查询 c/d 就是找 c 到 d 的路径，路径权重相乘。

**Q2（优化）**：如果查询量非常大（百万级），怎么优化？
> 用 Floyd 预处理 O(V³)，之后每次查询 O(1)。或者用带权并查集，每次查询近 O(1)。BFS 每次 O(V+E) 太慢。

**Q3（变体）**：如果等式是动态添加的（先查询一些，再加入新等式，再查询），怎么处理？
> 带权并查集最合适——`unite()` 就是动态添加等式，`find()` 就是查询。Floyd 需要重跑，BFS 可以但每次查询慢。

**Q4（扩展）**：如果给的是乘法关系而不是除法（a×b=k），怎么改？
> 本质一样。a×b=k 等价于 a=k/b，即 a/b 的关系无法直接建，需要引入额外约束或改变图的定义。如果 k 已知且 b 已知，则退化为赋值问题。

## 相关题型

- **[547. 省份数量](https://leetcode.cn/problems/number-of-provinces/)** — 并查集基础入门题，没有权重，纯粹判连通性。学会后给并查集加上权重就是本题的解法3
- **[990. 等式方程的可满足性](https://leetcode.cn/problems/satisfiability-of-equality-equations/)** — 并查集判等/不等关系，相当于本题的"无权"版本
- **[1976. 到达目的地的方案数](https://leetcode.cn/problems/number-of-ways-to-arrive-at-destination/)** — 带权图 + 路径搜索，复用本题的建图 + BFS/DFS 框架
- **[743. 网络延迟时间](https://leetcode.cn/problems/network-delay-time/)** — 带权图的单源最短路径，复用建图思路，区别是用加法（Dijkstra）而非乘法