// LeetCode 688: Knight Probability in Chessboard
// 解法一: 正向概率扩散 DP（滚动数组）
// 解法二: 记忆化递归 (Top-down)
// 时间: O(k * n^2), 空间: O(n^2) / O(k * n^2)

// ============================================================
// 核心思路:
// dp[i][j] = 骑士在当前步处于 (i,j) 的概率
// 初始: dp[row][col] = 1.0
// 每步: 将每个格子的概率 / 8 扩散到 8 个 L 形方向邻居
// 走出棋盘的概率自然消失，最终求所有格子概率之和
// ============================================================

// 解法一: 正向概率扩散 DP + 滚动数组
// 时间: O(k * n^2), 空间: O(n^2)
class Solution {
public:
    double knightProbability(int n, int k, int row, int column) {
        // 骑士的 8 个 L 形移动方向
        int dirs[8][2] = {
            {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
            {1, -2},  {1, 2},  {2, -1},  {2, 1}
        };

        // dp[i][j] = 当前步骑士处于 (i,j) 的概率
        vector<vector<double>> dp(n, vector<double>(n, 0.0));
        dp[row][column] = 1.0; // 初始位置概率为 1

        for (int step = 0; step < k; step++) {
            // 每步创建新的概率矩阵（必须清零）
            vector<vector<double>> ndp(n, vector<double>(n, 0.0));

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (dp[i][j] < 1e-15) continue; // 跳过零概率格子

                    // 将当前格子的概率均匀扩散到 8 个方向
                    double contribution = dp[i][j] / 8.0;
                    for (auto& d : dirs) {
                        int ni = i + d[0], nj = j + d[1];
                        // 只有在棋盘内才累加，走出棋盘的概率自然消失
                        if (ni >= 0 && ni < n && nj >= 0 && nj < n) {
                            ndp[ni][nj] += contribution;
                        }
                    }
                }
            }
            dp = move(ndp); // 滚动: 用新矩阵替代旧矩阵
        }

        // k 步后所有格子的概率之和 = 仍在棋盘上的概率
        double ans = 0.0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                ans += dp[i][j];
            }
        }
        return ans;
    }
};

// ============================================================
// 解法二: 记忆化递归 (Top-down DP)
// 时间: O(k * n^2), 空间: O(k * n^2)
//
// 定义 solve(steps, r, c) = 从 (r,c) 走 steps 步仍在棋盘上的概率
// base case: solve(0, r, c) = 1.0 (棋盘内)
// 转移: solve(steps, r, c) = (1/8) * sum(solve(steps-1, nr, nc))
//       其中 (nr, nc) 在棋盘内
// ============================================================

class Solution2 {
public:
    double knightProbability(int n, int k, int row, int column) {
        int dirs[8][2] = {
            {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
            {1, -2},  {1, 2},  {2, -1},  {2, 1}
        };

        // memo[step][i][j] = -1 表示未计算
        vector<vector<vector<double>>> memo(
            k + 1, vector<vector<double>>(n, vector<double>(n, -1.0))
        );

        // 递归函数: 从 (r,c) 走 steps 步仍在棋盘上的概率
        function<double(int, int, int)> solve = [&](int steps, int r, int c) -> double {
            // 出界: 概率为 0
            if (r < 0 || r >= n || c < 0 || c >= n) return 0.0;
            // 0 步: 在棋盘内, 概率为 1
            if (steps == 0) return 1.0;
            // 已缓存
            if (memo[steps][r][c] >= 0) return memo[steps][r][c];

            double prob = 0.0;
            for (auto& d : dirs) {
                prob += solve(steps - 1, r + d[0], c + d[1]);
            }
            prob /= 8.0;

            memo[steps][r][c] = prob;
            return prob;
        };

        return solve(k, row, column);
    }
};

// ============================================================
// 解法三: 反向收集 DP（面试变体写法）
// 时间: O(k * n^2), 空间: O(n^2)
//
// 与正向扩散等价，但每个格子从 8 个来源收集概率
// dp_new[i][j] = sum(dp_old[i-di][j-dj] / 8) for valid sources
// ============================================================

class Solution3 {
public:
    double knightProbability(int n, int k, int row, int column) {
        int dirs[8][2] = {
            {-2, -1}, {-2, 1}, {-1, -2}, {-1, 2},
            {1, -2},  {1, 2},  {2, -1},  {2, 1}
        };

        vector<vector<double>> dp(n, vector<double>(n, 0.0));
        dp[row][column] = 1.0;

        for (int step = 0; step < k; step++) {
            vector<vector<double>> ndp(n, vector<double>(n, 0.0));

            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    // (i,j) 在上一步可能从哪些位置跳来？
                    // 反向: 如果上一步在 (i-di, j-dj), 跳到 (i,j)
                    for (auto& d : dirs) {
                        int pi = i - d[0], pj = j - d[1]; // 来源位置
                        if (pi >= 0 && pi < n && pj >= 0 && pj < n) {
                            ndp[i][j] += dp[pi][pj] / 8.0;
                        }
                    }
                }
            }
            dp = move(ndp);
        }

        double ans = 0.0;
        for (int i = 0; i < n; i++)
            for (int j = 0; j < n; j++)
                ans += dp[i][j];
        return ans;
    }
};

// ============================================================
// 示例推演: n=3, k=2, row=0, column=0
//
// 初始 dp:
// [1.0, 0, 0]
// [ 0,  0, 0]
// [ 0,  0, 0]
//
// Step 1: 从 (0,0) 向 8 个方向扩散 1/8
//   有效方向: (0,0)+(1,2)=(1,2) ✓, (0,0)+(2,1)=(2,1) ✓
//   其余 6 个方向都出界
// dp after step 1:
// [0,     0,   0  ]
// [0,     0,   1/8]
// [0,   1/8,   0  ]
//
// Step 2: 从 (1,2) 和 (2,1) 各向 8 方向扩散 (1/8)/8 = 1/64
//   (1,2): 有效 → (0,0), (2,0) → 各 +1/64
//   (2,1): 有效 → (0,0), (0,2) → 各 +1/64
// dp after step 2:
// [2/64, 0, 1/64]
// [ 0,   0,  0  ]
// [1/64, 0,  0  ]
//
// 答案 = 2/64 + 1/64 + 1/64 = 4/64 = 0.0625
// ============================================================
