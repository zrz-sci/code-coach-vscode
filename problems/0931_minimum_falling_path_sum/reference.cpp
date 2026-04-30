/*
 * LeetCode 931: Minimum Falling Path Sum (下降路径最小和)
 * 难度: Medium | 标签: Array, Dynamic Programming, Matrix
 *
 * 题目:
 *   给定 n x n 矩阵 matrix, 找从第一行到最后一行的下降路径的最小和。
 *   从 (row, col) 可以走到 (row+1, col-1), (row+1, col), (row+1, col+1)。
 *
 * 转移方程:
 *   dp[i][j] = matrix[i][j] + min(dp[i-1][j-1], dp[i-1][j], dp[i-1][j+1])
 *   初始: dp[0][j] = matrix[0][j]
 *   答案: min(dp[n-1][0..n-1])
 *
 * DP 填表 (matrix = [[2,1,3],[6,5,4],[7,8,9]]):
 *
 *   原始矩阵:    DP表:
 *    2  1  3      2   1   3    <- row 0 (初始化)
 *    6  5  4      7   6   5    <- row 1 (逐格计算)
 *    7  8  9     13  13  14    <- row 2 (逐格计算)
 *
 *   答案 = min(13, 13, 14) = 13
 *
 * 空间优化路径:
 *   二维 DP O(n^2) -> 一维滚动 O(n) -> 原地修改 O(1)
 *   关键洞察: dp[i][j] 只依赖第 i-1 行, 不依赖更早的行
 */


// ================================================================
// 解法一: 原地修改 -- O(n^2) 时间, O(1) 空间 [推荐]
// ================================================================
// 直接在 matrix 上累加, 最省空间。
//
// 为什么可以原地修改?
//   处理第 i 行时:
//     matrix[i-1][...] 已经是 dp 值 (上一轮累加过)
//     matrix[i][j]     还是原始值 (本轮尚未处理)
//   读的是第 i-1 行, 写的是第 i 行, 不冲突!
//
// 边界处理:
//   j == 0:   没有左上方 -> 只从 dp[i-1][0] 和 dp[i-1][1] 转移
//   j == n-1: 没有右上方 -> 只从 dp[i-1][n-2] 和 dp[i-1][n-1] 转移
class Solution {
public:
    int minFallingPathSum(vector<vector<int>>& matrix) {
        int n = matrix.size();

        // 从第 1 行开始, 逐行更新
        for (int i = 1; i < n; i++) {
            for (int j = 0; j < n; j++) {
                // 正上方一定存在
                int best = matrix[i - 1][j];
                // 左上方 (j > 0 时存在)
                if (j > 0)
                    best = min(best, matrix[i - 1][j - 1]);
                // 右上方 (j < n-1 时存在)
                if (j < n - 1)
                    best = min(best, matrix[i - 1][j + 1]);
                // 累加到当前格
                matrix[i][j] += best;
            }
        }

        // 答案: 最后一行的最小值 (不是 dp[n-1][n-1]!)
        return *min_element(matrix[n - 1].begin(), matrix[n - 1].end());
    }
};


// ================================================================
// 解法二: 一维滚动数组 -- O(n^2) 时间, O(n) 空间
// ================================================================
// 不修改原矩阵, 只保留上一行的 DP 值。
//
// 为什么必须用两个数组 (prev + curr)?
//   如果只用一个数组从左到右更新:
//     更新 dp[0] 后, dp[0] 变成新值
//     计算 dp[1] 时需要 prev[0] (旧值), 但已被覆盖!
//   所以必须分开: prev 存旧值, curr 存新值, 处理完后滚动。
class Solution2 {
public:
    int minFallingPathSum(vector<vector<int>>& matrix) {
        int n = matrix.size();

        // prev: 上一行的 dp 值
        vector<int> prev(matrix[0].begin(), matrix[0].end());

        for (int i = 1; i < n; i++) {
            // curr: 当前行的 dp 值 (新开数组)
            vector<int> curr(n);
            for (int j = 0; j < n; j++) {
                int best = prev[j];                           // 正上方
                if (j > 0)     best = min(best, prev[j - 1]); // 左上方
                if (j < n - 1) best = min(best, prev[j + 1]); // 右上方
                curr[j] = matrix[i][j] + best;
            }
            // 滚动: 当前行变成下一轮的 "上一行"
            prev = move(curr);
        }

        return *min_element(prev.begin(), prev.end());
    }
};


// ================================================================
// 解法三: 记忆化递归 (Top-Down) -- O(n^2) 时间, O(n^2) 空间
// ================================================================
// 自顶向下思考: "从 (i,j) 出发到最后一行的最小路径和"
// 递归 + memo, 适合递归思维
class Solution3 {
public:
    int minFallingPathSum(vector<vector<int>>& matrix) {
        int n = matrix.size();
        // memo[i][j] = 从 (i,j) 出发到底部的最小和
        vector<vector<int>> memo(n, vector<int>(n, INT_MAX));

        int ans = INT_MAX;
        // 从第一行每个位置出发, 取最小
        for (int j = 0; j < n; j++) {
            ans = min(ans, dfs(matrix, memo, 0, j));
        }
        return ans;
    }

private:
    int dfs(vector<vector<int>>& matrix, vector<vector<int>>& memo,
            int i, int j) {
        int n = matrix.size();
        if (j < 0 || j >= n) return INT_MAX;     // 越界
        if (i == n - 1) return matrix[i][j];      // 到达最后一行
        if (memo[i][j] != INT_MAX) return memo[i][j]; // 已计算过

        // 向三个方向递归
        int down      = dfs(matrix, memo, i + 1, j);
        int downLeft  = dfs(matrix, memo, i + 1, j - 1);
        int downRight = dfs(matrix, memo, i + 1, j + 1);

        memo[i][j] = matrix[i][j] + min({down, downLeft, downRight});
        return memo[i][j];
    }
};


// ================================================================
// 解法四: 二维 DP (显式 dp 数组) -- O(n^2) 时间, O(n^2) 空间
// ================================================================
// 最直观的写法, 新建 dp 数组, 不修改原矩阵
class Solution4 {
public:
    int minFallingPathSum(vector<vector<int>>& matrix) {
        int n = matrix.size();
        vector<vector<int>> dp(n, vector<int>(n, 0));

        // 初始化第 0 行
        for (int j = 0; j < n; j++) {
            dp[0][j] = matrix[0][j];
        }

        // 逐行填表
        for (int i = 1; i < n; i++) {
            for (int j = 0; j < n; j++) {
                dp[i][j] = dp[i - 1][j];                          // 正上方
                if (j > 0)
                    dp[i][j] = min(dp[i][j], dp[i - 1][j - 1]);   // 左上方
                if (j < n - 1)
                    dp[i][j] = min(dp[i][j], dp[i - 1][j + 1]);   // 右上方
                dp[i][j] += matrix[i][j];                          // 加当前格
            }
        }

        return *min_element(dp[n - 1].begin(), dp[n - 1].end());
    }
};


/*
 * ================================================================
 * 复杂度分析:
 *
 * | 解法             | 时间     | 空间     | 是否修改输入 |
 * |-----------------|---------|---------|------------|
 * | 原地修改         | O(n^2)  | O(1)    | 是          |
 * | 一维滚动数组      | O(n^2)  | O(n)    | 否          |
 * | 记忆化递归        | O(n^2)  | O(n^2)  | 否          |
 * | 二维 DP 数组     | O(n^2)  | O(n^2)  | 否          |
 *
 * ================================================================
 * 空间优化思路:
 *
 *   二维 DP O(n^2) -> 一维滚动 O(n) -> 原地修改 O(1)
 *
 *   核心洞察: dp[i][j] 只依赖第 i-1 行
 *
 *   一维滚动: prev[] 和 curr[] 两个数组交替
 *     - prev 保存上一行的 dp 值 (读取用)
 *     - curr 保存当前行的 dp 值 (写入用)
 *     - 处理完一行后 prev = curr
 *
 *   原地修改: 直接把 matrix 当 dp 数组
 *     - 读第 i-1 行 (已完成), 写第 i 行 (正在处理), 不冲突
 *
 * ================================================================
 * 易错点:
 *
 * 1. 边界: j=0 没有左上, j=n-1 没有右上
 * 2. 答案是最后一行的 min, 不是 dp[n-1][n-1]
 * 3. 元素可为负数, 不能用 0 做初始值
 * 4. 滚动数组必须用两个数组交替, 一个会覆盖旧值
 * 5. n=1 时循环不执行, 直接返回 matrix[0][0]
 * ================================================================
 * 面试追问:
 *
 * Q1: 如果不允许修改原矩阵?
 *     用一维滚动数组, O(n) 空间。prev 和 curr 两个数组交替。
 *
 * Q2: 如果要输出具体路径?
 *     额外记录 choice[i][j] 表示 (i,j) 从哪个 (i-1,?) 转移,
 *     最后从末行最小值位置向上回溯。
 *
 * Q3: 如果矩阵不是方阵 (m x n)?
 *     转移方程不变, 外层循环 m 行, 内层循环 n 列。
 *
 * Q4: 为什么不能贪心?
 *     每步选择限制下一步范围, 局部最优不等于全局最优, 需要 DP。
 * ================================================================
 */
