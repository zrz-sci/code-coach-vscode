# 490. 迷宫 (The Maze)

## 核心思路

BFS/DFS 搜索，但球在每个方向上会一直滚动直到撞墙才停下。搜索的"节点"不是每个格子，而是球能停下的位置。从 start 出发，对每个停止点向四个方向模拟滚动，找到能到达 destination 的路径。

## 思维链

1. **理解运动规则**：球不是走一格停一格，而是朝一个方向滚到撞墙才停
2. **确定搜索状态**：只有球停下来的位置才是有效状态（搜索节点）
3. **模拟滚动**：从当前停止点，向四个方向一直走直到碰壁或出界，到达新的停止点
4. **BFS/DFS 遍历**：从 start 开始 BFS/DFS，访问所有可达的停止点
5. **判断到达**：球必须能在 destination **停下来**（不是路过），检查某个停止点是否恰好等于 destination
6. **防重复**：用 visited 数组标记已访问的停止点

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| BFS | O(m*n*(m+n)) | O(m*n) | ⭐ 推荐，层序清晰 |
| DFS | O(m*n*(m+n)) | O(m*n) | 代码简洁 |

## 关键提示

1. **停止条件**：球停下的条件是下一步碰墙或出界，不是踩到 destination
2. **不能只经过 destination**：球必须在 destination 停下，路过不算
3. **visited 标记停止点**：只标记球能停下来的位置，中间经过的格子不标记
4. **每次滚动最多走 max(m,n) 步**：所以单次模拟 O(m+n)

## 解法详解

### 解法1: BFS

```cpp
class Solution1 {
public:
    bool hasPath(vector<vector<int>>& maze, vector<int>& start, vector<int>& destination) {
        int m = maze.size(), n = maze[0].size();
        vector<vector<bool>> visited(m, vector<bool>(n, false));
        queue<pair<int,int>> q;
        
        q.push({start[0], start[1]});
        visited[start[0]][start[1]] = true;
        
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        
        while (!q.empty()) {
            auto [x, y] = q.front();
            q.pop();
            
            if (x == destination[0] && y == destination[1]) return true;
            
            for (auto& d : dirs) {
                int nx = x, ny = y;
                // 一直滚直到撞墙
                while (nx + d[0] >= 0 && nx + d[0] < m &&
                       ny + d[1] >= 0 && ny + d[1] < n &&
                       maze[nx + d[0]][ny + d[1]] == 0) {
                    nx += d[0];
                    ny += d[1];
                }
                // (nx, ny) 是停止点
                if (!visited[nx][ny]) {
                    visited[nx][ny] = true;
                    q.push({nx, ny});
                }
            }
        }
        
        return false;
    }
};
```

### 解法2: DFS

```cpp
class Solution2 {
public:
    bool hasPath(vector<vector<int>>& maze, vector<int>& start, vector<int>& destination) {
        int m = maze.size(), n = maze[0].size();
        vector<vector<bool>> visited(m, vector<bool>(n, false));
        return dfs(maze, start[0], start[1], destination, visited);
    }
    
    bool dfs(vector<vector<int>>& maze, int x, int y,
             vector<int>& dest, vector<vector<bool>>& visited) {
        if (x == dest[0] && y == dest[1]) return true;
        if (visited[x][y]) return false;
        
        visited[x][y] = true;
        int m = maze.size(), n = maze[0].size();
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        
        for (auto& d : dirs) {
            int nx = x, ny = y;
            while (nx + d[0] >= 0 && nx + d[0] < m &&
                   ny + d[1] >= 0 && ny + d[1] < n &&
                   maze[nx + d[0]][ny + d[1]] == 0) {
                nx += d[0];
                ny += d[1];
            }
            if (dfs(maze, nx, ny, dest, visited)) return true;
        }
        
        return false;
    }
};
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 原因 |
|-----------|-----------|------|
| 走一步就停 `nx += d[0]` 后直接用 | `while(...) { nx += d[0]; ny += d[1]; }` | 球会一直滚到撞墙 |
| 在滚动途中判断 `== destination` | 在停止后判断 | 球只有停下来才算到达 |
| `maze[nx][ny] == 0` 判断当前格 | `maze[nx+d[0]][ny+d[1]] == 0` 判断下一格 | 判断的是下一步能否继续滚 |
| 标记经过的每个格子 | 只标记停止点 | 中间路过的格子不是搜索状态 |

## 面试追问

**Q1: BFS 和 DFS 哪个更适合这道题？**
A: 两者都可以。如果只是判断可达性，DFS 代码更简洁。如果需要最少步数（如 LC 505 The Maze II），必须用 BFS 或 Dijkstra。

**Q2: 时间复杂度为什么是 O(m*n*(m+n))？**
A: 最多有 m*n 个停止点，每个停止点向 4 个方向滚动，每次滚动最多走 max(m,n) 步。所以总时间 = m*n * 4 * max(m,n) = O(m*n*(m+n))。

**Q3: 如何扩展到求最短路径（步数最少）？**
A: 这是 LC 505 The Maze II。需要用 Dijkstra（边权是滚动距离）或 BFS（如果每步权重为1）。不能用普通 BFS 因为边权不同。

## 相关题型

- [505. 迷宫 II](../0505_the_maze_ii/) - 求最短距离（Dijkstra）
- [499. 迷宫 III](../0499_the_maze_iii/) - 求最短路径字符串
- [200. 岛屿数量](../0200_number_of_islands/) - BFS/DFS 基础
- [1091. 二进制矩阵中的最短路径](../1091_shortest_path_in_binary_matrix/) - BFS 最短路
