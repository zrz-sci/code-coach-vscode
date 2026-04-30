# 542. 01 矩阵

## 核心思路

求每个格子到最近 0 的距离。关键洞察：**从所有 0 出发做多源 BFS**，层层扩展到 1 的位置，第几层就是距离几。也可用 DP 两遍扫描（左上→右下 + 右下→左上）。

## 思维链

1. 如果从每个 1 出发找最近 0 → 暴力 BFS，太慢
2. 逆向思维：从所有 0 **同时**出发，向外扩展
3. 多源 BFS：把所有 0 加入初始队列，1 设为 INT_MAX
4. BFS 层序扩展，每到一个新格子，距离 = 前一格 + 1
5. DP 方案：两遍扫描，第一遍从左上累积，第二遍从右下累积

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐 |
|------|-----------|-----------|------|
| 多源 BFS | O(m*n) | O(m*n) | ⭐⭐⭐ |
| 动态规划（两遍扫描） | O(m*n) | O(1) 原地 | ⭐⭐⭐ |

## 关键提示

1. **多源 BFS**：所有 0 同时入队，不是从 1 出发找 0
2. DP 两遍扫描：第一遍只看左和上，第二遍只看右和下
3. 初始化：0 的距离为 0，1 的距离设为 INT_MAX（或 m+n）
4. BFS 天然保证最短距离（层级 = 距离）

## 解法详解

### 解法一：多源 BFS（推荐）

```cpp
class Solution {
public:
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        int m = mat.size(), n = mat[0].size();
        vector<vector<int>> dist(m, vector<int>(n, INT_MAX));
        queue<pair<int, int>> q;

        // 所有 0 入队
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (mat[i][j] == 0) {
                    dist[i][j] = 0;
                    q.push({i, j});
                }
            }
        }

        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        while (!q.empty()) {
            auto [x, y] = q.front();
            q.pop();
            for (auto& d : dirs) {
                int nx = x + d[0], ny = y + d[1];
                if (nx >= 0 && nx < m && ny >= 0 && ny < n
                    && dist[nx][ny] > dist[x][y] + 1) {
                    dist[nx][ny] = dist[x][y] + 1;
                    q.push({nx, ny});
                }
            }
        }
        return dist;
    }
};
```

### 解法二：动态规划（两遍扫描）

```cpp
class Solution {
public:
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        int m = mat.size(), n = mat[0].size();
        vector<vector<int>> dist(m, vector<int>(n, m + n)); // 最大可能距离

        // 第一遍：从左上到右下，看 上方 和 左方
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (mat[i][j] == 0) {
                    dist[i][j] = 0;
                } else {
                    if (i > 0) dist[i][j] = min(dist[i][j], dist[i-1][j] + 1);
                    if (j > 0) dist[i][j] = min(dist[i][j], dist[i][j-1] + 1);
                }
            }
        }

        // 第二遍：从右下到左上，看 下方 和 右方
        for (int i = m - 1; i >= 0; i--) {
            for (int j = n - 1; j >= 0; j--) {
                if (i < m - 1) dist[i][j] = min(dist[i][j], dist[i+1][j] + 1);
                if (j < n - 1) dist[i][j] = min(dist[i][j], dist[i][j+1] + 1);
            }
        }
        return dist;
    }
};
```

## 易错点

| ✗ 错误写法 | ✓ 正确写法 | 说明 |
|-----------|-----------|------|
| 从每个 1 出发 BFS 找 0 | 从所有 0 同时出发 BFS | 前者 O(m^2*n^2)，后者 O(m*n) |
| DP 只做一遍扫描 | 必须做两遍（左上→右下 + 右下→左上） | 一遍只能覆盖两个方向 |
| 初始距离设为 0 | 1 的初始距离设为 INT_MAX 或 m+n | 否则 min 比较失效 |
| BFS 中不检查 `dist[nx][ny] > dist[x][y]+1` | 必须检查避免重复入队 | 只有更优时才更新 |

## 面试追问

**Q1: 多源 BFS 和单源 BFS 的区别？**
→ 多源 BFS 初始队列有多个起点，等价于添加一个虚拟超级源点连接所有 0。

**Q2: DP 方案为什么需要两遍？一遍行不行？**
→ 一遍只能从两个方向累积（左/上），右下方向的信息还未计算。两遍覆盖四个方向。

**Q3: 这题和 LeetCode 1765 (Map of Highest Peak) 有什么关系？**
→ 本质相同，1765 是从水域（0）出发求距离，只是题面不同。

## 相关题型

- [LeetCode 1765. Map of Highest Peak](https://leetcode.com/problems/map-of-highest-peak/) - 相同题目
- [LeetCode 286. Walls and Gates](https://leetcode.com/problems/walls-and-gates/) - 多源 BFS
- [LeetCode 994. Rotting Oranges](https://leetcode.com/problems/rotting-oranges/) - 多源 BFS 经典
- [LeetCode 1162. As Far from Land as Possible](https://leetcode.com/problems/as-far-from-land-as-possible/) - 多源 BFS
