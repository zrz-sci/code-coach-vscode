# 188. 买卖股票的最佳时机 IV

## 核心思路

本质上是一个**带状态约束的序列决策问题**：在 n 天的价格序列上，做最多 k 次"买→卖"交易（不能同时持有多股），使总利润最大。关键在于用 `(第几天, 已完成的交易次数, 是否持有股票)` 三个维度描述状态。

## 思维链

1. **读完题第一反应**：暴力枚举所有可能的买卖组合？k 次交易意味着要选 k 个不重叠的区间 [buy_i, sell_i]，枚举所有组合是指数级的，显然不行。

2. **换一种思考角度**：不要一次性选出 k 个区间，而是**逐天决策** —— 每天有三个选择：买入、卖出、什么都不做。这就变成了一个多阶段决策问题，天然适合 DP。

3. **状态怎么定义？** 每天的决策取决于两个东西：① 已经用了多少次交易（约束剩余机会），② 当前是否持有股票（决定能买还是能卖）。所以状态是 `dp[i][j][0/1]` = 第 i 天结束后，已完成 j 次交易，手上没有/有股票时的最大利润。

4. **转移方程怎么推？** 对于"不持有"状态，要么昨天就不持有（观望），要么今天卖出（从持有变为不持有）；对于"持有"状态，要么昨天就持有（观望），要么今天买入（从不持有变为持有）。

5. **优化**：当 k ≥ n/2 时，交易次数不构成约束（因为最多 n/2 次买卖就能覆盖所有上涨段），退化为"无限次交易"（Stock II），可以贪心 O(n) 解决。

6. **空间优化**：因为 dp[i] 只依赖 dp[i-1]，可以去掉天数维度，用滚动数组把空间从 O(nk) 降到 O(k)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力递归 | 枚举每天的买/卖/观望 | O(2^n) | O(n) 递归栈 | 能说出即可 |
| 记忆化搜索 | 递归+memo三维状态 | O(nk) | O(nk) | 能说出即可 |
| 三维 DP (递推) | dp[i][j][0/1] 填表 | O(nk) | O(nk) | ⭐ 必须写出 |
| 空间优化 DP | 滚动掉天数维度 | O(nk) | O(k) | 加分项 |

## 关键提示

1. **"什么时候算完成一次交易？"** — 你可以选择在"买入时 j+1"或"卖出时 j+1"，两种定义都行，但要一致。经典做法是**卖出时计数加1**。

2. **k ≥ n/2 的特殊处理** — 如果 k 很大，直接跑 O(nk) 会浪费。此时交易次数不构成实际约束，退化为 Stock II（无限次交易），可以贪心 O(n) 解决。

3. **初始化陷阱** — `dp[0][j][1]`（第0天就持有股票且已完成 j>0 次交易）是不可能的状态，应初始化为 `-INF`，不是 0。

4. **系列题关系**：
   - Stock I（k=1）：本题的特例
   - Stock II（k=∞）：本题的特例（去掉 j 维度）
   - Stock III（k=2）：本题的特例
   - 本题是**通用版**，掌握这道题等于掌握 I/II/III

5. **DP 填表方向**：
   ```
   // dp[i][j][0/1]，依赖关系：
   //   dp[i][j][0] = max(dp[i-1][j][0], dp[i-1][j-1][1] + prices[i])
   //                      ↑ 昨天不持有      ↑ 昨天持有,今天卖出(完成第j次)
   //   dp[i][j][1] = max(dp[i-1][j][1], dp[i-1][j][0] - prices[i])
   //                      ↑ 昨天持有        ↑ 昨天不持有,今天买入
   //
   //   天: i →→→→→      每天依赖前一天，所以从左到右遍历
   //   交易次数: j →→→   j 依赖 j-1，所以从小到大遍历
   ```

## 解法详解

### 解法1: 暴力递归 — O(2^n) / O(n)

**思考过程**: 最直觉的做法 —— 每天做决策，递归穷举所有选择。定义 `dfs(day, txn, holding)` 表示从第 day 天开始、已完成 txn 次交易、当前是否持有股票的最大利润。

```cpp
class Solution {
public:
    int maxProfit(int k, vector<int>& prices) {
        int n = prices.size();
        return dfs(prices, 0, 0, false, k);
    }
    
    int dfs(vector<int>& prices, int day, int txn, bool holding, int k) {
        // 终止：天数用完 或 交易次数用完(且不持有)
        if (day == prices.size()) return holding ? -1e9 : 0;
        if (txn == k && !holding) return 0;
        
        // 选择1：什么都不做
        int res = dfs(prices, day + 1, txn, holding, k);
        
        if (holding) {
            // 选择2：卖出 → 完成一次交易(txn+1)，变为不持有
            res = max(res, prices[day] + dfs(prices, day + 1, txn + 1, false, k));
        } else if (txn < k) {
            // 选择3：买入 → 交易次数不变(卖出时才算)，变为持有
            res = max(res, -prices[day] + dfs(prices, day + 1, txn, true, k));
        }
        return res;
    }
};
```

**关键点**: 指数级复杂度，但能帮你看到大量重叠子问题 —— 不同的决策路径会到达相同的 `(day, txn, holding)` 状态。

### 解法2: 记忆化搜索 — O(nk) / O(nk)

**从解法1优化**: 解法1的瓶颈是重复计算相同状态。状态总共只有 `n × k × 2` 种，加个 memo 数组即可。

```cpp
class Solution {
public:
    int maxProfit(int k, vector<int>& prices) {
        int n = prices.size();
        // memo[day][txn][holding]: -1 表示未计算
        vector<vector<vector<int>>> memo(n, vector<vector<int>>(k + 1, vector<int>(2, -1)));
        return dfs(prices, 0, 0, 0, k, memo);
    }
    
    int dfs(vector<int>& prices, int day, int txn, int holding, int k,
            vector<vector<vector<int>>>& memo) {
        if (day == (int)prices.size()) return holding ? -1e9 : 0;
        if (txn == k && !holding) return 0;
        if (memo[day][txn][holding] != -1) return memo[day][txn][holding];
        
        int res = dfs(prices, day + 1, txn, holding, k, memo); // 观望
        if (holding) {
            // 卖出：完成一笔交易
            res = max(res, prices[day] + dfs(prices, day + 1, txn + 1, false, k, memo));
        } else if (txn < k) {
            // 买入
            res = max(res, -prices[day] + dfs(prices, day + 1, txn, true, k, memo));
        }
        return memo[day][txn][holding] = res;
    }
};
```

**关键点**: 和暴力递归的代码几乎一样，只加了 memo 数组，但复杂度从指数级降到 O(nk)。

### 解法3: 三维 DP (递推) — O(nk) / O(nk) ⭐ 面试首选

**从记忆化到递推**: 把自顶向下的递归翻译成自底向上的填表。

状态定义换一种（更常用的）：`dp[i][j][0]` = 第 i 天结束、至今最多完成 j 次交易、不持有股票的最大利润。`dp[i][j][1]` = 持有股票。

**"卖出时计数"的转移**（j 代表已完成的交易数）：
```
dp[i][j][0] = max(dp[i-1][j][0],   dp[i-1][j-1][1] + prices[i])
                   观望(不持有)        卖出(完成第j次交易)

dp[i][j][1] = max(dp[i-1][j][1],   dp[i-1][j][0] - prices[i])
                   观望(持有)          买入(交易次数不变)
```

```
// DP 填表示例: prices=[3,2,6,5,0,3], k=2
//
// 天 | 价格 | dp[i][1][0] dp[i][1][1] | dp[i][2][0] dp[i][2][1]
// ---+------+-------------------------+-------------------------
//  0 |  3   |    0          -3        |    0          -3
//  1 |  2   |    0          -2        |    0          -2
//  2 |  6   |    4          -2        |    4          -2
//  3 |  5   |    4          -2        |    4          -2
//  4 |  0   |    4           0        |    4           4
//  5 |  3   |    4           0        |    7           4
//                                          ↑ 最终答案=7
```

```cpp
class Solution {
public:
    int maxProfit(int k, vector<int>& prices) {
        int n = prices.size();
        if (n == 0 || k == 0) return 0;
        
        // 优化：k >= n/2 时退化为无限次交易
        if (k >= n / 2) {
            int profit = 0;
            for (int i = 1; i < n; i++)
                profit += max(0, prices[i] - prices[i - 1]);
            return profit;
        }
        
        // dp[i][j][0]: 第i天结束, 完成了j次交易, 不持有
        // dp[i][j][1]: 第i天结束, 完成了j次交易, 持有
        vector<vector<vector<int>>> dp(n, vector<vector<int>>(k + 1, vector<int>(2, -1e9)));
        
        // 初始化第0天
        dp[0][0][0] = 0;         // 第0天，0次交易，不持有 → 利润0
        dp[0][0][1] = -prices[0]; // 第0天买入，0次完成交易，持有
        // dp[0][j>0][...] 保持 -INF，因为第0天不可能已完成交易
        
        for (int i = 1; i < n; i++) {
            for (int j = 0; j <= k; j++) {
                // 不持有：观望 or 卖出(完成第j次交易，需要 j>=1 且昨天 j-1 次持有)
                dp[i][j][0] = dp[i - 1][j][0]; // 观望
                if (j >= 1)
                    dp[i][j][0] = max(dp[i][j][0], dp[i - 1][j - 1][1] + prices[i]);
                
                // 持有：观望 or 买入(交易次数不变)
                dp[i][j][1] = dp[i - 1][j][1]; // 观望
                dp[i][j][1] = max(dp[i][j][1], dp[i - 1][j][0] - prices[i]); // 买入
            }
        }
        
        // 答案：最后一天，不持有，取所有交易次数的最大值
        int ans = 0;
        for (int j = 0; j <= k; j++)
            ans = max(ans, dp[n - 1][j][0]);
        return ans;
    }
};
```

**关键点**: 
- 初始化只有 `dp[0][0][0]=0` 和 `dp[0][0][1]=-prices[0]` 是合法的，其余都是 -INF
- 卖出时 j-1→j，买入时 j 不变 —— 这种定义下 j 代表"已完成的卖出次数"

### 解法4: 空间优化 DP — O(nk) / O(k)

**优化思路**: `dp[i]` 只依赖 `dp[i-1]`，去掉天数维度，用两个一维数组（或直接压缩到两个长度为 k+1 的数组）。

更进一步，经典的**买入价格/卖出利润数组**写法更简洁：用 `buy[j]` 和 `sell[j]` 分别表示完成第 j 次买入/卖出后的最大利润。

```cpp
class Solution {
public:
    int maxProfit(int k, vector<int>& prices) {
        int n = prices.size();
        if (n == 0 || k == 0) return 0;
        
        // k >= n/2 时退化为无限次交易（贪心）
        if (k >= n / 2) {
            int profit = 0;
            for (int i = 1; i < n; i++)
                profit += max(0, prices[i] - prices[i - 1]);
            return profit;
        }
        
        // buy[j]: 完成第 j 次买入后的最大利润 (利润为负，因为花了钱)
        // sell[j]: 完成第 j 次卖出后的最大利润
        vector<int> buy(k + 1, INT_MIN);  // 初始不可能状态
        vector<int> sell(k + 1, 0);        // 没交易就是0利润
        // 注意 sell[0]=0 是合法的(0次卖出=0利润)
        // buy[0]=INT_MIN 表示还没有第0次买入(不合法索引,不使用)
        
        for (int price : prices) {
            for (int j = 1; j <= k; j++) {
                // 第j次买入 = max(保持, 第j-1次卖出后再买)
                buy[j] = max(buy[j], sell[j - 1] - price);
                // 第j次卖出 = max(保持, 第j次买入后再卖)
                sell[j] = max(sell[j], buy[j] + price);
            }
        }
        
        // sell[k] 就是最多 k 次交易的最大利润
        // (不一定恰好 k 次，因为 sell 取了 max，不交易也不会更差)
        return sell[k];
    }
};
```

**关键点**: 
- `buy[j]` 和 `sell[j]` 在同一天内按 j=1,2,...,k 顺序更新，看似同一天的 sell 用了同一天更新的 buy，但这等价于"同一天买入又卖出"（利润为0），不影响正确性。
- 这个写法是 Stock 系列最经典的空间优化模板。

## 解法对比

| | 暴力递归 | 记忆化搜索 | 三维DP | 空间优化DP |
|---|---|---|---|---|
| 时间 | O(2^n) | O(nk) | O(nk) | O(nk) |
| 空间 | O(n) | O(nk) | O(nk) | O(k) |
| 代码量 | 中 | 中 | 中 | 短 |
| 适合场景 | 理解题意 | 递归直觉 | 面试标准答案 | 追问空间优化 |

面试中：先讲状态定义和转移方程（用解法3的思路），写代码时直接写解法4（空间优化版），因为代码更短且展示了优化意识。

## 易错点

1. **忘记 k ≥ n/2 的特殊处理**
   - ✗ 直接开 `dp[n][k+1][2]`，当 k=100, n=1000 时虽然不会超内存，但如果 k 更大就可能 TLE/MLE
   - ✓ 先判断 `if (k >= n/2)` 走贪心路径

2. **初始化用 0 而不是 -INF**
   - ✗ `dp[0][j][1] = 0`（第0天持有股票但没花钱？不对！）
   - ✓ `dp[0][j][1] = -INF`（对 j>0 的情况，第0天不可能已完成交易还持有股票）
   - ✓ `dp[0][0][1] = -prices[0]`（唯一合法的第0天持有状态）

3. **卖出计数 j 搞反**
   - ✗ 买入时 `j+1`，卖出时 `j` 不变 —— 如果定义 j 是"已完成的交易数"，那应该卖出时 j+1
   - ✓ 保持一致：选定"卖出时 j+1"或"买入时 j+1"，整个代码统一

4. **空间优化时 sell[j] 用了当天更新的 buy[j]**
   - 这其实是**正确的**！因为同天买卖利润为0，不会让答案变优。很多人以为这是 bug，反而用旧值反而不便。

## 面试追问

**Q1: 这道题和 Stock I/II/III 的关系是什么？**
→ Stock I 是 k=1；Stock III 是 k=2；Stock II 是 k=∞（即 k≥n/2）。本题是通用版，其他三道都是本题的特例。当 k=1 时 buy/sell 各一个变量即可；当 k=∞ 时去掉 j 维度用贪心。

**Q2: 为什么 k ≥ n/2 时可以退化为贪心？**
→ n 天最多形成 n/2 个不重叠的"买→卖"区间。如果 k ≥ n/2，交易次数不构成任何限制，等价于可以做无限次交易。此时贪心地收集所有上涨段即可（每天如果涨了就加上差价）。

**Q3: 如果还有"冷冻期"约束（卖出后下一天不能买入），怎么改？**
→ 买入的转移从 `dp[i-1][j][0] - prices[i]` 改为 `dp[i-2][j][0] - prices[i]`（或引入第三个状态"冷冻中"）。这就是 LeetCode 309 的扩展。

**Q4: 如果每次交易有手续费 fee，怎么改？**
→ 卖出时减去 fee：`dp[i][j][0] = dp[i-1][j-1][1] + prices[i] - fee`。这就是 LeetCode 714 的扩展。

## 相关题型

| 题号 | 题名 | 和本题的关系 |
|------|------|------------|
| 121 | Stock I | 本题 k=1 的特例，buy/sell 各一个变量即可 |
| 122 | Stock II | 本题 k=∞ 的特例，去掉 j 维度，贪心收集所有上涨段 |
| 123 | Stock III | 本题 k=2 的特例，展开 j=1,2 得到 4 个变量 |
| 309 | Stock with Cooldown | 在本题基础上，买入转移多看一天（i-2），增加冷冻状态 |
| 714 | Stock with Fee | 在本题基础上（k=∞），卖出时减去 fee |

**系列学习建议**：先做 121（理解一次交易），再做 122（无限次贪心），然后做 123（两次 → 引入 j 维度），最后做本题 188（通用 k 次）。这样就能感受到状态维度是怎么一步步加上去的。