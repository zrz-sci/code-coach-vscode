/*
 * 375. Guess Number Higher or Lower II
 *
 * 【题目本质】
 * 极小化极大（Minimax）博弈 + 区间 DP。
 * 在 [1,n] 中设计猜数策略，使最坏情况下付出的代价（猜错数 x 付 x 元）最小。
 * dp[i][j] = 保证在 [i,j] 范围猜对所需的最少金额。
 *
 * 【解法总览】
 * Solution1: 区间 DP（自底向上）— 按长度枚举区间         O(n^3) / O(n^2)
 * Solution2: 记忆化搜索（自顶向下）— 递归+缓存           O(n^3) / O(n^2)
 *
 * 【核心递推】
 * dp[i][j] = min{ k + max(dp[i][k-1], dp[k+1][j]) } for k in [i, j]
 *   - k: 当前猜的数字
 *   - max: 对手选最坏情况（左子区间或右子区间代价更大的那个）
 *   - min: 我们选最优猜测点
 */

// ===================== Solution1: 区间 DP（推荐）=====================
// 按区间长度从小到大填表
// 时间 O(n^3)，空间 O(n^2)
class Solution1 {
public:
    int getMoneyAmount(int n) {
        // dp[i][j]: 在 [i, j] 范围内保证猜对的最小代价
        // 数组开 n+2 防止 k+1 越界
        vector<vector<int>> dp(n + 2, vector<int>(n + 2, 0));

        for (int len = 2; len <= n; len++) {           // 区间长度
            for (int i = 1; i + len - 1 <= n; i++) {   // 区间起点
                int j = i + len - 1;                    // 区间终点
                dp[i][j] = INT_MAX;
                for (int k = i; k <= j; k++) {          // 枚举猜测点
                    // 猜 k 的代价：k + 更差的那边
                    int cost = k + max(dp[i][k - 1], dp[k + 1][j]);
                    dp[i][j] = min(dp[i][j], cost);
                }
            }
        }
        return dp[1][n];
    }
};

// ===================== Solution2: 记忆化搜索 =====================
// 自顶向下递归，更直观地表达 Minimax 思想
// 时间 O(n^3)，空间 O(n^2)
class Solution2 {
    vector<vector<int>> memo;

    int solve(int i, int j) {
        if (i >= j) return 0;  // 空区间或单元素，不用猜
        if (memo[i][j] != -1) return memo[i][j];

        int res = INT_MAX;
        for (int k = i; k <= j; k++) {
            // 猜 k：付 k 元 + 递归处理最坏的那个子区间
            int cost = k + max(solve(i, k - 1), solve(k + 1, j));
            res = min(res, cost);
        }
        return memo[i][j] = res;
    }

public:
    int getMoneyAmount(int n) {
        memo.assign(n + 2, vector<int>(n + 2, -1));
        return solve(1, n);
    }
};

/*
 * 【解法对比】
 * | 解法     | 时间   | 空间   | 优点                 | 缺点               |
 * |----------|--------|--------|---------------------|--------------------|
 * | Solution1| O(n^3) | O(n^2) | 无递归开销，好调试     | 填表顺序需想清楚     |
 * | Solution2| O(n^3) | O(n^2) | Minimax 语义更直观    | 递归栈开销           |
 *
 * 【易错点】
 * 1. min/max 嵌套搞反：外层是 min（我选最优），内层是 max（对手选最差）
 * 2. 以为二分就是最优：猜中间不一定最优，因为大数代价高
 * 3. dp 数组越界：k+1 可能到 n+1，需要开 (n+2) 大小
 * 4. 区间 DP 枚举顺序：必须先算短区间，才能用来算长区间
 * 5. base case：dp[i][i]=0（一个数直接猜）, dp[i][i-1]=0（空区间）
 *
 * 【面试追问】
 * Q1: n=10 时最优策略是什么？
 * A1: 先猜 7→如果大: 猜 9→{8,10}; 如果小: 猜 3→如果大: 猜 5→{4,6};
 *     如果小: 猜 1→{2}。最坏代价 7+9=16。
 *
 * Q2: 能否优化到 O(n^2)？
 * A2: 可以利用 Knuth 优化：最优分割点 k 的单调性。
 *     记录 opt[i][j] 为最优 k，则 opt[i][j-1] <= opt[i][j] <= opt[i+1][j]，
 *     但实现较复杂，面试中 O(n^3) 足够。
 *
 * Q3: 如果代价不是 k 而是常数 1（每猜一次付 1 元），答案是什么？
 * A3: 那就退化为最少猜测次数问题，答案是 ceil(log2(n))，即标准二分查找。
 */
