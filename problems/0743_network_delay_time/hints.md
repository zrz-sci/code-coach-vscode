# 743. Network Delay Time - 网络延迟时间

## 难度: Medium

## 标签: Graph, Shortest Path, Heap (Priority Queue), DFS, BFS

## 核心思路

这是一道经典的**单源最短路径**问题。从节点 k 发出信号，信号沿有向加权边传播，问所有节点都收到信号的最短时间。等价于：求从 k 到所有其他节点的最短距离，答案就是这些最短距离中的**最大值**。如果存在不可达节点则返回 -1。首选 **Dijkstra 算法**（因为边权非负），也可以用 Bellman-Ford 或 Floyd-Warshall。

## 思维链

1. **识别问题**: 有向加权图 + 单源出发 + 求到所有节点的距离 -> 经典单源最短路径
2. **选择算法**: 边权 w >= 0 -> Dijkstra 最优；如果有负权则需 Bellman-Ford
3. **建邻接表**: 用 `graph[u] = [(v, w), ...]` 存储有向边
4. **Dijkstra 执行**: 优先队列取距离最小的节点，松弛其所有出边
5. **收集答案**: 遍历所有节点的最短距离，取最大值。若有节点距离仍为 INF 则返回 -1
6. **节点编号**: 从 1 开始编号，数组大小需要 n+1

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| Dijkstra + 最小堆 | O(E log V) | O(V + E) | 最优解，适合稀疏图 |
| Dijkstra + 朴素遍历 | O(V^2 + E) | O(V + E) | 适合稠密图 (E 接近 V^2) |
| Bellman-Ford | O(V * E) | O(V) | 可处理负权边，本题不需要 |
| Floyd-Warshall | O(V^3) | O(V^2) | 全源最短路径，本题大材小用 |
| SPFA | O(V * E) 最坏 | O(V + E) | Bellman-Ford 的队列优化 |

其中 V = n (节点数), E = times.length (边数)。

## 关键提示

1. **节点编号从 1 开始**: 初始化 dist 数组大小为 n+1，索引 0 不使用
2. **惰性删除技巧**: Dijkstra 中从堆取出节点时，如果 `d > dist[u]` 说明是过时的条目，直接跳过
3. **不需要 visited 数组**: 用 `d > dist[u]` 的检查等价于 visited 判断，更简洁
4. **答案是 max 不是 sum**: "所有节点都收到信号"的时间等于最远节点的到达时间
5. **边数限制**: E 最多 6000，V 最多 100，图相对稀疏，用堆优化的 Dijkstra 最合适

## 解法详解

### 解法一：Dijkstra + 最小堆（推荐）

**步骤**:
1. 用邻接表建图 `graph[u] = [(v, w), ...]`
2. 初始化 `dist[k] = 0`，其余为 INT_MAX
3. 将 `(0, k)` 放入最小堆
4. 循环取堆顶 `(d, u)`:
   - 若 `d > dist[u]`，跳过（过时的条目）
   - 对 u 的每条出边 `(v, w)`，若 `dist[u] + w < dist[v]`，更新并入堆
5. 遍历 dist[1..n]，若有 INT_MAX 返回 -1，否则返回 max

**为什么需要惰性删除**: C++ 的 priority_queue 不支持 decrease-key 操作，所以可能堆中有同一节点的多个条目。通过 `d > dist[u]` 跳过过时条目来保证正确性。

**时间复杂度**: O(E log V) - 每条边最多入堆一次

**空间复杂度**: O(V + E)

### 解法二：Bellman-Ford

**步骤**:
1. 初始化 `dist[k] = 0`，其余为 INT_MAX
2. 重复 V-1 轮：遍历所有边 `(u, v, w)`，若 `dist[u] + w < dist[v]` 则更新
3. 收集答案同上

**特点**: 可以处理负权边（本题不需要），简单但较慢

**时间复杂度**: O(V * E)

**空间复杂度**: O(V)

### 解法三：Floyd-Warshall

**步骤**:
1. 建立 V*V 的距离矩阵，初始化为 INF
2. 填入直接边权
3. 三层循环做松弛：`dist[i][j] = min(dist[i][j], dist[i][mid] + dist[mid][j])`
4. 答案为 `max(dist[k][1..n])`

**特点**: 求出所有点对之间的最短路径，本题只需单源，是过度设计

**时间复杂度**: O(V^3)

**空间复杂度**: O(V^2)

## 易错点

1. **节点编号从 1 到 n**: 很容易忘记跳过索引 0，导致多算一个不存在的节点
2. **dist 初始值**: 必须用足够大的值（INT_MAX），不能用 -1 或 0
3. **INT_MAX 加法溢出**: `dist[u] + w` 在 dist[u] 为 INT_MAX 时溢出，需先检查 `dist[u] != INT_MAX`
4. **有向图 vs 无向图**: 题目明确是有向图，只添加单向边 u -> v，不要加反向边
5. **自环处理**: 题目保证 u != v（无自环），但如果有自环需要跳过
6. **堆中过时条目**: 不做 `d > dist[u]` 检查会导致已确定的节点被重复处理，虽然结果正确但效率降低

## 面试追问

1. **如果边权可能为负怎么办?** 使用 Bellman-Ford 算法，O(V*E)。如果有负环则无解
2. **Dijkstra vs BFS**: BFS 适用于所有边权相同的情况（无权图），Dijkstra 处理不同权重
3. **如何检测负环?** Bellman-Ford 第 V 轮仍有松弛则存在负环
4. **如果要求最短路径本身（而非距离）怎么办?** 维护 `prev[v]` 数组记录每个节点的前驱，从终点回溯即可
5. **如果图非常稠密（E 接近 V^2），哪种 Dijkstra 更优?** 朴素 Dijkstra O(V^2) 优于堆优化 O(E log V) = O(V^2 log V)
6. **能否用 BFS 解决本题?** 不能直接用 BFS（边权不同）。可以用 0-1 BFS 但仅适用于权重为 0 或 1 的图

## 相关题型

- [787. Cheapest Flights Within K Stops](https://leetcode.com/problems/cheapest-flights-within-k-stops/) - 限制中转次数的最短路径
- [1514. Path with Maximum Probability](https://leetcode.com/problems/path-with-maximum-probability/) - 最大概率路径（变形 Dijkstra）
- [505. The Maze II](https://leetcode.com/problems/the-maze-ii/) - 迷宫最短距离（Dijkstra 应用）
- [778. Swim in Rising Water](https://leetcode.com/problems/swim-in-rising-water/) - 最小瓶颈路径（Dijkstra 变体）
- [882. Reachable Nodes In Subdivided Graph](https://leetcode.com/problems/reachable-nodes-in-subdivided-graph/) - Dijkstra 的高级应用
- [1334. Find the City With the Smallest Number of Neighbors](https://leetcode.com/problems/find-the-city-with-the-smallest-number-of-neighbors-at-a-threshold-distance/) - Floyd-Warshall 应用
