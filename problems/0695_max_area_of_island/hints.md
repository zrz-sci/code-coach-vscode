# 695. 岛屿的最大面积

## 核心思路

本质上是在一个二维网格上找所有**连通分量**（四方向相邻的1的集合），并返回最大连通分量的大小。

## 思维链

1. **读完题第一反应**：这是一个在网格中"找连通区域"的问题。每个1可能和它上下左右的1相连，形成一个岛屿。我需要遍历所有岛屿，计算每个岛的面积，取最大值。

2. **怎么遍历一个岛屿？** 从一个值为1的格子出发，往四个方向扩展，把所有相连的1都找到 —— 这就是经典的 **Flood Fill** 问题，可以用 DFS 或 BFS。

3. **怎么避免重复访问？** 一个格子被访问后，如果不做标记，它会被相邻格子再次访问，导致死循环。最简单的方法：**访问后直接把 grid[i][j] 改成 0**（沉岛法），既标记了已访问，又不需要额外空间。

4. **整体流程**：遍历整个网格，遇到1就启动一次 DFS/BFS，计算该岛面积，更新最大值。遇到0或已访问的格子跳过。

5. **还有其他方法吗？** 除了 DFS/BFS，**并查集（Union Find）** 也能解决连通分量问题：把相邻的1合并到同一集合，最后找最大集合。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DFS (递归) | 遇到1就递归扩展四方向，沉岛标记 | O(m×n) | O(m×n) 栈深 | ⭐ 必须写出 |
| BFS (队列) | 遇到1就用队列逐层扩展 | O(m×n) | O(min(m,n)) | ⭐ 必须写出 |
| 并查集 | 相邻的1合并，找最大集合 | O(m×n·α) | O(m×n) | 加分项 |

## 关键提示

1. **沉岛法**：访问过的1直接改成0，省去 visited 数组。但如果不允许修改原数组，就需要额外的 visited 矩阵。

2. **方向数组**：用 `dx[] = {0,0,1,-1}` 和 `dy[] = {1,-1,0,0}` 来统一处理四个方向，避免写4个 if。

3. **DFS 返回值**：让 DFS 函数返回"从当前格子出发能扩展到的面积"，每个格子贡献1，加上四个方向递归返回的面积之和。

4. **BFS 入队时标记**：不是出队时标记！否则同一个格子会被多次入队，导致面积计算错误。

5. **网格示意（示例1局部）**：
```
    0 0 1 0 0 0 0 1 0 0 0 0 0
    0 0 0 0 0 0 0 1 1 1 0 0 0
    0 1 1 0 1 0 0 0 0 0 0 0 0
    0 1 0 0 1 1 0 0[1]0 1 0 0    ← 最大岛从这里开始
    0 1 0 0 1 1 0 0[1 1 1]0 0    ← 面积=6
    0 0 0 0 0 0 0 0 0 0[1]0 0
    0 0 0 0 0 0 0 1 1 1 0 0 0
    0 0 0 0 0 0 0 1 1 0 0 0 0
```

## 解法详解

### 解法1: DFS (递归沉岛) — O(m×n) / O(m×n) ⭐ 面试首选

**思考过程**：对于每个为1的格子，我想知道它所在岛屿有多大。从它出发，递归地向四个方向走，每走一步面积+1，走过的格子标记为0防止重复。所有递归返回后，就得到了这个岛的完整面积。

```cpp
class Solution {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int maxArea = 0;
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    maxArea = max(maxArea, dfs(grid, i, j, m, n));
                }
            }
        }
        return maxArea;
    }

private:
    int dfs(vector<vector<int>>& grid, int i, int j, int m, int n) {
        // 越界或遇到水，返回0
        if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] == 0)
            return 0;

        grid[i][j] = 0;  // 沉岛：标记已访问，防止重复计数

        // 当前格子面积1 + 四个方向递归扩展的面积
        return 1 + dfs(grid, i + 1, j, m, n)
                 + dfs(grid, i - 1, j, m, n)
                 + dfs(grid, i, j + 1, m, n)
                 + dfs(grid, i, j - 1, m, n);
    }
};
```

**关键点**：
- `grid[i][j] = 0` 必须在递归之前执行，否则相邻格子会回头访问当前格子，导致无限递归。
- 递归栈深度最坏情况是 m×n（所有格子都是1的蛇形路径）。

---

### 解法2: BFS (队列沉岛) — O(m×n) / O(min(m,n))

**从 DFS 到 BFS**：DFS 用递归栈，BFS 用显式队列。BFS 逐层扩展，先处理完当前层的所有邻居，再处理下一层。对于"求面积"这个问题，两者效果相同，但 BFS 不会有栈溢出风险。

```
BFS 扩展过程（局部示例）:
起点(3,8)=1, 入队

Queue: [(3,8)]           area=1
  扩展(3,8) → 邻居(4,8)=1
Queue: [(4,8)]           area=2
  扩展(4,8) → 邻居(4,9)=1, (5,10)已经不相邻...
Queue: [(4,9)]           area=3
  ...继续扩展直到队列为空
最终 area=6
```

```cpp
class Solution {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int maxArea = 0;
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    int area = 0;
                    queue<pair<int,int>> q;
                    q.push({i, j});
                    grid[i][j] = 0;  // 入队时就标记，不是出队时！

                    while (!q.empty()) {
                        auto [x, y] = q.front();
                        q.pop();
                        area++;

                        for (int d = 0; d < 4; d++) {
                            int nx = x + dx[d];
                            int ny = y + dy[d];
                            if (nx >= 0 && nx < m && ny >= 0 && ny < n
                                && grid[nx][ny] == 1) {
                                grid[nx][ny] = 0;  // 入队时标记
                                q.push({nx, ny});
                            }
                        }
                    }
                    maxArea = max(maxArea, area);
                }
            }
        }
        return maxArea;
    }
};
```

**关键点**：BFS 空间复杂度取决于队列最大长度，对于矩阵 BFS 最大约 O(min(m,n))（对角线上的元素同时在队列中）。

---

### 解法3: DFS (迭代栈版) — O(m×n) / O(m×n)

**思考过程**：面试官可能追问"能不能不用递归实现 DFS？"。用显式栈模拟递归调用栈即可。

```cpp
class Solution {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        int maxArea = 0;
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    int area = 0;
                    stack<pair<int,int>> stk;
                    stk.push({i, j});
                    grid[i][j] = 0;

                    while (!stk.empty()) {
                        auto [x, y] = stk.top();
                        stk.pop();
                        area++;

                        for (int d = 0; d < 4; d++) {
                            int nx = x + dx[d];
                            int ny = y + dy[d];
                            if (nx >= 0 && nx < m && ny >= 0 && ny < n
                                && grid[nx][ny] == 1) {
                                grid[nx][ny] = 0;
                                stk.push({nx, ny});
                            }
                        }
                    }
                    maxArea = max(maxArea, area);
                }
            }
        }
        return maxArea;
    }
};
```

---

### 解法4: 并查集 (Union Find) — O(m×n·α(m×n)) / O(m×n)

**不同视角**：把每个格子看作图中的节点，相邻的两个1之间有一条边。用并查集合并连通的节点，最后统计每个集合的大小，取最大值。

```cpp
class Solution {
public:
    vector<int> parent, rank_, size_;

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);  // 路径压缩
        return parent[x];
    }

    void unite(int a, int b) {
        int ra = find(a), rb = find(b);
        if (ra == rb) return;
        // 按秩合并
        if (rank_[ra] < rank_[rb]) swap(ra, rb);
        parent[rb] = ra;
        size_[ra] += size_[rb];  // 合并时累加集合大小
        if (rank_[ra] == rank_[rb]) rank_[ra]++;
    }

    int maxAreaOfIsland(vector<vector<int>>& grid) {
        int m = grid.size(), n = grid[0].size();
        parent.resize(m * n);
        rank_.resize(m * n, 0);
        size_.resize(m * n, 1);

        // 初始化：每个节点是自己的父亲
        for (int i = 0; i < m * n; i++) parent[i] = i;

        int dx[] = {1, 0};  // 只需向右和向下合并，避免重复
        int dy[] = {0, 1};

        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    for (int d = 0; d < 2; d++) {
                        int ni = i + dx[d], nj = j + dy[d];
                        if (ni < m && nj < n && grid[ni][nj] == 1) {
                            unite(i * n + j, ni * n + nj);
                        }
                    }
                }
            }
        }

        int maxArea = 0;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (grid[i][j] == 1)
                    maxArea = max(maxArea, size_[find(i * n + j)]);
        return maxArea;
    }
};
```

**关键点**：
- 二维坐标 `(i,j)` 映射到一维 `i*n+j`。
- 合并时只需看右方和下方，因为左方和上方的合并已经在前面的遍历中处理过了。
- `α(n)` 是反阿克曼函数，实际几乎为常数。

## 解法对比

| | DFS递归 | BFS | DFS迭代栈 | 并查集 |
|---|---|---|---|---|
| 时间 | O(m×n) | O(m×n) | O(m×n) | O(m×n·α) |
| 空间 | O(m×n) 栈深 | O(min(m,n)) | O(m×n) | O(m×n) |
| 代码简洁度 | ⭐最简洁 | 中等 | 中等 | 较复杂 |
| 栈溢出风险 | 有（大矩阵） | 无 | 无 | 无 |
| 适用场景 | 面试首选 | 工程实践 | 面试追问 | 动态连通性 |

**面试推荐**：先写 DFS 递归（最快写完），被追问栈溢出时改 BFS 或迭代栈。并查集在面试中一般不会作为首选，但如果题目涉及"动态加边/删边"则并查集更优。

## 易错点

1. **BFS 出队时才标记 → 重复入队**
   - ✗ `出队后 grid[x][y] = 0` → 同一个格子被多个邻居入队，面积被多算
   - ✓ `入队时 grid[nx][ny] = 0` → 保证每个格子只入队一次

2. **DFS 忘记先沉岛就递归 → 死循环**
   - ✗ 先递归四方向，最后才 `grid[i][j] = 0`
   - ✓ 进入 DFS 函数后**立刻** `grid[i][j] = 0`，再递归

3. **并查集只找 parent 不找 root**
   - ✗ `size_[i*n+j]` → 这不一定是根节点的 size
   - ✓ `size_[find(i*n+j)]` → 必须找到根节点再取 size

4. **修改了原数组但题目不允许**
   - 如果不能修改原数组，需要用额外的 `visited[m][n]` 数组
   - 或者先沉岛，最后再恢复（两次遍历）

## 面试追问

**Q1: 基础理解 — 为什么 DFS 和 BFS 都能解这道题？它们遍历顺序有什么不同？**
→ 两者都能遍历连通分量的所有节点。DFS 沿一条路径走到底再回溯（深度优先），BFS 逐层扩展（广度优先）。对于"求面积"只需要访问所有节点，不关心顺序，所以两者等价。

**Q2: 优化 — 如果矩阵很大（10000×10000），递归 DFS 会怎样？怎么解决？**
→ 递归深度可能达到 10^8，栈溢出。解决方案：改用 BFS（队列在堆上分配，不受栈大小限制）或迭代 DFS（显式栈）。

**Q3: 变体 — 如果不允许修改原数组，空间怎么优化？**
→ 用一个 `vector<vector<bool>> visited` 额外标记。空间从 O(1)（沉岛法不算额外空间）变为 O(m×n)。无法避免，因为必须记录哪些格子已访问。

**Q4: 进阶 — 如果格子可以动态变化（0→1 或 1→0），怎么高效维护最大岛屿面积？**
→ 这就是 LeetCode 305 的思路。用并查集，每次 `0→1` 时尝试与四邻居合并，维护全局最大 size。DFS/BFS 需要每次重新遍历，不适合动态场景。

## 相关题型

| 题号 | 题目 | 关联 |
|------|------|------|
| **200** | 岛屿数量 | 本题的简化版 — 只需计数连通分量个数，不需要算面积。DFS 框架完全相同，去掉返回面积的逻辑即可 |
| **463** | 岛屿的周长 | 同样的网格遍历 — 区别：不是求面积而是求边界。每个1贡献4条边，减去与相邻1共享的边 |
| **827** | 最大人工岛 | 本题的升级版 — 先用本题的方法给每个岛编号并算面积，然后枚举每个0看翻转后能连接哪些岛 |
| **305** | 岛屿数量 II | 动态版 — 格子逐个变为1，每次查询岛屿数量。必须用并查集，复用本题解法4的 union-find 框架 |
| **733** | 图像渲染 | Flood Fill 原型题 — 复用完全相同的 DFS/BFS 框架，只是把"沉岛"改成"染色" |