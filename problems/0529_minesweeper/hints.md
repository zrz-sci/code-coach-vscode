# 529. 扫雷游戏 (Minesweeper)

**难度**: Medium | **标签**: DFS, BFS, Array, Matrix

## 核心思路

模拟扫雷游戏的点击操作：点击地雷直接结束，点击空白格根据周围地雷数决定是显示数字还是递归展开。本质是**矩阵上的 BFS/DFS 连通区域展开**。

## 思维链

1. 判断点击位置是地雷 `'M'` 还是空白 `'E'`
2. 若是地雷，标记为 `'X'`，直接返回
3. 若是空白，统计 8 个方向的相邻地雷数
4. 若地雷数 > 0，标记为对应数字字符
5. 若地雷数 == 0，标记为 `'B'`，对 8 个方向的 `'E'` 递归处理

## 解法概览

| 解法 | 时间复杂度 | 空间复杂度 | 推荐度 |
|------|-----------|-----------|--------|
| DFS 递归 | O(m*n) | O(m*n) | ⭐⭐⭐⭐⭐ |
| BFS 队列 | O(m*n) | O(m*n) | ⭐⭐⭐⭐ |

## 关键提示

1. 8 个方向遍历用方向数组 `dirs`，包含对角线
2. 只有当前格子周围地雷数为 0 时才递归展开邻居
3. 递归前先修改当前格子状态（避免重复访问），无需额外 visited 数组
4. 注意只对 `'E'`（未揭开的空白）做递归，已揭开的不处理

## 解法详解

### 解法一：DFS 递归（推荐）

```cpp
class Solution {
public:
    // 8个方向：上下左右 + 4个对角线
    int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
    
    vector<vector<char>> updateBoard(vector<vector<char>>& board, vector<int>& click) {
        int r = click[0], c = click[1];
        // 点击到地雷，游戏结束
        if (board[r][c] == 'M') {
            board[r][c] = 'X';
            return board;
        }
        dfs(board, r, c);
        return board;
    }
    
    void dfs(vector<vector<char>>& board, int r, int c) {
        int m = board.size(), n = board[0].size();
        // 统计周围地雷数
        int mineCount = 0;
        for (auto& d : dirs) {
            int nr = r + d[0], nc = c + d[1];
            if (nr >= 0 && nr < m && nc >= 0 && nc < n && board[nr][nc] == 'M') {
                mineCount++;
            }
        }
        if (mineCount > 0) {
            // 周围有地雷，标记数字
            board[r][c] = '0' + mineCount;
        } else {
            // 周围无地雷，标记为B并递归展开
            board[r][c] = 'B';
            for (auto& d : dirs) {
                int nr = r + d[0], nc = c + d[1];
                if (nr >= 0 && nr < m && nc >= 0 && nc < n && board[nr][nc] == 'E') {
                    dfs(board, nr, nc);
                }
            }
        }
    }
};
```

### 解法二：BFS 队列

```cpp
class Solution {
public:
    vector<vector<char>> updateBoard(vector<vector<char>>& board, vector<int>& click) {
        int m = board.size(), n = board[0].size();
        int r = click[0], c = click[1];
        if (board[r][c] == 'M') {
            board[r][c] = 'X';
            return board;
        }
        
        int dirs[8][2] = {{-1,-1},{-1,0},{-1,1},{0,-1},{0,1},{1,-1},{1,0},{1,1}};
        queue<pair<int,int>> q;
        q.push({r, c});
        board[r][c] = 'B'; // 先标记防止重复入队
        
        while (!q.empty()) {
            auto [cr, cc] = q.front(); q.pop();
            // 统计周围地雷数
            int mineCount = 0;
            for (auto& d : dirs) {
                int nr = cr + d[0], nc = cc + d[1];
                if (nr >= 0 && nr < m && nc >= 0 && nc < n && board[nr][nc] == 'M')
                    mineCount++;
            }
            if (mineCount > 0) {
                board[cr][cc] = '0' + mineCount;
            } else {
                // 无地雷，展开邻居
                for (auto& d : dirs) {
                    int nr = cr + d[0], nc = cc + d[1];
                    if (nr >= 0 && nr < m && nc >= 0 && nc < n && board[nr][nc] == 'E') {
                        board[nr][nc] = 'B'; // 先标记
                        q.push({nr, nc});
                    }
                }
            }
        }
        return board;
    }
};
```

## 易错点

| 错误写法 | 正确写法 |
|---------|---------|
| ✗ 只检查上下左右4个方向 | ✓ 必须检查8个方向（含对角线） |
| ✗ 递归时不检查 `board[nr][nc] == 'E'` | ✓ 只对未揭开的 `'E'` 格子递归 |
| ✗ BFS 中出队后才标记 visited | ✓ 入队前就标记，避免重复入队 |
| ✗ 数字转换写成 `mineCount + '0'` 漏了类型 | ✓ `'0' + mineCount` 确保 char 类型 |

## 面试追问

**Q1: DFS 和 BFS 哪个更适合这题？有什么区别？**

> 两者均可。DFS 代码更简洁，BFS 按层展开更接近真实扫雷行为。DFS 可能栈溢出（矩阵很大时），BFS 更安全。

**Q2: 如何支持多次点击操作？**

> 每次调用 updateBoard 时只处理一次点击。已经揭开的格子不会再次处理（不是 'E' 或 'M' 就跳过）。

**Q3: 如果要求返回本次点击揭开了多少个格子，怎么修改？**

> 在 DFS/BFS 中维护一个计数器，每次将 'E' 修改为其他状态时计数器 +1。

## 相关题型

- [200. 岛屿数量](https://leetcode.com/problems/number-of-islands/) - 矩阵 DFS/BFS
- [130. 被围绕的区域](https://leetcode.com/problems/surrounded-regions/) - 矩阵连通区域
- [733. 图像渲染](https://leetcode.com/problems/flood-fill/) - Flood Fill 算法
