# 787. K 站中转内最便宜的航班

## 核心思路

本题本质上是一个**带步数限制的单源最短路径问题**：从 `src` 到 `dst`，路径上最多经过 `k+1` 条边（k 站中转 = k+1 段航班），求最小花费。关键约束在于"最多 k 次中转"，这使得经典的 Dijkstra 需要改造，而 Bellman-Ford 天然适配。

## 思维链

1. **读完题第一反应**：这是一个带权有向图的最短路径问题，但多了一个"最多 k 次中转"的约束。暴力做法是 DFS/BFS 枚举所有路径，但会超时。

2. **回忆最短路算法**：Dijkstra 适合无负权图，Bellman-Ford 按"轮数"松弛。注意 Bellman-Ford 的第 `t` 轮松弛恰好求的是"最多经过 `t` 条边"的最短路——这不正好对应 k 次中转（k+1 条边）吗？

3. **Bellman-Ford 天然匹配**：只需要跑 `k+1` 轮松弛即可。每轮遍历所有边做松弛。关键技巧：每轮松弛必须基于**上一轮的结果**（而不是本轮已更新的值），否则一轮中可能"连锁松弛"多条边，相当于多走了几步。

4. **DP 视角理解**：定义 `dp[t][v]` = 从 src 出发，最多经过 t 条边到达 v 的最小花费。转移：`dp[t][v] = min(dp[t-1][u] + w(u,v))` 对所有边 `(u,v,w)`。这就是 Bellman-Ford 的 DP 本质。

5. **还有别的解法吗？**：BFS 逐层扩展（层 = 中转次数）也能做，本质和 Bellman-Ford 一样是按步数展开。Dijkstra + 状态扩展（把 `(节点, 中转次数)` 作为状态）也可以，但需要注意不能简单剪枝。

6. **最终选择**：Bellman-Ford 代码最简洁、最不容易写错，是面试首选。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| Bellman-Ford (k+1轮) | 按边松弛 k+1 轮 | O(k·E) | O(n) | ⭐ 必须写出 |
| BFS 逐层扩展 | 按中转次数 BFS | O(k·E) | O(n) | 能说出即可 |
| DP 二维表 | dp[t][v] 填表 | O(k·E) | O(k·n) | 能说出即可 |
| Dijkstra + 状态扩展 | (node, stops) 作为状态 | O(E·k·log(n·k)) | O(n·k) | 加分项 |

## 关键提示

1. **k 次中转 = k+1 条边**：Bellman-Ford 跑 `k+1` 轮，不是 `k` 轮。

2. **每轮松弛必须用上一轮的快照**：如果直接在当前 dp 数组上更新，一次循环中 `0→1→2` 可能连续松弛两条边，相当于多走了一步。必须拷贝上一轮的数组。

3. **Dijkstra 在本题不能简单用**：普通 Dijkstra 一旦确定节点最短距离就不再更新，但这里可能一个节点通过更多步数反而花费更少，又因为步数限制不能走更多步。需要把 `(cost, node, stops)` 作为状态。

4. **图的结构示意**（示例1）：
```
    0 --100--> 1 --100--> 2
    ^         |  \        |
    |        600  100    200
    |         v    |      v
    +---100---+    +----> 3
    
    k=1: 0→1→3 = 700 ✓ (1次中转)
         0→1→2→3 = 400 ✗ (2次中转，超出限制)
```

5. **初始化**：`dist[src] = 0`，其余为 `INF`。最终看 `dist[dst]` 是否仍为 `INF`。

## 解法详解

### 解法1: Bellman-Ford (k+1轮松弛) — O(k·E) / O(n) ⭐ 面试首选

**思考过程**：Bellman-Ford 算法第 t 轮松弛后，`dist[v]` 存的是"最多经过 t 条边从 src 到 v 的最短距离"。我们的约束是最多 k 次中转 = k+1 条边，所以只需跑 k+1 轮。每轮松弛前，必须**复制一份上一轮的 dist 数组**，用拷贝来做松弛源，防止同一轮内连锁更新。

```cpp
class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        const int INF = 1e9;
        vector<int> dist(n, INF);
        dist[src] = 0;
        
        // k 次中转 = 最多 k+1 条边，所以松弛 k+1 轮
        for (int t = 0; t <= k; t++) {
            // 关键：用上一轮的快照来松弛，防止连锁更新
            vector<int> prev = dist;
            for (auto& f : flights) {
                int u = f[0], v = f[1], w = f[2];
                if (prev[u] < INF) {
                    dist[v] = min(dist[v], prev[u] + w);
                }
            }
        }
        
        return dist[dst] == INF ? -1 : dist[dst];
    }
};
```

**关键点**：
- `vector<int> prev = dist` 这行是精髓。如果省略这行直接用 `dist[u]` 松弛，会导致一轮中多条边被连续松弛。
- 循环 `k+1` 次不是 `k` 次。

---

### 解法2: BFS 逐层扩展 — O(k·E) / O(n)

**思考过程**：把问题看成 BFS：从 src 出发，每一层代表一次中转。最多扩展 k+1 层。每层更新到达各节点的最小花费。

```cpp
class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        // 建邻接表
        vector<vector<pair<int,int>>> graph(n);
        for (auto& f : flights) {
            graph[f[0]].push_back({f[1], f[2]});
        }
        
        const int INF = 1e9;
        vector<int> dist(n, INF);
        dist[src] = 0;
        
        // BFS：最多 k+1 层（k 次中转 = k+1 条边）
        queue<pair<int,int>> q; // (node, cost)
        q.push({src, 0});
        int stops = 0;
        
        while (!q.empty() && stops <= k) {
            int sz = q.size();
            // 每层开始前拷贝快照，和 Bellman-Ford 同理
            vector<int> temp = dist;
            while (sz--) {
                auto [u, costU] = q.front(); q.pop();
                for (auto& [v, w] : graph[u]) {
                    if (costU + w < temp[v]) {
                        temp[v] = costU + w;
                        q.push({v, costU + w});
                    }
                }
            }
            dist = temp;
            stops++;
        }
        
        return dist[dst] == INF ? -1 : dist[dst];
    }
};
```

**从解法1的关系**：本质相同——都是按"步数"逐层松弛。BFS 用队列组织遍历顺序，Bellman-Ford 直接遍历所有边。

---

### 解法3: DP 二维表 — O(k·E) / O(k·n)

**思考过程**：显式定义 `dp[t][v]` = 从 src 出发，最多经过 t 条边，到达节点 v 的最小花费。

```
状态转移：dp[t][v] = min(dp[t-1][v], min over all edges (u→v,w) of dp[t-1][u] + w)
初始化：dp[0][src] = 0, 其余 INF
答案：dp[k+1][dst]
```

```
// DP 填表方向 (示例1, n=4, k=1 → 填2行):
//
//        node:  0    1    2    3
// t=0 (0边):   0   INF  INF  INF
// t=1 (1边):   0   100  INF  600    ← 从 0→1(100), 1→3(600,但t=0时1=INF所以不行)
//                                       实际: 0→1 得 100, 但 1→3 需要 t=1 时 dist[1]=100
//                                       所以 dp[1][3] 要从 dp[0][1]+600 = INF 
//                                       dp[1][1] = dp[0][0]+100 = 100
//                                       dp[1][3] = INF (因为 dp[0][1]=INF, dp[0][2]=INF)
// t=2 (2边):   0   100  200  700    ← dp[1][1]+600=700, dp[1][2]+200 但 dp[1][2]=INF
//                                       最终 dp[2][3] = 700
```

```cpp
class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        const int INF = 1e9;
        // dp[t][v]: 最多经过 t 条边到 v 的最小花费
        vector<vector<int>> dp(k + 2, vector<int>(n, INF));
        dp[0][src] = 0;
        
        for (int t = 1; t <= k + 1; t++) {
            dp[t] = dp[t - 1]; // 不走新边的情况（继承上一行）
            for (auto& f : flights) {
                int u = f[0], v = f[1], w = f[2];
                if (dp[t - 1][u] < INF) {
                    dp[t][v] = min(dp[t][v], dp[t - 1][u] + w);
                }
            }
        }
        
        return dp[k + 1][dst] == INF ? -1 : dp[k + 1][dst];
    }
};
```

**空间优化**：注意 `dp[t]` 只依赖 `dp[t-1]`，所以可以用两个一维数组滚动——这就退化成了解法1的 Bellman-Ford！

---

### 解法4: Dijkstra + 状态扩展 — O(E·k·log(n·k)) / O(n·k)

**思考过程**：普通 Dijkstra 不考虑步数。我们把状态定义为 `(cost, node, stopsUsed)`，用优先队列按 cost 排序。对于同一个 node，如果用了更少的 stops 到达，即使 cost 更高也不应该剪掉（因为更少的 stops 意味着后续还有余量）。

```cpp
class Solution {
public:
    int findCheapestPrice(int n, vector<vector<int>>& flights, int src, int dst, int k) {
        vector<vector<pair<int,int>>> graph(n);
        for (auto& f : flights) {
            graph[f[0]].push_back({f[1], f[2]});
        }
        
        // (cost, node, stopsUsed)
        priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>, greater<>> pq;
        pq.push({0, src, 0});
        
        // visited[v] = 到达 v 时用过的最少 stops 数
        // 如果当前 stops >= visited[v]，说明之前已经用更少步数到达过 v 且更便宜，可以跳过
        vector<int> visited(n, INT_MAX);
        
        while (!pq.empty()) {
            auto [cost, u, stops] = pq.top(); pq.pop();
            
            if (u == dst) return cost;
            if (stops > k) continue;       // 已经中转超过 k 次
            if (stops >= visited[u]) continue; // 之前用更少 stops 到过 u
            visited[u] = stops;
            
            for (auto& [v, w] : graph[u]) {
                pq.push({cost + w, v, stops + 1});
            }
        }
        
        return -1;
    }
};
```

**关键点**：`visited[u]` 记录的是到达 u 时使用的最少 stops，不是最小 cost。这和普通 Dijkstra 的剪枝逻辑不同。

## 解法对比

| | Bellman-Ford | BFS | DP 二维 | Dijkstra |
|---|---|---|---|---|
| 时间 | O((k+1)·E) | O((k+1)·E) | O((k+1)·E) | O(E·k·log(nk)) |
| 空间 | O(n) | O(n+队列) | O(k·n) | O(n·k+堆) |
| 代码量 | 最少 | 中等 | 中等 | 较多 |
| 面试推荐 | ⭐ 首选 | ✓ 备选 | ✓ 理解 DP 本质 | 加分项 |
| 核心优势 | 简洁，不需邻接表 | 直觉清晰 | 能看到完整 DP 表 | 可提前终止 |

**什么时候选哪个？**
- 面试默认写 Bellman-Ford：代码最短，最不容易出错
- 想展示 DP 功底：写二维 DP 版本，然后口述可以空间优化
- 图非常稀疏且 k 很小：Dijkstra 可能因提前找到答案而更快

## 易错点

1. **松弛时用了本轮已更新的值**：
   - ✗ 直接在 `dist` 上松弛：`dist[v] = min(dist[v], dist[u] + w)`
   - ✓ 用上一轮的拷贝：`dist[v] = min(dist[v], prev[u] + w)`
   - 原因：不拷贝会导致一轮内 `0→1→2` 被连续松弛，相当于走了两条边但只算了一轮

2. **循环次数搞错**：
   - ✗ `for (int t = 0; t < k; t++)` — 只松弛了 k 轮 = 最多 k 条边 = k-1 次中转
   - ✓ `for (int t = 0; t <= k; t++)` — 松弛 k+1 轮 = 最多 k+1 条边 = k 次中转

3. **Dijkstra 的剪枝条件写错**：
   - ✗ 用 `dist[u] < cost` 剪枝（和普通 Dijkstra 一样）
   - ✓ 用 `stops >= visited[u]` 剪枝（因为同一节点可能被不同 stops 数访问）

4. **忘记判断 `prev[u] < INF`**：
   - ✗ `dist[v] = min(dist[v], prev[u] + w)` 当 `prev[u] = INF` 时溢出
   - ✓ 先检查 `if (prev[u] < INF)` 再松弛

## 面试追问

**Q1: 为什么不能直接用 Dijkstra？**
普通 Dijkstra 对每个节点只确定一次最短距离，但本题有步数限制。可能存在：到节点 u 的最短距离用了很多步，而一条花费稍高但步数少的路径在后续才是最优解。所以需要把 `(node, stops)` 作为联合状态。

**Q2: Bellman-Ford 为什么每轮要拷贝数组？能不能不拷贝？**
不拷贝的话，一轮松弛中 A→B→C 可能先更新了 B 的值，然后用更新后的 B 去更新 C，相当于一轮走了两条边。拷贝确保每轮只"多走一条边"。这对应 DP 中"用 dp[t-1] 更新 dp[t]"的正确性。

**Q3: 如果查询很多次（不同的 src, dst, k），怎么优化？**
可以预处理 Floyd-Warshall 的变体：`dp[k][i][j]` = 从 i 到 j 最多经过 k 条边的最短路。预处理 O(k·n³)，每次查询 O(1)。或者如果 src 固定只有 dst 和 k 变化，一次 Bellman-Ford 跑满 n-1 轮，把每轮的 dist 都存下来。

**Q4: 如果边权可以为负（有补贴的航班），还能用吗？**
Bellman-Ford 天然支持负权边！只要没有负环（本题是 DAG 或限制步数所以不怕），Bellman-Ford 依然正确。但 Dijkstra 就不行了。

## 相关题型

- **743. 网络延迟时间** — 无步数限制的单源最短路，直接用 Dijkstra/Bellman-Ford。本题多了 k 的约束，Bellman-Ford 只需限制轮数即可。
- **1334. 阈值距离内邻居最少的城市** — Floyd-Warshall 多源最短路。如果本题改成"所有城市对之间带步数限制"就需要类似思路。
- **399. 除法求值** — 图上的路径查询，BFS/DFS 遍历图。复用"建图+BFS搜索"的框架。
- **Bellman-Ford 系列**：本题是 Bellman-Ford 最经典的应用场景——限制边数的最短路。