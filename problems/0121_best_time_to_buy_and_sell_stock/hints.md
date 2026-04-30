# 121. 买卖股票的最佳时机

## 核心思路

本质上是在数组中找一对 `(i, j)`，其中 `i < j`，使得 `prices[j] - prices[i]` 最大。也就是**在"过去的最低点买入"的前提下，找到未来的最高卖出价**。

## 思维链

1. **读完题第一反应**：枚举所有买卖日的组合 `(i, j)`，算每种利润，取最大值 → 暴力 O(n²)
2. **暴力解的瓶颈在哪？**：对于每个卖出日 `j`，我们遍历了 `j` 之前的所有日子来找最低买入价。这个"找之前最小值"的操作重复了大量计算。
3. **怎么突破？**：如果我从左往右遍历，随时维护一个"到目前为止的最低价格"，那我在第 `j` 天卖出时，最优买入价就是 `minPrice`，利润就是 `prices[j] - minPrice`。
4. **一次遍历搞定**：用一个变量 `minPrice` 追踪历史最低价，用另一个变量 `maxProfit` 追踪最大利润。遍历一次就能得到答案。
5. **DP 视角（系列题基础）**：定义状态 `dp[i][0]` = 第 i 天不持股的最大利润，`dp[i][1]` = 第 i 天持股的最大利润。这是股票系列题的通用框架，虽然本题杀鸡焉用牛刀，但理解了 DP 框架后做 Stock II/III/IV 会非常顺畅。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力枚举 | 枚举所有买卖对 | O(n²) | O(1) | 能说出即可 |
| 一次遍历（贪心/维护最小值） | 维护历史最低价，实时算利润 | O(n) | O(1) | ⭐ 必须写出 |
| 动态规划（状态机） | dp[i][0/1] 持股/不持股 | O(n) | O(n)→O(1) | 加分项（系列题框架） |

## 关键提示

1. **提示1**：如果你在第 `j` 天卖出，利润最大化等价于在第 `0` 到 `j-1` 天中找到最低价买入。你需要对每个 `j` 都重新搜索吗？
2. **提示2**：从左到右扫描时，"到目前为止的最低价"是单调更新的——只有遇到更低的价格才更新，否则不变。
3. **提示3**：`maxProfit` 永远 ≥ 0，因为最差情况是不交易（利润为 0）。不需要额外判断"是否存在盈利"。
4. **提示4（系列题铺垫）**：如果改成"可以买卖多次"呢？如果"最多买卖 k 次"呢？思考一下通用的状态定义。

```
价格走势示意 [7, 1, 5, 3, 6, 4]:

价格
  7  *
  6  |              *
  5  |     *
  4  |                    *
  3  |           *
  2  |
  1  |  *  ← 历史最低点 (minPrice = 1)
     +--+--+--+--+--+--→ 天数
     0  1  2  3  4  5

在第4天卖出: profit = 6 - 1 = 5 ← 最大利润
```

## 解法详解

### 解法1: 暴力枚举 — O(n²) / O(1)

**思考过程**: 最直觉的方式——试遍所有可能的买入日和卖出日，取利润最大的组合。

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        int maxProfit = 0;
        for (int i = 0; i < n; i++) {           // 枚举买入日
            for (int j = i + 1; j < n; j++) {    // 枚举卖出日（必须在买入之后）
                int profit = prices[j] - prices[i];
                maxProfit = max(maxProfit, profit);
            }
        }
        return maxProfit;
    }
};
```

**关键点**: 
- 初始化 `maxProfit = 0` 而非 `INT_MIN`，因为"不交易"利润为 0，这天然处理了股价持续下降的情况。
- n = 10⁵ 时，O(n²) = 10¹⁰ 会超时。瓶颈在于：对每个卖出日 `j`，都重新扫描了 `[0, j-1]` 来找最低买入价。

### 解法2: 一次遍历（维护历史最低价） — O(n) / O(1) ⭐ 面试首选

**从解法1优化**: 解法1的内层循环做的事情是"找 `j` 之前的最小值"。如果我们一边遍历一边用变量记住"到目前为止的最低价"，内层循环就消失了。

```cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int minPrice = INT_MAX;   // 到目前为止遇到的最低价格
        int maxProfit = 0;        // 到目前为止能获得的最大利润
        
        for (int price : prices) {
            // 先更新最低价（考虑今天是否是更好的买入时机）
            minPrice = min(minPrice, price);
            // 再算今天卖出的利润（用历史最低价买入）
            maxProfit = max(maxProfit, price - minPrice);
        }
        
        return maxProfit;
    }
};
```

**思维过程可视化**:
```
prices:   [7,   1,   5,   3,   6,   4]
          day0 day1 day2 day3 day4 day5

遍历过程:
day0: minPrice=min(MAX,7)=7,  profit=7-7=0,  maxProfit=0
day1: minPrice=min(7,1)=1,    profit=1-1=0,  maxProfit=0
day2: minPrice=min(1,5)=1,    profit=5-1=4,  maxProfit=4
day3: minPrice=min(1,3)=1,    profit=3-1=2,  maxProfit=4
day4: minPrice=min(1,6)=1,    profit=6-1=5,  maxProfit=5  ← 答案
day5: minPrice=min(1,4)=1,    profit=4-1=3,  maxProfit=5
```

**关键点**: 
- `minPrice` 和 `maxProfit` 的更新顺序：先更新 `minPrice`，再算利润。这样即使当天是最低价，`price - minPrice = 0`，不会出现负利润。
- 或者反过来也行：先算利润再更新 `minPrice`，因为 `price - minPrice` 在 `minPrice` 还没更新时计算，等价于"不在同一天买卖"。两种顺序都正确，但含义略有不同。

### 解法3: 动态规划（状态机框架） — O(n) / O(n) → O(1)

**从系列题角度思考**: 这个解法对本题来说"过度设计"，但它是股票系列题（121/122/123/188/309/714）的通用框架。掌握这个框架后，后续系列题只需微调状态转移。

**状态定义**:
- `dp[i][0]`：第 i 天结束时，**不持有**股票的最大利润
- `dp[i][1]`：第 i 天结束时，**持有**股票的最大利润

**状态转移**:
```
dp[i][0] = max(dp[i-1][0],            // 昨天就不持有，今天啥也不做
               dp[i-1][1] + prices[i]) // 昨天持有，今天卖出

dp[i][1] = max(dp[i-1][1],            // 昨天就持有，今天啥也不做
               0 - prices[i])          // 今天买入（注意：只能买一次，所以买入前利润是0，不是dp[i-1][0]）
               ↑ 这里是关键！Stock II 改成 dp[i-1][0] - prices[i] 就能多次买卖
```

**DP 表填充示例**:
```
prices: [7, 1, 5, 3, 6, 4]

         dp[i][0]  dp[i][1]
init:      0        -INF    (还没开始)
day0:      0         -7     (不买/买入7)
day1:      0         -1     (不变/买入1更好)
day2:      4         -1     (卖出5-1=4/保持-1)
day3:      4         -1     (保持4/保持-1)
day4:      5         -1     (卖出6-1=5/保持-1)
day5:      5         -1     (保持5/保持-1)

答案: dp[5][0] = 5
```

```cpp
// 完整 DP 版本
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int n = prices.size();
        // dp[i][0] = 第i天不持股的最大利润
        // dp[i][1] = 第i天持股的最大利润
        vector<vector<int>> dp(n, vector<int>(2));
        
        dp[0][0] = 0;                // 第0天不买
        dp[0][1] = -prices[0];       // 第0天买入
        
        for (int i = 1; i < n; i++) {
            dp[i][0] = max(dp[i-1][0], dp[i-1][1] + prices[i]);
            dp[i][1] = max(dp[i-1][1], -prices[i]);  // 只买一次，所以是 0 - prices[i]
            //                          ↑ Stock II 这里改成 dp[i-1][0] - prices[i]
        }
        
        return dp[n-1][0];  // 最后一天不持股一定比持股利润高
    }
};
```

**空间优化（滚动变量）**: `dp[i]` 只依赖 `dp[i-1]`，所以只需两个变量。

```cpp
// 空间优化版本 — O(n) / O(1)
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        int noStock = 0;              // dp[i][0]: 不持股
        int hasStock = -prices[0];    // dp[i][1]: 持股
        
        for (int i = 1; i < prices.size(); i++) {
            noStock = max(noStock, hasStock + prices[i]);
            hasStock = max(hasStock, -prices[i]);
        }
        
        return noStock;
    }
};
```

**关键点**: 观察空间优化后的代码，`hasStock = max(hasStock, -prices[i])` 就是在维护 `-minPrice`，而 `noStock = max(noStock, hasStock + prices[i])` 就是 `max(maxProfit, prices[i] - minPrice)`。所以解法2和解法3的空间优化版**本质上是同一个东西**！

## 解法对比

|  | 暴力枚举 | 一次遍历 | 状态机 DP |
|--|---------|---------|----------|
| 时间 | O(n²) | O(n) | O(n) |
| 空间 | O(1) | O(1) | O(n)→O(1) |
| 核心思想 | 穷举所有对 | 维护历史最低价 | 持股/不持股状态转移 |
| 适用场景 | 理解题意 | 本题最佳 | 股票系列通用框架 |
| 面试推荐 | 作为起点 | ⭐ 首选 | 追问系列题时展示 |

## 易错点

1. **✗** `int minPrice = 0;` → **✓** `int minPrice = INT_MAX;`
   - 初始化为 0 会导致 `prices[i] - 0 = prices[i]`，等于凭空赚了钱。初始化为 `INT_MAX` 保证第一天的价格一定会更新 `minPrice`。

2. **✗** 先算利润再判断是否更新 `minPrice`，但 `maxProfit` 初始化为 `INT_MIN`：
   ```cpp
   maxProfit = max(maxProfit, price - minPrice); // 第一天: price - INT_MAX 溢出!
   ```
   → **✓** `maxProfit` 初始化为 `0`（不交易的利润），或者先更新 `minPrice` 再算利润。

3. **✗** DP 版本中买入时写成 `dp[i][1] = max(dp[i-1][1], dp[i-1][0] - prices[i])`
   → **✓** 本题只能买一次，买入前利润必须是 0，所以是 `max(dp[i-1][1], 0 - prices[i])`。如果写成 `dp[i-1][0] - prices[i]`，那就变成了 Stock II（可以多次交易）。

4. **✗** 返回 `dp[n-1][1]`（最后一天还持有股票）
   → **✓** 返回 `dp[n-1][0]`（最后一天不持有），因为手上还有股票意味着钱被套住了，利润一定 ≤ 卖出后的利润。

## 面试追问

**Q1: 暴力解的时间复杂度是什么？能通过这道题吗？**
→ O(n²)，n = 10⁵ 时约 10¹⁰ 次运算，会超时。瓶颈在于：对每个卖出日都重新搜索之前的最低买入价。

**Q2: 一次遍历的做法为什么是正确的？会不会漏掉某个最优解？**
→ 不会。最优解一定是某天卖出，卖出日之前的最低价买入。遍历到任意卖出日时，`minPrice` 已经精确记录了之前所有日子的最低价，所以每个可能的卖出日都被考虑到了，不会遗漏。

**Q3: 如果改成"可以买卖多次"（LeetCode 122），怎么改？**
→ 贪心做法：只要明天比今天高就买卖，`profit += max(0, prices[i] - prices[i-1])`。DP 做法：把 `dp[i][1] = max(dp[i-1][1], 0 - prices[i])` 改成 `dp[i][1] = max(dp[i-1][1], dp[i-1][0] - prices[i])`，即买入时可以基于之前卖出的利润，而非从 0 开始。

**Q4: 如果最多买卖 k 次（LeetCode 188），怎么扩展？**
→ 增加一个维度：`dp[i][k][0/1]`，表示第 i 天、最多还能交易 k 次、持股/不持股。买入时 k 减 1（或卖出时减 1，约定一致即可）。

## 相关题型

- **122. 买卖股票的最佳时机 II** — 复用解法3的 DP 框架，只改一行：买入转移从 `0 - prices[i]` 改成 `dp[i-1][0] - prices[i]`（允许多次交易）
- **123. 买卖股票的最佳时机 III** — 最多 2 次交易，DP 加维度：`dp[i][k][0/1]`，k ∈ {1, 2}
- **188. 买卖股票的最佳时机 IV** — 最多 k 次交易，是 123 的泛化
- **309. 最佳买卖股票时机含冷冻期** — 卖出后有 1 天冷冻期，买入转移从 `dp[i-1][0]` 改成 `dp[i-2][0]`
- **714. 买卖股票的最佳时机含手续费** — 每次交易扣手续费，卖出时减 `fee`
- **53. 最大子数组和** — 同样是"维护一个历史最优值，一次遍历求解"的思路模式