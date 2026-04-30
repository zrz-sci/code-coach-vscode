/*
 * LeetCode 121: Best Time to Buy and Sell Stock
 * 
 * 【题目本质】
 * 在数组中找 i < j 使得 prices[j] - prices[i] 最大。
 * 等价于：对每个卖出日，找它之前的最低买入价。
 *
 * 【解法总览】
 * 解法1: 暴力枚举所有买卖对      — O(n²) / O(1)  — 最直觉，理解题意
 * 解法2: 一次遍历维护历史最低价   — O(n)  / O(1)  — 面试首选
 * 解法3: 状态机 DP               — O(n)  / O(n)  — 股票系列通用框架
 * 解法3b: 状态机 DP 空间优化     — O(n)  / O(1)  — 与解法2本质相同
 *
 * 【股票系列题关系】
 * 本题(121)是基础：只能买卖1次。
 * → 122: 不限次数 — 买入转移改为 dp[i-1][0] - prices[i]
 * → 123: 最多2次 — 增加交易次数维度 k
 * → 188: 最多k次 — 123 的泛化
 * → 309: 含冷冻期 — 买入时看 dp[i-2][0]
 * → 714: 含手续费 — 卖出时减 fee
 */

// ============================================================
// 解法1: 暴力枚举 — 枚举所有 (买入日, 卖出日) 对
// 时间: O(n²)  空间: O(1)
//
// 【思路】
// 最直觉的方法：试遍所有可能的买入日 i 和卖出日 j (j > i)，
// 计算利润 prices[j] - prices[i]，取最大值。
//
// 瓶颈：对每个 j，都要回头扫描 [0, j-1] 找最小值，大量重复。
// n = 10^5 时约 10^10 次运算，会超时。
// ============================================================
class Solution1 {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        int maxProfit = 0;  // 初始化为 0：不交易的利润，兜底负利润的情况
        
        for (int i = 0; i < n; i++) {           // 枚举买入日
            for (int j = i + 1; j < n; j++) {    // 枚举卖出日
                int profit = prices[j] - prices[i];
                maxProfit = max(maxProfit, profit);
            }
        }
        
        return maxProfit;
    }
};

// ============================================================
// 解法2: 一次遍历（维护历史最低价） — 面试首选 ⭐
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 解法1的瓶颈是"对每个卖出日，重新搜索之前的最低价"。
// 
// 关键观察：从左往右遍历时，用 minPrice 变量记录"到目前为止的最低价"。
// 当遍历到第 j 天时，minPrice 就是 [0, j] 中的最小值。
// 这样，第 j 天卖出的最大利润 = prices[j] - minPrice，O(1) 搞定。
//
// 遍历过程 (prices = [7, 1, 5, 3, 6, 4]):
//
//   day | price | minPrice | profit | maxProfit
//   ----+-------+----------+--------+----------
//    0  |   7   |    7     |   0    |    0
//    1  |   1   |    1     |   0    |    0
//    2  |   5   |    1     |   4    |    4
//    3  |   3   |    1     |   2    |    4
//    4  |   6   |    1     |   5    |    5     ← 答案
//    5  |   4   |    1     |   3    |    5
// ============================================================
class Solution2 {
public:
    int maxProfit(vector<int>& prices) {
        int minPrice = INT_MAX;   // 历史最低价，初始化为极大值
        int maxProfit = 0;        // 最大利润，初始化为 0（不交易）
        
        for (int price : prices) {
            // 先更新历史最低价（今天是否是更好的买入时机？）
            minPrice = min(minPrice, price);
            // 再计算今天卖出的利润（一定 >= 0，因为 price >= minPrice）
            maxProfit = max(maxProfit, price - minPrice);
        }
        
        return maxProfit;
    }
};

// ============================================================
// 解法3: 动态规划（状态机框架） — 股票系列通用解法
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 本题用解法2就够了，但状态机 DP 是股票系列 6 道题的通用框架。
// 掌握了这个框架，后续题目只需修改状态转移方程的一两行。
//
// 状态定义:
//   dp[i][0] = 第 i 天结束时，不持有股票的最大利润
//   dp[i][1] = 第 i 天结束时，持有股票的最大利润
//
// 状态转移:
//   dp[i][0] = max(dp[i-1][0],             // 前一天就不持有，继续不动
//                  dp[i-1][1] + prices[i])  // 前一天持有，今天卖出
//
//   dp[i][1] = max(dp[i-1][1],             // 前一天就持有，继续持有
//                  0 - prices[i])           // 今天买入(只能买一次，买入前利润=0)
//               ↑ 关键区别！
//               Stock II 改成: dp[i-1][0] - prices[i]（可以基于之前的利润买入）
//
// 初始化:
//   dp[0][0] = 0            第0天不买
//   dp[0][1] = -prices[0]   第0天买入
//
// DP 填表过程 (prices = [7, 1, 5, 3, 6, 4]):
//
//          dp[i][0]  dp[i][1]
//   day0:     0        -7       (不买 / 买入价格7)
//   day1:     0        -1       (不变 / 买入价格1更划算)
//   day2:     4        -1       (卖出: -1+5=4 / 保持-1)
//   day3:     4        -1       (保持4 / 保持-1)
//   day4:     5        -1       (卖出: -1+6=5 / 保持-1)
//   day5:     5        -1       (保持5 / 保持-1)
//
//   答案: dp[5][0] = 5
// ============================================================
class Solution3 {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        vector<vector<int>> dp(n, vector<int>(2));
        
        dp[0][0] = 0;
        dp[0][1] = -prices[0];
        
        for (int i = 1; i < n; i++) {
            // 不持股：保持 or 卖出
            dp[i][0] = max(dp[i-1][0], dp[i-1][1] + prices[i]);
            // 持股：保持 or 买入（只能买一次，所以从 0 开始扣）
            dp[i][1] = max(dp[i-1][1], 0 - prices[i]);
            // ↑ 系列题差异点:
            //   Stock I  (121): 0 - prices[i]              （只买一次）
            //   Stock II (122): dp[i-1][0] - prices[i]     （可多次买卖）
            //   Stock+冷冻(309): dp[i-2][0] - prices[i]    （卖出后冷冻1天）
            //   Stock+手续费(714): 卖出时减 fee             （dp[i-1][1]+prices[i]-fee）
        }
        
        return dp[n-1][0];  // 最后一天不持股的利润 >= 持股的利润
    }
};

// ============================================================
// 解法3b: 状态机 DP 空间优化 — O(n) / O(1)
//
// 【思路】
// dp[i] 只依赖 dp[i-1]，所以用两个滚动变量即可。
// 优化后会发现：noStock 就是 maxProfit，hasStock 就是 -minPrice。
// 所以解法2和解法3b本质上是同一回事！
// ============================================================
class Solution3b {
public:
    int maxProfit(vector<int>& prices) {
        int noStock = 0;             // dp[i][0]: 不持股
        int hasStock = -prices[0];   // dp[i][1]: 持股
        
        for (int i = 1; i < (int)prices.size(); i++) {
            noStock = max(noStock, hasStock + prices[i]);
            hasStock = max(hasStock, -prices[i]);
            // hasStock = max(hasStock, -prices[i]) 
            // 等价于 hasStock = -min(-hasStock, prices[i]) = -minPrice
            // noStock = max(noStock, -minPrice + prices[i]) = maxProfit
            // 与解法2完全等价！
        }
        
        return noStock;
    }
};

// ============================================================
// 【解法对比】
//
// 解法1 (暴力):     理解题意用，面试中用 30 秒讲清楚即可
// 解法2 (一次遍历):  面试首选，代码短、思路清晰、O(n)/O(1) 最优
// 解法3 (状态机DP):  本题杀鸡用牛刀，但是股票系列 6 道题的通用模板
//                    面试追问"如果可以多次交易？"时展示这个框架加分
// 解法3b (DP优化):   展示 DP 空间优化 + 与解法2的等价关系
//
// 【易错点】
// 1. minPrice 初始化为 0 而非 INT_MAX
//    ✗ int minPrice = 0;
//      → 第一天 price=7, profit = 7-0 = 7, 凭空赚钱
//    ✓ int minPrice = INT_MAX;
//      → 第一天一定更新 minPrice = prices[0]
//
// 2. maxProfit 初始化为 INT_MIN 导致溢出
//    ✗ int maxProfit = INT_MIN; 
//      如果先算 profit = prices[0] - INT_MAX 就溢出了
//    ✓ int maxProfit = 0;
//      不交易利润为 0，天然处理"无利可图"的情况
//
// 3. DP 版本买入转移写错（这是系列题最常见的 bug）
//    ✗ dp[i][1] = max(dp[i-1][1], dp[i-1][0] - prices[i]);
//      → 这是 Stock II（多次交易），不是 Stock I
//    ✓ dp[i][1] = max(dp[i-1][1], 0 - prices[i]);
//      → 只能买一次，买入前利润必须是 0
//
// 4. 返回值搞混
//    ✗ return dp[n-1][1];  // 最后一天还持有股票
//    ✓ return dp[n-1][0];  // 最后一天应该不持有，利润更大
//
// 【面试追问】
//
// Q1: 暴力 O(n²) 的瓶颈在哪？怎么优化？
//   → 对每个卖出日重复搜索之前最低价。用一个变量维护即可，O(n²) → O(n)。
//
// Q2: 一次遍历为什么不会漏解？
//   → 最优解 = 某天卖出 + 该天之前的最低价买入。
//     遍历到每一天时，minPrice 精确记录了之前的最低价，所有卖出日都被检查过。
//
// Q3: 改成"可以多次交易"(Stock II, LC 122) 怎么做？
//   → 贪心: 只要 prices[i] > prices[i-1] 就赚差价
//     DP: 买入转移从 "0 - prices[i]" 改成 "dp[i-1][0] - prices[i]"
//     即允许在之前卖出获利的基础上再次买入。
//
// Q4: 改成"最多 k 次交易"(Stock IV, LC 188) 怎么扩展？
//   → 增加维度: dp[i][j][0/1]，j 表示已完成的交易次数 (0~k)
//     买入时消耗一次交易: dp[i][j][1] = max(dp[i-1][j][1], dp[i-1][j-1][0] - prices[i])
//     n*k 的状态空间，O(nk) 时间。
// ============================================================
