# 934. 最短的桥

## 核心思路

本题本质上是：**在一个矩阵中找到两个连通分量（岛屿），然后求它们之间的最短距离**。最短距离 = 需要翻转的 0 的最小数目。

## 思维链

1. **读完题第一反应**：矩阵中恰好有两个岛，要求把最少的水变成陆地把它们连起来。最朴素的想法——枚举岛1的每个格子到岛2的每个格子的曼哈顿距离，取最小值？但这要求两个岛不会"绕弯"连接，曼哈顿距离不等于实际翻转数...等等，其实在网格上只能上下左右走，**曼哈顿距离恰好就是路径长度**！所以暴力法可行。

2. **暴力解的瓶颈**：先用 DFS/BFS 找出两个岛的所有格子，然后双重循环枚举每对格子求曼哈顿距离。两个岛各有 O(n²) 个格子，最坏 O(n⁴)。能更快吗？

3. **关键观察——多源BFS**：如果我们把整个岛1的所有边界格子**同时**作为 BFS 的起点，向外一层一层扩展（每层代表翻转一个 0），第一次碰到岛2的格子时，扩展的层数就是答案。这就是经典的**多源 BFS 求最短距离**。

4. **分两步走**：
   - **Step 1**：找到岛1的所有格子（DFS 或 BFS），标记它们，并将它们全部加入 BFS 队列。
   - **Step 2**：从岛1出发做多源 BFS，每扩展一层 `level++`，碰到岛2（值为1且未被标记）时返回 `level`。

5. **为什么这是最优**：多源 BFS 保证了"同时从所有边界出发"，第一次到达目标就是最短路，时间 O(n²)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 找出两岛所有格子，枚举每对曼哈顿距离 | O(n⁴) | O(n²) | 能说出即可 |
| DFS找岛 + 多源BFS | DFS标记岛1，BFS向岛2扩展 | O(n²) | O(n²) | ⭐ 必须写出 |
| BFS找岛 + 多源BFS | 全程BFS，不用递归 | O(n²) | O(n²) | 加分项（纯迭代） |

## 关键提示

1. **怎么区分两个岛？** 找到第一个 `1` 就用 DFS/BFS 把整个连通分量标记掉（改成 `2` 或用 visited 数组），剩下的 `1` 就是第二个岛。

2. **多源BFS的本质**：把一个岛的所有格子同时入队，等价于在岛的边界"同时向外膨胀"，膨胀的层数就是桥的长度。

3. **BFS层数 = 翻转的0个数**：每扩展一层，意味着把水域(0)翻转成陆地(1)，扩展了多少层就翻转了多少个0。

4. **标记技巧**：可以直接修改 grid 值来标记已访问，比如将岛1标记为 `2`，这样不需要额外的 visited 数组。

5. **ASCII 示意图**（示例3）：
```
原始矩阵:              DFS标记岛1后:          BFS扩展过程:
1 1 1 1 1              2 2 2 2 2              2 2 2 2 2
1 0 0 0 1              2 0 0 0 2              2 3 3 3 2  ← level=1 扩展
1 0 1 0 1              2 0 1 0 2              2 3 1 3 2  ← 碰到1!返回1
1 0 0 0 1              2 0 0 0 2              
1 1 1 1 1              2 2 2 2 2              

岛1(外圈)标记为2，中间的1是岛2。
BFS从所有的2出发，向内扩展1层就碰到了岛2 → 答案=1
```

## 解法详解

### 解法1: 暴力枚举曼哈顿距离 — O(n⁴) / O(n²)

**思考过程**：最直接的想法——把两个岛的格子分别收集起来，然后枚举所有(岛1格子, 岛2格子)对，计算曼哈顿距离 `|r1-r2| + |c1-c2|`，取最小值再减1（因为起终点本身是陆地不需翻转）。

```cpp
class Solution {
public:
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    
    void dfs(vector<vector<int>>& grid, int r, int c, vector<pair<int,int>>& island) {
        int n = grid.size();
        if (r < 0 || r >= n || c < 0 || c >= n || grid[r][c] != 1) return;
        grid[r][c] = 2; // 标记已访问
        island.push_back({r, c});
        for (int d = 0; d < 4; d++) {
            dfs(grid, r + dx[d], c + dy[d], island);
        }
    }
    
    int shortestBridge(vector<vector<int>>& grid) {
        int n = grid.size();
        vector<pair<int,int>> island1, island2;
        
        // 找到第一个岛
        bool found = false;
        for (int i = 0; i < n && !found; i++) {
            for (int j = 0; j < n && !found; j++) {
                if (grid[i][j] == 1) {
                    dfs(grid, i, j, island1);
                    found = true;
                }
            }
        }
        
        // 剩下的1就是第二个岛
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    island2.push_back({i, j});
                }
            }
        }
        
        // 枚举所有对，求最小曼哈顿距离
        int minDist = INT_MAX;
        for (auto& [r1, c1] : island1) {
            for (auto& [r2, c2] : island2) {
                minDist = min(minDist, abs(r1 - r2) + abs(c1 - c2));
            }
        }
        
        // 曼哈顿距离包含了起终点之间的格子数，但起终点本身是陆地
        // 距离为d，中间需要翻转的水域 = d - 1
        return minDist - 1;
    }
};
```

**关键点**：
- 曼哈顿距离在网格上 = 最短路径长度，这个成立是因为没有障碍物阻挡。
- `minDist - 1` 是因为两个相邻的陆地格子曼哈顿距离为1但不需要翻转任何0。
- 时间复杂度：两个岛各最多 O(n²) 个格子，枚举对是 O(n⁴)。

### 解法2: DFS找岛 + 多源BFS ⭐ 面试首选 — O(n²) / O(n²)

**从解法1优化**：解法1的瓶颈是枚举所有格子对。多源BFS的思路是：把岛1的所有格子同时入队，然后一层一层向外扩展。这等价于"从岛1整体出发，找到到岛2的最短距离"，时间降为 O(n²)。

```
// 多源BFS扩展过程（示例2）:
// 
// 原始:     DFS标记岛1:    BFS level=1:    BFS level=2:
// 0 1 0     0 2 0          0 2 0           0 2 0
// 0 0 0     0 0 0          0 3 0           3 3 3
// 0 0 1     0 0 1          0 0 1           3 3 1 ← 碰到1!
//                                          答案=2
// 
// 说明: 2=岛1, 3=BFS扩展标记, 1=岛2
```

```cpp
class Solution {
public:
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    
    // DFS: 标记岛1的所有格子为2，同时加入BFS队列
    void dfs(vector<vector<int>>& grid, int r, int c, queue<pair<int,int>>& q) {
        int n = grid.size();
        if (r < 0 || r >= n || c < 0 || c >= n || grid[r][c] != 1) return;
        grid[r][c] = 2;  // 标记为"已属于岛1"
        q.push({r, c});  // 所有岛1格子都是BFS起点
        for (int d = 0; d < 4; d++) {
            dfs(grid, r + dx[d], c + dy[d], q);
        }
    }
    
    int shortestBridge(vector<vector<int>>& grid) {
        int n = grid.size();
        queue<pair<int,int>> q;
        
        // Step 1: 找到第一个1，用DFS标记整个岛1
        bool found = false;
        for (int i = 0; i < n && !found; i++) {
            for (int j = 0; j < n && !found; j++) {
                if (grid[i][j] == 1) {
                    dfs(grid, i, j, q);
                    found = true;
                }
            }
        }
        
        // Step 2: 多源BFS，从岛1向外扩展
        int level = 0;
        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; i++) {
                auto [r, c] = q.front();
                q.pop();
                for (int d = 0; d < 4; d++) {
                    int nr = r + dx[d];
                    int nc = c + dy[d];
                    if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
                    if (grid[nr][nc] == 2) continue;  // 已访问（岛1或扩展过的水域）
                    if (grid[nr][nc] == 1) return level; // 碰到岛2！
                    // grid[nr][nc] == 0，水域，标记并入队
                    grid[nr][nc] = 2;
                    q.push({nr, nc});
                }
            }
            level++;  // 扩展一层 = 翻转一层水域
        }
        
        return -1; // 不会到达，题目保证有两个岛
    }
};
```

**关键点**：
- **为什么 DFS 标记为 2**：区分"岛1的格子/已扩展的水域"（值2）和"岛2的格子"（值1），BFS 碰到 1 就知道到达了岛2。
- **level 的含义**：当前 BFS 的层数，即从岛1边界出发经过了多少层水域。第0层是岛1本身的邻居检查，所以碰到岛2时返回的 level 就是翻转的 0 的个数。
- **时间 O(n²)**：每个格子最多入队一次，总共 n² 个格子。

### 解法3: 纯BFS（找岛+扩展都用BFS）— O(n²) / O(n²)

**动机**：解法2的DFS在极端情况下（岛很大时）可能栈溢出。用BFS找岛可以避免递归深度问题。思路完全一致，只是Step1也改成BFS。

```cpp
class Solution {
public:
    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};
    
    int shortestBridge(vector<vector<int>>& grid) {
        int n = grid.size();
        queue<pair<int,int>> q;
        
        // Step 1: BFS找到并标记岛1
        bool found = false;
        for (int i = 0; i < n && !found; i++) {
            for (int j = 0; j < n && !found; j++) {
                if (grid[i][j] == 1) {
                    // 用BFS标记岛1
                    queue<pair<int,int>> bfsIsland;
                    bfsIsland.push({i, j});
                    grid[i][j] = 2;
                    while (!bfsIsland.empty()) {
                        auto [r, c] = bfsIsland.front();
                        bfsIsland.pop();
                        q.push({r, c});  // 同时加入扩展队列
                        for (int d = 0; d < 4; d++) {
                            int nr = r + dx[d], nc = c + dy[d];
                            if (nr >= 0 && nr < n && nc >= 0 && nc < n && grid[nr][nc] == 1) {
                                grid[nr][nc] = 2;
                                bfsIsland.push({nr, nc});
                            }
                        }
                    }
                    found = true;
                }
            }
        }
        
        // Step 2: 多源BFS扩展（和解法2完全一样）
        int level = 0;
        while (!q.empty()) {
            int size = q.size();
            for (int i = 0; i < size; i++) {
                auto [r, c] = q.front();
                q.pop();
                for (int d = 0; d < 4; d++) {
                    int nr = r + dx[d], nc = c + dy[d];
                    if (nr < 0 || nr >= n || nc < 0 || nc >= n) continue;
                    if (grid[nr][nc] == 2) continue;
                    if (grid[nr][nc] == 1) return level;
                    grid[nr][nc] = 2;
                    q.push({nr, nc});
                }
            }
            level++;
        }
        return -1;
    }
};
```

**关键点**：逻辑和解法2完全一致，唯一区别是用BFS代替DFS标记岛1。面试中如果面试官担心递归深度，可以提到这个变体。

## 解法对比

| | 暴力枚举 | DFS+多源BFS | 纯BFS |
|---|---|---|---|
| 时间 | O(n⁴) | O(n²) | O(n²) |
| 空间 | O(n²) | O(n²) | O(n²) |
| 核心思路 | 枚举所有对 | 多源BFS最短路 | 同左 |
| 递归风险 | 有(DFS找岛) | 有(DFS找岛) | 无 |
| 面试推荐 | 说思路即可 | ⭐ 首选 | 追问"如何避免栈溢出"时 |

- **解法1→解法2的关键转变**：从"枚举所有对"到"从一侧出发BFS"，本质是把"点到点"的距离问题转化为"集合到集合"的最短距离问题，用多源BFS解决。
- **解法2 vs 解法3**：逻辑完全一样，只是找岛的方式不同。DFS代码更简洁，BFS更安全。

## 易错点

1. **✗ BFS扩展时碰到 `grid[nr][nc] == 1` 后还入队继续扩展**
   → ✓ 碰到 1（岛2）应立即 `return level`。不需要继续扩展，因为BFS保证第一次到达就是最短的。

2. **✗ `level++` 放在 for 循环里面**
   → ✓ `level++` 必须放在处理完当前层所有节点**之后**（即 `for(int i=0;i<size;i++)` 的外面）。每一层代表一步扩展。

3. **✗ DFS 标记岛1时忘记将格子加入 BFS 队列**
   → ✓ 岛1 的**所有**格子都要入队（不只是边界），因为内部格子的邻居方向也可能朝向岛2。实际上内部格子的四个邻居都是岛1（值为2），BFS会自动跳过它们，所以全部入队不影响正确性，只是多了一些无效出队。
   
   > 进阶优化：只把岛1的边界格子入队（即至少有一个邻居是0的格子），可以减少无效操作。但面试中全部入队更简洁且正确。

4. **✗ 暴力解中返回 `minDist` 而不是 `minDist - 1`**
   → ✓ 两个相邻格子(r1,c1)和(r2,c2)的曼哈顿距离为1，但它们之间不需要翻转任何0。翻转数 = 曼哈顿距离 - 1。

5. **✗ DFS 中 grid 大小用 `grid.size()` 但忘记传引用，导致每次递归都拷贝整个矩阵**
   → ✓ `dfs(vector<vector<int>>& grid, ...)` 注意 `&` 引用。

## 面试追问

**Q1（基础理解）：为什么多源BFS第一次碰到岛2就是最短的？**
> BFS的基本性质——按层扩展，所有距离为 d 的节点一定在距离为 d+1 的节点之前被访问。所以第一次到达就是最短距离。

**Q2（优化）：能否只把岛1的边界格子入队，而不是所有格子？**
> 可以。岛1内部格子的四个邻居都是岛1自身，BFS扩展时都会被跳过。只入队边界格子（至少一个邻居是0）可以减少队列初始大小，但不影响时间复杂度（仍是 O(n²)），因为内部格子出队后不会产生新的入队操作。面试中提到这一点是加分项。

**Q3（变体）：如果矩阵中有 k 个岛，要求连接所有岛的最小翻转数？**
> 这变成了最小生成树(MST)问题。先找出所有岛，然后对每对岛求最短距离（多源BFS），构建完全图后用 Kruskal/Prim 求 MST。对应 LeetCode 1568. Minimum Number of Days to Disconnect Island 的思路变体。

**Q4（空间）：如果不允许修改原矩阵怎么办？**
> 使用额外的 `visited` 二维数组来标记，而不是直接修改 grid 的值。空间复杂度不变（仍 O(n²)），但多了一次数组分配。

## 相关题型

- **200. 岛屿数量** — 复用 DFS/BFS 标记连通分量的技巧，本题的 Step1 就是岛屿数量的子问题。
- **286. 墙与门 (Walls and Gates)** — 多源 BFS 的经典模板题，从所有门同时出发扩展。学会本题后做286可以直接复用多源BFS模板。
- **1162. 地图分析 (As Far from Land as Possible)** — 多源 BFS 求最远距离，和本题方向相反（本题求最近，1162求最远），但扩展框架完全一致。
- **127. 单词接龙 (Word Ladder)** — BFS 求最短变换路径，本质相同：每一层代表一步变换，第一次到达目标就是最短。