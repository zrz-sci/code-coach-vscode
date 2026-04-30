# 576. Out of Boundary Paths - 出界的路径数

## 核心思路

在 m x n 网格中，球从起点出发，最多移动 maxMove 步，问有多少条路径能让球移出边界。这是一道经典的 **三维 DP** 题，状态为 `dp[步数][行][列]`，每步向四方向扩散，越界则累加到答案。可用滚动数组优化空间。

## 思维链

1. **状态定义**：dp[k][i][j] = 经过 k 步后球恰好位于 (i,j) 的路径数
2. **初始状态**：dp[0][startRow][startColumn] = 1
3. **状态转移**：对于 dp[k][i][j] 中的每条路径，尝试四个方向
   - 如果新位置越界：result += dp[k][i][j]（成功出界）
   - 如果新位置合法：dp[k+1][ni][nj] += dp[k][i][j]
4. **答案累加**：每一步的越界路径都计入最终结果（最多 maxMove 步，不是恰好）
5. **取模**：结果对 10^9+7 取模，注意中间计算也要取模防溢出
6. **空间优化**：第 k+1 步只依赖第 k 步，滚动数组优化为 O(m*n)

## 解法概览

| 解法 | 时间 | 空间 | 适用场景 | 推荐 |
|------|------|------|----------|------|
| 正向 DP + 滚动数组 | O(maxMove*m*n) | O(m*n) | 面试首选，直觉清晰 | ⭐ |
| 记忆化搜索 (DFS + memo) | O(maxMove*m*n) | O(maxMove*m*n) | 递归思维更自然 | |
| 三维 DP 不优化 | O(maxMove*m*n) | O(maxMove*m*n) | 最直白但空间大 | |

## 关键提示

- **正向 DP vs 反向 DP**：正向（从起点往外扩散）比反向（从边界往内收拢）更直观
- **为什么不是恰好 maxMove 步**：题目说"at most maxMove moves"，所以 0~maxMove 步内任何时刻出界都算
- **取模时机**：每次加法后立即取模，避免 long long 溢出
- **方向数组**：dirs = {{-1,0},{1,0},{0,-1},{0,1}}，标准四方向写法
- maxMove=0 时如果起点就在边界也不算出界（因为没有移动）

## 解法详解

### 解法一：正向 DP + 滚动数组（推荐）

```cpp
class Solution {
public:
    int findPaths(int m, int n, int maxMove, int startRow, int startColumn) {
        const int MOD = 1e9 + 7;
        // dp[i][j] = 当前步数下球位于 (i,j) 的路径数
        vector<vector<long>> dp(m, vector<long>(n, 0));
        dp[startRow][startColumn] = 1;
        
        int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
        long result = 0;
        
        for (int move = 0; move < maxMove; ++move) {
            vector<vector<long>> newDp(m, vector<long>(n, 0));
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (dp[i][j] == 0) continue;
                    for (auto& d : dirs) {
                        int ni = i + d[0], nj = j + d[1];
                        if (ni < 0 || ni >= m || nj < 0 || nj >= n) {
                            // 越界，累加到结果
                            result = (result + dp[i][j]) % MOD;
                        } else {
                            // 未越界，传递到新位置
                            newDp[ni][nj] = (newDp[ni][nj] + dp[i][j]) % MOD;
                        }
                    }
                }
            }
            dp = move(newDp);
        }
        return (int)result;
    }
};
```

**复杂度分析**：
- 时间 O(maxMove * m * n)：外层 maxMove 次循环，内层遍历 m*n 格子
- 空间 O(m * n)：滚动数组只保留当前一层

### 解法二：记忆化搜索

```cpp
class Solution {
public:
    int findPaths(int m, int n, int maxMove, int startRow, int startColumn) {
        const int MOD = 1e9 + 7;
        // memo[moves][i][j] = 从 (i,j) 出发，剩余 moves 步能出界的路径数
        vector<vector<vector<int>>> memo(maxMove + 1,
            vector<vector<int>>(m, vector<int>(n, -1)));
        
        function<int(int, int, int)> dfs = [&](int moves, int i, int j) -> int {
            // 越界 = 找到一条出界路径
            if (i < 0 || i >= m || j < 0 || j >= n) return 1;
            // 步数用完还没出界
            if (moves == 0) return 0;
            if (memo[moves][i][j] != -1) return memo[moves][i][j];
            
            long paths = 0;
            int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
            for (auto& d : dirs) {
                paths = (paths + dfs(moves - 1, i + d[0], j + d[1])) % MOD;
            }
            return memo[moves][i][j] = (int)paths;
        };
        
        return dfs(maxMove, startRow, startColumn);
    }
};
```

**复杂度分析**：
- 时间 O(maxMove * m * n)：每个状态最多计算一次
- 空间 O(maxMove * m * n)：memo 三维数组 + 递归栈

### 解法三：三维 DP 不优化（最直白）

```cpp
class Solution {
public:
    int findPaths(int m, int n, int maxMove, int startRow, int startColumn) {
        const int MOD = 1e9 + 7;
        // dp[k][i][j] = 用 k 步到达 (i,j) 的路径数
        vector<vector<vector<long>>> dp(maxMove + 1,
            vector<vector<long>>(m, vector<long>(n, 0)));
        dp[0][startRow][startColumn] = 1;
        
        int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
        long result = 0;
        
        for (int k = 0; k < maxMove; ++k) {
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (dp[k][i][j] == 0) continue;
                    for (auto& d : dirs) {
                        int ni = i + d[0], nj = j + d[1];
                        if (ni < 0 || ni >= m || nj < 0 || nj >= n) {
                            result = (result + dp[k][i][j]) % MOD;
                        } else {
                            dp[k+1][ni][nj] = (dp[k+1][ni][nj] + dp[k][i][j]) % MOD;
                        }
                    }
                }
            }
        }
        return (int)result;
    }
};
```

## 易错点

| 错误写法 | 正确写法 | 说明 |
|----------|----------|------|
| ✗ 只统计恰好 maxMove 步出界的路径 | ✓ 每一步出界都累加到 result | "at most"不是"exactly" |
| ✗ 忘记对中间加法取模 | ✓ 每次 += 后立即 % MOD | long long 也可能溢出 |
| ✗ `int dirs[] = {0,1,0,-1,0}` 的索引用错 | ✓ 用 4x2 数组或 pair 更清晰 | 方向数组写法要一致 |
| ✗ newDp 没有重新初始化为 0 | ✓ 每一步新建全零的 newDp | 滚动数组必须重置 |
| ✗ dp[i][j] 跳过检查直接计算 | ✓ `if (dp[i][j] == 0) continue;` | 大量格子值为 0，跳过可显著加速 |

## 面试追问

**Q1：如果网格中有些格子是障碍物（不可通过），怎么修改？**

在状态转移时额外检查目标格子是否为障碍物。如果是障碍物则跳过（不往那个方向移动），其余逻辑不变。初始化时如果起点是障碍物则直接返回 0。

**Q2：如果只统计"恰好 maxMove 步出界"的路径数，怎么改？**

不在每一步累加 result，而是只在第 maxMove 步的越界时累加。或者用记忆化搜索，base case 改为 moves==0 时检查是否越界。

**Q3：此题能否用 BFS 解？时间复杂度一样吗？**

可以用 BFS 按层扩展，每层代表一步。实际上正向 DP 本质就是 BFS 的全量扩展。时间复杂度相同 O(maxMove*m*n)。但 BFS 的队列可能存储大量重复坐标（因为同一格可以被多条路径到达），不如 DP 数组高效。DP 数组天然合并了到达同一格的路径数。

## 相关题型

| 题号 | 题目 | 关联点 |
|------|------|--------|
| 62 | Unique Paths | 网格 DP 基础，从起点到终点的路径数 |
| 63 | Unique Paths II | 带障碍物的网格 DP |
| 64 | Minimum Path Sum | 网格 DP + 最小路径和 |
| 688 | Knight Probability in Chessboard | 极相似：棋盘上移动后留在棋盘内的概率 |
| 935 | Knight Dialer | 类似 DP + 方向跳跃 + 取模 |
