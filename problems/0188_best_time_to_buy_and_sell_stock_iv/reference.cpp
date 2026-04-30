/*
 * LeetCode 188: 买卖股票的最佳时机 IV (Best Time to Buy and Sell Stock IV)
 *
 * 【题目本质】
 * 在 n 天的价格序列上，做最多 k 次不重叠的"买→卖"交易，求最大总利润。
 * 这是 Stock 系列的通用版：Stock I(k=1), II(k=∞), III(k=2) 都是特例。
 *
 * 【解法总览】
 * 解法1: 暴力递归       — O(2^n) / O(n)   — 穷举所有决策，理解题意
 * 解法2: 记忆化搜索     — O(nk) / O(nk)   — 递归 + memo，自顶向下
 * 解法3: 三维 DP (递推) — O(nk) / O(nk)   — 面试标准答案
 * 解法4: 空间优化 DP    — O(nk) / O(k)    — 滚动掉天数维度，最优
 *
 * 【系列题对比】
 * - Stock I  (121): k=1 → buy/sell 各一个变量
 * - Stock II (122): k=∞ → 去掉 j 维度，贪心
 * - Stock III(123): k=2 → j 展开为 1,2 → 4个变量 (buy1,sell1,buy2,sell2)
 * - Stock IV (188): 通用 k → 本题，用数组 buy[1..k], sell[1..k]
 *   ^^ 和 123 的区别：把 4 个变量改成长度为 k 的数组，循环处理
 * - Cooldown (309): 在本题基础上，买入转移看 i-2 天 (或加冷冻状态)
 * - Fee     (714): 在本题基础上 (k=∞)，卖出时减去 fee
 */

// ============================================================
// 解法1: 暴力递归 — 穷举每天的买/卖/观望
// 时间: O(2^n)  空间: O(n) 递归栈
//
// 【思路】
// 最直觉的想法：站在每一天，根据当前状态决定做什么。
// 用三个参数描述状态：第几天、已完成几次交易、是否持有。
// 不同的决策路径会到达相同的状态 → 存在大量重叠子问题。
// 这个解法帮助我们看到重叠子问题，为后续优化做铺垫。
//
// 决策树示意 (prices=[2,4,1], k=1):
//                         (day0, txn0, noHold)
//                        /                   \
//              buy@2                          skip
//         (day1, txn0, hold)            (day1, txn0, noHold)
//          /          \                   /             \
//    sell@4           skip           buy@4              skip
// (day2,txn1,no)  (day2,txn0,hold)  (day2,txn0,hold) (day2,txn0,no)
//    profit=2           ...               ...              ...
// ============================================================
class Solution1 {
public:
    int maxProfit(int k, vector<int>& prices) {
        return dfs(prices, 0, 0, false, k);
    }

private:
    int dfs(vector<int>& prices, int day, int txn, bool holding, int k) {
        if (day == (int)prices.size()) {
            // 最后一天之后，如果还持有股票则不合法(给极小值)
            return holding ? -1e9 : 0;
        }
        if (txn == k && !holding) {
            // 已完成 k 次交易且不持有，后面不能再交易
            return 0;
        }

        // 选择1：观望
        int res = dfs(prices, day + 1, txn, holding, k);

        if (holding) {
            // 选择2：卖出 → 完成一次交易(txn+1)
            res = max(res, prices[day] + dfs(prices, day + 1, txn + 1, false, k));
        } else if (txn < k) {
            // 选择3：买入 → 交易计数不变(卖出时才+1)
            res = max(res, -prices[day] + dfs(prices, day + 1, txn, true, k));
        }
        return res;
    }
};

// ============================================================
// 解法2: 记忆化搜索 — 递归 + memo
// 时间: O(nk)  空间: O(nk)
//
// 【思路】
// 解法1的瓶颈：相同的 (day, txn, holding) 被计算多次。
// 状态空间大小只有 n × (k+1) × 2，加个 memo 缓存即可。
// 代码和解法1几乎一样，只多了 memo 的读/写。
// ============================================================
class Solution2 {
public:
    int maxProfit(int k, vector<int>& prices) {
        int n = prices.size();
        // memo[day][txn][holding], -1 表示未计算
        vector<vector<vector<int>>> memo(n, vector<vector<int>>(k + 1, vector<int>(2, -1)));
        return dfs(prices, 0, 0, 0, k, memo);
    }

private:
    int dfs(vector<int>& prices, int day, int txn, int holding, int k,
            vector<vector<vector<int>>>& memo) {
        if (day == (int)prices.size()) return holding ? -1e9 : 0;
        if (txn == k && !holding) return 0;
        if (memo[day][txn][holding] != -1) return memo[day][txn][holding];

        int res = dfs(prices, day + 1, txn, holding, k, memo);
        if (holding) {
            res = max(res, prices[day] + dfs(prices, day + 1, txn + 1, 0, k, memo));
        } else if (txn < k) {
            res = max(res, -prices[day] + dfs(prices, day + 1, txn, 1, k, memo));
        }
        return memo[day][txn][holding] = res;
    }
};

// ============================================================
// 解法3: 三维 DP (递推) — 面试标准答案
// 时间: O(nk)  空间: O(nk)
//
// 【思路】
// 把记忆化搜索翻译成自底向上的填表。
// 状态: dp[i][j][0] = 第i天结束, 已完成j次交易, 不持有
//       dp[i][j][1] = 第i天结束, 已完成j次交易, 持有
//
// 转移 (卖出时完成交易):
//   dp[i][j][0] = max(dp[i-1][j][0], dp[i-1][j-1][1] + prices[i])
//                       观望              卖出(完成第j次)
//   dp[i][j][1] = max(dp[i-1][j][1], dp[i-1][j][0] - prices[i])
//                       观望              买入(次数不变)
//
// DP 填表示例: prices=[3,2,6,5,0,3], k=2
//
//  天i | price | j=0:不持/持  | j=1:不持/持  | j=2:不持/持
//  ----+-------+-------------+-------------+-------------
//   0  |   3   |  0 / -3     | -INF/-INF   | -INF/-INF
//   1  |   2   |  0 / -2     |  1   / -2   | -INF/-INF
//   2  |   6   |  0 / -2     |  4   / -2   |  2   / -INF
//   3  |   5   |  0 / -2     |  4   / -2   |  4   / 2
//   4  |   0   |  0 /  0     |  4   /  4   |  4   / 4
//   5  |   3   |  0 /  0     |  4   /  4   |  7   / 4
//                                              ↑ 答案=7
// ============================================================
class Solution3 {
public:
    int maxProfit(int k, vector<int>& prices) {
        int n = prices.size();
        if (n == 0 || k == 0) return 0;

        // 优化: 当 k >= n/2 时，交易次数不构成限制，退化为无限次交易
        // 因为 n 天最多只能做 n/2 次买卖
        if (k >= n / 2) {
            int profit = 0;
            for (int i = 1; i < n; i++)
                profit += max(0, prices[i] - prices[i - 1]);
            return profit;
        }

        // 用 -1e9 表示不合法状态(而非0)，避免不合法状态参与转移
        const int NEG_INF = -1e9;
        vector<vector<vector<int>>> dp(n, vector<vector<int>>(k + 1, vector<int>(2, NEG_INF)));

        // 初始化第0天：唯一合法的状态
        dp[0][0][0] = 0;           // 第0天,0次交易,不持有 → 利润0
        dp[0][0][1] = -prices[0];  // 第0天买入,0次完成交易,持有

        for (int i = 1; i < n; i++) {
            for (int j = 0; j <= k; j++) {
                // 不持有: 观望 or 卖出(需 j>=1, 从 j-1 次持有状态转移)
                dp[i][j][0] = dp[i - 1][j][0];
                if (j >= 1) {
                    dp[i][j][0] = max(dp[i][j][0],
                                      dp[i - 1][j - 1][1] + prices[i]);
                }

                // 持有: 观望 or 买入(交易次数不变)
                dp[i][j][1] = dp[i - 1][j][1];
                dp[i][j][1] = max(dp[i][j][1],
                                  dp[i - 1][j][0] - prices[i]);
            }
        }

        // 答案: 最后一天不持有，取所有交易次数的最大值
        int ans = 0;
        for (int j = 0; j <= k; j++)
            ans = max(ans, dp[n - 1][j][0]);
        return ans;
    }
};

// ============================================================
// 解法4: 空间优化 DP — 滚动数组 + 经典 buy/sell 写法
// 时间: O(nk)  空间: O(k)
//
// 【思路】
// dp[i] 只依赖 dp[i-1]，可以去掉天数维度。
// 用 buy[j] 表示"完成第j次买入后的最大利润"(负值，因为花了钱)
// 用 sell[j] 表示"完成第j次卖出后的最大利润"
//
// 每天更新:
//   buy[j]  = max(buy[j],  sell[j-1] - price)  // 第j次买入
//   sell[j] = max(sell[j], buy[j] + price)       // 第j次卖出
//
// 注意 sell[j] 用了当天更新后的 buy[j]，这等价于同天买卖(利润0)，
// 不会让答案变优，所以是正确的。
//
// 和 Stock III (123) 的代码区别:
//   123 写死了 buy1, sell1, buy2, sell2 四个变量
//   188 改成数组 buy[1..k], sell[1..k]，用循环处理
//   只是从"展开的4个变量"改成"循环的数组"，逻辑完全一样
// ============================================================
class Solution4 {
public:
    int maxProfit(int k, vector<int>& prices) {
        int n = prices.size();
        if (n == 0 || k == 0) return 0;

        // k >= n/2 时退化为贪心（无限次交易）
        if (k >= n / 2) {
            int profit = 0;
            for (int i = 1; i < n; i++)
                profit += max(0, prices[i] - prices[i - 1]);
            return profit;
        }

        // buy[j]: 第j次买入后的最大利润 (初始为极小值, 表示还没买过)
        // sell[j]: 第j次卖出后的最大利润 (初始 sell[0]=0 表示没交易)
        vector<int> buy(k + 1, INT_MIN);
        vector<int> sell(k + 1, 0);

        for (int price : prices) {
            for (int j = 1; j <= k; j++) {
                // 第j次买入: 保持 or 在第j-1次卖出后买入
                buy[j] = max(buy[j], sell[j - 1] - price);
                // 第j次卖出: 保持 or 在第j次买入后卖出
                sell[j] = max(sell[j], buy[j] + price);
            }
        }

        // sell[k] 即为最多 k 次交易的最大利润
        return sell[k];
    }
};

// ============================================================
// 【解法对比】
//
// | 解法     | 时间    | 空间   | 特点                           |
// |----------|---------|--------|-------------------------------|
// | 暴力递归 | O(2^n)  | O(n)   | 展示决策过程，不可能AC         |
// | 记忆化   | O(nk)   | O(nk)  | 自顶向下，递归直觉             |
// | 三维DP   | O(nk)   | O(nk)  | 面试标准答案，逻辑清晰         |
// | 空间优化 | O(nk)   | O(k)   | 代码最短，展示优化功力         |
//
// 面试建议: 先说状态定义(解法3思路)，写代码直接写解法4
//
// 【易错点】
// 1. 初始化 buy 用 INT_MIN 而非 0
//    ✗ buy[j] = 0   → 表示"不花钱就持有了"，答案偏大
//    ✓ buy[j] = INT_MIN → 不合法状态不会参与 max 运算
//
// 2. 忘记 k >= n/2 的特判
//    ✗ 直接跑 O(nk) 当 k 很大时浪费时间
//    ✓ 先判断退化条件，走贪心 O(n)
//
// 3. sell[j] = max(sell[j], buy[j] + price) 中 buy[j] 是当天更新的
//    很多人以为应该用"昨天的 buy[j]"，但当天更新的 buy[j] 只可能
//    让 sell[j] 多算一次"当天买当天卖"(利润为0)，不影响最优性。
//    如果不放心，可以反向遍历 j: for(j=k;j>=1;j--) 来避免此问题。
//
// 4. j 的含义不一致
//    ✗ 买入时 j+1 但卖出时也 j+1 → 一次交易被记了两次
//    ✓ 选定"卖出时计数"或"买入时计数"，全程统一
//
// 【面试追问】
// Q1: 这道题和 Stock I/II/III 的关系？
//     → I 是 k=1, II 是 k=∞, III 是 k=2。本题是通用版。
//        代码区别: 123 用 buy1,sell1,buy2,sell2 四个变量
//        188 改成 buy[1..k], sell[1..k] 数组 + 循环
//
// Q2: k >= n/2 为什么可以贪心？
//     → n天最多 n/2 个不重叠买卖区间，k超过这个数就没有约束了
//        等价于 Stock II，贪心收集所有上涨段
//
// Q3: 如果加上冷冻期(卖出后下一天不能买)怎么改？
//     → 买入的转移看 i-2 天的不持有状态:
//        buy[j] = max(buy[j], sell_prev_prev[j-1] - price)
//        或者引入第三个状态"冷冻中"
//        这就是 LeetCode 309 的扩展
//
// Q4: 如果每笔交易有手续费 fee 怎么改？
//     → 卖出时减去 fee: sell[j] = max(sell[j], buy[j] + price - fee)
//        这就是 LeetCode 714 的扩展
//
// 【相关题型】
// 121 Stock I    — k=1 特例，buy/sell各一个变量
// 122 Stock II   — k=∞ 特例，贪心收集所有price[i]-price[i-1]>0
// 123 Stock III  — k=2 特例，展开循环得到4个变量
// 309 Cooldown   — 复用本题框架，买入转移多隔一天
// 714 Stock+Fee  — 复用本题框架(k=∞版)，卖出减fee
// ============================================================
