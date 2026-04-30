# 827. 最大人工岛

## 核心思路

本质上问的是：**在一个网格中，将最多一个 0 变成 1 后，能形成的最大连通分量有多大？** 关键在于如何高效地知道一个 0 格子周围有哪些不同的岛屿，以及这些岛屿各有多大。

## 思维链

1. **读完题第一反应**：暴力做法 —— 对每个 0，尝试把它变成 1，然后 BFS/DFS 算整个网格的最大岛屿面积。每个 0 都要重新遍历一遍，代价巨大。

2. **暴力解的瓶颈**：n×n 网格中 0 的个数最多 n²，每次 BFS/DFS 也是 O(n²)，总复杂度 O(n⁴)。当 n=500 时约 625 亿次操作，完全不可行。

3. **换个角度想**：把一个 0 变成 1，本质是**把这个 0 周围的不同岛屿连起来**。如果我们提前知道每个岛屿的面积，那只要看这个 0 的上下左右邻居分别属于哪些岛、面积各多少，加起来再 +1 就行了。

4. **怎么提前知道每个岛的面积？** 先用一次 DFS/BFS 给每个连通分量标上唯一编号（着色），同时记录每个编号对应的面积。

5. **合并时怎么去重？** 一个 0 的四个邻居可能属于同一个岛屿，面积不能重复加。用 `set` 收集邻居所属的不同岛屿编号即可。

6. **特殊情况**：如果整个网格全是 1（没有 0 可以翻转），答案就是 n×n。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力 BFS | 对每个 0，临时变 1 后 BFS 求最大岛 | O(n⁴) | O(n²) | 能说出即可 |
| 岛屿编号 + 枚举 0 (DFS) | 先标色算面积，再枚举 0 合并 | O(n²) | O(n²) | ⭐ 必须写出 |
| Union-Find | 并查集维护连通性和大小 | O(n²·α(n²)) | O(n²) | 加分项 |

## 关键提示

1. **着色思想**：把每个岛屿染上不同的颜色（编号从 2 开始，避免与 0/1 混淆），同时用一个 `map` 记录 `{编号: 面积}`。

2. **枚举 0 而不是枚举 1**：翻转的是 0，所以第二遍只遍历 0 格子，看它四周有哪些不同的岛屿。

3. **去重是关键**：
```
// 示例: grid = [[1,0],[0,1]]
//
// 着色后:
//   2  0
//   0  3
//
// 面积: {2: 1, 3: 1}
//
// 枚举 (0,1) 这个0:
//   上方无, 下方=3, 左方=2, 右方无
//   不同岛: {2, 3}, 合并面积 = 1 + 1 + 1(自身) = 3 ✓
//
// 枚举 (1,0) 这个0:
//   上方=2, 下方无, 左方无, 右方=3
//   不同岛: {2, 3}, 合并面积 = 1 + 1 + 1 = 3 ✓
```

4. **全 1 特判**：如果没有任何 0，答案直接是 n×n。或者说，初始答案用所有岛中最大的来初始化即可覆盖此情况。

5. **Union-Find 版本**：把所有 1 和相邻 1 合并，然后对每个 0，查它四个邻居所在集合的大小之和（去重），思路一样但用并查集维护。

## 解法详解

### 解法1: 暴力 BFS — O(n⁴) / O(n²)

**思考过程**：最直觉的做法，对每个 0 格子，假设把它变成 1，然后重新计算最大岛面积。

```cpp
class Solution {
public:
    int largestIsland(vector<vector<int>>& grid) {
        int n = grid.size(), ans = 0;
        bool hasZero = false;
        int dx[] = {0,0,1,-1}, dy[] = {1,-1,0,0};
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 0) {
                    hasZero = true;
                    grid[i][j] = 1; // 临时翻转
                    
                    // BFS 找包含 (i,j) 的连通分量大小
                    vector<vector<bool>> visited(n, vector<bool>(n, false));
                    queue<pair<int,int>> q;
                    q.push({i, j});
                    visited[i][j] = true;
                    int area = 0;
                    while (!q.empty()) {
                        auto [x, y] = q.front(); q.pop();
                        area++;
                        for (int d = 0; d < 4; d++) {
                            int nx = x+dx[d], ny = y+dy[d];
                            if (nx>=0 && nx<n && ny>=0 && ny<n 
                                && !visited[nx][ny] && grid[nx][ny]==1) {
                                visited[nx][ny] = true;
                                q.push({nx, ny});
                            }
                        }
                    }
                    ans = max(ans, area);
                    grid[i][j] = 0; // 恢复
                }
            }
        }
        return hasZero ? ans : n * n;
    }
};
```

**关键点**：n=500 时 O(n⁴) ≈ 6.25×10¹⁰，会超时。但面试中能说出这个思路证明你理解了题意。

---

### 解法2: 岛屿编号 + 枚举 0 (DFS) — O(n²) / O(n²) ⭐ 面试首选

**从解法1优化**：解法1的瓶颈是每个 0 都要重新 BFS。我们发现岛屿本身不会变，变的只是一个 0 连接了周围的岛屿。所以先把所有岛屿预处理好（编号+面积），然后枚举 0 时只需 O(1) 查表。

```
// 完整流程示例:
// 原始:            着色:            面积表:
// 1 1 0 0          2 2 0 0          {2: 3,
// 1 0 0 0    →     2 0 0 0     →     3: 4}
// 0 0 1 1          0 0 3 3
// 0 1 1 1          0 3 3 3
//
// 枚举 (0,2) 这个0:
//   邻居编号集合: {2}(上方无, 右=0, 下=0, 左=2)
//   面积 = 3 + 1 = 4
//
// 枚举 (1,1) 这个0:
//   邻居编号集合: {2, 3}(上=2, 左=2已有, 下=0, 右=0)
//   等等，下方(2,1)=3? 不，(2,1)=0
//   重新看: 上=2, 右=0, 下=0, 左=2 → {2}, 面积=3+1=4
//
// 枚举 (2,0) 这个0:
//   上=2, 右=0, 下=0, 左=无 → {2}, 面积=3+1=4
//
// 枚举 (2,1) 这个0:
//   上=0, 右=3, 下=3已有, 左=0 → {3}, 面积=4+1=5
//
// 枚举 (1,2) 这个0:
//   上=0, 右=0, 下=3, 左=0 → {3}, 面积=4+1=5
//
// 最大 = max(所有枚举结果) 看具体哪个最大
```

```cpp
class Solution {
public:
    int largestIsland(vector<vector<int>>& grid) {
        int n = grid.size();
        int dx[] = {0,0,1,-1}, dy[] = {1,-1,0,0};
        
        // Step 1: DFS 着色，给每个岛屿标编号(从2开始)，记录面积
        unordered_map<int, int> area; // 编号 → 面积
        int colorId = 2;
        
        // DFS 函数：将 (x,y) 所在岛屿全部染成 color，返回面积
        function<int(int, int, int)> dfs = [&](int x, int y, int color) -> int {
            if (x < 0 || x >= n || y < 0 || y >= n) return 0;
            if (grid[x][y] != 1) return 0; // 不是未着色的陆地
            grid[x][y] = color;
            int size = 1;
            for (int d = 0; d < 4; d++) {
                size += dfs(x + dx[d], y + dy[d], color);
            }
            return size;
        };
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    int size = dfs(i, j, colorId);
                    area[colorId] = size;
                    colorId++;
                }
            }
        }
        
        // Step 2: 枚举每个 0，看翻转后能合并多大的岛
        int ans = 0;
        // 先用最大岛屿面积初始化（处理全1的情况）
        for (auto& [id, sz] : area) {
            ans = max(ans, sz);
        }
        
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 0) {
                    unordered_set<int> neighbors; // 相邻的不同岛屿编号
                    for (int d = 0; d < 4; d++) {
                        int ni = i + dx[d], nj = j + dy[d];
                        if (ni >= 0 && ni < n && nj >= 0 && nj < n 
                            && grid[ni][nj] > 1) {
                            neighbors.insert(grid[ni][nj]);
                        }
                    }
                    int total = 1; // 翻转的这个0本身
                    for (int id : neighbors) {
                        total += area[id];
                    }
                    ans = max(ans, total);
                }
            }
        }
        
        return ans;
    }
};
```

**关键点**：
- 编号从 2 开始，因为 0 和 1 已被使用
- 用 `set` 去重，防止同一个岛的面积被加多次
- 初始答案取已有最大岛面积，处理全 1 矩阵的边界情况

---

### 解法3: Union-Find — O(n²·α(n²)) / O(n²) 

**不同角度**：用并查集维护连通性，本质思路相同，但实现方式不同。并查集天然支持合并和查询集合大小。

```cpp
class Solution {
public:
    vector<int> parent, rank_, sz;
    
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    
    void unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return;
        if (rank_[a] < rank_[b]) swap(a, b);
        parent[b] = a;
        sz[a] += sz[b];
        if (rank_[a] == rank_[b]) rank_[a]++;
    }
    
    int largestIsland(vector<vector<int>>& grid) {
        int n = grid.size();
        int total = n * n;
        parent.resize(total);
        rank_.resize(total, 0);
        sz.resize(total, 1);
        iota(parent.begin(), parent.end(), 0); // parent[i] = i
        
        int dx[] = {0, 1}, dy[] = {1, 0}; // 只往右和下合并，避免重复
        
        // Step 1: 合并所有相邻的 1
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 1) {
                    for (int d = 0; d < 2; d++) {
                        int ni = i + dx[d], nj = j + dy[d];
                        if (ni < n && nj < n && grid[ni][nj] == 1) {
                            unite(i * n + j, ni * n + nj);
                        }
                    }
                }
            }
        }
        
        // Step 2: 枚举每个 0
        int ans = 0;
        // 先取所有岛中最大的（处理全1情况）
        for (int i = 0; i < total; i++) {
            if (grid[i / n][i % n] == 1) {
                ans = max(ans, sz[find(i)]);
            }
        }
        
        int dirs[][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == 0) {
                    unordered_set<int> roots;
                    for (auto& [di, dj] : dirs) {
                        int ni = i + di, nj = j + dj;
                        if (ni >= 0 && ni < n && nj >= 0 && nj < n 
                            && grid[ni][nj] == 1) {
                            roots.insert(find(ni * n + nj));
                        }
                    }
                    int total_size = 1;
                    for (int r : roots) {
                        total_size += sz[r];
                    }
                    ans = max(ans, total_size);
                }
            }
        }
        
        return ans;
    }
};
```

**关键点**：Union-Find 的优势在于当有多次动态合并查询时更方便扩展。对于此题来说，DFS 着色更直观。

---

## 解法对比

| | 暴力 BFS | 岛屿编号 + DFS | Union-Find |
|---|---|---|---|
| 时间 | O(n⁴) | O(n²) | O(n²·α) ≈ O(n²) |
| 空间 | O(n²) | O(n²) | O(n²) |
| 实现复杂度 | 简单 | 中等 | 中等偏高 |
| 核心思想 | 暴力枚举 | 预处理 + 查表 | 并查集维护连通性 |
| 面试推荐 | 说出思路即可 | ⭐ 首选 | 展示数据结构功底 |

**关键区别**：暴力解对每个 0 都重新遍历整个网格；优化解只遍历两次网格（一次着色，一次枚举 0），核心改进是**把岛屿信息预处理出来**。

## 易错点

1. **同一岛屿面积重复累加**
   - ✗ 直接把四个邻居的面积加起来（如果左邻和上邻属于同一个岛，面积会加两次）
   - ✓ 用 `set` 收集不同的岛屿编号，对每个编号只加一次面积

2. **着色编号与原始值冲突**
   - ✗ 从编号 1 开始着色（与原始的 `1` 混淆，DFS 会误判未着色的 1）
   - ✓ 从编号 2 开始，因为 0 是海洋、1 是未着色陆地

3. **忘记处理全 1 矩阵**
   - ✗ 只在枚举 0 时更新 `ans`，如果没有 0 则 `ans` 始终为 0
   - ✓ 用所有岛屿的最大面积初始化 `ans`

4. **DFS 着色时检查条件写错**
   - ✗ `if (grid[x][y] == 0) return 0;` — 已着色的格子（值 ≥ 2）也会被重新访问
   - ✓ `if (grid[x][y] != 1) return 0;` — 只处理未着色的陆地（值恰好为 1）

5. **Union-Find 中方向数组的选择**
   - 构建阶段只需向右和向下（2 个方向），避免重复合并
   - 查询阶段需要上下左右（4 个方向），因为要查看所有邻居

## 面试追问

**Q1: 暴力解的复杂度是多少？瓶颈在哪？**
> O(n⁴)。瓶颈在于每翻转一个 0，都要重新 BFS 整个网格。但岛屿结构在翻转前后几乎不变，大量计算是重复的。

**Q2: 如何把"重复计算岛屿面积"优化掉？**
> 提前用一次遍历给所有岛屿编号并算好面积。翻转 0 时，只需查看其四个邻居的编号和面积，O(1) 查表合并。总复杂度降到 O(n²)。

**Q3: 如果允许翻转 k 个 0 呢？**
> 这是一个更复杂的问题。k 很小时，可以枚举 k 个 0 的组合（但组合数可能很大）。一般方法是 BFS 从每个岛屿向外扩展 k 步，看能合并哪些岛屿。这没有简单的 O(n²) 解法，需要根据 k 的范围和约束选择策略。

**Q4: DFS 着色和 Union-Find 各自的优劣？**
> DFS 着色更直观、代码更简洁，适合一次性预处理的场景。Union-Find 更适合动态合并（比如 grid 会在线更新），且天然支持按秩合并和路径压缩。本题两者性能相当。

## 相关题型

- **200. 岛屿数量** — 复用 DFS/BFS 遍历连通分量的模板，本题在此基础上增加了"编号+统计面积"的步骤
- **695. 岛屿的最大面积** — 复用 DFS 求单个连通分量面积的逻辑，是本题 Step 1 的子问题
- **305. 岛屿数量 II（动态添加陆地）** — 用 Union-Find 解决动态版本，复用本题 Union-Find 解法的框架
- **934. 最短的桥** — 同样需要先识别不同岛屿（着色），然后 BFS 找两个岛之间的最短距离