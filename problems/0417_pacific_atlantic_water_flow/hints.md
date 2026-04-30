# 417. Pacific Atlantic Water Flow (太平洋大西洋水流问题)

## 核心思路
正向思考（从每个点看能否流到两洋）太慢。**逆向思维**：从海洋边界出发反向 BFS/DFS，标记能"逆流"到达的所有格子，最终取两个海洋可达集合的**交集**。

## 思维链
1. 暴力：从每个格子 DFS 看能否到达两个海洋 → O(m^2 * n^2) 太慢
2. 逆向：从太平洋边界（上+左）反向搜索，标记所有可达格子 → pacific 集合
3. 从大西洋边界（下+右）反向搜索，标记所有可达格子 → atlantic 集合
4. 反向搜索时条件反转：邻居高度 >= 当前高度才能走（水往高处"逆流"）
5. 结果 = pacific ∩ atlantic

## 解法概览表

| 解法 | 时间 | 空间 | 特点 |
|------|------|------|------|
| ⭐ 反向 BFS | O(m*n) | O(m*n) | 直观，面试首选 |
| 反向 DFS | O(m*n) | O(m*n) | 递归实现，代码更短 |

## 关键提示
1. **逆向搜索**：从海洋边界出发，方向是"逆流而上"（邻居 >= 当前）
2. 太平洋边界 = 第0行 + 第0列；大西洋边界 = 最后一行 + 最后一列
3. 需要两个 visited 矩阵，分别记录两个海洋的可达性
4. 最后遍历所有格子，找两个矩阵都为 true 的位置

## 解法详解

### 解法一：反向 BFS（推荐）
```cpp
class Solution {
public:
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
        int m = heights.size(), n = heights[0].size();
        vector<vector<bool>> pacific(m, vector<bool>(n, false));
        vector<vector<bool>> atlantic(m, vector<bool>(n, false));
        queue<pair<int,int>> pq, aq;
        
        for (int i = 0; i < m; i++) {
            pq.push({i, 0}); pacific[i][0] = true;
            aq.push({i, n-1}); atlantic[i][n-1] = true;
        }
        for (int j = 0; j < n; j++) {
            pq.push({0, j}); pacific[0][j] = true;
            aq.push({m-1, j}); atlantic[m-1][j] = true;
        }
        
        auto bfs = [&](queue<pair<int,int>>& q, vector<vector<bool>>& visited) {
            int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
            while (!q.empty()) {
                auto [r, c] = q.front(); q.pop();
                for (auto& d : dirs) {
                    int nr = r + d[0], nc = c + d[1];
                    if (nr >= 0 && nr < m && nc >= 0 && nc < n &&
                        !visited[nr][nc] && heights[nr][nc] >= heights[r][c]) {
                        visited[nr][nc] = true;
                        q.push({nr, nc});
                    }
                }
            }
        };
        
        bfs(pq, pacific);
        bfs(aq, atlantic);
        
        vector<vector<int>> res;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (pacific[i][j] && atlantic[i][j])
                    res.push_back({i, j});
        return res;
    }
};
```

### 解法二：反向 DFS
```cpp
class Solution {
public:
    int m, n;
    void dfs(vector<vector<int>>& heights, vector<vector<bool>>& visited, int r, int c) {
        visited[r][c] = true;
        int dirs[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};
        for (auto& d : dirs) {
            int nr = r + d[0], nc = c + d[1];
            if (nr >= 0 && nr < m && nc >= 0 && nc < n &&
                !visited[nr][nc] && heights[nr][nc] >= heights[r][c]) {
                dfs(heights, visited, nr, nc);
            }
        }
    }
    
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
        m = heights.size(); n = heights[0].size();
        vector<vector<bool>> pacific(m, vector<bool>(n, false));
        vector<vector<bool>> atlantic(m, vector<bool>(n, false));
        
        for (int i = 0; i < m; i++) {
            dfs(heights, pacific, i, 0);
            dfs(heights, atlantic, i, n-1);
        }
        for (int j = 0; j < n; j++) {
            dfs(heights, pacific, 0, j);
            dfs(heights, atlantic, m-1, j);
        }
        
        vector<vector<int>> res;
        for (int i = 0; i < m; i++)
            for (int j = 0; j < n; j++)
                if (pacific[i][j] && atlantic[i][j])
                    res.push_back({i, j});
        return res;
    }
};
```

## 易错点
- ✗ 正向搜索（从每个点出发）→ ✓ 逆向搜索（从边界出发），复杂度从 O(m^2n^2) 降到 O(mn)
- ✗ 反向搜索时用 `<=` → ✓ 应该用 `>=`（逆流而上，邻居要更高或等高）
- ✗ 初始化边界时漏掉角点 → ✓ 角点属于两个海洋，两次循环都会覆盖

## 面试追问
**Q1: 为什么反向搜索更优？**
A: 正向从每个点搜索到两个海洋是 O(m^2*n^2)。反向只需从边界搜两次，每个格子最多访问两次，O(m*n)。

**Q2: BFS vs DFS 选哪个？**
A: BFS 不会栈溢出，适合大矩阵（200x200）。DFS 代码更短但递归深度可达 40000。面试中 BFS 更安全。

**Q3: 能否只用一个 visited 矩阵？**
A: 可以用位标记（bit 0 = pacific, bit 1 = atlantic），结果就是值为 3 的格子。但代码可读性降低。

## 相关题型
- [130. Surrounded Regions](https://leetcode.com/problems/surrounded-regions/) - 类似的边界反向搜索
- [200. Number of Islands](https://leetcode.com/problems/number-of-islands/) - 网格 BFS/DFS 基础
- [329. Longest Increasing Path in a Matrix](https://leetcode.com/problems/longest-increasing-path-in-a-matrix/) - 矩阵中的搜索 + 记忆化
