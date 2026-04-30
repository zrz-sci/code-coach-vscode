# 499. The Maze III - 迷宫 III

## 核心思路
球在迷宫中滚动直到碰墙才停下，途中经过洞会掉入。需找到最短距离路径，若多条最短路径则取字典序最小的指令串。本质是**带权图的最短路径 + 字典序比较**，使用 Dijkstra 算法（优先队列），状态为 `(距离, 路径字符串, 行, 列)`。

## 思维链
1. 球从起点出发，向四个方向滚动直到碰壁或掉洞
2. 每次滚动的步数就是边权（不是1），所以不能用普通 BFS
3. 需要 Dijkstra：优先弹出距离最小的状态
4. 同距离时，比较路径字符串的字典序
5. 滚动过程中需检测是否经过洞口

## 解法概览表

| 解法 | 时间复杂度 | 空间复杂度 | 特点 |
|------|-----------|-----------|------|
| ⭐ Dijkstra + 优先队列 | O(mn·log(mn)·k) | O(mn·k) | 标准最优解，k为路径长度 |
| BFS + dist数组 | O(mn·max(m,n)) | O(mn·k) | 需额外松弛判断 |

## 关键提示
1. **方向顺序**：d, l, r, u（字典序），这样自然保证同距离时字典序优先
2. **滚动模拟**：球一直滚到碰墙，但途中要检查是否经过 hole
3. **去重条件**：`dist[r][c]` 记录到达 (r,c) 的最短距离 + 最小路径
4. **距离是步数**：每滚一格算一步，不是每次操作算一步

## 解法详解

### 解法一：Dijkstra + 优先队列 ⭐

**思路**：将迷宫建模为带权图，每个"停靠点"（碰墙停下的位置或洞口）是节点，滚动步数是边权。用最小堆按 `(距离, 路径)` 排序。

```cpp
class Solution {
public:
    string findShortestWay(vector<vector<int>>& maze, vector<int>& ball, vector<int>& hole) {
        int m = maze.size(), n = maze[0].size();
        // dist[i][j] = {最短距离, 字典序最小路径}
        vector<vector<pair<int,string>>> dist(m, vector<pair<int,string>>(n, {INT_MAX, ""}));
        dist[ball[0]][ball[1]] = {0, ""};
        
        // {距离, 路径, 行, 列}
        priority_queue<tuple<int,string,int,int>,
                       vector<tuple<int,string,int,int>>,
                       greater<>> pq;
        pq.push({0, "", ball[0], ball[1]});
        
        // 方向按字典序: d, l, r, u
        vector<int> dr = {1, 0, 0, -1};
        vector<int> dc = {0, -1, 1, 0};
        string dirs = "dlru";
        
        while (!pq.empty()) {
            auto [d, path, r, c] = pq.top(); pq.pop();
            
            // 已有更优解，跳过
            if (make_pair(d, path) > dist[r][c]) continue;
            
            for (int i = 0; i < 4; i++) {
                int nr = r, nc = c, steps = 0;
                // 滚动直到碰墙
                while (nr + dr[i] >= 0 && nr + dr[i] < m &&
                       nc + dc[i] >= 0 && nc + dc[i] < n &&
                       maze[nr + dr[i]][nc + dc[i]] == 0) {
                    nr += dr[i];
                    nc += dc[i];
                    steps++;
                    // 经过洞口则停下
                    if (nr == hole[0] && nc == hole[1]) break;
                }
                
                string newPath = path + dirs[i];
                int newDist = d + steps;
                pair<int,string> newState = {newDist, newPath};
                
                if (steps > 0 && newState < dist[nr][nc]) {
                    dist[nr][nc] = newState;
                    pq.push({newDist, newPath, nr, nc});
                }
            }
        }
        
        auto [bestDist, bestPath] = dist[hole[0]][hole[1]];
        return bestDist == INT_MAX ? "impossible" : bestPath;
    }
};
```

### 解法二：BFS + 松弛

**思路**：用普通队列进行 BFS，对每个位置维护 `(最短距离, 最小路径)`，当发现更优解时加入队列继续松弛。类似 Bellman-Ford 思路但效率较低。

```cpp
class Solution {
public:
    string findShortestWay(vector<vector<int>>& maze, vector<int>& ball, vector<int>& hole) {
        int m = maze.size(), n = maze[0].size();
        vector<vector<int>> bestDist(m, vector<int>(n, INT_MAX));
        vector<vector<string>> bestPath(m, vector<string>(n, ""));
        bestDist[ball[0]][ball[1]] = 0;
        
        queue<pair<int,int>> q;
        q.push({ball[0], ball[1]});
        
        int dr[] = {1, 0, 0, -1};
        int dc[] = {0, -1, 1, 0};
        string dirs = "dlru";
        
        while (!q.empty()) {
            auto [r, c] = q.front(); q.pop();
            
            for (int i = 0; i < 4; i++) {
                int nr = r, nc = c, steps = 0;
                while (nr + dr[i] >= 0 && nr + dr[i] < m &&
                       nc + dc[i] >= 0 && nc + dc[i] < n &&
                       maze[nr + dr[i]][nc + dc[i]] == 0) {
                    nr += dr[i]; nc += dc[i]; steps++;
                    if (nr == hole[0] && nc == hole[1]) break;
                }
                int nd = bestDist[r][c] + steps;
                string np = bestPath[r][c] + dirs[i];
                if (steps > 0 && (nd < bestDist[nr][nc] ||
                    (nd == bestDist[nr][nc] && np < bestPath[nr][nc]))) {
                    bestDist[nr][nc] = nd;
                    bestPath[nr][nc] = np;
                    q.push({nr, nc});
                }
            }
        }
        
        return bestDist[hole[0]][hole[1]] == INT_MAX ? "impossible" : bestPath[hole[0]][hole[1]];
    }
};
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| 用普通 BFS（边权=1） | 用 Dijkstra（边权=步数） | 每次滚动步数不同，边权不为1 |
| 滚动时不检测洞口 | 滚动循环中 `if (nr==hole[0] && nc==hole[1]) break` | 球经过洞就掉进去 |
| 只比较距离去重 | 比较 `(距离, 路径)` 二元组 | 同距离要取字典序更小的路径 |
| `steps == 0` 时仍入队 | 跳过 `steps == 0`（原地不动） | 碰墙无法移动时不应处理 |

## 面试追问

**Q1: 为什么不能用普通 BFS？**
> 因为球每次滚动到碰墙的步数不同（边权不同），普通 BFS 只适用于边权为1的图。需要用 Dijkstra 处理带权最短路径。

**Q2: 如何保证字典序最小？**
> 在优先队列中，状态按 `(距离, 路径字符串)` 排序。距离相同时，路径字符串字典序小的先弹出。同时方向遍历按 d→l→r→u 字典序进行。

**Q3: 与 The Maze I/II 的区别是什么？**
> Maze I 只问可达性（BFS/DFS），Maze II 求最短距离（Dijkstra 但不记路径），Maze III 需要最短距离 + 字典序最小路径 + 洞口检测（Dijkstra + 路径记录 + 滚动中检测 hole）。

## 相关题型
- [490. The Maze](https://leetcode.com/problems/the-maze/) - 迷宫可达性（BFS/DFS）
- [505. The Maze II](https://leetcode.com/problems/the-maze-ii/) - 迷宫最短距离（Dijkstra）
- [787. Cheapest Flights Within K Stops](https://leetcode.com/problems/cheapest-flights-within-k-stops/) - 带限制的最短路径
- [743. Network Delay Time](https://leetcode.com/problems/network-delay-time/) - Dijkstra 经典应用
