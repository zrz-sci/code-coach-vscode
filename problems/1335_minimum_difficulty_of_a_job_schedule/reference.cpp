/*
 * LeetCode 1335: Minimum Difficulty of a Job Schedule
 *
 * 【题目本质】
 * 将长度为 n 的数组按顺序切成 d 段（每段至少1个元素），
 * 使得每段最大值之和最小。经典的区间分割 DP。
 *
 * 【解法总览】
 * 解法1: 记忆化搜索（自顶向下 DP） — O(n²d) / O(nd)  — 最直觉
 * 解法2: 递推 DP（自底向上）         — O(n²d) / O(nd)  — 面试首选
 * 解法3: 递推 DP + 空间优化          — O(n²d) / O(n)   — 追问优化
 * 解法4: 单调栈优化 DP               — O(nd)  / O(n)   — 最优/进阶
 */

// ============================================================
// 解法1: 记忆化搜索 — 递归地"切一刀"
// 时间: O(n²d)  空间: O(nd)
//
// 【思路】
// 最自然的递归思路：dfs(i, k) 表示"从工作 i 开始，还需要 k 天完成"
// 的最小总难度。每次枚举当天做工作 i..j，取 max 作为当天难度，
// 递归处理剩余。发现大量重叠子问题，加 memo 即可。
//
// 状态数 O(nd)，每个状态转移枚举 O(n)，总共 O(n²d)。
//
// 递归树示意 (jobs=[6,5,4,3,2,1], d=2):
//
//                    dfs(0, 2)
//               /    |    |    |    \
//         day1=[0]  [0,1] [0,2] [0,3] [0,4]
//         max=6    max=6  max=6  max=6  max=6
//             |      |      |      |      |
//       dfs(1,1) dfs(2,1) dfs(3,1) dfs(4,1) dfs(5,1)
//       max=5    max=4    max=3    max=2    max=1
//
//  最优路径: day1=[0..4](max=6) + day2=[5](max=1) = 7
// ============================================================
class Solution1 {
public:
    int minDifficulty(vector<int>& jobDifficulty, int d) {
        int n = jobDifficulty.size();
        if (n < d) return -1;

        // memo[i][k]: 从工作 i 开始，分 k 天，-1 表示未计算
        vector<vector<int>> memo(n, vector<int>(d + 1, -1));
        return dfs(jobDifficulty, 0, d, memo);
    }

private:
    int dfs(vector<int>& jobs, int i, int k, vector<vector<int>>& memo) {
        int n = jobs.size();

        // base case: 最后一天，把剩余工作全做完
        if (k == 1) {
            int mx = 0;
            for (int j = i; j < n; j++) mx = max(mx, jobs[j]);
            return mx;
        }

        if (memo[i][k] != -1) return memo[i][k];

        int res = INT_MAX;
        int mx = 0;

        // 枚举今天做工作 i..j
        // j 最多到 n-k，因为后面还需要 k-1 天各至少一个工作
        for (int j = i; j <= n - k; j++) {
            mx = max(mx, jobs[j]);  // 递推维护区间 max，O(1)
            res = min(res, mx + dfs(jobs, j + 1, k - 1, memo));
        }

        memo[i][k] = res;
        return res;
    }
};

// ============================================================
// 解法2: 自底向上递推 DP
// 时间: O(n²d)  空间: O(nd)
//
// 【思路】
// 把记忆化搜索翻译成填表。
// dp[i][k] = 完成工作 0..i，用恰好 k 天的最小总难度。
//
// 转移方程：
//   dp[i][k] = min over j in [k-1, i] { dp[j-1][k-1] + max(jobs[j..i]) }
//   含义：第 k 天做工作 j..i，前 k-1 天做工作 0..j-1
//
// j 从 i 倒着枚举，这样 max(jobs[j..i]) 可以递推。
//
// DP 填表方向：
//
//        i→  0    1    2    3    4    5
//  k=1:     6    6    6    6    6    6    ← 前缀 max
//  k=2:     -    11   10   9    8    7   ← 枚举切割点
//                                    ↑ 答案
//
//  dp[5][2] 的计算过程 (j 从 5 到 1):
//    j=5: dp[4][1] + max(jobs[5..5]) = 6 + 1 = 7
//    j=4: dp[3][1] + max(jobs[4..5]) = 6 + 2 = 8
//    j=3: dp[2][1] + max(jobs[3..5]) = 6 + 3 = 9
//    j=2: dp[1][1] + max(jobs[2..5]) = 6 + 4 = 10
//    j=1: dp[0][1] + max(jobs[1..5]) = 6 + 5 = 11
//  最小值 = 7 ✓
// ============================================================
class Solution2 {
public:
    int minDifficulty(vector<int>& jobDifficulty, int d) {
        int n = jobDifficulty.size();
        if (n < d) return -1;

        const int INF = 1e9;
        vector<vector<int>> dp(n, vector<int>(d + 1, INF));

        // base case: k=1，第一天做工作 0..i
        int mx = 0;
        for (int i = 0; i < n; i++) {
            mx = max(mx, jobDifficulty[i]);
            dp[i][1] = mx;
        }

        // 填表：天数 k 从 2 到 d
        for (int k = 2; k <= d; k++) {
            for (int i = k - 1; i < n; i++) {  // 至少要有 k 个工作
                int curMax = 0;
                // j 从 i 倒着枚举到 k-1
                // 为什么是 k-1？因为前面 0..j-1 需要 k-1 天，至少 k-1 个工作，j-1 >= k-2 → j >= k-1
                for (int j = i; j >= k - 1; j--) {
                    curMax = max(curMax, jobDifficulty[j]);
                    dp[i][k] = min(dp[i][k], dp[j - 1][k - 1] + curMax);
                }
            }
        }

        return dp[n - 1][d];
    }
};

// ============================================================
// 解法3: 递推 DP + 滚动数组空间优化
// 时间: O(n²d)  空间: O(n)
//
// 【思路】
// 解法2中 dp[i][k] 只依赖 dp[*][k-1]，即当前天只依赖前一天。
// 因此可以用两个一维数组交替使用（或 newDp + dp 交换）。
//
// 注意不能原地覆盖 dp，因为内层 j 循环需要读 dp[j-1]（上一层的值），
// 如果直接写到 dp[i] 可能会覆盖还没用到的 dp[j-1]。
// ============================================================
class Solution3 {
public:
    int minDifficulty(vector<int>& jobDifficulty, int d) {
        int n = jobDifficulty.size();
        if (n < d) return -1;

        const int INF = 1e9;
        vector<int> dp(n, INF);

        // base case: k=1
        int mx = 0;
        for (int i = 0; i < n; i++) {
            mx = max(mx, jobDifficulty[i]);
            dp[i] = mx;
        }

        for (int k = 2; k <= d; k++) {
            vector<int> newDp(n, INF);
            for (int i = k - 1; i < n; i++) {
                int curMax = 0;
                for (int j = i; j >= k - 1; j--) {
                    curMax = max(curMax, jobDifficulty[j]);
                    // dp[j-1] 此时是上一层 (k-1) 的值，没有被覆盖
                    newDp[i] = min(newDp[i], dp[j - 1] + curMax);
                }
            }
            dp = newDp;  // 滚动：当前层变成下一轮的"上一层"
        }

        return dp[n - 1];
    }
};

// ============================================================
// 解法4: 单调栈优化 DP
// 时间: O(nd)  空间: O(n)
//
// 【思路】
// 解法3的内层对每个 i 枚举所有切割点 j，O(n)，总共 O(n²d)。
// 
// 关键观察：在计算 dp_new[i] 时，转移为：
//   dp_new[i] = min over j { dp_old[j-1] + max(jobs[j..i]) }
//
// 当 i 向右扩展一个新元素 jobs[i] 时：
// - 如果 jobs[i] 很大，它会成为很多区间的新 max，
//   "覆盖"掉那些旧的较小的 max。
// - 单调栈维护一个严格递减的 jobs 值序列。
//
// 栈中每个元素 (idx, minCost) 含义：
// - idx: 工作索引
// - minCost: 以 jobs[idx] 作为当天最大值的最优方案的总代价
//
// 当 jobs[i] 出现时：
// 1. 默认方案：工作 i 独立成一天 → dp_old[i-1] + jobs[i]
// 2. 弹出栈中所有 jobs[idx] <= jobs[i] 的元素
//    因为 jobs[i] 更大，它替代了 jobs[idx] 成为区间最大值
//    代价更新：cost - jobs[idx] + jobs[i]
// 3. 如果栈非空，栈顶 jobs[top] > jobs[i]
//    可以把 i 并入栈顶那天（max 不变，还是 jobs[top]）
//    → 取 min(minCost, stk.top().second)
//
// 每个元素最多入栈出栈各一次 → 内层均摊 O(n) → 总 O(nd)
// ============================================================
class Solution4 {
public:
    int minDifficulty(vector<int>& jobDifficulty, int d) {
        int n = jobDifficulty.size();
        if (n < d) return -1;

        const int INF = 1e9;
        vector<int> dp(n, INF);

        // base case: k=1，dp[i] = max(jobs[0..i])
        int mx = 0;
        for (int i = 0; i < n; i++) {
            mx = max(mx, jobDifficulty[i]);
            dp[i] = mx;
        }

        for (int k = 2; k <= d; k++) {
            vector<int> newDp(n, INF);
            // 单调栈：栈内 jobDifficulty 严格递减
            // pair: (工作索引, 该位置对应的最优代价)
            stack<pair<int, int>> stk;

            for (int i = k - 1; i < n; i++) {
                // 方案1: 工作 i 独立成第 k 天
                int minCost = dp[i - 1] + jobDifficulty[i];

                // 弹出栈中所有 <= jobs[i] 的元素
                // jobs[i] 会替代它们成为区间最大值
                while (!stk.empty() &&
                       jobDifficulty[stk.top().first] <= jobDifficulty[i]) {
                    auto [idx, cost] = stk.top();
                    stk.pop();
                    // 旧方案中 jobs[idx] 是最大值，现在被 jobs[i] 替代
                    // 代价变化：减去 jobs[idx]，加上 jobs[i]
                    minCost = min(minCost, cost - jobDifficulty[idx] + jobDifficulty[i]);
                }

                // 方案2: 如果栈非空，栈顶 jobs 值 > jobs[i]
                // 把工作 i 并入栈顶那天，max 不变（栈顶更大）
                if (!stk.empty()) {
                    minCost = min(minCost, stk.top().second);
                }

                stk.push({i, minCost});
                newDp[i] = minCost;
            }

            dp = newDp;
        }

        return dp[n - 1];
    }
};

// ============================================================
// 【解法对比】
//
// | 解法 | 时间     | 空间   | 适用场景               |
// |------|----------|--------|------------------------|
// | 记忆化| O(n²d)  | O(nd)  | 思路最清晰，面试首选   |
// | 递推  | O(n²d)  | O(nd)  | 标准 DP 写法           |
// | 滚动  | O(n²d)  | O(n)   | 追问空间优化时         |
// | 单调栈| O(nd)   | O(n)   | 追问时间优化的加分项   |
//
// n=300, d=10 → O(n²d) ≈ 90万，完全够用。
// 单调栈优化在 n 更大时才有明显优势。
//
// 【易错点】
// 1. 忘记判 n < d 返回 -1
//    → 工作数不够每天至少做一个
//
// 2. 枚举范围 j >= k-1 而不是 j >= 0
//    → ✗ for(j=i; j>=0; j--) 
//    → ✓ for(j=i; j>=k-1; j--)
//    → 前面 0..j-1 需要填满 k-1 天，至少 k-1 个工作
//
// 3. 滚动数组原地覆盖
//    → ✗ dp[i] = min(dp[i], dp[j-1] + curMax)  
//         // dp[j-1] 可能已被本轮覆盖
//    → ✓ 使用 newDp 临时数组
//
// 4. base case: dp[i][1] = max(jobs[0..i]) 不是 jobs[i]
//    → 第一天做所有前 i+1 个工作，难度是前缀最大值
//
// 5. 单调栈弹出时忘记做代价修正
//    → ✗ minCost = min(minCost, cost)
//    → ✓ minCost = min(minCost, cost - jobs[idx] + jobs[i])
//
// 【面试追问链】
// Q1: 暴力递归复杂度多少？瓶颈在哪？
//     → 状态 O(nd)，转移 O(n)，总 O(n²d)。瓶颈在内层枚举切割点。
//
// Q2: 能否空间优化？
//     → 每层 k 只依赖 k-1 层，用滚动数组 O(n)。
//     → 注意必须用 newDp 临时数组，不能原地覆盖。
//
// Q3: 能否将 O(n²d) 优化到 O(nd)？
//     → 单调栈优化。jobs[i] 增大时会批量覆盖旧的区间最大值，
//        栈维护递减序列，每个元素入栈出栈各一次，均摊 O(1)。
//
// Q4: 这道题和"分割数组的最大值"(LC 410)有什么联系？
//     → 都是"数组分成 k 段"的 DP。区别：本题每段取 max 再求 sum，
//        410 每段取 sum 再取 max。DP 框架相同，目标函数不同。
// ============================================================
