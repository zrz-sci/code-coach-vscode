# LCR 105. 岛屿的最大面积 (Max Area of Island)

## 难度：Medium | 标签：DFS, BFS, Union Find, Array, Matrix

同主站 695. Max Area of Island

---

## 题目描述

给定一个由 `0`（水）和 `1`（陆地）组成的二维网格。岛屿是水平或垂直方向上相邻的 `1` 的连通区域。网格边界外为水域。求所有岛屿中面积最大的，没有岛屿则返回 0。

**约束**：`1 <= m, n <= 50`，`grid[i][j]` 为 0 或 1。

---

## Hint 1：问题本质 -- 图的连通分量

把网格想象成一张无向图：
- 每个值为 1 的格子是一个**节点**
- 上下左右相邻的两个 1 之间有一条**边**
- 一个岛屿就是一个**连通分量**
- 岛屿面积 = 连通分量的节点数

问题转化为：**求最大连通分量的大小**。

```
网格:
  0 0 [1] 0 0
  0 [1][1] 0 0
  0 [1] 0  0 0

对应的图:
  (0,2)
    |
  (1,1)--(1,2)
    |
  (2,1)

这 4 个节点互相连通 → 面积 = 4
```

---

## Hint 2：遍历框架 -- 扫描 + 搜索

```
maxArea = 0
for 每个格子 (i, j):
    if grid[i][j] == 1:
        area = search(grid, i, j)   // DFS 或 BFS
        maxArea = max(maxArea, area)
return maxArea
```

每次遇到一个未访问的 1，启动搜索探索整个岛屿，得到面积后更新全局最大值。

**关键问题**：如何避免同一个格子被重复访问？

---

## Hint 3：沉岛法 -- 最简洁的标记方式

访问过的 1 直接改成 0（"沉入水中"）。这样后续扫描和搜索都不会再碰到它。

```
搜索过程:

初始:           搜索(1,1):     搜索完成:
  0 0 1 0 0     0 0 1 0 0      0 0 0 0 0
  0 1 1 0 0     0[0]0 0 0  →   0 0 0 0 0
  0 1 0 0 0     0 0 0 0 0      0 0 0 0 0
                  ↑ 沉岛
```

优点：不需要额外的 visited 数组，O(1) 额外空间（不算递归栈）。

---

## Hint 4：DFS 递归实现

DFS 函数返回以 `(i,j)` 为起点的连通区域面积：

```
dfs(grid, i, j):
    if 越界 or grid[i][j] == 0:
        return 0           // 水域或边界，面积为 0
    grid[i][j] = 0         // 沉岛
    return 1               // 当前格子
         + dfs(i+1, j)     // 下
         + dfs(i-1, j)     // 上
         + dfs(i, j+1)     // 右
         + dfs(i, j-1)     // 左
```

**递归展开示例**（简化网格）：

```
grid:
  1 1
  1 0

dfs(0,0):
  grid[0][0]=0, area=1
  + dfs(1,0):              // 下
      grid[1][0]=0, area=1
      + dfs(2,0)=0         // 越界
      + dfs(0,0)=0         // 已沉
      + dfs(1,1)=0         // 水
      + dfs(1,-1)=0        // 越界
      return 1
  + dfs(-1,0)=0            // 越界
  + dfs(0,1):              // 右
      grid[0][1]=0, area=1
      + dfs(1,1)=0
      + dfs(-1,1)=0
      + dfs(0,2)=0
      + dfs(0,0)=0         // 已沉
      return 1
  + dfs(0,-1)=0            // 越界
  return 1 + 1 + 0 + 1 + 0 = 3
```

---

## Hint 5：BFS 替代方案 + 四方向模板

DFS 在极端情况下（如 50x50 全是 1）递归深度可达 2500。如果担心栈溢出，用 BFS：

```cpp
int bfs(grid, startI, startJ) {
    queue<pair<int,int>> q;
    q.push({startI, startJ});
    grid[startI][startJ] = 0;  // 入队时标记!
    int area = 0;
    
    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};
    
    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        area++;
        for (int d = 0; d < 4; d++) {
            int nx = x + dx[d], ny = y + dy[d];
            if (合法 && grid[nx][ny] == 1) {
                grid[nx][ny] = 0;  // 入队时标记, 不是出队时!
                q.push({nx, ny});
            }
        }
    }
    return area;
}
```

**BFS 易错点**：必须在**入队时**就标记 `grid[nx][ny] = 0`。如果等到出队时才标记，同一格子会被多个邻居重复入队，导致面积算多、时间浪费。

---

## 完整代码（DFS 沉岛法）

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
        if (i < 0 || i >= m || j < 0 || j >= n || grid[i][j] == 0)
            return 0;
        grid[i][j] = 0;  // 沉岛
        return 1 + dfs(grid, i+1, j, m, n)
                 + dfs(grid, i-1, j, m, n)
                 + dfs(grid, i, j+1, m, n)
                 + dfs(grid, i, j-1, m, n);
    }
};
```

---

## 复杂度分析

| 维度 | DFS | BFS |
|------|-----|-----|
| 时间 | O(m*n) | O(m*n) |
| 空间 | O(m*n) 递归栈 | O(min(m,n)) 队列 |

每个格子最多被访问一次（访问后变成 0，不再被搜索）。

---

## 解法对比

| 解法 | 代码量 | 空间 | 适用场景 |
|------|--------|------|----------|
| DFS 沉岛法 | 最短 | O(m*n) 递归栈 | 面试首选 |
| BFS 沉岛法 | 中等 | O(min(m,n)) | 大网格避免栈溢出 |
| Union-Find | 最长 | O(m*n) | 动态添加陆地 (LC 305) |

---

## 面试追问

1. **不能修改原数组怎么办？** 用额外的 `visited` 二维数组标记，或者搜索完后恢复原值（回溯）。空间从 O(1) 变为 O(m*n)。

2. **DFS 栈溢出风险**：50x50 全是 1 时递归深度可达 2500。改用 BFS 或迭代 DFS（显式栈）。面试官可能追问递归深度限制。

3. **如果支持动态添加陆地（如 LC 305）**：DFS/BFS 每次都要重新搜索，Union-Find 可以 O(1) 合并，O(alpha(n)) 查询，更适合动态场景。

4. **八方向连通 vs 四方向连通**：本题只要求四方向（上下左右）。如果改成八方向（含对角线），只需把方向数组从 4 扩展到 8，逻辑不变。

---

## 关联题目

| 题目 | 关联 |
|------|------|
| 200. 岛屿数量 | 同框架，只计数不求面积 |
| 695. Max Area of Island | 本题原题 |
| 463. 岛屿的周长 | 同框架，但收集周长信息 |
| 827. 最大人工岛 | 翻转一个 0，合并相邻岛屿求最大 |

---

## 常见错误

1. **忘记边界检查**：DFS 入口必须先判断 `i, j` 是否合法。
2. **BFS 出队才标记**：导致同一格子重复入队，面积多算。
3. **八方向搜索**：题目只要求四方向（上下左右），不包含对角线。
4. **修改了原数组不可接受时**：用 visited 数组或搜索后恢复原值。

---

## 岛屿系列通用框架

```
for i in [0, m):
    for j in [0, n):
        if grid[i][j] == 1:
            info = search(grid, i, j)   // DFS/BFS
            更新答案(info)

不同题目的区别仅在于 search 内部收集什么信息:
  200 岛屿数量  → 不收集, 只计启动次数
  695 最大面积  → 收集面积, 取 max
  463 岛屿周长  → 收集边界边数
  827 最大人工岛 → 标记岛屿 ID + 面积, 枚举翻转
```
