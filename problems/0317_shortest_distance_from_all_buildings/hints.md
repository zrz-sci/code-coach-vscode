# 317. 离建筑物最近的距离

## 核心思路

本质上是一个**多源 BFS** 问题：从每栋建筑出发做 BFS 计算到所有空地的最短距离，然后在所有**能到达全部建筑**的空地中找总距离最小的。

## 思维链

1. **读完题第一反应**：对每个空地 `(i,j)`，计算它到所有建筑的最短距离之和，取最小值。怎么算最短距离？BFS！→ 暴力做法：对每个空地做 BFS 到所有建筑。

2. **暴力解的瓶颈**：空地数量可能很多（最多 2500 个），每个空地做一次 BFS 是 O(m×n)，再乘建筑数量验证可达性，整体效率不高。而且从空地出发 BFS 需要找到所有建筑，路径可能被障碍截断导致大量无效搜索。

3. **换个方向**：空地可能很多，但建筑数量通常较少。如果**从每栋建筑出发做 BFS**，一次 BFS 就能算出该建筑到所有可达空地的距离。建筑数 B 通常远小于空地数，总复杂度 O(B × m × n)。

4. **关键约束**：目标空地必须能到达**所有**建筑。所以需要额外记录每个空地被多少栋建筑的 BFS 访问过（`reachCount`），只有 `reachCount == totalBuildings` 的空地才是候选。

5. **优化技巧**：可以用一个递减标记代替 visited 数组 —— 第 1 轮 BFS 只访问值为 0 的格子，访问后改成 -1；第 2 轮只访问 -1 的格子，访问后改成 -2……这样天然过滤掉"上一轮就不可达"的格子，实现剪枝。

6. **不可达判断**：如果某次 BFS 发现无法从建筑到达某个之前可达的空地，说明那个空地不可能到达所有建筑，后续直接跳过。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 从空地出发 BFS | 对每个空地做 BFS 找所有建筑 | O(E×m×n)，E=空地数 | O(m×n) | 能说出即可 |
| 从建筑出发 BFS | 对每栋建筑做 BFS，累加距离 | O(B×m×n)，B=建筑数 | O(m×n) | ⭐ 必须写出 |
| 从建筑出发 BFS + 递减标记剪枝 | 用格子值递减代替 visited，天然剪枝 | O(B×m×n)，但实际更快 | O(m×n) | 加分项 |

## 关键提示

1. **方向反转**：不要从空地找建筑，而是从建筑找空地。建筑数量远少于空地数量。

2. **可达性检查**：一个空地必须被**所有**建筑的 BFS 都访问到，才是合法候选。用 `reachCount[i][j]` 记录。

3. **剪枝**：如果某栋建筑的 BFS 无法到达任何新空地（被障碍完全隔开），可以直接返回 -1。

4. **递减标记技巧**：第 k 次 BFS（从第 k 栋建筑出发），只访问值为 `-(k-1)` 的格子（即前 k-1 栋建筑都能到达的格子），访问后改为 `-k`。这样不需要额外的 visited 数组和 reachCount 数组。

5. **网格 BFS 示意图**（示例1）：
```
// 网格:
//   1  0  2  0  1       建筑: (0,0), (0,4), (2,2)
//   0  0  0  0  0       障碍: (0,2)
//   0  0  1  0  0
//
// 从 (0,0) BFS:          从 (0,4) BFS:          从 (2,2) BFS:
//   B  1  X  .  .         .  .  X  1  B          .  .  .  .  .
//   1  2  3  4  5         5  4  3  2  1          2  2  .  2  2
//   2  3  B  5  6         6  5  B  3  2          1  1  B  1  1
//
// 累加距离:
//   B  .  X  .  B       (1,2) = 3+3+1 = 7 ← 最小!
//   .  .  7  .  .
//   .  .  B  .  .
```

## 解法详解

### 解法1: 从空地出发 BFS — O(E×m×n) / O(m×n)

**思考过程**：最直觉的想法——遍历每个空地，对它做 BFS 计算到所有建筑的距离之和。

```cpp
class Solution {
public:
    int shortestDistance(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int totalBuildings = 0;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (grid[i][j] == 1) totalBuildings++;
        
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        int result = INT_MAX;
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] != 0) continue; // 只从空地出发
                
                // BFS 计算 (i,j) 到所有建筑的距离和
                vector<vector<bool>> visited(m, vector<bool>(n, false));
                queue<pair<int,int>> q;
                q.push({i, j});
                visited[i][j] = true;
                int dist = 0, totalDist = 0, reached = 0;
                
                while (!q.empty() && reached < totalBuildings) {
                    int sz = q.size();
                    dist++;
                    while (sz--) {
                        auto [x, y] = q.front(); q.pop();
                        for (auto& d : dirs) {
                            int nx = x + d[0], ny = y + d[1];
                            if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
                            if (visited[nx][ny]) continue;
                            visited[nx][ny] = true;
                            if (grid[nx][ny] == 1) {
                                totalDist += dist;
                                reached++;
                            } else if (grid[nx][ny] == 0) {
                                q.push({nx, ny});
                            }
                            // grid[nx][ny]==2 是障碍，不入队
                        }
                    }
                }
                
                if (reached == totalBuildings)
                    result = min(result, totalDist);
            }
        }
        return result == INT_MAX ? -1 : result;
    }
};
```

**关键点**：
- 空地数量可能远多于建筑数量，导致大量重复 BFS。
- 每次 BFS 必须验证 `reached == totalBuildings`，否则该空地不合法。

---

### 解法2: 从建筑出发 BFS — O(B×m×n) / O(m×n) ⭐ 面试首选

**从解法1优化**：解法1对每个空地做 BFS，空地多时很慢。反转思路：从每栋建筑做 BFS，把距离累加到空地上。

```cpp
class Solution {
public:
    int shortestDistance(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int totalBuildings = 0;
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        
        // distSum[i][j]: 空地(i,j)到所有建筑的距离之和
        // reachCount[i][j]: 有多少栋建筑能到达(i,j)
        vector<vector<int>> distSum(m, vector<int>(n, 0));
        vector<vector<int>> reachCount(m, vector<int>(n, 0));
        
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (grid[i][j] == 1) totalBuildings++;
        
        // 从每栋建筑出发 BFS
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] != 1) continue;
                
                vector<vector<bool>> visited(m, vector<bool>(n, false));
                queue<pair<int,int>> q;
                q.push({i, j});
                visited[i][j] = true;
                int dist = 0;
                
                while (!q.empty()) {
                    int sz = q.size();
                    dist++;
                    while (sz--) {
                        auto [x, y] = q.front(); q.pop();
                        for (auto& d : dirs) {
                            int nx = x + d[0], ny = y + d[1];
                            if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
                            if (visited[nx][ny] || grid[nx][ny] != 0) continue;
                            visited[nx][ny] = true;
                            distSum[nx][ny] += dist;
                            reachCount[nx][ny]++;
                            q.push({nx, ny});
                        }
                    }
                }
            }
        }
        
        // 找能到达所有建筑的空地中距离和最小的
        int result = INT_MAX;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (grid[i][j] == 0 && reachCount[i][j] == totalBuildings)
                    result = min(result, distSum[i][j]);
        
        return result == INT_MAX ? -1 : result;
    }
};
```

**关键点**：
- BFS 从建筑出发时只能走空地（`grid[nx][ny] == 0`），建筑和障碍都不能穿过。
- `reachCount` 是关键——空地 `(i,j)` 必须被所有建筑的 BFS 都到达才有效。

---

### 解法3: 从建筑出发 BFS + 递减标记剪枝 — O(B×m×n) / O(m×n)

**进一步优化**：解法2需要额外的 `visited` 和 `reachCount` 数组。利用一个巧妙的递减标记，同时实现 visited 功能和可达性过滤。

核心想法：第 k 次 BFS（k 从 0 开始），只允许访问值为 `-k` 的格子，访问后改为 `-(k+1)`。这意味着一个格子只有被前 k 次 BFS 都访问过，才会参与第 k+1 次 BFS。

```cpp
class Solution {
public:
    int shortestDistance(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        
        vector<vector<int>> distSum(m, vector<int>(n, 0));
        int result = INT_MAX;
        int emptyLandMark = 0; // 第0次BFS期望的空地值是0，之后递减
        
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] != 1) continue;
                
                // 从建筑(i,j)出发BFS
                result = INT_MAX; // 每轮重置，最终结果在最后一轮确定
                queue<pair<int,int>> q;
                q.push({i, j});
                int dist = 0;
                
                while (!q.empty()) {
                    int sz = q.size();
                    dist++;
                    while (sz--) {
                        auto [x, y] = q.front(); q.pop();
                        for (auto& d : dirs) {
                            int nx = x + d[0], ny = y + d[1];
                            if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
                            // 只访问值等于 emptyLandMark 的格子
                            if (grid[nx][ny] != emptyLandMark) continue;
                            grid[nx][ny]--; // 标记为 emptyLandMark-1
                            distSum[nx][ny] += dist;
                            q.push({nx, ny});
                            result = min(result, distSum[nx][ny]);
                        }
                    }
                }
                emptyLandMark--; // 下一轮期望值递减
            }
        }
        return result == INT_MAX ? -1 : result;
    }
};
```

**关键点**：
- 不需要 `visited` 数组：因为格子值在被访问后立刻改变，不会被同一轮 BFS 重复访问。
- 不需要 `reachCount` 数组：只有被前 k 轮都访问过的格子值才等于 `emptyLandMark`，天然过滤了不可达的格子。
- `result` 在每轮 BFS 开始时重置为 `INT_MAX`，只在最后一轮有效——因为只有最后一轮的 `result` 代表到达所有建筑的距离和。

## 解法对比

| | 解法1: 从空地BFS | 解法2: 从建筑BFS | 解法3: 递减标记 |
|---|---|---|---|
| 时间 | O(E×m×n) | O(B×m×n) | O(B×m×n) |
| 空间 | O(m×n) | O(m×n)额外3个 | O(m×n)仅distSum |
| 核心差异 | 空地多时慢 | 建筑少时快 | 剪枝+省空间 |
| 代码复杂度 | 简单 | 中等 | 稍复杂 |

**面试推荐**：解法2是标准答案，解法3是加分项。解法1能展示理解但效率不够。

## 易错点

1. **✗ BFS 从建筑出发时把其他建筑也入队**
   ```cpp
   // 错误：只检查了 visited，没检查 grid 值
   if (!visited[nx][ny]) q.push({nx, ny});
   ```
   **✓ 正确：只能走空地**
   ```cpp
   if (!visited[nx][ny] && grid[nx][ny] == 0) q.push({nx, ny});
   ```
   建筑和障碍都是不可穿越的。

2. **✗ 忘记检查 reachCount，认为只要距离和最小就行**
   ```cpp
   // 错误：没有验证是否所有建筑都能到达
   result = min(result, distSum[i][j]);
   ```
   **✓ 正确：必须判断可达性**
   ```cpp
   if (reachCount[i][j] == totalBuildings)
       result = min(result, distSum[i][j]);
   ```

3. **✗ 解法3中 `result` 没有在每轮 BFS 重置**
   如果不重置，上一轮的 `result` 可能是只到达了部分建筑的值，被错误保留。

4. **✗ dist 变量放在了 while 外面没有正确递增**
   `dist` 必须在每层 BFS 扩展时 +1，不是每次出队 +1。层序遍历的经典错误。

## 面试追问

**Q1: 为什么从建筑出发而不是从空地出发？**
→ 建筑数量通常远少于空地数量。BFS 次数 = 出发点数量 × O(m×n)，出发点越少越快。

**Q2: 如果某栋建筑被障碍完全包围，怎么提前返回？**
→ 在每轮 BFS 结束后检查：如果这一轮没有访问到任何空地（队列很快就空了），说明这栋建筑不可达，直接返回 -1。

**Q3: 解法3的递减标记为什么是正确的？**
→ 初始空地值为 0。第 1 轮 BFS 把可达空地改为 -1。第 2 轮只访问 -1 的格子（即第 1 轮可达的），改为 -2。以此类推，第 k 轮只处理被前 k-1 栋建筑都能到达的格子。最后一轮处理的就是被所有建筑都能到达的空地。

**Q4: 如果网格非常大但建筑很少（B=2），有更优解法吗？**
→ 当 B 极小时，O(B×m×n) 已经接近最优。但如果允许预处理，可以考虑双向 BFS。不过对于本题约束 (m,n ≤ 50)，解法2/3足够。

## 相关题型

- **286. 墙与门 (Walls and Gates)** — 同样是多源 BFS 从特殊点出发更新空地距离。区别：286 只需要最近的一个门的距离，本题需要所有建筑的距离之和。
- **1162. 地图分析 (As Far from Land as Possible)** — 多源 BFS 从所有陆地出发找最远的水域。复用相同的多源 BFS 模板，但目标是最大距离而非最小。
- **994. 腐烂的橘子 (Rotting Oranges)** — 多源 BFS 经典入门题。从所有腐烂橘子同时出发。本题可以看作其升级版（多轮 BFS + 距离累加）。