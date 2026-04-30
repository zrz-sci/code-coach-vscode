# 505. 迷宫 II

## 核心思路

**带权最短路径问题**：球在迷宫中滚动直到撞墙才停，每次滚动的距离不同（边权不等），因此需要 Dijkstra 算法求最短距离，而不能用普通 BFS。

## 思维链

1. **建模为图**：每个"停止位置"是一个节点，球从一个停止位置滚到下一个停止位置的距离是边权
2. **为什么不能用普通 BFS**：BFS 适用于等权图，但这里每次滚动距离不同（1步 vs 多步），属于加权图
3. **Dijkstra 适用性**：所有边权为正（滚动距离 >= 1），非负权图的单源最短路用 Dijkstra
4. **滚动模拟**：每个方向上，球一直滚直到碰壁或出界，记录滚动步数作为边权
5. **距离数组**：`dist[r][c]` 记录从 start 到 (r,c) 的最短距离，初始为 INT_MAX
6. **终止判断**：球必须在 destination 处"停下来"（碰壁停止），不是经过就算

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| Dijkstra + 优先队列 | O(mn log(mn)) | O(mn) | ⭐⭐⭐⭐⭐ |
| BFS + 距离松弛 | O(mn * max(m,n)) | O(mn) | ⭐⭐⭐ |

## 关键提示

- 球**不能中途停下**，必须滚到撞墙才停 —— 这是和普通最短路的关键区别
- 即使球经过 destination，如果没有在那里碰壁停下，也不算到达
- 距离定义：从 start 到 destination 经过的**空格数**（不含 start，含 destination）
- 同一个停止位置可能被多条路径到达，需要取最短的（Dijkstra 的松弛操作）

## 解法详解

### 解法1: Dijkstra + 优先队列（推荐）

**思路**：用最小堆实现 Dijkstra，每次取距离最小的停止位置扩展，对四个方向模拟滚动。

```cpp
class Solution {
public:
    int shortestDistance(vector<vector<int>>& maze, vector<int>& start, vector<int>& destination) {
        int m = maze.size(), n = maze[0].size();
        vector<vector<int>> dist(m, vector<int>(n, INT_MAX));
        dist[start[0]][start[1]] = 0;
        
        // 最小堆：(距离, 行, 列)
        priority_queue<tuple<int,int,int>, vector<tuple<int,int,int>>, greater<>> pq;
        pq.push({0, start[0], start[1]});
        
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        
        while (!pq.empty()) {
            auto [d, r, c] = pq.top(); pq.pop();
            
            // 如果已经找到更短路径，跳过
            if (d > dist[r][c]) continue;
            // 到达目的地
            if (r == destination[0] && c == destination[1]) return d;
            
            for (auto& dir : dirs) {
                int nr = r, nc = c, steps = 0;
                // 沿方向滚到撞墙
                while (nr + dir[0] >= 0 && nr + dir[0] < m &&
                       nc + dir[1] >= 0 && nc + dir[1] < n &&
                       maze[nr + dir[0]][nc + dir[1]] == 0) {
                    nr += dir[0];
                    nc += dir[1];
                    steps++;
                }
                // 松弛操作
                if (dist[r][c] + steps < dist[nr][nc]) {
                    dist[nr][nc] = dist[r][c] + steps;
                    pq.push({dist[nr][nc], nr, nc});
                }
            }
        }
        
        return -1;
    }
};
```

### 解法2: BFS + 距离松弛

**思路**：用普通队列做 BFS，但允许重复入队（当发现更短路径时）。比 Dijkstra 慢但更易理解。

```cpp
class Solution {
public:
    int shortestDistance(vector<vector<int>>& maze, vector<int>& start, vector<int>& destination) {
        int m = maze.size(), n = maze[0].size();
        vector<vector<int>> dist(m, vector<int>(n, INT_MAX));
        dist[start[0]][start[1]] = 0;
        
        queue<pair<int,int>> q;
        q.push({start[0], start[1]});
        
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        
        while (!q.empty()) {
            auto [r, c] = q.front(); q.pop();
            
            for (auto& dir : dirs) {
                int nr = r, nc = c, steps = 0;
                while (nr + dir[0] >= 0 && nr + dir[0] < m &&
                       nc + dir[1] >= 0 && nc + dir[1] < n &&
                       maze[nr + dir[0]][nc + dir[1]] == 0) {
                    nr += dir[0];
                    nc += dir[1];
                    steps++;
                }
                if (dist[r][c] + steps < dist[nr][nc]) {
                    dist[nr][nc] = dist[r][c] + steps;
                    q.push({nr, nc});
                }
            }
        }
        
        return dist[destination[0]][destination[1]] == INT_MAX ? -1 : dist[destination[0]][destination[1]];
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|---------|---------|------|
| ✗ 用 visited 数组标记已访问 | ✓ 用 dist 数组做松弛 | 同一位置可能被更短路径再次到达 |
| ✗ 球到达 destination 坐标就返回 | ✓ 球必须在 destination 碰壁停下 | 经过不算，必须停在那里 |
| ✗ 每次移动一格（普通BFS） | ✓ 一直滚到撞墙 | 球不能中途停下 |
| ✗ 边权都是 1 | ✓ 边权 = 滚动的格子数 | 不同方向滚动距离不同 |

## 面试追问

**Q1: 和 490. The Maze 有什么区别？**

> 490 只问能否到达（BFS/DFS 即可），505 要求最短距离（需要 Dijkstra），490 的边权相等性不影响可达性判断。

**Q2: 为什么不能用普通 BFS 直接求最短路？**

> 普通 BFS 保证"层数最少"，但这里每层的距离不同。先出队的节点不一定距离最小，所以需要 Dijkstra（优先队列按距离排序）。

**Q3: 如果球可以在任意位置停下，问题变成什么？**

> 变成普通的 BFS 最短路（每步移动一格，边权都是 1），直接 BFS 即可。

## 相关题型

- [490. The Maze](https://leetcode.com/problems/the-maze/) - 迷宫可达性（BFS/DFS）
- [499. The Maze III](https://leetcode.com/problems/the-maze-iii/) - 迷宫最短路+字典序路径
- [787. Cheapest Flights Within K Stops](https://leetcode.com/problems/cheapest-flights-within-k-stops/) - 带限制的 Dijkstra
- [743. Network Delay Time](https://leetcode.com/problems/network-delay-time/) - 经典 Dijkstra
