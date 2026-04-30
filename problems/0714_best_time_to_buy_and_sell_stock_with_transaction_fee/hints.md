# 714. 买卖股票的最佳时机含手续费

## 核心思路

本质上是**带手续费的无限次交易股票问题**：每天只有两种状态（持有/不持有股票），每次卖出时扣除手续费，求最终最大利润。这是 Stock II（不限次数交易）的变体，区别仅在于卖出时多扣一笔 fee。

## 思维链

1. **读完题第一反应**：每天可以选择买/卖/什么都不做，交易次数不限但每次要扣手续费。暴力方法是枚举所有买卖组合，但这是指数级复杂度。

2. **发现状态结构**：每一天结束后，我只关心两件事——"今天结束时我手里有没有股票"以及"我的最大利润是多少"。这天然是一个**两状态 DP**。

3. **定义状态**：`hold[i]` = 第 i 天结束时**持有**股票的最大利润，`cash[i]` = 第 i 天结束时**不持有**股票的最大利润。

4. **推导转移方程**：
   - `cash[i] = max(cash[i-1], hold[i-1] + prices[i] - fee)` — 要么继续空仓，要么今天卖出（扣手续费）
   - `hold[i] = max(hold[i-1], cash[i-1] - prices[i])` — 要么继续持有，要么今天买入

5. **空间优化**：`cash[i]` 和 `hold[i]` 只依赖上一天的值，所以只需两个变量滚动即可，空间从 O(n) 降到 O(1)。

6. **贪心角度（可选）**：也可以用贪心思维——把 fee 合并到买入成本里，跟踪"有效买入价"，遇到利润就累计，遇到更低价就更新买入价。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| DP 数组 | 两状态 DP，cash/hold 数组 | O(n) | O(n) | 能说出即可 |
| DP 空间优化 | 滚动变量 | O(n) | O(1) | ⭐ 必须写出 |
| 贪心 | 跟踪有效买入成本 | O(n) | O(1) | 加分项 |

## 关键提示

1. **状态只有两种**：任何一天结束后要么"手里有股票"，要么"手里没股票"。所有决策（买/卖/不动）都是在这两个状态之间转移。

2. **手续费怎么处理**：fee 可以加在"卖出"时扣（`hold + price - fee → cash`），也可以加在"买入"时扣（`cash - price - fee → hold`），选一种即可，但不要两边都扣！

3. **和 Stock II 的唯一区别**：Stock II（122题）的转移方程是 `cash[i] = max(cash[i-1], hold[i-1] + prices[i])`，本题只多了一个 `- fee`。

4. **贪心思路的关键洞察**：如果把 fee 并入买入成本，定义 `minCost = 买入价 + fee`，那么问题变成"什么时候 `price > minCost` 就卖出获利"。但难点在于——卖出后如果价格继续涨，需要"反悔"机制。

5. **DP 填表方向**：从左到右，每天只依赖前一天。
   ```
   // 示例: prices = [1, 3, 2, 8, 4, 9], fee = 2
   // day:   0    1    2    3    4    5
   // cash:  0    0    0    5    5    8
   // hold: -1   -1   -1   -1    1    1
   //        ↑买入 ↑观望 ↑观望 ↑卖出 ↑买入 ↑卖出
   ```

## 解法详解

### 解法1: DP 数组 — O(n) / O(n)

**思考过程**：每天结束有两种状态（持有/不持有股票），每种状态的最大利润可以从前一天的两种状态推导出来。这是经典的股票系列 DP 框架。

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices, int fee) {
        int n = prices.size();
        // cash[i]: 第i天结束不持有股票的最大利润
        // hold[i]: 第i天结束持有股票的最大利润
        vector<int> cash(n), hold(n);
        
        cash[0] = 0;              // 第0天不买，利润为0
        hold[0] = -prices[0];     // 第0天买入，花费prices[0]
        
        for (int i = 1; i < n; i++) {
            // 不持有: 昨天就没有 / 今天卖出(扣手续费)
            cash[i] = max(cash[i-1], hold[i-1] + prices[i] - fee);
            // 持有: 昨天就持有 / 今天买入
            hold[i] = max(hold[i-1], cash[i-1] - prices[i]);
        }
        
        // 最后一天不持有股票一定 >= 持有股票
        return cash[n-1];
    }
};
```

**关键点**：初始化 `hold[0] = -prices[0]` 表示第0天买入后"利润为负"，这是正确的——你花了钱还没卖。

### 解法2: DP 空间优化 — O(n) / O(1) ⭐ 面试首选

**从解法1优化**：cash[i] 和 hold[i] 只依赖 cash[i-1] 和 hold[i-1]，所以不需要数组，两个变量滚动即可。

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices, int fee) {
        int cash = 0;             // 不持有股票的最大利润
        int hold = -prices[0];    // 持有股票的最大利润
        
        for (int i = 1; i < (int)prices.size(); i++) {
            // 注意: 这里用的是更新前的 hold，所以顺序无所谓
            // 因为 cash 更新用 old_hold，hold 更新用 old_cash
            // 实际上这里有个微妙点——cash 先更新了会影响 hold 吗？
            // cash 变大只会让 hold 更大（买入更多本金），不会更差
            // 但严谨做法是用临时变量
            int newCash = max(cash, hold + prices[i] - fee);
            int newHold = max(hold, cash - prices[i]);
            cash = newCash;
            hold = newHold;
        }
        
        return cash;
    }
};
```

**关键点**：两个状态互相依赖，更新时用临时变量保存新值，避免覆盖。（实际上本题先更新 cash 再更新 hold 也是正确的，因为用了"更大的 cash"去买入只会让 hold 不变或更优，但面试时用临时变量更保险、更易解释。）

### 解法3: 贪心 — O(n) / O(1)

**思路**：把手续费合并到买入成本中。维护一个"有效买入成本" `minCost = 买入价 + fee`。

- 如果 `prices[i] < minCost - fee`，说明今天的买入价更低，更新买入成本
- 如果 `prices[i] > minCost`，说明卖出有利润，累加利润，**同时把 minCost 更新为 prices[i]**（关键的"反悔"机制——如果明天价格更高，相当于今天没卖）

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices, int fee) {
        int profit = 0;
        int minCost = prices[0] + fee;  // 有效买入成本 = 买入价 + 手续费
        
        for (int i = 1; i < (int)prices.size(); i++) {
            if (prices[i] + fee < minCost) {
                // 发现更低的买入价，更新成本
                minCost = prices[i] + fee;
            } else if (prices[i] > minCost) {
                // 卖出有利可图
                profit += prices[i] - minCost;
                // "反悔"机制: 如果明天价格更高，等于今天没卖
                // 把 minCost 设为 prices[i]（不加fee，因为这笔交易已经付过了）
                minCost = prices[i];
            }
            // 如果 minCost - fee <= prices[i] <= minCost，什么都不做
        }
        
        return profit;
    }
};
```

**贪心的"反悔"机制详解**：

```
// prices = [1, 3, 2, 8, 4, 9], fee = 2
// 
// i=0: minCost = 1+2 = 3
// i=1: price=3, 3 == minCost, 不买不卖
// i=2: price=2, 2+2=4 > 3, 不更新; 2 < 3, 不卖 → 什么都不做
// i=3: price=8, 8 > 3 → profit += 8-3 = 5, minCost = 8
//      (如果明天更高, 相当于今天没卖, 只需补差价)
// i=4: price=4, 4+2=6 < 8 → minCost = 6
//      (比当前持有成本低, 重新买入)
// i=5: price=9, 9 > 6 → profit += 9-6 = 3, minCost = 9
// 
// 总利润 = 5 + 3 = 8 ✓
```

## 解法对比

| | DP 数组 | DP 空间优化 | 贪心 |
|---|---------|-------------|------|
| 时间 | O(n) | O(n) | O(n) |
| 空间 | O(n) | O(1) | O(1) |
| 可读性 | 最清晰 | 清晰 | 需要理解反悔机制 |
| 通用性 | 股票系列通用框架 | 同上 | 仅适用于无限次+手续费 |
| 面试推荐 | 解释思路用 | ⭐ 写代码用 | 加分项 |

**什么时候选哪个？**
- DP 空间优化是面试最佳选择：思路清晰、代码简短、容易扩展到其他股票变体
- 贪心适合展示更深的理解，但不容易扩展到含冷却期等变体

## 易错点

1. **手续费重复扣除**：
   - ✗ 买入时扣 fee，卖出时又扣 fee → 每笔交易扣了两次
   - ✓ 只在一侧扣：要么 `cash = hold + price - fee`，要么 `hold = cash - price - fee`

2. **贪心反悔机制遗漏**：
   - ✗ 卖出后 `minCost = prices[i] + fee`（又加了一次 fee）
   - ✓ 卖出后 `minCost = prices[i]`（这笔交易的 fee 已经在之前的 minCost 里扣过了）

3. **DP 空间优化时覆盖问题**：
   - ✗ `cash = max(cash, hold + prices[i] - fee); hold = max(hold, cash - prices[i]);` — 第二行用了已更新的 cash
   - ✓ 用临时变量保存，或证明更新顺序的正确性

4. **初始化错误**：
   - ✗ `hold = 0`（第0天持有股票利润不可能是0）
   - ✓ `hold = -prices[0]`（第0天买入，利润为负）

## 面试追问

**Q1: 这道题和 122. Stock II 有什么区别？代码改了哪里？**

只改了一行：卖出时多减一个 fee。
- Stock II: `cash = max(cash, hold + prices[i])`
- 本题:     `cash = max(cash, hold + prices[i] - fee)` ← 唯一区别

**Q2: 如果 fee 非常大，会出现什么情况？贪心还能工作吗？**

fee 很大时，很多交易不值得做（卖出利润不够覆盖手续费），最终可能只做一两次交易甚至不交易。DP 和贪心都能正确处理——DP 中 `hold + price - fee` 小于 `cash` 时自然不会卖出；贪心中 `price <= minCost` 时不会触发卖出。

**Q3: 如果还有冷却期（卖出后隔一天才能买入），怎么改？**

在 DP 中多加一个状态维度或多一个变量。买入时不能用 `cash[i-1]`（昨天刚卖出），要用 `cash[i-2]`（前天的不持有状态）。这就是 309. Stock with Cooldown。核心区别：
- 本题: `hold[i] = max(hold[i-1], cash[i-1] - prices[i])`
- 冷却期: `hold[i] = max(hold[i-1], cash[i-2] - prices[i])`

## 相关题型

| 题目 | 与本题的关系 | 代码差异 |
|------|-------------|----------|
| **121. Stock I**（只能一次交易） | 基础版，只需记录最低买入价 | 不需要 DP，一个变量即可 |
| **122. Stock II**（无限次无手续费） | 本题去掉 fee 就是它 | `cash` 转移方程去掉 `- fee` |
| **309. Stock with Cooldown** | 卖出后冷却一天 | `hold` 的转移用 `cash[i-2]` 而非 `cash[i-1]` |
| **123. Stock III**（最多两次交易） | 限制交易次数，增加一维状态 | 两维 DP: `dp[k][state]`, k=0,1,2 |
| **188. Stock IV**（最多 k 次交易） | 通用版本 | 三维→二维 DP: `dp[k][0/1]` |

**系列总结**：所有股票题共享同一个 DP 框架——`cash/hold` 两状态转移，区别只在于限制条件（次数、手续费、冷却期）如何修改转移方程。掌握本题的框架后，其他变体只需调整一两行代码。