// LeetCode 741. Cherry Pickup
// 解法一：三维 DP - 两人同时走
// Time: O(n^3), Space: O(n^3)
// 核心转化：一去一回 -> 两人同时从(0,0)走到(n-1,n-1)

class Solution {
public:
    int cherryPickup(vector<vector<int>>& grid) {
        int n = grid.size();
        // dp[t][r1][r2]: 走了 t 步，人1在第r1行，人2在第r2行的最大樱桃数
        // c1 = t - r1, c2 = t - r2
        vector<vector<vector<int>>> dp(
            2 * n - 1,
            vector<vector<int>>(n, vector<int>(n, INT_MIN))
        );
        dp[0][0][0] = grid[0][0];

        for (int t = 1; t < 2 * n - 1; t++) {
            // r 的有效范围
            int rMin = max(0, t - (n - 1));
            int rMax = min(t, n - 1);

            for (int r1 = rMin; r1 <= rMax; r1++) {
                int c1 = t - r1;
                if (c1 < 0 || c1 >= n) continue;
                if (grid[r1][c1] == -1) continue; // 荆棘

                for (int r2 = rMin; r2 <= rMax; r2++) {
                    int c2 = t - r2;
                    if (c2 < 0 || c2 >= n) continue;
                    if (grid[r2][c2] == -1) continue; // 荆棘

                    // 当前格子的樱桃
                    int cherries = grid[r1][c1];
                    if (r1 != r2) {
                        cherries += grid[r2][c2]; // 不同格子才加两次
                    }

                    // 从四个前驱状态转移
                    int best = INT_MIN;
                    for (int pr1 : {r1, r1 - 1}) {
                        for (int pr2 : {r2, r2 - 1}) {
                            if (pr1 >= 0 && pr2 >= 0 &&
                                dp[t - 1][pr1][pr2] != INT_MIN) {
                                best = max(best, dp[t - 1][pr1][pr2]);
                            }
                        }
                    }

                    if (best != INT_MIN) {
                        dp[t][r1][r2] = best + cherries;
                    }
                }
            }
        }

        return max(0, dp[2 * n - 2][n - 1][n - 1]);
    }
};


// 解法二：滚动数组空间优化
// Time: O(n^3), Space: O(n^2)
// 只保留上一步的 dp 状态

class Solution2 {
public:
    int cherryPickup(vector<vector<int>>& grid) {
        int n = grid.size();
        // 只用两层 dp：prev 和 curr
        vector<vector<int>> prev(n, vector<int>(n, INT_MIN));
        vector<vector<int>> curr(n, vector<int>(n, INT_MIN));
        prev[0][0] = grid[0][0];

        for (int t = 1; t < 2 * n - 1; t++) {
            int rMin = max(0, t - (n - 1));
            int rMax = min(t, n - 1);

            // 重置 curr
            for (auto& row : curr) fill(row.begin(), row.end(), INT_MIN);

            for (int r1 = rMin; r1 <= rMax; r1++) {
                int c1 = t - r1;
                if (c1 < 0 || c1 >= n || grid[r1][c1] == -1) continue;

                for (int r2 = rMin; r2 <= rMax; r2++) {
                    int c2 = t - r2;
                    if (c2 < 0 || c2 >= n || grid[r2][c2] == -1) continue;

                    int cherries = grid[r1][c1];
                    if (r1 != r2) cherries += grid[r2][c2];

                    int best = INT_MIN;
                    for (int pr1 : {r1, r1 - 1}) {
                        for (int pr2 : {r2, r2 - 1}) {
                            if (pr1 >= 0 && pr2 >= 0 &&
                                prev[pr1][pr2] != INT_MIN) {
                                best = max(best, prev[pr1][pr2]);
                            }
                        }
                    }

                    if (best != INT_MIN) {
                        curr[r1][r2] = best + cherries;
                    }
                }
            }

            swap(prev, curr);
        }

        return max(0, prev[n - 1][n - 1]);
    }
};


// 解法三：记忆化搜索（自顶向下）
// Time: O(n^3), Space: O(n^3)
// 更直观的递归写法

class Solution3 {
public:
    int n;
    vector<vector<int>>* gridPtr;
    // memo[r1][c1][r2]：从当前位置到终点的最大樱桃数
    int memo[51][51][51];

    int cherryPickup(vector<vector<int>>& grid) {
        n = grid.size();
        gridPtr = &grid;
        memset(memo, -1, sizeof(memo));
        return max(0, dfs(grid, 0, 0, 0));
    }

    // 两人分别在 (r1,c1) 和 (r2,c2)，c2 = r1+c1-r2
    int dfs(vector<vector<int>>& grid, int r1, int c1, int r2) {
        int c2 = r1 + c1 - r2;

        // 越界或遇到荆棘
        if (r1 >= n || c1 >= n || r2 >= n || c2 >= n) return INT_MIN;
        if (grid[r1][c1] == -1 || grid[r2][c2] == -1) return INT_MIN;

        // 到达终点
        if (r1 == n - 1 && c1 == n - 1) return grid[r1][c1];

        if (memo[r1][c1][r2] != -1) return memo[r1][c1][r2];

        // 当前格子的樱桃
        int cherries = grid[r1][c1];
        if (r1 != r2) cherries += grid[r2][c2];

        // 四种移动组合：(右右, 右下, 下右, 下下)
        int best = INT_MIN;
        int candidates[4] = {
            dfs(grid, r1, c1 + 1, r2),         // 人1右，人2右 (c2+1)
            dfs(grid, r1, c1 + 1, r2 + 1),     // 人1右，人2下
            dfs(grid, r1 + 1, c1, r2),          // 人1下，人2右
            dfs(grid, r1 + 1, c1, r2 + 1)       // 人1下，人2下
        };

        for (int cand : candidates) {
            best = max(best, cand);
        }

        memo[r1][c1][r2] = (best == INT_MIN) ? INT_MIN : best + cherries;
        return memo[r1][c1][r2];
    }
};
