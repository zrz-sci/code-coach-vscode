// ============================================================
// 576. Out of Boundary Paths
// 难度: Medium | 标签: Dynamic Programming
// ============================================================

// ============================================================
// 解法一: 正向 DP + 滚动数组 (推荐)
// 思路: dp[i][j] = 当前步数下球位于 (i,j) 的路径数
//       每步尝试四方向移动，越界则累加到结果
// 时间: O(maxMove * m * n) | 空间: O(m * n)
// ============================================================
class Solution {
public:
    int findPaths(int m, int n, int maxMove, int startRow, int startColumn) {
        const int MOD = 1e9 + 7;
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
                            result = (result + dp[i][j]) % MOD;
                        } else {
                            newDp[ni][nj] = (newDp[ni][nj] + dp[i][j]) % MOD;
                        }
                    }
                }
            }
            dp = newDp;
        }

        return (int)result;
    }
};

// ============================================================
// 解法二: 记忆化搜索 (DFS + memo)
// 思路: dfs(moves, i, j) = 从 (i,j) 出发，剩余 moves 步能出界的路径数
// 时间: O(maxMove * m * n) | 空间: O(maxMove * m * n)
// ============================================================
// class Solution {
// public:
//     int findPaths(int m, int n, int maxMove, int startRow, int startColumn) {
//         const int MOD = 1e9 + 7;
//         vector<vector<vector<int>>> memo(maxMove + 1,
//             vector<vector<int>>(m, vector<int>(n, -1)));
//
//         function<int(int, int, int)> dfs = [&](int moves, int i, int j) -> int {
//             if (i < 0 || i >= m || j < 0 || j >= n) return 1;
//             if (moves == 0) return 0;
//             if (memo[moves][i][j] != -1) return memo[moves][i][j];
//
//             long paths = 0;
//             int dirs[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
//             for (auto& d : dirs) {
//                 paths = (paths + dfs(moves - 1, i + d[0], j + d[1])) % MOD;
//             }
//             return memo[moves][i][j] = (int)paths;
//         };
//
//         return dfs(maxMove, startRow, startColumn);
//     }
// };

// ============================================================
// 关键知识点:
// 1. 正向 DP: 从起点扩散，每步检查是否越界
// 2. 滚动数组: 第 k+1 步只依赖第 k 步，O(m*n) 空间
// 3. 取模: 每次加法后立即 % MOD，防止溢出
// 4. "at most" maxMove: 任何一步出界都计入结果
// 5. 跳过零值优化: dp[i][j]==0 时直接 continue
// ============================================================
