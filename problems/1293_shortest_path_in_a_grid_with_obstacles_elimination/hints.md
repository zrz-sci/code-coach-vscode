# 1293. 网格中的最短路径

## 核心思路

本质上就是一个**带额外状态维度的 BFS 最短路径问题**：普通 BFS 的状态是 `(x, y)`，这道题因为可以消除最多 `k` 个障碍物，所以状态变成了 `(x, y, 剩余可消除次数)`。在这个三维状态空间里跑 BFS，第一次到达终点就是最短路径。

## 思维链

1. **读完题第一反应**：从左上到右下的最短路径 → BFS 是经典做法。但这里多了一个"可以消除 k 个障碍物"的条件，普通 BFS 的 visited 二维数组不够用了。

2. **为什么普通 BFS 不行？** 同一个格子 `(x, y)` 可能被多次访问——一次是剩余 3 次消除机会到达的，一次是剩余 1 次消除机会到达的。虽然步数可能一样，但后续能走的路径完全不同。如果只记录 `(x, y)` 是否访问过，就会漏掉有用的状态。

3. **状态扩展**：把状态定义为 `(x, y, remain)`，其中 `remain` 是剩余可消除障碍物的次数。两个不同的 `remain` 值代表两个不同的状态，互不冲突。

4. **visited 也要三维**：`visited[x][y][remain]` 记录该状态是否被访问过。BFS 保证第一次到达某状态时步数最少。

5. **剪枝优化**：如果 `k >= m + n - 3`（即消除次数足够多，可以直接走曼哈顿路径），那最短路径就是 `m + n - 2`，直接返回即可。

6. **复杂度评估**：状态数 = m × n × (k+1)，每个状态最多扩展 4 个邻居，总复杂度 O(m × n × k)，在给定约束下（m, n ≤ 40, k ≤ 1600）完全可行。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| BFS + 三维状态 | 状态 = (x, y, remain)，BFS 搜索 | O(m·n·k) | O(m·n·k) | ⭐ 必须写出 |
| BFS + 曼哈顿剪枝 | 在上述基础上加 k ≥ m+n-3 的快速判断 | O(m·n·k) | O(m·n·k) | 加分项 |
| A* 搜索（进阶） | 用曼哈顿距离做启发式估价 | 通常更快 | O(m·n·k) | 加分项 |

## 关键提示

1. **关键洞察**：同一个格子用不同的"剩余消除次数"到达，代表的是**完全不同的状态**。不能简单地用二维 visited。

2. **BFS vs DFS**：这道题求最短步数，BFS 天然逐层扩展，第一次到达终点就是最短路。DFS 需要遍历所有路径取最小值，效率低得多。

3. **遇到障碍物时的处理**：如果下一个格子是障碍物且 `remain > 0`，就可以消除它（`remain - 1`）继续走；如果 `remain == 0`，这条路走不通。

4. **网格示意图 + BFS 状态扩展**：
```
// grid (5×3):
//   0 0 0        从 (0,0) 到 (4,2)
//   1 1 0        k = 1
//   0 0 0
//   0 1 1        消除 (3,2) 处的障碍
//   0 0 0
//
// BFS 状态: (row, col, remain)
// 起点: (0, 0, 1)  步数=0
// 
// 层0: (0,0,1)
// 层1: (0,1,1)
// 层2: (0,2,1)
// 层3: (1,2,1)       ← grid[1][2]=0, 不消耗
// 层4: (2,2,1)
// 层5: (3,2,0)       ← grid[3][2]=1, 消耗1次, remain 1→0
// 层6: (4,2,0)       ← 到达终点! 步数=6
```

5. **曼哈顿距离剪枝**：从 (0,0) 到 (m-1,n-1) 的最短路径至少 `m+n-2` 步，路上最多有 `m+n-3` 个障碍物（起点终点都是 0）。如果 `k >= m+n-3`，直接返回 `m+n-2`。

## 解法详解

### 解法1: BFS + 三维状态 — O(m·n·k) / O(m·n·k) ⭐ 面试首选

**思考过程**：
- 求最短路径 → BFS。
- 普通网格 BFS 的状态是 `(x, y)`，但这里"剩余消除次数"不同意味着后续能走的路不同，所以必须把 `remain` 也纳入状态。
- 三维 visited 数组记录每个状态是否已访问，防止重复入队。
- 遇到障碍物：如果 remain > 0，消耗一次消除机会继续走；否则跳过。

```cpp
class Solution {
public:
    int shortestPath(vector<vector<int>>& grid, int k) {
        int m = grid.size(), n = grid[0].size();
        
        // 剪枝：如果消除次数足够，直接走曼哈顿最短路
        if (k >= m + n - 3) return m + n - 2;
        
        // visited[i][j][r] = 是否以剩余 r 次消除机会访问过 (i,j)
        vector<vector<vector<bool>>> visited(m, vector<vector<bool>>(n, vector<bool>(k + 1, false)));
        
        // BFS 队列: {行, 列, 剩余消除次数}
        queue<tuple<int, int, int>> q;
        q.push({0, 0, k});
        visited[0][0][k] = true;
        
        int steps = 0;
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        
        while (!q.empty()) {
            int sz = q.size();
            // 逐层处理，每一层代表多走一步
            for (int i = 0; i < sz; i++) {
                auto [x, y, remain] = q.front();
                q.pop();
                
                // 到达终点
                if (x == m - 1 && y == n - 1) return steps;
                
                for (auto& d : dirs) {
                    int nx = x + d[0], ny = y + d[1];
                    if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
                    
                    int nRemain = remain - grid[nx][ny];
                    // grid[nx][ny]=1 时消耗一次消除，=0 时不消耗
                    
                    // 消除次数不够 或 该状态已访问过 → 跳过
                    if (nRemain < 0 || visited[nx][ny][nRemain]) continue;
                    
                    visited[nx][ny][nRemain] = true;
                    q.push({nx, ny, nRemain});
                }
            }
            steps++;
        }
        
        return -1; // 无法到达
    }
};
```

**关键点**：
- `nRemain = remain - grid[nx][ny]` 这一行非常巧妙：grid 值为 0 时不消耗，为 1 时自动减 1。
- 终点检查放在出队时（而非入队时），这样 `steps` 的语义清晰：当前层的所有节点距离起点恰好 `steps` 步。

### 解法2: A* 搜索（进阶） — 通常更快 / O(m·n·k)

**从解法1优化**：BFS 是"盲目"地逐层扩展，A* 用启发式函数优先扩展"看起来离终点更近"的状态，通常能更快找到答案。

**启发式函数**：曼哈顿距离 `h(x,y) = (m-1-x) + (n-1-y)`，这是到终点的最少步数下界（不考虑障碍物），保证了 A* 的最优性（admissible heuristic）。

```cpp
class Solution {
public:
    int shortestPath(vector<vector<int>>& grid, int k) {
        int m = grid.size(), n = grid[0].size();
        if (k >= m + n - 3) return m + n - 2;
        
        // 状态: {f值, 步数g, 行, 列, 剩余消除次数}
        // f = g + h, 其中 h = 曼哈顿距离
        // 小顶堆：f 值小的优先
        using State = tuple<int, int, int, int, int>;
        priority_queue<State, vector<State>, greater<State>> pq;
        
        vector<vector<vector<bool>>> visited(m, vector<vector<bool>>(n, vector<bool>(k + 1, false)));
        
        int h0 = (m - 1) + (n - 1);
        pq.push({h0, 0, 0, 0, k}); // {f, g, x, y, remain}
        
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        
        while (!pq.empty()) {
            auto [f, g, x, y, remain] = pq.top();
            pq.pop();
            
            // 到达终点
            if (x == m - 1 && y == n - 1) return g;
            
            // 可能重复出队（优先队列不方便删除），用 visited 过滤
            if (visited[x][y][remain]) continue;
            visited[x][y][remain] = true;
            
            for (auto& d : dirs) {
                int nx = x + d[0], ny = y + d[1];
                if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
                
                int nRemain = remain - grid[nx][ny];
                if (nRemain < 0 || visited[nx][ny][nRemain]) continue;
                
                int ng = g + 1;
                int nh = (m - 1 - nx) + (n - 1 - ny);
                pq.push({ng + nh, ng, nx, ny, nRemain});
            }
        }
        
        return -1;
    }
};
```

**关键点**：
- A* 的正确性依赖于启发式函数是 admissible 的（不会高估真实距离）。曼哈顿距离满足这个条件。
- visited 的检查放在出队时，因为优先队列可能对同一个状态入队多次，第一次出队的一定是最优的。
- 最坏情况复杂度同 BFS，但在实践中通常快很多。

## 解法对比

| | BFS + 三维状态 | A* 搜索 |
|---|---|---|
| **实现难度** | 简单，标准 BFS 模板 | 中等，需要理解 A* 和启发式函数 |
| **最坏时间** | O(m·n·k) | O(m·n·k) — 退化情况同 BFS |
| **实际性能** | 稳定 | 通常更快（启发式引导方向） |
| **面试推荐** | ⭐ 首选：好写好讲 | 加分项：展示算法功底 |
| **visited 检查** | 入队前检查 | 出队时检查（惰性删除） |

**面试建议**：先写 BFS 解法，如果面试官追问优化，再提 A*。BFS 解法已经足够通过本题。

## 易错点

1. **visited 只用二维数组**
   - ✗ `visited[x][y] = true` — 同一个格子用不同 remain 到达是不同状态，二维 visited 会错误地剪掉有效路径
   - ✓ `visited[x][y][remain] = true` — 三维 visited，每个 (x, y, remain) 独立

2. **忘记处理 k 很大的情况**
   - ✗ 不加剪枝直接 BFS，当 k = m*n 时，visited 数组巨大，可能 MLE/TLE
   - ✓ `if (k >= m + n - 3) return m + n - 2;` — 消除次数足够多时直接返回曼哈顿距离

3. **steps 计数时机错误**
   - ✗ 在 `q.push` 的时候 steps++，导致计数混乱
   - ✓ 每处理完一层后 `steps++`，或者把 steps 放进状态元组里

4. **nRemain 计算错误**
   - ✗ `nRemain = remain - 1`（无论空地还是障碍都减 1）
   - ✓ `nRemain = remain - grid[nx][ny]`（只有踩到障碍才消耗消除次数）

5. **终点检查在入队时导致错误**
   - 虽然对 BFS 来说入队时检查也可以（因为同层），但更安全的做法是出队时检查，避免边界 case（如起点就是终点：1×1 网格）

## 面试追问

**Q1: 为什么 BFS 而不是 DFS？**
> BFS 天然按层扩展，第一次到达终点就是最短路径。DFS 需要遍历所有可能路径取最小值，在这种题上效率极低（指数级）。

**Q2: visited 为什么要三维？同一个格子为什么可能被多次访问？**
> 因为到达同一个格子时，剩余消除次数不同代表着"后续能力"不同。remain=3 和 remain=1 虽然位置相同，但一个能穿过更多障碍，可能找到更短的路径。必须分开记录。

**Q3: 如果 k 非常大（比如 k = m*n），怎么优化？**
> 当 `k >= m+n-3` 时，从起点到终点的最短路径上最多有 `m+n-3` 个障碍物（路径长度 m+n-2，减去起点和终点），所以可以直接返回曼哈顿距离 `m+n-2`。这个剪枝极其重要，避免了不必要的巨大状态空间。

**Q4: 能否用 A* 来加速？启发式函数怎么设计？**
> 用曼哈顿距离 `h(x,y) = (m-1-x) + (n-1-y)` 作为启发式函数。它是 admissible 的（不会高估），因此 A* 能保证找到最优解。实际表现上，A* 会优先探索"朝终点方向"的状态，减少不必要的搜索。

**Q5: 这道题和普通网格最短路有什么本质区别？**
> 本质区别是状态空间的维度。普通网格最短路的状态是 `(x, y)` — 二维，所以用二维 visited。这道题因为有额外的"消除预算"约束，状态变成 `(x, y, remain)` — 三维。这是一个通用思想：任何影响后续决策的变量都必须纳入状态。

## 相关题型

- **LeetCode 1091 二进制矩阵中的最短路径** — 标准网格 BFS（无障碍消除），是本题的"简化版"。复用完全相同的 BFS 模板，只是不需要第三维状态。
- **LeetCode 752 打开转盘锁** — BFS 搜索状态空间的经典应用。每个状态是一个四位字符串，和本题"三维状态 BFS"思路相通：定义清楚状态 → BFS 逐层扩展 → visited 标记。
- **LeetCode 127 单词接龙** — 也是 BFS 求最短路，状态是"当前单词"。关键是如何高效地找到邻居状态。
- **LeetCode 787 K 站中转内最便宜的航班** — 同样是"带额外约束的最短路"：最多 k 次中转。但用 BFS/Bellman-Ford 而非 Dijkstra（因为边权不全为 1）。复用"状态加一维"的思想。