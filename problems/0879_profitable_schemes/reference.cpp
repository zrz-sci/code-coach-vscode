/*
 * LeetCode 879: Profitable Schemes (盈利计划)
 *
 * 【题目本质】
 * 有 G 个犯罪活动，每个需 group[i] 人、赚 profit[i]。
 * 总人数 <= n，求满足总利润 >= minProfit 的方案数 (mod 10^9+7)。
 * 本质: 带两个约束（人数上限+利润下限）的 0/1 背包计数。
 *
 * 【解法总览】
 * 解法1: 二维 DP (推荐)        — O(G*n*P) / O(n*P)
 * 解法2: 二维 DP (正向推)       — O(G*n*P) / O(n*P)
 * 解法3: 三维 DP (理解用)       — O(G*n*P) / O(G*n*P)
 */

// ============================================================
// 解法1: 二维 DP — 反向填表（面试首选）
// 时间: O(G * n * minProfit)  空间: O(n * minProfit)
//
// 【状态定义】
// dp[j][k] = 使用 j 个人，利润为 k 的方案数。
// 利润超过 minProfit 的都 clamp 到 minProfit（因为只关心是否达标）。
//
// 【转移方程】
// 对于每个活动 i (需要 g=group[i] 人, 赚 p=profit[i]):
//   不选: dp[j][k] 保持不变
//   选:   dp[j][newK] += dp[j-g][k]
//         其中 newK = min(k + p, minProfit)
//
// 【为什么 clamp 到 minProfit？】
// 利润 30 和利润 50 只要都 >= minProfit 就都是有效方案。
// 如果不 clamp，利润维度最大到 sum(profit) ≈ 10000，太大。
// clamp 后利润维度最大 minProfit ≈ 100，空间大大减小。
//
// 【遍历顺序】
// 外层: 活动 i = 0 到 G-1
// 内层: j 从 n 倒序到 g （0/1 背包经典倒序）
//       k 从 minProfit 倒序到 0
// 倒序保证每个活动最多选一次。
//
// 示例: n=5, minProfit=3, group=[2,2], profit=[2,3]
//
// 初始: dp[0][0] = 1 (什么都不选)
//
// 活动0 (g=2, p=2):
//   j=5→2, k=3→0:
//     j=2, k=0: newK=min(0+2,3)=2, dp[2][2] += dp[0][0] = 1
//     ...其他转移类似
//   dp 状态: dp[0][0]=1, dp[2][2]=1
//
// 活动1 (g=2, p=3):
//   j=4, k=2: newK=min(2+3,3)=3, dp[4][3] += dp[2][2] = 1
//   j=2, k=0: newK=min(0+3,3)=3, dp[2][3] += dp[0][0] = 1
//   dp 状态: dp[0][0]=1, dp[2][2]=1, dp[2][3]=1, dp[4][3]=1
//
// 答案: sum(dp[j][3] for j=0..5) = 0+0+1+0+1+0 = 2 ✓
// ============================================================
class Solution1 {
public:
    int profitableSchemes(int n, int minProfit,
                          vector<int>& group, vector<int>& profit) {
        const int MOD = 1e9 + 7;
        int G = group.size();

        // dp[j][k]: 用 j 人，利润为 k 的方案数
        // k 的范围 [0, minProfit]，超过 minProfit 的 clamp 到 minProfit
        vector<vector<int>> dp(n + 1, vector<int>(minProfit + 1, 0));
        dp[0][0] = 1; // 不选任何活动: 0人0利润，1种方案

        for (int i = 0; i < G; i++) {
            int g = group[i]; // 本活动需要的人数
            int p = profit[i]; // 本活动的利润

            // 倒序遍历 (0/1 背包，保证每个活动最多选一次)
            for (int j = n; j >= g; j--) {
                for (int k = minProfit; k >= 0; k--) {
                    // 选活动 i: 利润从 k 变为 min(k+p, minProfit)
                    int newK = min(k + p, minProfit);
                    dp[j][newK] = (dp[j][newK] + dp[j - g][k]) % MOD;
                }
            }
        }

        // 答案: 所有人数下利润 == minProfit（包含 >= minProfit）的方案总和
        int ans = 0;
        for (int j = 0; j <= n; j++) {
            ans = (ans + dp[j][minProfit]) % MOD;
        }
        return ans;
    }
};

// ============================================================
// 解法2: 二维 DP — 另一种转移视角
// 时间: O(G * n * minProfit)  空间: O(n * minProfit)
//
// 【与解法1的区别】
// 这里用"从目标状态回溯"的视角:
//   dp[j][k] += dp[j-g][max(0, k-p)]
//
// max(0, k-p) 的含义:
//   如果目标利润是 k，选了活动 i (利润 p) 后，
//   之前只需要达到 max(0, k-p) 利润就够了。
//   k-p < 0 时取 0，因为利润不能为负。
//
// 这种写法和解法1等价，但转移方向不同:
//   解法1: "从旧状态推到新状态" (push)
//   解法2: "新状态从旧状态拉取" (pull)
// ============================================================
class Solution2 {
public:
    int profitableSchemes(int n, int minProfit,
                          vector<int>& group, vector<int>& profit) {
        const int MOD = 1e9 + 7;
        int G = group.size();

        vector<vector<int>> dp(n + 1, vector<int>(minProfit + 1, 0));
        dp[0][0] = 1;

        for (int i = 0; i < G; i++) {
            int g = group[i], p = profit[i];

            // 倒序遍历
            for (int j = n; j >= g; j--) {
                for (int k = minProfit; k >= 0; k--) {
                    // 从之前的状态 dp[j-g][max(0,k-p)] 转移过来
                    int prevK = max(0, k - p);
                    dp[j][k] = (dp[j][k] + dp[j - g][prevK]) % MOD;
                }
            }
        }

        // 答案: 利润恰好为 minProfit 的方案（包含所有 >= minProfit 的）
        int ans = 0;
        for (int j = 0; j <= n; j++) {
            ans = (ans + dp[j][minProfit]) % MOD;
        }
        return ans;
    }
};

// ============================================================
// 解法3: 三维 DP — 最直观但空间大
// 时间: O(G * n * minProfit)  空间: O(G * n * minProfit)
//
// 显式保留活动维度 i，适合理解但空间太大。
// 实际提交建议用解法1或解法2。
// ============================================================
class Solution3 {
public:
    int profitableSchemes(int n, int minProfit,
                          vector<int>& group, vector<int>& profit) {
        const int MOD = 1e9 + 7;
        int G = group.size();

        // dp[i][j][k]: 前 i 个活动，j 人，利润 k 的方案数
        vector<vector<vector<int>>> dp(
            G + 1, vector<vector<int>>(n + 1, vector<int>(minProfit + 1, 0)));
        dp[0][0][0] = 1;

        for (int i = 0; i < G; i++) {
            int g = group[i], p = profit[i];
            for (int j = 0; j <= n; j++) {
                for (int k = 0; k <= minProfit; k++) {
                    // 不选活动 i
                    dp[i + 1][j][k] = (dp[i + 1][j][k] + dp[i][j][k]) % MOD;

                    // 选活动 i (需要 j+g <= n)
                    if (j + g <= n) {
                        int newK = min(k + p, minProfit);
                        dp[i + 1][j + g][newK] =
                            (dp[i + 1][j + g][newK] + dp[i][j][k]) % MOD;
                    }
                }
            }
        }

        // 答案
        int ans = 0;
        for (int j = 0; j <= n; j++) {
            ans = (ans + dp[G][j][minProfit]) % MOD;
        }
        return ans;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法    | 时间          | 空间         | 代码清晰度 |
// |---------|--------------|-------------|-----------|
// | 解法1   | O(G*n*P)     | O(n*P)      | 高         |
// | 解法2   | O(G*n*P)     | O(n*P)      | 高         |
// | 解法3   | O(G*n*P)     | O(G*n*P)    | 最高       |
//
// G, n, P 各最大 100 → O(10^6) 完全可行。
// 面试选择: 解法1或2均可，解法3可用于讲解思路。
//
// ============================================================
// 【易错点总结】
//
// 1. 不 clamp 利润导致数组越界:
//    ✗ dp[j][k + p] += ... → k+p 可能超过 minProfit，越界
//    ✓ newK = min(k + p, minProfit); dp[j][newK] += ...
//
// 2. 忘记初始化 dp[0][0] = 1:
//    ✗ dp 全为 0 → 没有"种子"状态，所有转移结果都是 0
//    ✓ dp[0][0] = 1 → "不选任何活动"这个空方案是起点
//
// 3. 正序遍历导致重复选择:
//    ✗ for j from g to n → 同一活动可能被选多次(完全背包)
//    ✓ for j from n to g → 每个活动最多选一次(0/1背包)
//
// 4. 答案只取 dp[n][minProfit]:
//    ✗ return dp[n][minProfit]; → 只算了恰好用 n 人的方案
//    ✓ sum(dp[j][minProfit]) for j=0..n → 用 0~n 人的方案都算
//
// 5. 取模遗漏:
//    ✗ dp[j][k] = dp[j][k] + dp[j-g][prevK]; → 可能 int 溢出
//    ✓ dp[j][k] = (dp[j][k] + dp[j-g][prevK]) % MOD;
//
// ============================================================
// 【面试追问与回答】
//
// Q: 这和普通 0/1 背包有什么区别？
// A: 普通 0/1 背包有"重量上界"一个约束，本题有"人数上界"和
//    "利润下界"两个约束。利润下界通过 clamp 技巧处理，
//    把"至少达到 minProfit"转化为"恰好等于 minProfit（含溢出）"。
//
// Q: 如果每个活动可以执行多次怎么办？
// A: 把 0/1 背包改成完全背包：内层遍历改为正序
//    (j 从 g 到 n，k 从 0 到 minProfit)。
//
// Q: 时间复杂度 G*n*P = 100*100*100 = 10^6，如果约束更大怎么办？
// A: 如果约束到 10^4 级别（10^12 总操作），需要数学优化或近似。
//    但对于本题的约束 100，标准 DP 绰绰有余。
//
// ============================================================
// 【相关题型】
//
// - 494. 目标和: 0/1 背包计数，每个元素加或减，求方案数。
// - 416. 分割等和子集: 0/1 背包判定，能否选出子集和 == total/2。
// - 474. 一和零: 二维约束 (0个数+1个数) 的 0/1 背包。
// - 518. 零钱兑换 II: 完全背包计数。
// - 1049. 最后一块石头重量 II: 0/1 背包变体求最小差。
// ============================================================
