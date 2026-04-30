/*
 * LeetCode 309: Best Time to Buy and Sell Stock with Cooldown
 *
 * 【题目本质】
 * 无限次买卖股票，但卖出后需冷冻1天才能买入。
 * 本质是带状态约束的多阶段决策问题 → 状态机 DP。
 *
 * 【系列题定位】
 * 122(无限次无冷冻) → 309(本题，加冷冻期) → 714(加手续费)
 * 和 122 的唯一区别：把"不持有"拆成"刚卖出(sold)"和"空闲(rest)"
 * 买入只能从 rest 状态转移，不能从 sold 状态转移 ← 冷冻期约束
 *
 * 【状态转移图】
 *
 *        buy (花钱)
 *  rest ────────────→ hold
 *   ↑                   │
 *   │ cooldown          │ sell (收钱)
 *   │ (等一天)          ↓
 *  sold ←────────────  hold
 *   │
 *   └──→ rest (冷冻期结束，回到空闲)
 *
 * 【解法总览】
 * 解法1: 暴力递归 (DFS)          — O(3^n) / O(n)   — 理解问题
 * 解法2: 记忆化搜索              — O(n)   / O(n)   — 消除重叠子问题
 * 解法3: 状态机 DP (数组版)       — O(n)   / O(n)   — 面试首选
 * 解法4: 状态机 DP (滚动变量)     — O(n)   / O(1)   — 空间最优
 */

// ============================================================
// 解法1: 暴力递归 (DFS)
// 时间: O(3^n)  空间: O(n) 递归栈
//
// 【思路】
// 每天处于三种状态之一: rest(空闲), hold(持有), sold(刚卖出)
// 从每种状态出发枚举所有可能的决策:
//   rest → 买入(转hold) 或 跳过(留rest)
//   hold → 卖出(转sold) 或 跳过(留hold)
//   sold → 强制冷冻一天(转rest)
//
// 为什么从这里开始？因为它直接展示了状态和转移的含义，
// 是后续所有优化的出发点。
// ============================================================
class Solution1 {
public:
    int maxProfit(vector<int>& prices) {
        return dfs(prices, 0, 0);
    }

private:
    // state: 0=rest, 1=hold, 2=sold
    int dfs(vector<int>& prices, int i, int state) {
        if (i >= (int)prices.size()) return 0;

        if (state == 0) { // rest: 可以买入或继续空闲
            int buy  = dfs(prices, i + 1, 1) - prices[i];
            int skip = dfs(prices, i + 1, 0);
            return max(buy, skip);
        } else if (state == 1) { // hold: 可以卖出或继续持有
            int sell = dfs(prices, i + 1, 2) + prices[i];
            int skip = dfs(prices, i + 1, 1);
            return max(sell, skip);
        } else { // sold: 冷冻期，必须休息一天
            return dfs(prices, i + 1, 0);
        }
    }
};

// ============================================================
// 解法2: 记忆化搜索 — 从解法1消除重叠子问题
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 解法1的瓶颈：大量重复计算。例如 dfs(3, rest) 可能被调用多次。
// 总共只有 n × 3 种 (i, state) 组合，用 memo 缓存即可。
// 这是从暴力到 DP 的标准过渡步骤。
// ============================================================
class Solution2 {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        vector<vector<int>> memo(n, vector<int>(3, -1));
        return dfs(prices, 0, 0, memo);
    }

private:
    int dfs(vector<int>& prices, int i, int state, vector<vector<int>>& memo) {
        if (i >= (int)prices.size()) return 0;
        if (memo[i][state] != -1) return memo[i][state];

        int res;
        if (state == 0) {
            res = max(dfs(prices, i + 1, 1, memo) - prices[i],
                      dfs(prices, i + 1, 0, memo));
        } else if (state == 1) {
            res = max(dfs(prices, i + 1, 2, memo) + prices[i],
                      dfs(prices, i + 1, 1, memo));
        } else {
            res = dfs(prices, i + 1, 0, memo);
        }

        return memo[i][state] = res;
    }
};

// ============================================================
// 解法3: 状态机 DP (数组版) — 面试首选 ⭐
// 时间: O(n)  空间: O(n)
//
// 【思路】
// 把记忆化搜索翻译成自底向上的递推。
// 定义三个数组：
//   hold[i] = 第i天结束时持有股票的最大利润
//   sold[i] = 第i天结束时刚卖出股票的最大利润
//   rest[i] = 第i天结束时空闲（不持有且非冷冻）的最大利润
//
// 转移方程：
//   hold[i] = max(hold[i-1], rest[i-1] - prices[i])
//              ↑ 继续持有     ↑ 从空闲状态买入（冷冻期约束在这里！）
//
//   sold[i] = hold[i-1] + prices[i]
//              ↑ 昨天持有，今天卖出
//
//   rest[i] = max(rest[i-1], sold[i-1])
//              ↑ 继续空闲     ↑ 昨天刚卖出，今天冷冻期结束
//
// 【DP 填表示例: prices = [1, 2, 3, 0, 2]】
//
// day     |  0    1    2    3    4
// price   |  1    2    3    0    2
// --------|---------------------------
// hold    | -1   -1   -1    1    1
// sold    |  0    1    2   -1    3
// rest    |  0    0    1    2    2
//
// 答案 = max(sold[4], rest[4]) = max(3, 2) = 3
//
// 【和 122 题的代码差异】
// 122 题: hold[i] = max(hold[i-1], cash[i-1] - prices[i])   ← cash = 唯一的"不持有"状态
// 本题:   hold[i] = max(hold[i-1], rest[i-1] - prices[i])   ← rest 是"不持有且非冷冻"
//         多了 sold 和 rest 的区分，这就是冷冻期的全部代码差异
// ============================================================
class Solution3 {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        if (n <= 1) return 0;

        vector<int> hold(n), sold(n), rest(n);

        // 第0天初始化
        hold[0] = -prices[0];  // 买入花了 prices[0]，利润为负
        sold[0] = 0;           // 第0天不可能刚卖出
        rest[0] = 0;           // 什么都不做

        for (int i = 1; i < n; i++) {
            // 今天持有 = 昨天就持有 OR 昨天空闲+今天买入
            hold[i] = max(hold[i - 1], rest[i - 1] - prices[i]);

            // 今天刚卖出 = 昨天持有+今天卖出收钱
            sold[i] = hold[i - 1] + prices[i];

            // 今天空闲 = 昨天就空闲 OR 昨天刚卖出(冷冻期过了)
            rest[i] = max(rest[i - 1], sold[i - 1]);
        }

        // 最终答案：不持有股票的两种情况取最大值
        // （持有股票意味着还没套现，不可能是最优）
        return max(sold[n - 1], rest[n - 1]);
    }
};

// ============================================================
// 解法4: 状态机 DP (滚动变量) — 空间最优
// 时间: O(n)  空间: O(1)
//
// 【思路】
// 解法3中 hold[i], sold[i], rest[i] 只依赖 i-1 的值，
// 不需要整个数组，用三个变量滚动即可。
//
// 【关键陷阱】
// 更新顺序！三个变量相互依赖旧值：
//   新hold 依赖 旧rest
//   新sold 依赖 旧hold
//   新rest 依赖 旧sold
// 如果先更新 hold，再用新 hold 算 sold，结果就错了。
// 解决方案：用临时变量保存旧值。
// ============================================================
class Solution4 {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        if (n <= 1) return 0;

        int hold = -prices[0];  // 持有
        int sold = 0;           // 刚卖出
        int rest = 0;           // 空闲

        for (int i = 1; i < n; i++) {
            // 必须先保存旧值，因为三个变量互相依赖
            int prev_hold = hold;
            int prev_sold = sold;
            int prev_rest = rest;

            hold = max(prev_hold, prev_rest - prices[i]);  // 继续持有 or 买入
            sold = prev_hold + prices[i];                   // 卖出
            rest = max(prev_rest, prev_sold);               // 继续空闲 or 冷冻期结束
        }

        return max(sold, rest);
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间   | 空间  | 核心区别                    |
// |------------|--------|-------|-----------------------------|
// | 暴力递归   | O(3^n) | O(n)  | 直接枚举，有大量重复计算    |
// | 记忆化搜索 | O(n)   | O(n)  | 加 memo 消除重复            |
// | DP 数组版  | O(n)   | O(n)  | 自底向上递推，更直观        |
// | DP 滚动版  | O(n)   | O(1)  | 只保留前一天的三个值        |
//
// 面试建议：先画状态转移图，直接写解法3（数组版），
// 如果面试官追问空间优化，改成解法4。
//
// ============================================================
// 【易错点】
//
// 1. hold 初始化为 0 而不是 -prices[0]
//    ✗ hold[0] = 0       → 相当于"免费"拿到了股票
//    ✓ hold[0] = -prices[0] → 买入要花钱，利润为负
//
// 2. 滚动变量更新顺序错误（最常见 bug）
//    ✗ hold = max(hold, rest - prices[i]);
//      sold = hold + prices[i];   // BUG: hold 已经是新值了！
//    ✓ 用 prev_hold/prev_sold/prev_rest 保存旧值后再更新
//
// 3. 冷冻期约束写错
//    ✗ hold[i] = max(hold[i-1], sold[i-1] - prices[i])
//      → 卖出后立刻买入，没有冷冻期！
//    ✓ hold[i] = max(hold[i-1], rest[i-1] - prices[i])
//      → 只能从 rest 状态买入
//
// 4. 返回值只返回 sold 忘了 rest
//    ✗ return sold[n-1]  → 如果最优策略不交易就漏了
//    ✓ return max(sold[n-1], rest[n-1])
//
// ============================================================
// 【面试追问（递进链）】
//
// Q1: 为什么需要三个状态而不是两个？
// A1: 两个状态(hold/not_hold)时，not_hold 无法区分"刚卖出(明天冷冻)"
//     和"早就卖了(明天可买入)"，会导致冷冻期约束无法实现。
//
// Q2: 能否 O(1) 空间？
// A2: 可以，每天只依赖前一天三个值，用三个滚动变量即可。
//     注意用临时变量保存旧值防止更新顺序 bug。
//
// Q3: 如果冷冻期变成 k 天怎么办？
// A3: 方案1: sold 后面接 k 个 cooldown 阶段，只有最后一个阶段能转到 rest。
//     方案2: 更简洁地，hold[i] = max(hold[i-1], sold[i-k-1] - prices[i])，
//     即只能从 k+1 天前的 sold 状态买入。需要保留 sold 的完整数组。
//
// Q4: 这道题能用贪心吗？
// A4: 不能。冷冻期引入了"后效性"——今天卖不卖影响后天能不能买。
//     例如 [1,2,3,0,2]，贪心在 day1 卖出赚1，但最优是 day2 卖出赚2。
//     后效性问题必须用 DP 全局考虑。
// ============================================================
