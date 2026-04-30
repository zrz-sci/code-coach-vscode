# 1335. 工作计划的最低难度

## 核心思路

本质上是一个**数组分割问题**：将长度为 n 的数组按顺序切成 d 段（每段至少一个元素），使得每段最大值之和最小。这是经典的区间 DP 模型。

## 思维链

1. **读完题第一反应**：工作必须按顺序做，每天至少做一项，一天的难度 = 当天做的工作中的最大值。我们要把 n 个工作分成 d 天，求总难度最小。→ 这不就是把数组切 d-1 刀，分成 d 段吗？

2. **暴力怎么做？** 枚举所有切法。第一天可以做工作 0~0、0~1、...、0~(n-d)，剩下的分给后面 d-1 天递归处理。这是一个递归穷举。

3. **发现重叠子问题**：当我们递归到"从第 i 个工作开始，还剩 j 天"时，这个子问题可能被多次计算。→ 加上记忆化！

4. **状态定义**：`dp[i][j]` = 从第 i 个工作开始、分成 j 天完成的最小总难度。转移时枚举第 j 天做工作 i~k，难度为 `max(jobDifficulty[i..k])`，剩下的递归 `dp[k+1][j-1]`。

5. **进一步优化**：枚举切割点时需要反复求区间最大值，可以在枚举过程中递推维护 max，避免重复计算。

6. **终极优化（单调栈）**：利用单调栈在 O(n) 时间内完成一层 DP 的转移，将总复杂度从 O(n²d) 降到 O(nd)。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力递归 | 枚举所有切法 | O(n! / ...) 指数级 | O(d) 栈深 | 能说出即可 |
| 记忆化搜索 | 自顶向下 + memo | O(n²d) | O(nd) | ⭐ 必须写出 |
| 递推 DP | 自底向上填表 | O(n²d) | O(nd) → O(n) | ⭐ 必须写出 |
| 单调栈优化 DP | 栈优化转移 | O(nd) | O(n) | 加分项 |

## 关键提示

1. **无解条件**：如果 `n < d`，工作数不够每天至少一项，直接返回 -1。

2. **状态转移的核心**：第 k 天做工作 `[j, j+1, ..., i]`，这天的难度是 `max(jobDifficulty[j..i])`。枚举 j 的过程中，max 可以**从右往左**递推维护。

3. **填表方向**：天数 k 从 1 到 d，工作索引 i 从小到大。`dp[i][k]` 依赖 `dp[j-1][k-1]`（j ≤ i），所以上一天的结果要先算好。

4. **空间优化**：每一天只依赖前一天的结果，可以用滚动数组把空间从 O(nd) 降到 O(n)。

5. **单调栈优化直觉**：当我们从左到右计算 `dp[i][k]` 时，如果 `jobDifficulty[i]` 很大，它会"覆盖"之前的一些决策，单调栈能高效找到这些被覆盖的位置。

```
// DP 填表示意 (jobDifficulty = [6,5,4,3,2,1], d = 2):
//
//        job0  job1  job2  job3  job4  job5
// day1:   6     6     6     6     6     6     ← 前 i+1 个工作的最大值
// day2:   -     11    10    9     8     7     ← min over all cuts
//                                       ^
//                                    答案 = 7
//
// dp[5][2] = min(dp[0][1]+max(1..5), dp[1][1]+max(2..5), ..., dp[4][1]+max(5..5))
//          = min(6+5, 6+4, 6+3, 6+2, 6+1) = 7
```

## 解法详解

### 解法1: 暴力递归（记忆化搜索） — O(n²d) / O(nd)

**思考过程**：最自然的想法——递归地"切一刀"。定义 `dfs(i, k)` 表示从工作 i 开始、还需要 k 天完成的最小难度。每次枚举当天做到哪里，取区间最大值作为当天难度，递归剩余部分。发现大量重复计算，用 memo 数组记忆化。

```cpp
class Solution {
public:
    int minDifficulty(vector<int>& jobDifficulty, int d) {
        int n = jobDifficulty.size();
        if (n < d) return -1;
        
        // memo[i][k] = 从工作i开始，分k天完成的最小难度，-1表示未计算
        vector<vector<int>> memo(n, vector<int>(d + 1, -1));
        
        return dfs(jobDifficulty, 0, d, memo);
    }
    
    int dfs(vector<int>& jobs, int i, int k, vector<vector<int>>& memo) {
        int n = jobs.size();
        // 最后一天：剩下的工作必须全做，难度 = 区间最大值
        if (k == 1) {
            int mx = 0;
            for (int j = i; j < n; j++) mx = max(mx, jobs[j]);
            return mx;
        }
        
        if (memo[i][k] != -1) return memo[i][k];
        
        int res = INT_MAX;
        int mx = 0;
        // 枚举今天做工作 i..j，至少要给后面 k-1 天各留一个工作
        for (int j = i; j <= n - k; j++) {
            mx = max(mx, jobs[j]);  // 递推维护区间最大值，避免重复计算
            // 今天难度mx + 剩余最小难度
            res = min(res, mx + dfs(jobs, j + 1, k - 1, memo));
        }
        
        memo[i][k] = res;
        return res;
    }
};
```

**关键点**：
- `j` 的上界是 `n - k`，不是 `n - 1`，因为后面还要留 `k-1` 天每天至少一个工作
- `mx` 在 for 循环中递推维护，不需要每次重新算区间最大值

### 解法2: 自底向上递推 DP — O(n²d) / O(nd) → O(n) ⭐ 面试首选

**从解法1翻译**：记忆化搜索天然可以翻译成递推。定义 `dp[i][k]` = 完成前 i+1 个工作（索引 0..i），用 k 天的最小总难度。

转移方程：枚举第 k 天从工作 j 开始做到工作 i：
```
dp[i][k] = min over j { dp[j-1][k-1] + max(jobs[j..i]) }
```
其中 j 从 i 倒着枚举到 k-1（保证前 k-1 天每天至少一个工作），这样 max 可以递推。

```
// 转移示意：
//  工作:  [0] [1] [2] ... [j-1] | [j] [j+1] ... [i]
//         ←—— 前 k-1 天做完 ——→   ←—— 第 k 天做 ——→
//         dp[j-1][k-1]              max(jobs[j..i])
```

```cpp
class Solution {
public:
    int minDifficulty(vector<int>& jobDifficulty, int d) {
        int n = jobDifficulty.size();
        if (n < d) return -1;
        
        // dp[i][k]: 完成工作 0..i，用 k 天的最小总难度
        const int INF = 1e9;
        vector<vector<int>> dp(n, vector<int>(d + 1, INF));
        
        // base case: k=1，第一天做工作 0..i，难度 = max(jobs[0..i])
        int mx = 0;
        for (int i = 0; i < n; i++) {
            mx = max(mx, jobDifficulty[i]);
            dp[i][1] = mx;
        }
        
        // 填表：天数从 2 到 d
        for (int k = 2; k <= d; k++) {
            // i 至少要有 k 个工作，所以 i >= k-1
            for (int i = k - 1; i < n; i++) {
                // 枚举第 k 天从工作 j 做到工作 i
                // j 的范围：[k-1, i]，保证前面至少 k-1 个工作给 k-1 天
                int curMax = 0;
                for (int j = i; j >= k - 1; j--) {
                    curMax = max(curMax, jobDifficulty[j]);
                    // j == 0 时没有 dp[j-1][k-1]，但 k>=2 且 j>=k-1>=1，所以 j>=1
                    dp[i][k] = min(dp[i][k], dp[j - 1][k - 1] + curMax);
                }
            }
        }
        
        return dp[n - 1][d];
    }
};
```

**空间优化版（滚动数组）**：每一层 k 只依赖 k-1 层，可以用一维数组。

```cpp
class Solution {
public:
    int minDifficulty(vector<int>& jobDifficulty, int d) {
        int n = jobDifficulty.size();
        if (n < d) return -1;
        
        const int INF = 1e9;
        vector<int> dp(n, INF);  // dp[i] = 用当前天数完成工作 0..i 的最小难度
        
        // base case: 第 1 天
        int mx = 0;
        for (int i = 0; i < n; i++) {
            mx = max(mx, jobDifficulty[i]);
            dp[i] = mx;
        }
        
        // 第 2 天到第 d 天
        for (int k = 2; k <= d; k++) {
            // 必须从右往左更新！因为 dp[j-1] 需要的是上一层(k-1)的值
            // 如果从左往右会覆盖掉还没用的旧值
            vector<int> newDp(n, INF);  // 或者用临时数组
            for (int i = k - 1; i < n; i++) {
                int curMax = 0;
                for (int j = i; j >= k - 1; j--) {
                    curMax = max(curMax, jobDifficulty[j]);
                    newDp[i] = min(newDp[i], dp[j - 1] + curMax);
                }
            }
            dp = newDp;
        }
        
        return dp[n - 1];
    }
};
```

**关键点**：
- 滚动数组需要临时数组 `newDp`，不能原地覆盖（因为内层循环 j 从右往左读 `dp[j-1]`，如果直接写 dp 会污染）

### 解法3: 单调栈优化 DP — O(nd) / O(n)

**从解法2的瓶颈出发**：解法2对每个 `(i, k)` 要枚举所有可能的切割点 j，内层循环 O(n)，总共 O(n²d)。能否减少内层枚举？

**关键观察**：当 `jobDifficulty[i]` 出现时，它可能成为某些区间的最大值，从而"覆盖"之前较小的 max。具体来说：

- 如果 `jobs[i] >= jobs[i-1]`，那么把工作 i 放到和 i-1 同一天，这天的 max 还是 `jobs[i]`，不会更差。
- 单调栈维护一个递减序列，栈中每个元素记录"如果第 k 天从这里开始到 i，最优解是多少"。

```cpp
class Solution {
public:
    int minDifficulty(vector<int>& jobDifficulty, int d) {
        int n = jobDifficulty.size();
        if (n < d) return -1;
        
        const int INF = 1e9;
        vector<int> dp(n, INF);
        
        // base case: 第 1 天
        int mx = 0;
        for (int i = 0; i < n; i++) {
            mx = max(mx, jobDifficulty[i]);
            dp[i] = mx;
        }
        
        for (int k = 2; k <= d; k++) {
            vector<int> newDp(n, INF);
            // 单调栈：存 (工作索引)，栈内 jobDifficulty 单调递减
            // 附带记录该位置对应的最优 dp 值
            stack<pair<int, int>> stk; // (index, minCost)
            
            for (int i = k - 1; i < n; i++) {
                // 默认选择：工作 i 自己单独一天，前面 0..i-1 用 k-1 天
                int minCost = dp[i - 1] + jobDifficulty[i];
                
                // 弹出栈中所有 jobDifficulty <= jobDifficulty[i] 的元素
                // 因为 jobs[i] 更大，它会成为这些区间的新最大值
                while (!stk.empty() && jobDifficulty[stk.top().first] <= jobDifficulty[i]) {
                    auto [idx, cost] = stk.top();
                    stk.pop();
                    // jobs[i] 替换 jobs[idx] 成为区间最大值
                    // 差值：增加了 jobs[i] - jobs[idx]，但之前 cost 已经包含了 jobs[idx]
                    // 所以新的 cost = cost - jobs[idx] + jobs[i]
                    // 但实际上我们只需取 min
                    minCost = min(minCost, cost - jobDifficulty[idx] + jobDifficulty[i]);
                }
                
                // 如果栈非空，栈顶元素比 jobs[i] 大
                // 可以选择让栈顶的那天包含到 i（栈顶的 max 不变，因为它更大）
                if (!stk.empty()) {
                    minCost = min(minCost, stk.top().second);
                }
                
                stk.push({i, minCost});
                newDp[i] = minCost;
            }
            dp = newDp;
        }
        
        return dp[n - 1];
    }
};
```

**为什么单调栈有效？**
- 在标准 DP 转移 `dp[i][k] = min{ dp[j-1][k-1] + max(jobs[j..i]) }` 中，随着 i 增大，`max(jobs[j..i])` 可能因为 `jobs[i]` 变大而批量更新。
- 单调栈维护的是一组"候选切割方案"，当 `jobs[i]` 比某些之前的最大值更大时，这些方案的代价会变化，栈能 O(1) 均摊处理这些更新。
- 每个元素最多入栈出栈各一次，所以内层均摊 O(n)，总体 O(nd)。

## 解法对比

| | 记忆化搜索 | 递推 DP | 单调栈优化 |
|---|---|---|---|
| 时间 | O(n²d) | O(n²d) | O(nd) |
| 空间 | O(nd) | O(n) 滚动 | O(n) |
| 代码复杂度 | 简单 | 中等 | 较高 |
| 面试推荐 | ✓ 首选 | ✓ 追问优化 | 加分项 |

**选择建议**：
- 面试中先写**记忆化搜索**（思路最直接），然后如果面试官追问可以翻译成递推并空间优化。
- 单调栈优化在 n=300, d=10 的数据范围下不是必须的（O(n²d) ≈ 900000 已经很快），但如果被追问"能否更快"，这是加分答案。

## 易错点

1. **忘记判断无解**：
   - ✗ 忘记 `if (n < d) return -1;`
   - ✓ 工作数 < 天数时不可能每天至少做一个

2. **枚举范围错误**：
   - ✗ `for (int j = i; j >= 0; j--)` — 第 k 天之前需要 k-1 天各至少一个工作
   - ✓ `for (int j = i; j >= k - 1; j--)` — j 最小值是 k-1

3. **滚动数组覆盖旧值**：
   - ✗ 直接在 dp 上更新：`dp[i] = min(dp[i], dp[j-1] + curMax)` — dp[j-1] 可能已被当前层覆盖
   - ✓ 使用 newDp 临时数组，或者确保更新顺序不冲突

4. **base case 初始化**：
   - ✗ `dp[0][1] = jobs[0]` 然后忘记其他 `dp[i][1]`
   - ✓ `dp[i][1] = max(jobs[0], jobs[1], ..., jobs[i])` — 第 1 天做全部，取前缀最大值

5. **单调栈优化中的代价更新**：
   - ✗ 弹出时忘记用 `cost - jobs[idx] + jobs[i]` 更新 minCost
   - ✓ 新的 jobs[i] 替代了旧的区间最大值 jobs[idx]，差额需要修正

## 面试追问

**Q1: 暴力解的思路是什么？复杂度？**
→ 枚举所有 d-1 个切割点的组合，C(n-1, d-1) 种，每种要算 d 个区间最大值。暴力复杂度指数级。加记忆化后 O(n²d)，因为状态 O(nd)，每个状态转移 O(n)。

**Q2: 能否空间优化？**
→ 可以。每层 k 只依赖 k-1 层，用滚动数组（两个一维数组或 newDp），空间从 O(nd) 降到 O(n)。

**Q3: 能否将时间从 O(n²d) 优化到 O(nd)？**
→ 用单调栈。观察到 `jobs[i]` 增大时会批量覆盖之前区间的最大值，单调栈维护递减序列，每个元素入栈出栈各一次，均摊 O(1)。

**Q4: 如果 d 很大（比如 d 接近 n），有什么特殊情况？**
→ 当 d == n 时，每天恰好一个工作，答案 = sum(jobDifficulty)。当 d > n 时返回 -1。d 接近 n 时大部分天都只做一个工作，DP 状态虽然多但转移范围极小。

## 相关题型

- **410. 分割数组的最大值** — 同样是"数组切成 k 段"的模型，但目标是最小化各段之和的最大值。可复用同样的 `dp[i][k]` 框架，区别：本题每段取 max 再求和，410 每段取 sum 再取 max。
- **1011. 在 D 天内送达包裹的能力** — 也是分段问题，但用二分答案 + 贪心验证，是另一种思路。
- **1043. 分隔数组以得到最大和** — 分段后每段全部替换为段内最大值，求最大总和。同样的区间 DP + 枚举切割点思路。
- **Stock 系列 (121/122/123/188)** — 状态设计上有相似之处：用"第几个阶段"作为一个 DP 维度。