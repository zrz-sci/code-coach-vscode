# 1473. 粉刷房子 III

## 核心思路

本质是一个**三维 DP** 问题：对于每个房子，你需要决定它的颜色，同时追踪到当前位置形成了多少个街区。状态由三个维度决定：**哪个房子、涂什么颜色、形成了几个街区**。

## 思维链

1. **读完题第一反应**：每个未涂色的房子有 n 种选择，总共最多 n^m 种方案，暴力枚举所有方案并检查街区数 → 指数级，不可行。

2. **发现子结构**：从左到右逐个处理房子。当我处理第 i 个房子时，我只关心：前 i 个房子用了多少个街区？第 i 个房子是什么颜色（因为下一个房子是否开启新街区取决于和第 i 个颜色是否相同）。这就是 DP 的信号！

3. **定义状态**：`dp[i][j][k]` = 前 i+1 个房子，第 i 个房子颜色为 j，恰好形成 k 个街区时的最小花费。三个维度分别对应：位置、颜色、街区数。

4. **状态转移**：第 i 个房子颜色为 j，第 i-1 个房子颜色为 j'：
   - 若 `j == j'`：同色不增加街区 → `dp[i][j][k] = dp[i-1][j][k] + cost_i_j`
   - 若 `j != j'`：异色多一个街区 → `dp[i][j][k] = dp[i-1][j'][k-1] + cost_i_j`

5. **处理已涂色房子**：如果 `houses[i] != 0`，第 i 个房子颜色固定，不花钱，只能在该颜色上转移。

6. **答案**：`min(dp[m-1][j][target])` 对所有颜色 j 取最小值。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 记忆化搜索 | 自顶向下递归 + memo | O(m·n·target·n) | O(m·n·target) | 能说出即可 |
| 三维递推 DP | 自底向上填表 | O(m·n²·target) | O(m·n·target) | ⭐ 必须写出 |
| 空间优化 DP | 滚动数组降一维 | O(m·n²·target) | O(n·target) | 加分项 |

## 关键提示

1. **状态需要三维**：仅仅知道"前 i 个房子的最小花费"不够，还需要知道第 i 个房子的颜色（决定是否开新街区）和当前街区数（判断是否达到 target）。

2. **已涂色 vs 未涂色**：已涂色的房子颜色固定、花费为 0，未涂色的遍历所有颜色、加上对应 cost。两者的转移方程完全一样，只是可选颜色范围不同。

3. **街区数的上下界剪枝**：到第 i 个房子时，街区数最多为 i+1（每个房子都不同色），最少为 1。如果 `k > i+1` 或 `k > target`，直接跳过。

4. **初始化**：`dp[0][j][1]` 是第一个房子涂色 j 形成 1 个街区的花费，其余初始化为 INF（表示不可达）。

5. **颜色编号注意**：`houses[i]` 从 1 开始，`cost[i][j]` 的 j 从 0 开始对应颜色 j+1。

## 解法详解

### 解法1: 记忆化搜索 — O(m·n²·target) / O(m·n·target)

**思考过程**: 最自然的想法是递归——对每个房子做决定，把"还剩多少街区要凑"传递下去。加上 memo 避免重复计算。

```cpp
class Solution {
public:
    int minCost(vector<int>& houses, vector<vector<int>>& cost, int m, int n, int target) {
        // memo[i][j][k]: 从第i个房子开始，第i个房子颜色为j，还需要形成k个街区的最小花费
        // 用 -1 表示未计算，用 INT_MAX 表示不可达
        vector<vector<vector<int>>> memo(m, vector<vector<int>>(n + 1, vector<int>(target + 1, -1)));
        
        int ans = INT_MAX;
        // 第一个房子特殊处理：它一定开启第1个街区
        if (houses[0] != 0) {
            // 颜色固定，花费0
            int res = dfs(houses, cost, memo, m, n, 0, houses[0], target);
            ans = min(ans, res);
        } else {
            // 尝试所有颜色
            for (int j = 1; j <= n; j++) {
                int res = dfs(houses, cost, memo, m, n, 0, j, target);
                ans = min(ans, res);
            }
        }
        return ans == INT_MAX ? -1 : ans;
    }
    
private:
    // 从第i个房子开始处理，第i个房子颜色为prevColor，还需要形成remain个街区
    int dfs(vector<int>& houses, vector<vector<int>>& cost, 
            vector<vector<vector<int>>>& memo,
            int m, int n, int i, int prevColor, int remain) {
        // 街区数用完了但房子没处理完 → 不可行（remain可能变负）
        if (remain < 0) return INT_MAX;
        // 所有房子处理完
        if (i == m) return remain == 0 ? 0 : INT_MAX;
        // 剩余房子不够凑出 remain 个街区
        if (remain > m - i) return INT_MAX;
        
        if (memo[i][prevColor][remain] != -1) return memo[i][prevColor][remain];
        
        int res = INT_MAX;
        if (houses[i] != 0) {
            // 颜色固定
            int newRemain = (houses[i] == prevColor) ? remain : remain - 1;
            int sub = dfs(houses, cost, memo, m, n, i + 1, houses[i], newRemain);
            res = sub; // 不花钱
        } else {
            // 尝试每种颜色
            for (int j = 1; j <= n; j++) {
                int newRemain = (j == prevColor) ? remain : remain - 1;
                int sub = dfs(houses, cost, memo, m, n, i + 1, j, newRemain);
                if (sub != INT_MAX) {
                    res = min(res, sub + cost[i][j - 1]); // j-1 因为cost是0-indexed
                }
            }
        }
        
        memo[i][prevColor][remain] = res;
        return res;
    }
};
```

**关键点**: 递归参数中 prevColor 用 0 表示"还没有前一个颜色"（第一个房子之前），这恰好和 houses 中 0 的含义区分开。注意 remain 可能为负数要提前截断。

### 解法2: 三维递推 DP — O(m·n²·target) / O(m·n·target) ⭐ 面试首选

**从解法1转化**: 把自顶向下的递归翻译成自底向上的 for 循环填表。

```
DP 表的填充示意（houses=[0,0,0], n=2, target=2）:

dp[i][j][k] = 前 i+1 个房子，第 i 个颜色为 j，恰好 k 个街区的最小花费

         k=1    k=2    k=3
i=0 j=1  c01     ∞      ∞     ← 第一个房子只能形成1个街区
    j=2  c02     ∞      ∞

i=1 j=1  dp[0][1][1]    dp[0][2][1]+c11    ∞
                  ↑同色不加街区   ↑异色加街区
    j=2  dp[0][2][1]    dp[0][1][1]+c12    ∞

i=2 ...（类推）

填表方向: i 从左到右（0→m-1），对每个 i 遍历 j（颜色）和 k（街区数）
依赖关系: dp[i][j][k] 依赖 dp[i-1][所有j'][k] 和 dp[i-1][所有j'≠j][k-1]
```

```cpp
class Solution {
public:
    int minCost(vector<int>& houses, vector<vector<int>>& cost, int m, int n, int target) {
        const int INF = 1e8; // 用1e8而非INT_MAX，避免加法溢出
        
        // dp[i][j][k]: 前i+1个房子，第i个房子颜色为j(1~n)，恰好k个街区
        vector<vector<vector<int>>> dp(m, vector<vector<int>>(n + 1, vector<int>(target + 1, INF)));
        
        // ===== 初始化第一个房子 =====
        if (houses[0] != 0) {
            // 颜色固定，花费0，形成1个街区
            dp[0][houses[0]][1] = 0;
        } else {
            // 尝试每种颜色
            for (int j = 1; j <= n; j++) {
                dp[0][j][1] = cost[0][j - 1];
            }
        }
        
        // ===== 逐个房子填表 =====
        for (int i = 1; i < m; i++) {
            // 确定第i个房子可以选哪些颜色
            int lo, hi; // 颜色范围 [lo, hi]
            if (houses[i] != 0) {
                lo = hi = houses[i]; // 颜色固定
            } else {
                lo = 1; hi = n; // 所有颜色都可选
            }
            
            for (int j = lo; j <= hi; j++) {
                int paintCost = (houses[i] != 0) ? 0 : cost[i][j - 1];
                
                for (int k = 1; k <= min(i + 1, target); k++) {
                    // 情况1: 和前一个房子同色 → 不增加街区
                    // dp[i][j][k] = dp[i-1][j][k] + paintCost
                    int val = dp[i - 1][j][k];
                    
                    // 情况2: 和前一个房子异色 → 增加一个街区
                    // dp[i][j][k] = min(dp[i-1][j'][k-1]) + paintCost, 对所有 j'!=j
                    if (k >= 2) { // k-1>=1 才有意义
                        for (int j2 = 1; j2 <= n; j2++) {
                            if (j2 != j) {
                                val = min(val, dp[i - 1][j2][k - 1]);
                            }
                        }
                    }
                    
                    dp[i][j][k] = min(dp[i][j][k], val + paintCost);
                }
            }
        }
        
        // ===== 取答案 =====
        int ans = INF;
        for (int j = 1; j <= n; j++) {
            ans = min(ans, dp[m - 1][j][target]);
        }
        return ans >= INF ? -1 : ans;
    }
};
```

**关键点**: 
- 用 `1e8` 而非 `INT_MAX` 作为 INF，因为会做加法，INT_MAX 会溢出。
- 颜色从 1 开始编号（和题目一致），dp 数组大小为 `n+1`。
- 街区数 k 的上界是 `min(i+1, target)`，不需要更大。

### 解法3: 空间优化 DP — O(m·n²·target) / O(n·target)

**从解法2优化**: `dp[i]` 只依赖 `dp[i-1]`，经典滚动数组场景，只保留两层。

```cpp
class Solution {
public:
    int minCost(vector<int>& houses, vector<vector<int>>& cost, int m, int n, int target) {
        const int INF = 1e8;
        
        // prev[j][k]: 上一行的 dp 值
        // curr[j][k]: 当前行的 dp 值
        vector<vector<int>> prev(n + 1, vector<int>(target + 1, INF));
        vector<vector<int>> curr(n + 1, vector<int>(target + 1, INF));
        
        // 初始化第一个房子
        if (houses[0] != 0) {
            prev[houses[0]][1] = 0;
        } else {
            for (int j = 1; j <= n; j++) {
                prev[j][1] = cost[0][j - 1];
            }
        }
        
        for (int i = 1; i < m; i++) {
            // 重置 curr
            for (int j = 0; j <= n; j++)
                fill(curr[j].begin(), curr[j].end(), INF);
            
            int lo, hi;
            if (houses[i] != 0) {
                lo = hi = houses[i];
            } else {
                lo = 1; hi = n;
            }
            
            for (int j = lo; j <= hi; j++) {
                int paintCost = (houses[i] != 0) ? 0 : cost[i][j - 1];
                
                for (int k = 1; k <= min(i + 1, target); k++) {
                    int val = prev[j][k]; // 同色
                    
                    if (k >= 2) {
                        for (int j2 = 1; j2 <= n; j2++) {
                            if (j2 != j) {
                                val = min(val, prev[j2][k - 1]); // 异色
                            }
                        }
                    }
                    
                    curr[j][k] = min(curr[j][k], val + paintCost);
                }
            }
            
            swap(prev, curr); // O(1) 交换
        }
        
        int ans = INF;
        for (int j = 1; j <= n; j++) {
            ans = min(ans, prev[j][target]);
        }
        return ans >= INF ? -1 : ans;
    }
};
```

## 解法对比

| | 记忆化搜索 | 递推 DP | 空间优化 DP |
|---|---|---|---|
| 时间 | O(m·n²·target) | O(m·n²·target) | O(m·n²·target) |
| 空间 | O(m·n·target) + 递归栈 | O(m·n·target) | O(n·target) |
| 思维难度 | 较低（自然递归） | 中等（需想清楚填表顺序） | 中等偏高 |
| 面试推荐 | 时间紧时的保底选择 | ⭐ 首选 | 追问空间优化时给出 |

**关键区别**: 记忆化搜索和递推 DP 本质上完全等价，只是实现方式不同。空间优化利用了"只依赖上一行"的特性，额外进阶优化可以对"异色最小值"预处理，将 O(n²) 的内层循环降到 O(n)（维护每个 k 的最小和次小值），但面试中一般不要求。

## 易错点

1. **✗ 用 `INT_MAX` 作为 INF**：`dp[i-1][j2][k-1] + paintCost` 会整数溢出 → **✓ 用 `1e8` 或 `1e7`**，因为最多 100 个房子 × 10^4 花费 = 10^6，远小于 1e8。

2. **✗ 忘记已涂色房子花费为 0**：对已涂色房子仍然加 `cost[i][j-1]` → **✓ 判断 `houses[i] != 0` 时 paintCost = 0。**

3. **✗ 颜色编号搞混**：`houses[i]` 是 1-indexed，`cost[i][j]` 是 0-indexed → **✓ 访问 cost 时用 `cost[i][j-1]`。**

4. **✗ dp 初始化时 `dp[0][j][0]` 设为 0**：第一个房子至少形成 1 个街区 → **✓ base case 是 `dp[0][j][1]`，k=0 时应为 INF。**

5. **✗ 遗漏"房子已涂色但和 target 矛盾"的情况**：如示例4，所有房子已涂色且形成 4 个街区但 target=3 → **✓ 最终检查 `ans >= INF` 时返回 -1。**

## 面试追问

**Q1: 状态为什么需要三维？能不能用二维？**
> 不能。仅靠 `dp[i][k]`（第 i 个房子、k 个街区）不够，因为第 i+1 个房子是否开新街区取决于和第 i 个的颜色是否相同。必须记录第 i 个房子的颜色。

**Q2: 内层遍历"所有异色 j'"是 O(n)，能否优化？**
> 可以。对每个 k，预处理 `dp[i-1][*][k]` 的最小值和次小值（及其对应颜色）。当 j 不等于最小值的颜色时取最小值，等于时取次小值。这样内层从 O(n) 降到 O(1)，总体从 O(m·n²·target) 降到 O(m·n·target)。

**Q3: 如果 n 非常大（比如 10^5），这道题怎么办？**
> 此时 O(m·n²·target) 不可接受。必须用 Q2 的优化，降到 O(m·n·target)。同时空间也要优化成滚动数组 O(n·target)。

## 相关题型

- **256. 粉刷房子** — 本题的简化版（没有街区限制），可以复用"相邻房子不同色"的状态转移框架，但只需要二维 DP。
- **265. 粉刷房子 II** — n 种颜色版本，核心优化技巧（维护最小和次小值避免 O(n²)）可以直接复用到本题的 Q2 追问。
- **1289. 下降路径最小和 II** — 同样需要"排除自身颜色取其他列最小值"的技巧，维护 top-2 最小值完全一致。