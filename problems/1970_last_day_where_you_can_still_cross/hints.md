# 1970. 你能穿过矩阵的最后一天

## 核心思路

本质上是一个**连通性的时间节点问题**：陆地逐渐被水淹没，找从上到下连通路径断开前的最后一天。关键洞察是"正向淹没找断开"不好做，但"逆向添加陆地找连通"或"二分答案+验证连通性"非常自然。

## 思维链

1. **读完题第一反应**：每天淹一格，问最晚哪天还能从第一行走到最后一行。最朴素的做法——对每一天，构建当天的矩阵，然后 BFS/DFS 检查是否从顶部到底部连通。时间 O(row×col × row×col) = O((row×col)²)，太慢。

2. **暴力解的瓶颈**：每天都做一次完整的 BFS/DFS，天数最多 row×col 次，每次 BFS O(row×col)，总共 O((row×col)²)。

3. **关键观察——答案具有单调性**：如果第 d 天还能过，那第 d-1 天（水更少）一定也能过。如果第 d 天过不了，第 d+1 天也一定过不了。这说明存在一个分界点，左边全能过，右边全不能过 → **二分答案**！

4. **二分 + BFS 验证**：二分天数 mid，把前 mid 天淹没的格子标记为水，然后 BFS 检查能否从第一行到最后一行。总共 O(row×col × log(row×col))。

5. **更优思路——逆向并查集**：正向是"不断移除陆地"（并查集不擅长删除），逆向是"不断添加陆地"（并查集擅长合并）。从最后一天开始倒着恢复陆地，每恢复一格就与邻居合并，当第一行和最后一行第一次连通时，当前天数就是答案。

6. **并查集实现细节**：用两个虚拟节点 `topVirtual` 和 `bottomVirtual`，分别连接第一行和最后一行的陆地节点。当 topVirtual 和 bottomVirtual 连通时，说明可以穿越。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力 BFS | 每天检查一次连通性 | O((RC)²) | O(RC) | 能说出即可 |
| 二分 + BFS | 二分天数，BFS验证 | O(RC·log(RC)) | O(RC) | ⭐ 必须写出 |
| 逆向并查集 | 倒序恢复陆地，Union-Find | O(RC·α(RC)) ≈ O(RC) | O(RC) | 加分项 |

> R = row, C = col, RC = row × col

## 关键提示

- **提示1**：如果第 d 天能走通，第 d-1 天一定也能走通。这个单调性暗示了什么搜索策略？

- **提示2**：二分的 check 函数：把前 mid 天的格子标记为水，然后从第一行所有陆地出发 BFS，看能否到达最后一行。

- **提示3**：并查集擅长"加边"（合并），不擅长"删边"。能不能把"逐渐淹没"反转成"逐渐恢复"？

- **提示4**：并查集方案中，用两个虚拟节点代表"第一行整体"和"最后一行整体"，避免遍历整行检查连通性。

- **提示5**：注意题目坐标是 **1-based**，记得转换到 0-based。

```
// 矩阵示意 (3×3, 前3天淹没后):
// Day0(全陆地)  Day1([1,2]淹)  Day2([2,1]淹)  Day3([3,3]淹)
//  0 0 0        0 1 0          0 1 0          0 1 0
//  0 0 0        0 0 0          1 0 0          1 0 0
//  0 0 0        0 0 0          0 0 0          0 0 1
//
// Day3: 从第1行的(0,0)出发 → (1,1) → (2,1) → (2,0) → 到达最后一行 ✓
// Day4: [2,2]淹后中间被切断，无法穿越 ✗
```

## 解法详解

### 解法1: 暴力 — 每天 BFS 检查 — O((RC)²) / O(RC)

**思考过程**：最直觉的方式——模拟每一天的矩阵状态，用 BFS 检查从顶行到底行是否连通。从最后一天往前找，第一个连通的天就是答案。或者从第 1 天开始，找最后一个连通的天。

```cpp
class Solution {
public:
    int latestDayToCross(int row, int col, vector<vector<int>>& cells) {
        int n = cells.size();
        // 从后往前找第一天能连通的
        for (int day = n; day >= 1; day--) {
            // 构建第 day 天的矩阵
            vector<vector<int>> grid(row, vector<int>(col, 0));
            for (int i = 0; i < day; i++) {
                grid[cells[i][0] - 1][cells[i][1] - 1] = 1; // 1-based转0-based
            }
            if (canCross(grid, row, col)) return day;
        }
        return 0;
    }
    
    bool canCross(vector<vector<int>>& grid, int row, int col) {
        // BFS 从第一行所有陆地出发
        queue<pair<int,int>> q;
        vector<vector<bool>> visited(row, vector<bool>(col, false));
        for (int j = 0; j < col; j++) {
            if (grid[0][j] == 0) {
                q.push({0, j});
                visited[0][j] = true;
            }
        }
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};
        while (!q.empty()) {
            auto [x, y] = q.front(); q.pop();
            if (x == row - 1) return true; // 到达最后一行
            for (int d = 0; d < 4; d++) {
                int nx = x + dx[d], ny = y + dy[d];
                if (nx >= 0 && nx < row && ny >= 0 && ny < col 
                    && !visited[nx][ny] && grid[nx][ny] == 0) {
                    visited[nx][ny] = true;
                    q.push({nx, ny});
                }
            }
        }
        return false;
    }
};
```

**关键点**：这个解法太慢，面试中说明思路即可，目的是引出二分优化。

---

### 解法2: 二分 + BFS — O(RC·log(RC)) / O(RC) ⭐ 面试首选

**从解法1优化**：解法1对每天都做 BFS，但我们发现答案具有单调性——存在一个分界天数 d，d 及之前能走通，d+1 及之后走不通。这就是经典的二分答案模型。

```
// 二分搜索空间示意:
// 天数:  1  2  3  4  5  6  7  8  9
// 能过: ✓  ✓  ✓  ✓  ✗  ✗  ✗  ✗  ✗
//                   ↑
//                 答案=4
//
// Round1: lo=1, hi=9, mid=5 → 不能过 → hi=4
// Round2: lo=1, hi=4, mid=2 → 能过   → lo=3
// Round3: lo=3, hi=4, mid=3 → 能过   → lo=4
// Round4: lo=4, hi=4, mid=4 → 能过   → lo=5
// lo > hi, 返回 hi=4 ✓
```

```cpp
class Solution {
public:
    int latestDayToCross(int row, int col, vector<vector<int>>& cells) {
        int lo = 1, hi = cells.size();
        int ans = 0;
        while (lo <= hi) {
            int mid = lo + (hi - lo) / 2;
            if (canCross(row, col, cells, mid)) {
                ans = mid;      // mid天还能过，尝试更晚的天
                lo = mid + 1;
            } else {
                hi = mid - 1;   // mid天已不能过，尝试更早的天
            }
        }
        return ans;
    }
    
    bool canCross(int row, int col, vector<vector<int>>& cells, int day) {
        // 构建前 day 天的矩阵
        vector<vector<int>> grid(row, vector<int>(col, 0));
        for (int i = 0; i < day; i++) {
            grid[cells[i][0] - 1][cells[i][1] - 1] = 1;
        }
        // BFS 从第一行陆地出发，检查能否到达最后一行
        queue<pair<int,int>> q;
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};
        for (int j = 0; j < col; j++) {
            if (grid[0][j] == 0) {
                q.push({0, j});
                grid[0][j] = 1; // 标记已访问（直接改grid，省空间）
            }
        }
        while (!q.empty()) {
            auto [x, y] = q.front(); q.pop();
            if (x == row - 1) return true;
            for (int d = 0; d < 4; d++) {
                int nx = x + dx[d], ny = y + dy[d];
                if (nx >= 0 && nx < row && ny >= 0 && ny < col 
                    && grid[nx][ny] == 0) {
                    grid[nx][ny] = 1; // 标记已访问
                    q.push({nx, ny});
                }
            }
        }
        return false;
    }
};
```

**关键点**：
- 二分的正确性依赖单调性：水只会越来越多，通路只会越来越难。
- check 函数中直接将 grid 当 visited 用（将访问过的陆地改为水），避免额外开 visited 数组。
- 二分范围：`lo=1, hi=row*col`，但答案最大不超过 `(row-1)*col`（至少要留一条纵向通路），不过用 `cells.size()` 更简洁。

---

### 解法3: 逆向并查集 — O(RC·α(RC)) / O(RC) — 加分项

**完全不同的视角**：正向是"删除陆地"，并查集不擅长。但如果**逆序**处理——从全是水的状态开始，逐天恢复陆地——就变成了"添加节点并合并"，正是并查集的强项。

当恢复某个格子后，顶行和底行第一次连通，那一天就是答案。

```
// 逆向过程示意 (3×3):
// 全水状态 → 恢复cells[8]=[3,1] → 恢复cells[7]=[3,2] → ... 
//
// 使用两个虚拟节点:
//   topVirtual (编号 row*col)    — 连接所有第一行的陆地
//   bottomVirtual (编号 row*col+1) — 连接所有最后一行的陆地
//
//   topVirtual
//   /   |   \
//  (0,0)(0,1)(0,2)   ← 第一行陆地恢复时连到 topVirtual
//       |
//      ...            ← 中间层陆地互相连
//       |
//  (2,0)(2,1)(2,2)   ← 最后一行陆地恢复时连到 bottomVirtual
//   \   |   /
//   bottomVirtual
//
// 当 find(topVirtual) == find(bottomVirtual) 时，上下连通！
```

```cpp
class Solution {
public:
    vector<int> parent, rank_;
    
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]); // 路径压缩
        return parent[x];
    }
    
    void unite(int a, int b) {
        int pa = find(a), pb = find(b);
        if (pa == pb) return;
        // 按秩合并
        if (rank_[pa] < rank_[pb]) swap(pa, pb);
        parent[pb] = pa;
        if (rank_[pa] == rank_[pb]) rank_[pa]++;
    }
    
    int latestDayToCross(int row, int col, vector<vector<int>>& cells) {
        int n = row * col;
        int topVirtual = n, bottomVirtual = n + 1;
        parent.resize(n + 2);
        rank_.resize(n + 2, 0);
        iota(parent.begin(), parent.end(), 0); // parent[i] = i
        
        // 初始状态：全是水
        vector<vector<int>> grid(row, vector<int>(col, 1));
        
        int dx[] = {0, 0, 1, -1};
        int dy[] = {1, -1, 0, 0};
        
        // 逆序恢复陆地
        for (int i = n - 1; i >= 0; i--) {
            int r = cells[i][0] - 1, c = cells[i][1] - 1; // 转0-based
            grid[r][c] = 0; // 恢复为陆地
            
            int idx = r * col + c; // 一维编号
            
            // 与四个方向的已有陆地合并
            for (int d = 0; d < 4; d++) {
                int nr = r + dx[d], nc = c + dy[d];
                if (nr >= 0 && nr < row && nc >= 0 && nc < col 
                    && grid[nr][nc] == 0) {
                    unite(idx, nr * col + nc);
                }
            }
            
            // 如果在第一行，连到 topVirtual
            if (r == 0) unite(idx, topVirtual);
            // 如果在最后一行，连到 bottomVirtual
            if (r == row - 1) unite(idx, bottomVirtual);
            
            // 检查上下是否连通
            if (find(topVirtual) == find(bottomVirtual)) {
                return i; // 第 i 天（0-indexed）就是答案
                // cells是1-indexed的天数，cells[i]对应第i+1天
                // 但答案是"最后一天能走通"，恢复cells[i]后能走通
                // 意味着第i天（淹没前）还是第i天？
                // 注意：cells[i]在第i+1天淹没(1-based天数)
                // 逆序恢复cells[i]意味着这格不被淹，
                // 即第i天(0-based) = 前i天淹没后的状态
                // 能连通说明前i天淹完后仍可穿越，答案是i
            }
        }
        return 0; // 不会到达这里（题目保证day=0全是陆地）
    }
};
```

**关键点**：
- 逆序遍历 `i` 从 `n-1` 到 `0`：恢复 `cells[i]` 意味着只有前 `i` 天的水。当连通时，说明前 `i` 天淹完后仍可穿越，答案就是 `i`。
- 虚拟节点避免了每次检查整行是否有节点连通的开销。
- 并查集的路径压缩+按秩合并使得每次操作接近 O(1)。

---

## 解法对比

| | 暴力 BFS | 二分 + BFS | 逆向并查集 |
|---|---|---|---|
| 时间 | O((RC)²) | O(RC·log(RC)) | O(RC·α(RC)) ≈ O(RC) |
| 空间 | O(RC) | O(RC) | O(RC) |
| 思维难度 | 低 | 中 | 高 |
| 面试推荐 | 说思路 | ⭐ 首选 | 加分 |
| 核心技巧 | 模拟 | 二分答案+连通性检查 | 逆向思维+Union-Find |

**什么时候选哪个？**
- 面试时间紧张 → 二分 + BFS，代码量适中，思路清晰
- 面试官追问更优 → 逆向并查集，展示对并查集的深入理解
- 并查集方案理论上更快（少一个 log 因子），但实际中 BFS 的常数也很小

## 易错点

1. **1-based 转 0-based 忘记减1**
   - ✗ `grid[cells[i][0]][cells[i][1]] = 1;` — 越界或位置错误
   - ✓ `grid[cells[i][0]-1][cells[i][1]-1] = 1;`

2. **二分答案的边界和返回值**
   - ✗ `return lo;` 或 `return mid;` — 循环结束时 mid 可能不是最终答案
   - ✓ 用独立变量 `ans` 记录最后一个满足条件的 mid，返回 `ans`

3. **BFS 中忘记标记起点为已访问**
   - ✗ 只在出队时标记 → 同一个起点被多次入队
   - ✓ 入队时立即标记 `grid[0][j] = 1;`

4. **并查集方案中，恢复后的天数计算错误**
   - `cells[i]` 对应第 `i+1` 天的淹没（1-based）。逆序恢复 `cells[i]` 后的状态等价于前 `i` 天淹没后的矩阵。所以连通时答案是 `i`，不是 `i+1` 也不是 `i-1`。

5. **并查集忘记连接虚拟节点**
   - 恢复的格子如果在第一行或最后一行，必须同时与虚拟节点合并，否则无法检测上下连通。

## 面试追问

**Q1（基础理解）**：暴力解的时间复杂度是什么？瓶颈在哪？
> 每天一次 BFS = O(RC)，共 RC 天，总 O((RC)²)。瓶颈在于每天独立检查连通性，没有利用"只改了一格"的增量信息。

**Q2（优化到二分）**：为什么能二分？check 函数怎么设计？
> 水只增不减，连通性单调递减，存在分界点。check(day)：前 day 天淹完后 BFS 检查是否连通。二分找最后一个 check=true 的 day。

**Q3（进一步优化）**：能否不用二分做到接近 O(RC)？
> 逆向并查集。把"删除陆地"反转成"添加陆地"，利用并查集增量合并。加上路径压缩和按秩合并，每次操作 O(α(n)) ≈ O(1)。

**Q4（变体）**：如果不是从上到下，而是从左到右呢？
> 改虚拟节点：leftVirtual 连接第一列，rightVirtual 连接最后一列。其他逻辑不变。

**Q5（变体）**：如果每天淹没的是随机一块，想查询任意天的连通性？
> 预处理：逆向并查集一遍，记录每天的连通状态。查询 O(1)。

## 相关题型

- **200. 岛屿数量** — BFS/DFS 在矩阵上的基础应用，本题的 check 函数就是岛屿连通性判断的变体
- **130. 被围绕的区域** — 同样可以用逆向思维（从边界 DFS）或并查集+虚拟节点
- **803. 打砖块** — 同样的"逆向并查集"套路：正向是删除（难），逆向是添加（易）。学会本题后可直接复用逆向并查集+虚拟节点的框架
- **1102. 得分最高的路径** — 二分+BFS 验证的经典应用，check 函数不同但框架一致
- **778. 水位上升的泳池中游泳** — 同样是二分答案+BFS，区别是阈值含义不同（水位 vs 天数）