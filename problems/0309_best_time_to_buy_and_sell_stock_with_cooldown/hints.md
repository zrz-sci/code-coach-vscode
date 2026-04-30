# 309. 买卖股票的最佳时机含冷冻期

## 核心思路

这道题本质上是**带状态约束的多阶段决策问题**：每天可以买入/卖出/什么都不做，但卖出后下一天不能买入（冷冻期），求最终最大利润。关键在于定义清楚每天可能处于哪些**状态**，以及状态之间如何转移。

## 系列题定位

本题属于「股票买卖」系列：
- **122. 买卖股票 II**：无限次交易，无冷冻期 → 每天两个状态（持有/不持有）
- **309. 本题**：无限次交易，**卖出后冷冻1天** → 需要把"不持有"拆成两个子状态
- **714. 买卖股票含手续费**：无限次交易，每次交易扣手续费 → 在122基础上卖出时减 fee

和 122 题的核心区别：**122 的"不持有"状态不需要区分"刚卖出"和"卖出已过一天"，而本题必须区分，因为"刚卖出"的下一天不能买入。**

## 思维链

1. **读完题第一反应**：可以无限次买卖，但卖出后隔一天才能买入。暴力做法是枚举所有可能的买卖时间点组合，指数级复杂度，不可行。

2. **尝试 DP 思路**：每天的决策取决于"当前持有不持有"以及"是否在冷冻期"，这是典型的**状态机 DP**。

3. **定义状态**：关键问题——需要几个状态？
   - 直觉上想用两个：持有股票 / 不持有股票
   - 但"不持有"有两种情况：① 今天刚卖出（明天冷冻）② 早就卖了或从没买过（明天可买）
   - 所以需要**三个状态**：
     - `hold`：持有股票
     - `sold`：今天刚卖出（触发冷冻期）
     - `rest`：不持有且不在冷冻期（可以买入）

4. **推导转移方程**：
   ```
   今天 hold = max(昨天 hold, 昨天 rest - price)   // 继续持有 或 从rest状态买入
   今天 sold = 昨天 hold + price                    // 今天卖出
   今天 rest = max(昨天 rest, 昨天 sold)            // 继续rest 或 从sold进入rest(冷冻期过了)
   ```

5. **初始化**：第0天可以买入(`hold = -prices[0]`)，不可能刚卖出(`sold = 0` 但更合理是 `sold = 0` 表示没卖也没亏)，rest 初始利润0。

6. **空间优化**：每天只依赖前一天的三个值，所以不需要数组，三个变量滚动即可。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力递归 | 枚举每天的决策，DFS | O(3^n) | O(n) | 能说出即可 |
| 记忆化搜索 | 递归 + memo，自顶向下 | O(n) | O(n) | 能说出即可 |
| 状态机 DP（数组） | 三状态递推，自底向上 | O(n) | O(n) | ⭐ 必须写出 |
| 状态机 DP（滚动变量） | 空间优化到 O(1) | O(n) | O(1) | 加分项 |

## 关键提示

1. **状态怎么定？** 想想每天结束后你可能处于哪几种"姿态"——持有股票、刚卖出、空闲中。画出状态转移图再写代码。

2. **为什么不能只用两个状态？** 如果只用 hold/not_hold，在 not_hold 状态下你不知道"昨天是否卖出"，就无法判断今天能否买入。

3. **状态转移图（必须画出来再写代码）**：
   ```
          buy (from rest)
    rest ─────────────→ hold
     ↑                    │
     │ (cooldown)   sell  │
     │                    ↓
    sold ←────────────── hold
     │
     └──→ rest (冷冻期结束)
   ```

4. **初始化陷阱**：`hold` 初始值不能是 0，应该是 `-prices[0]`（买入要花钱）。`sold` 初始值是 0 还是 `-INF`？想清楚第0天能不能处于"刚卖出"状态。

5. **和 122 题的代码差异**：122 题只需要 `hold` 和 `cash` 两个变量，本题把 `cash` 拆成 `sold` 和 `rest`，转移方程多了一行。

## 解法详解

### 解法1: 暴力递归（DFS）— O(3^n) / O(n)

**思考过程**：每天有三种决策（买/卖/什么都不做），用递归枚举所有可能，传入当前天数和当前状态。这是理解问题的起点。

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        // 从第0天、rest状态开始
        return dfs(prices, 0, 0); // state: 0=rest, 1=hold, 2=sold
    }
    
    // 返回从第i天开始、处于state状态时的最大利润
    int dfs(vector<int>& prices, int i, int state) {
        if (i >= prices.size()) return 0;
        
        if (state == 0) { // rest: 可以买入或继续rest
            int buy  = dfs(prices, i + 1, 1) - prices[i]; // 买入，花钱
            int skip = dfs(prices, i + 1, 0);              // 什么都不做
            return max(buy, skip);
        } 
        else if (state == 1) { // hold: 可以卖出或继续持有
            int sell = dfs(prices, i + 1, 2) + prices[i];  // 卖出，收钱
            int skip = dfs(prices, i + 1, 1);              // 继续持有
            return max(sell, skip);
        } 
        else { // sold: 冷冻期，只能进入rest
            return dfs(prices, i + 1, 0);                  // 强制rest一天
        }
    }
};
```

**关键点**：这个解法指数级复杂度，但它**清晰地展示了三个状态和转移逻辑**，是后续所有优化的基础。

### 解法2: 记忆化搜索 — O(n) / O(n)

**从解法1优化**：解法1有大量重复计算（比如"第3天处于rest状态"会被算很多次）。加一个 memo 数组缓存即可。

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        // memo[i][state]：从第i天、state状态开始的最大利润，-1表示未计算
        vector<vector<int>> memo(n, vector<int>(3, -1));
        return dfs(prices, 0, 0, memo);
    }
    
    int dfs(vector<int>& prices, int i, int state, vector<vector<int>>& memo) {
        if (i >= prices.size()) return 0;
        if (memo[i][state] != -1) return memo[i][state];
        
        int res;
        if (state == 0) { // rest
            res = max(dfs(prices, i + 1, 1, memo) - prices[i],  // 买入
                      dfs(prices, i + 1, 0, memo));              // 跳过
        } else if (state == 1) { // hold
            res = max(dfs(prices, i + 1, 2, memo) + prices[i],  // 卖出
                      dfs(prices, i + 1, 1, memo));              // 持有
        } else { // sold → 冷冻一天
            res = dfs(prices, i + 1, 0, memo);
        }
        
        return memo[i][state] = res;
    }
};
```

**关键点**：状态总数 = n × 3，每个状态只算一次，所以时间 O(n)。

### 解法3: 状态机 DP（数组版）— O(n) / O(n) ⭐ 面试首选

**从记忆化搜索翻译成递推**：自底向上填表，更直观也更快（省去递归开销）。

```
// DP 填表方向：从左到右，每天更新三个状态
//
// 以 prices = [1, 2, 3, 0, 2] 为例:
//
// day     |  0    1    2    3    4
// price   |  1    2    3    0    2
// --------|---------------------------
// hold    | -1   -1   -1    1    1    ← 持有股票时的最大利润
// sold    |  0    1    2   -1    3    ← 今天刚卖出的最大利润
// rest    |  0    0    1    2    2    ← 空闲状态的最大利润
//
// 最终答案 = max(sold[4], rest[4]) = max(3, 2) = 3
// (不可能在 hold 状态结束，因为持有股票等于还没变现)
```

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        if (n <= 1) return 0;
        
        vector<int> hold(n), sold(n), rest(n);
        
        // 初始化第0天
        hold[0] = -prices[0];  // 买入花了 prices[0]
        sold[0] = 0;           // 第0天不可能"刚卖出"，设0（也可设INT_MIN，但0不影响正确性）
        rest[0] = 0;           // 什么都不做，利润0
        
        for (int i = 1; i < n; i++) {
            // 持有 = 昨天就持有 OR 昨天rest+今天买入
            hold[i] = max(hold[i-1], rest[i-1] - prices[i]);
            
            // 今天刚卖 = 昨天持有+今天卖出
            sold[i] = hold[i-1] + prices[i];
            
            // 空闲 = 昨天就空闲 OR 昨天刚卖(冷冻期过了)
            rest[i] = max(rest[i-1], sold[i-1]);
        }
        
        // 最终不持有股票才是最优（持有意味着还没套现）
        return max(sold[n-1], rest[n-1]);
    }
};
```

**关键点**：
- 状态转移的核心是 `hold[i]` 只能从 `rest[i-1]` 买入（不能从 `sold[i-1]`），这就是冷冻期的约束。
- 和 **122 题** 的区别仅此一点：122 题的 `hold[i] = max(hold[i-1], cash[i-1] - prices[i])`，cash 不区分 sold 和 rest。

### 解法4: 状态机 DP（滚动变量）— O(n) / O(1) 🎯 最优

**从解法3空间优化**：每天只依赖前一天的三个值，不需要数组。

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        if (n <= 1) return 0;
        
        int hold = -prices[0];  // 持有
        int sold = 0;           // 刚卖出
        int rest = 0;           // 空闲
        
        for (int i = 1; i < n; i++) {
            // 注意：必须用旧值计算，所以先保存
            int prev_hold = hold;
            int prev_sold = sold;
            int prev_rest = rest;
            
            hold = max(prev_hold, prev_rest - prices[i]);
            sold = prev_hold + prices[i];
            rest = max(prev_rest, prev_sold);
        }
        
        return max(sold, rest);
    }
};
```

**关键点**：必须用临时变量保存旧值！如果直接 `hold = max(hold, rest - prices[i])` 再 `sold = hold + prices[i]`，`hold` 已经被更新了，`sold` 用的就是新的 `hold`，结果错误。

## 解法对比

| | 暴力递归 | 记忆化搜索 | 状态机DP(数组) | 状态机DP(滚动) |
|---|---|---|---|---|
| 时间 | O(3^n) | O(n) | O(n) | O(n) |
| 空间 | O(n)栈 | O(n) | O(n) | **O(1)** |
| 思维难度 | 低 | 低 | 中 | 中 |
| 面试推荐 | 说思路 | 过渡 | ⭐首选 | 追问时写 |

**选择建议**：面试先说清三个状态的含义和转移图，然后直接写解法3（数组版），被追问空间优化再改成解法4。

## 易错点

1. **hold 初始化为 0 而不是 -prices[0]**
   - ✗ `hold = 0` → 第0天买入后利润变成 `0 - prices[1]`？逻辑混乱
   - ✓ `hold = -prices[0]` → 买入就是花钱，利润为负

2. **滚动变量更新顺序错误**
   - ✗ 先更新 hold，再用新 hold 算 sold：
     ```cpp
     hold = max(hold, rest - prices[i]);
     sold = hold + prices[i];  // BUG! hold已经被改了
     ```
   - ✓ 用临时变量保存旧值，或者按正确顺序（sold 先算）：
     ```cpp
     int prev_hold = hold;
     hold = max(hold, rest - prices[i]);
     sold = prev_hold + prices[i];
     rest = max(rest, sold_prev);  // sold_prev 也要保存
     ```

3. **漏掉冷冻期约束**
   - ✗ `hold[i] = max(hold[i-1], sold[i-1] - prices[i])` → 卖出后立刻买入，没有冷冻
   - ✓ `hold[i] = max(hold[i-1], rest[i-1] - prices[i])` → 只能从 rest 状态买入

4. **返回值忘了取 max(sold, rest)**
   - ✗ `return sold[n-1]` → 万一最优策略是不交易（rest），漏了
   - ✓ `return max(sold[n-1], rest[n-1])`

## 面试追问

**Q1: 这道题和 122（无限次交易无冷冻期）的区别是什么？代码改了哪里？**
> 122 题只需要两个状态 hold 和 cash，本题因为冷冻期，需要把 cash 拆成 sold 和 rest。转移方程中，hold 只能从 rest 买入（而不是从任意"不持有"状态买入），这就是唯一的代码差异。

**Q2: 能否用 O(1) 空间？**
> 可以。每天的三个状态只依赖前一天的三个值，用三个滚动变量即可。注意更新时要用旧值，所以需要临时变量保存。见解法4。

**Q3: 如果冷冻期变成 k 天呢？**
> 把 rest 拆成 k 个阶段：`rest0, rest1, ..., rest_{k-1}`，表示"距离上次卖出已过 0, 1, ..., k-1 天"。只有 `rest_{k-1}` 才能转移到 hold（买入）。或者更简洁地，用 `sold[i]` 只转移到 `rest[i+k]`：`hold[i] = max(hold[i-1], rest[i-1] - prices[i])`，`rest[i] = max(rest[i-1], sold[i-k])`。

**Q4: 能否用贪心？**
> 不能。冷冻期使得局部最优（每次有利润就卖）不一定全局最优。例如 `[1,2,3,0,2]`：贪心会在第1天卖出赚1元，但最优是第2天卖出赚2元+第4天卖出赚2元=3元。冷冻期引入了"后效性"，必须用 DP。

## 相关题型

- **121. 买卖股票 I** — 只能交易一次，最简单的基础版，复用"持有/不持有"两状态框架
- **122. 买卖股票 II** — 无限次交易无冷冻期，本题把 II 的 `cash` 拆成 `sold` + `rest` 就是本题
- **714. 买卖股票含手续费** — 无限次交易+手续费，在 122 基础上卖出时 `sold = hold + price - fee`，改一个常数项
- **123. 买卖股票 III** — 最多2次交易，需要增加"交易次数"维度
- **188. 买卖股票 IV** — 最多k次交易，123 的泛化版