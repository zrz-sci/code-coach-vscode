/*
 * LeetCode 714: Best Time to Buy and Sell Stock with Transaction Fee
 * 
 * 【题目本质】
 * 无限次交易+每次卖出扣手续费，求最大利润。
 * 是 Stock II (122) 的直接变体，转移方程仅多减一个 fee。
 * 
 * 【系列定位】
 * Stock I (121)   — 只能一次交易
 * Stock II (122)  — 无限次交易，无手续费
 * Stock III (123) — 最多2次交易
 * Stock IV (188)  — 最多k次交易
 * Cooldown (309)  — 无限次，卖后冷却1天
 * Fee (714)       — 无限次，每次扣手续费  ← 本题
 * 
 * 与 Stock II 的代码差异仅一行:
 *   Stock II:  cash = max(cash, hold + prices[i])
 *   本题:      cash = max(cash, hold + prices[i] - fee)  ← 唯一区别
 * 
 * 【解法总览】
 * 解法1: DP 数组       — O(n) / O(n) — 最清晰，展示填表过程
 * 解法2: DP 空间优化   — O(n) / O(1) — 面试首选
 * 解法3: 贪心          — O(n) / O(1) — 加分项，需理解反悔机制
 */

// ============================================================
// 解法1: DP 数组 — 两状态经典框架
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 每天结束只有两种状态:
//   cash[i] = 第i天结束不持有股票的最大利润
//   hold[i] = 第i天结束持有股票的最大利润
//
// 转移方程:
//   cash[i] = max(cash[i-1],        // 昨天就不持有，今天不操作
//                 hold[i-1] + prices[i] - fee)  // 今天卖出(扣手续费)
//   hold[i] = max(hold[i-1],        // 昨天就持有，今天不操作
//                 cash[i-1] - prices[i])        // 今天买入
//
// DP 填表过程 (prices = [1,3,2,8,4,9], fee = 2):
//
//   day:    0    1    2    3    4    5
//   cash:   0    0    0    5    5    8
//   hold:  -1   -1   -1   -1    1    1
//           ↑    ↑    ↑    ↑    ↑    ↑
//          买入  观望  观望  卖出  买入  卖出
//
//   cash[3] = max(cash[2]=0, hold[2]+8-2) = max(0, -1+6) = 5  ← 卖出
//   hold[4] = max(hold[3]=-1, cash[3]-4)  = max(-1, 5-4) = 1  ← 买入
//   cash[5] = max(cash[4]=5, hold[4]+9-2) = max(5, 1+7)  = 8  ← 卖出
//
//   答案: cash[5] = 8
// ============================================================
class Solution1 {
public:
    int maxProfit(vector<int>& prices, int fee) {
        int n = prices.size();
        vector<int> cash(n), hold(n);
        
        // 初始化: 第0天不买利润为0，买入利润为负
        cash[0] = 0;
        hold[0] = -prices[0];
        
        for (int i = 1; i < n; i++) {
            // 不持有: 延续昨天 vs 今天卖出(减手续费)
            cash[i] = max(cash[i - 1], hold[i - 1] + prices[i] - fee);
            // 持有: 延续昨天 vs 今天买入
            hold[i] = max(hold[i - 1], cash[i - 1] - prices[i]);
        }
        
        // 最后一天不持有 >= 持有(卖掉总是不亏)
        return cash[n - 1];
    }
};

// ============================================================
// 解法2: DP 空间优化 — 滚动变量 (面试首选)
// 时间: O(n)  空间: O(1)
//
// 【思路】
// cash[i] 和 hold[i] 只依赖 i-1 天的值，
// 所以不需要数组，两个变量滚动更新即可。
//
// 依赖关系:
//   cash_new ← cash_old, hold_old
//   hold_new ← hold_old, cash_old
// 两者都依赖"旧值"，所以用临时变量保存新值后再赋值。
//
// 注意: 实际上先更新 cash 再更新 hold 也是正确的——
// 因为 cash 变大后 hold = max(hold, cash - price) 只会变大或不变，
// 而"用更大的cash买入"等价于"先卖再买"是合法操作。
// 但面试时用临时变量更安全、更好解释。
// ============================================================
class Solution2 {
public:
    int maxProfit(vector<int>& prices, int fee) {
        int cash = 0;
        int hold = -prices[0];
        
        for (int i = 1; i < (int)prices.size(); i++) {
            // 用临时变量保存新值，避免覆盖问题
            int newCash = max(cash, hold + prices[i] - fee);
            int newHold = max(hold, cash - prices[i]);
            cash = newCash;
            hold = newHold;
        }
        
        return cash;
    }
};

// ============================================================
// 解法3: 贪心 — 有效买入成本 + 反悔机制
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 核心洞察: 把手续费合并到买入成本中。
// 定义 minCost = 当前有效的"买入价 + fee"。
//
// 遍历每一天:
// 1) prices[i] + fee < minCost → 发现更便宜的买入点，更新 minCost
// 2) prices[i] > minCost      → 卖出有利润，累加利润
//    关键: 卖出后 minCost = prices[i] (不加fee!)
//    这是"反悔"机制 — 如果明天价格继续涨，
//    相当于今天没卖，只需补差价。
//    因为这笔交易的fee已经在minCost里付过了。
// 3) 否则什么都不做。
//
// 模拟过程 (prices = [1,3,2,8,4,9], fee = 2):
//
//   i=0: minCost = 1+2 = 3, profit = 0
//   i=1: price=3, 3==minCost → 不买不卖
//   i=2: price=2, 2+2=4 > 3 → 不更新; 2 < 3 → 不卖 → 什么都不做
//   i=3: price=8, 8 > 3 → profit += 8-3=5, minCost=8
//        (如果后面更高, 相当于今天没卖)
//   i=4: price=4, 4+2=6 < 8 → minCost=6 (找到更低买入点)
//   i=5: price=9, 9 > 6 → profit += 9-6=3, minCost=9
//
//   总利润 = 5+3 = 8 ✓
// ============================================================
class Solution3 {
public:
    int maxProfit(vector<int>& prices, int fee) {
        int profit = 0;
        int minCost = prices[0] + fee;  // 有效买入成本 = 价格 + 手续费
        
        for (int i = 1; i < (int)prices.size(); i++) {
            if (prices[i] + fee < minCost) {
                // 情况1: 更低的买入成本
                minCost = prices[i] + fee;
            } else if (prices[i] > minCost) {
                // 情况2: 卖出有利可图
                profit += prices[i] - minCost;
                // 反悔机制: minCost = prices[i] 而不是 prices[i] + fee
                // 因为如果明天价格更高, 相当于今天没卖,
                // fee 已经在最初的 minCost 里扣过了, 不需要再扣
                minCost = prices[i];
            }
            // 情况3: minCost-fee <= prices[i] <= minCost → 什么都不做
        }
        
        return profit;
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间 | 空间 | 优点              | 缺点              |
// |-----------|------|------|-------------------|-------------------|
// | DP 数组    | O(n) | O(n) | 最清晰，可打印填表 | 空间多余           |
// | DP 空间优化 | O(n) | O(1) | 面试最佳选择      | 需注意覆盖问题      |
// | 贪心       | O(n) | O(1) | 代码最短          | 反悔机制不易理解    |
//
// 推荐: 面试先说DP思路(展示思维框架)，然后写空间优化版(展示优化能力)。
//       如果面试官追问能否不用DP，再给出贪心解法。
//
// 【系列代码差异对照】
//
// Stock II (122):
//   cash = max(cash, hold + prices[i])         // 无手续费
//   hold = max(hold, cash - prices[i])
//
// Stock with Fee (714) 本题:
//   cash = max(cash, hold + prices[i] - fee)   // ← 只改这一行，多减 fee
//   hold = max(hold, cash - prices[i])
//
// Stock with Cooldown (309):
//   cash = max(cash, hold + prices[i])
//   hold = max(hold, prev_cash - prices[i])    // ← 用前天的cash，不是昨天的
//   // 需要额外变量保存 cash 的前一天值
//
// 【易错点】
//
// 1. 手续费扣两次:
//    ✗ cash = hold + price - fee; hold = cash - price - fee;
//      → 买入扣了fee，卖出又扣了fee，每笔交易扣了两次
//    ✓ 只在卖出时扣: cash = hold + price - fee; hold = cash - price;
//      或只在买入时扣: cash = hold + price; hold = cash - price - fee;
//
// 2. hold 初始化为 0:
//    ✗ hold = 0 → 第0天持有股票利润不可能是0
//    ✓ hold = -prices[0] → 第0天买入，利润为负
//
// 3. 贪心反悔时多加了 fee:
//    ✗ 卖出后 minCost = prices[i] + fee → 导致连续上涨时每天都扣fee
//    ✓ 卖出后 minCost = prices[i] → fee在最初买入时已包含在minCost中
//
// 4. DP空间优化覆盖旧值:
//    ✗ cash = max(cash, hold + prices[i] - fee);
//       hold = max(hold, cash - prices[i]);  // 这里的cash已经是新值!
//    ✓ 用临时变量: newCash = ...; newHold = ...; cash=newCash; hold=newHold;
//
// 【面试追问】
//
// Q1(基础): "这道题和Stock II的区别是什么？代码改了哪里？"
//   → 只改了cash的转移方程，多减一个fee。思路完全一样。
//
// Q2(优化): "能不能用贪心解？贪心的正确性怎么保证？"
//   → 见解法3。正确性：fee并入买入成本后，反悔机制保证了
//     连续上涨时只扣一次fee（等效于在最高点卖出）。
//
// Q3(变体): "如果同时有手续费和冷却期呢？贪心还能用吗？"
//   → 贪心难以处理冷却期，因为卖出后不能立即买入，破坏了
//     反悔机制的前提。此时必须用DP，增加一个"冷却"状态:
//     hold[i] = max(hold[i-1], cool[i-1] - prices[i])
//     cash[i] = max(cash[i-1], hold[i-1] + prices[i] - fee)
//     cool[i] = cash[i-1]  // 冷却 = 昨天刚卖出
// ============================================================
