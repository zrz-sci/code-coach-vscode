/*
 * 【题目本质】
 * n 栋房子、k 种颜色，相邻房子不能同色，求粉刷所有房子的最小总费用。
 * 是 256. Paint House (3色) 的推广。
 *
 * 【解法总览】
 * 解法1: DP + 最小/次小值优化 - O(nk) 时间 O(1) 空间 ⭐ 面试首选
 * 解法2: 朴素 DP - O(nk^2) 时间 O(k) 空间
 */

// ============================================================
// 解法1: DP + 最小/次小值优化 ⭐ 推荐
// 核心优化：每一行只需要上一行的最小值、次小值和最小值颜色索引
// 当前颜色 j == prevMinIdx 时用次小值，否则用最小值
// 时间 O(nk)，空间 O(1)
// ============================================================
class Solution1 {
public:
    int minCostII(vector<vector<int>>& costs) {
        int n = costs.size(), k = costs[0].size();

        // prevMin1: 上一行最小费用
        // prevMin2: 上一行次小费用
        // prevIdx:  上一行最小费用对应的颜色
        int prevMin1 = 0, prevMin2 = 0, prevIdx = -1;

        for (int i = 0; i < n; i++) {
            int curMin1 = INT_MAX, curMin2 = INT_MAX, curIdx = -1;

            for (int j = 0; j < k; j++) {
                // 转移：加上一行的最优值（避开相同颜色）
                int val = costs[i][j] + (j == prevIdx ? prevMin2 : prevMin1);

                // 更新当前行的最小值和次小值
                if (val < curMin1) {
                    curMin2 = curMin1;
                    curMin1 = val;
                    curIdx = j;
                } else if (val < curMin2) {
                    curMin2 = val;
                }
            }

            prevMin1 = curMin1;
            prevMin2 = curMin2;
            prevIdx = curIdx;
        }

        return prevMin1;
    }
};

// ============================================================
// 解法2: 朴素 DP（滚动数组优化空间）
// dp[j] = 粉刷到当前房子且用颜色 j 的最小花费
// 转移时遍历所有 c != j 找最小值
// 时间 O(nk^2)，空间 O(k)
// ============================================================
class Solution2 {
public:
    int minCostII(vector<vector<int>>& costs) {
        int n = costs.size(), k = costs[0].size();

        // 初始化：第一栋房子
        vector<int> dp(costs[0].begin(), costs[0].end());

        for (int i = 1; i < n; i++) {
            vector<int> ndp(k, INT_MAX);
            for (int j = 0; j < k; j++) {
                for (int c = 0; c < k; c++) {
                    if (c != j) {
                        ndp[j] = min(ndp[j], dp[c] + costs[i][j]);
                    }
                }
            }
            dp = ndp;
        }

        return *min_element(dp.begin(), dp.end());
    }
};

/*
 * 【解法对比】
 * | 解法        | 时间     | 空间   | 适用场景         |
 * |------------|---------|-------|-----------------|
 * | min1/min2  | O(nk)   | O(1)  | 最优解，面试必须   |
 * | 朴素 DP    | O(nk^2) | O(k)  | k 很小时也可      |
 *
 * 【易错点】
 * 1. 只记录最小值忘记记录次小值
 *    -> 当 j 恰好等于上一行最小值颜色时，应该用次小值
 * 2. prevIdx 初始化为 0 而非 -1
 *    -> 初始化 -1 确保第一行所有颜色都能使用 prevMin1(=0)
 * 3. 更新 curMin1/curMin2 顺序错误
 *    -> 必须先判断 < curMin1，再判断 < curMin2
 *    -> 更新 curMin1 时要先把旧 curMin1 赋给 curMin2
 * 4. k = 1 且 n > 1 时无解
 *    -> 实际上题目约束 k >= 2，但面试中可以提一下
 *
 * 【面试追问】
 * Q: 为什么只需要最小和次小两个值？
 * A: 对于颜色 j，如果 j 不是上一行最小值的颜色，直接用最小值；
 *    如果 j 恰好是上一行最小值的颜色，就用次小值。
 *    不可能两个不同颜色都需要跳过最小值。
 *
 * Q: 如果要求输出具体方案呢？
 * A: 额外维护 choice[i] 记录每行选了哪个颜色，最后从末行回溯。
 */
