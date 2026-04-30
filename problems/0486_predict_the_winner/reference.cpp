/*
 * 486. Predict the Winner
 * 难度: Medium | 标签: Recursion, Array, Math, Dynamic Programming, Game Theory
 *
 * 【题目本质】
 * 双人零和博弈: 两人轮流从数组两端取数, 先手是否能赢(含平局)。
 * 核心: 区间 DP, dp[i][j] = 当前玩家面对 nums[i..j] 时的最大净分差。
 *
 * 【解法总览】
 * Solution1: 区间DP 2D        O(n^2)  O(n^2)  ⭐ 经典博弈DP
 * Solution2: 空间优化1D DP     O(n^2)  O(n)    面试加分
 * Solution3: 记忆化递归        O(n^2)  O(n^2)  直觉最清晰
 */

// ===================== Solution1: 区间 DP (2D) =====================
// dp[i][j] = 面对 nums[i..j] 时当前玩家能获得的最大净分差
// 转移: dp[i][j] = max(nums[i] - dp[i+1][j], nums[j] - dp[i][j-1])
class Solution1 {
public:
    bool predictTheWinner(vector<int>& nums) {
        int n = nums.size();
        vector<vector<int>> dp(n, vector<int>(n, 0));

        // 基础: 区间长度为 1
        for (int i = 0; i < n; i++) {
            dp[i][i] = nums[i];
        }

        // 按区间长度递增填表
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;
                int pickLeft  = nums[i] - dp[i + 1][j];
                int pickRight = nums[j] - dp[i][j - 1];
                dp[i][j] = max(pickLeft, pickRight);
            }
        }

        // 玩家1先手, 净差 >= 0 即为赢 (含平局)
        return dp[0][n - 1] >= 0;
    }
};

// ===================== Solution2: 空间优化 1D =====================
// 观察: 填表时 dp[i][j] 依赖 dp[i+1][j] 和 dp[i][j-1]
// 按列填充, dp[i] 复用, 只需一维数组
class Solution2 {
public:
    bool predictTheWinner(vector<int>& nums) {
        int n = nums.size();
        vector<int> dp(n);

        // 初始: dp[i] = dp_2d[i][i] = nums[i]
        for (int i = 0; i < n; i++) {
            dp[i] = nums[i];
        }

        // 每轮扩展区间长度
        for (int len = 2; len <= n; len++) {
            for (int i = 0; i <= n - len; i++) {
                int j = i + len - 1;
                // dp[i+1] = dp_2d[i+1][j] (本轮已更新)
                // dp[i]   = dp_2d[i][j-1] (上轮值)
                dp[i] = max(nums[i] - dp[i + 1],
                            nums[j] - dp[i]);
            }
        }

        return dp[0] >= 0;
    }
};

// ===================== Solution3: 记忆化递归 =====================
// 自顶向下, 直观表达 Minimax 思想
class Solution3 {
public:
    bool predictTheWinner(vector<int>& nums) {
        int n = nums.size();
        vector<vector<int>> memo(n, vector<int>(n, INT_MIN));
        return solve(nums, 0, n - 1, memo) >= 0;
    }

private:
    int solve(vector<int>& nums, int i, int j, vector<vector<int>>& memo) {
        if (i == j) return nums[i];
        if (memo[i][j] != INT_MIN) return memo[i][j];

        int pickL = nums[i] - solve(nums, i + 1, j, memo);
        int pickR = nums[j] - solve(nums, i, j - 1, memo);

        memo[i][j] = max(pickL, pickR);
        return memo[i][j];
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 |
 * |------|------|------|------|
 * | Solution1 2D DP | O(n^2) | O(n^2) | 经典标准, 面试首选 |
 * | Solution2 1D DP | O(n^2) | O(n) | 空间优化, 展示功底 |
 * | Solution3 记忆化 | O(n^2) | O(n^2) | 最直观, 易于理解 |
 *
 * 【易错点】
 * 1. 净分差的减号: nums[i] - dp[i+1][j], 不是加号
 *    选了 nums[i] 后对手成为当前玩家, 对手的净差对我是负的
 * 2. 平局判断: >= 0 不是 > 0, 题目说平局玩家1也算赢
 * 3. 记忆化初始值: 用 INT_MIN 而非 0, 因为 0 可能是合法答案
 * 4. 区间遍历顺序: 按长度从小到大, 保证子问题已求解
 * 5. 1D优化时的依赖: dp[i] 同时代表两个角色的值, 需理解清楚
 *
 * 【面试追问】
 * Q1: 零和博弈中"净分差"的含义?
 *     total = sum(nums) 固定, player1 得分 p1, player2 得分 p2 = total - p1。
 *     净差 = p1 - p2 = 2*p1 - total。所以净差 >= 0 等价于 p1 >= total/2。
 * Q2: n 很大时(比如 n=10000)还能用这个方法吗?
 *     O(n^2) 时间和空间, n=10000 约 10^8 可能超时。
 *     但本题 n <= 20, 远不会超时。
 * Q3: 如果每次可以取任意位置的数(不限于两端)?
 *     变成NP问题, 不能用区间DP, 需要状态压缩(2^n)。
 */
