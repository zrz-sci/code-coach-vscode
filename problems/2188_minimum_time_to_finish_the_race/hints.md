# 2188. 完成比赛的最少时间

## 核心思路

本质是一个**分段优化问题**：把 `numLaps` 圈比赛切割成若干段，每段用同一条轮胎不换胎跑若干圈，段与段之间花 `changeTime` 换胎。我们需要找到最优的切割方式使总时间最少。

## 思维链

1. **读完题第一反应**：每一段（不换胎连续跑若干圈）的最优轮胎选择和段长度是关键。暴力做法是枚举所有可能的分段方案，但 `numLaps` 最大 1000，分段方案是指数级的。

2. **关键观察 — 每段最多跑几圈？** 轮胎每圈耗时指数增长 `f * r^(x-1)`。当 `r >= 2` 时，连续跑 x 圈的第 x 圈耗时 = `f * r^(x-1)`。由于 `f <= 10^5`，`changeTime <= 10^5`，当某一圈的耗时超过 `changeTime + f_min`（换胎+重新跑第1圈），继续跑就一定不如换胎。因为 `r >= 2`，所以 `f * 2^(x-1) <= 10^5 + 10^5 = 2*10^5`，这意味着 **每段最多跑约 17-18 圈**！

3. **预处理 `best[j]`**：对于"不换胎连续跑 j 圈的最小总耗时"，我们可以枚举每种轮胎，计算它连续跑 1 圈、2 圈、…、最多 ~18 圈的累计时间，取所有轮胎中的最小值。

4. **DP 分段**：令 `dp[i]` = 跑完前 i 圈的最少时间。转移：`dp[i] = min(dp[i-j] + changeTime + best[j])`，其中 j 从 1 到 min(i, maxLap)。注意第一段不需要 `changeTime`。

5. **技巧处理第一段**：可以给每个 `dp` 值都加上 `changeTime`，最后答案减去 `changeTime`（因为第一段不换胎，多算了一次）。

6. **复杂度**：预处理 O(n * maxLap)，DP O(numLaps * maxLap)。其中 maxLap ≈ 18，所以总体近似线性。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 预处理 + DP | 预处理每段最优耗时，DP分段 | O(n·M + numLaps·M) | O(numLaps + M) | ⭐ 必须写出 |

其中 M ≈ 18 是每段最大圈数，n 是轮胎种类数。

> 这道题本质只有一种核心解法（预处理+DP），但关键在于**发现 M 很小**这个洞察。没有这个洞察就无法设计高效算法。

## 关键提示

1. **指数增长的威力**：`r >= 2` 意味着耗时至少以 2 的幂增长。连续跑 18 圈时，单圈耗时至少 `1 * 2^17 = 131072`，已经超过 `changeTime` 的最大值 `10^5`。所以每段最多跑 ~18 圈。

2. **预处理 `best[j]`**：不要在 DP 中枚举轮胎种类，提前算好"连续跑 j 圈的最优总时间"即可。

3. **换胎代价的巧妙处理**：给每段都加 `changeTime`（包括第一段），最后总答案减去一个 `changeTime`。这样 DP 转移公式统一，不需要特判第一段。

4. **溢出风险**：计算 `f * r^(x-1)` 时可能溢出 `int`，需要用 `long long` 并设置上限提前退出。

5. **去重优化**：如果两种轮胎 `[f1, r1]` 和 `[f2, r2]` 满足 `f1 <= f2` 且 `r1 <= r2`，那么第二种在所有圈数上都不会更优，可以过滤掉（非必须，但能加速）。

## 解法详解

### 解法1: 预处理 + 线性 DP — O(n·M + numLaps·M) / O(numLaps + M) ⭐ 面试首选

**思考过程**：

核心洞察是 **指数增长 → 每段圈数有上界 M**。

- 第 x 圈耗时 `f * r^(x-1)`，当这个值超过 `changeTime + 1`（换胎后用最快轮胎跑1圈的下界），继续用同一条胎就不如换。
- `r >= 2` 时，`r^(x-1) >= 2^(x-1)`，所以 `f * 2^(x-1) > changeTime + f` 时就该换胎。
- 保守上界：`2^(x-1) > 2 * 10^5`，即 `x-1 > 17`，所以 **M ≤ 18**。

有了这个上界，分两步：

**Step 1: 预处理 `best[j]`** — 用某条轮胎不换胎连跑 j 圈的最小总时间。

```
best[j] = min over all tires i { f_i + f_i*r_i + f_i*r_i^2 + ... + f_i*r_i^(j-1) }
```

**Step 2: DP**

```
dp[0] = 0 (跑0圈不花时间)
dp[i] = min { dp[i-j] + changeTime + best[j] }  for j = 1..min(i, M)
```

最终答案 = `dp[numLaps] - changeTime`（第一段多算了一次换胎）。

```
// DP 填表方向 (从左到右):
//
// dp[0]=0   dp[1]   dp[2]   dp[3]   dp[4]  ...  dp[numLaps]
//   ↑         ↑       ↑       ↑       ↑
//   |    dp[0]+best[1] |  dp[1]+best[2] |
//   |         |   dp[0]+best[2] |  dp[2]+best[2]
//   |         |        |   dp[0]+best[3] |
//   每个 dp[i] 向前看最多 M 个位置
//
// 示例: tires=[[2,3],[3,4]], changeTime=5, numLaps=4
// best[1] = min(2, 3) = 2
// best[2] = min(2+6, 3+12) = 8
// best[3] = min(2+6+18, 3+12+48) = 26
// 
// dp[0] = 0
// dp[1] = dp[0] + 5 + 2 = 7
// dp[2] = min(dp[1]+5+2, dp[0]+5+8) = min(14, 13) = 13
// dp[3] = min(dp[2]+5+2, dp[1]+5+8, dp[0]+5+26) = min(20, 20, 31) = 20
// dp[4] = min(dp[3]+5+2, dp[2]+5+8, dp[1]+5+26) = min(27, 26, 38) = 26
// 答案 = dp[4] - 5 = 21 ✓
```

```cpp
class Solution {
public:
    int minimumFinishTime(vector<vector<int>>& tires, int changeTime, int numLaps) {
        // Step 1: 预处理 best[j] — 不换胎连跑 j 圈的最小总时间
        // 由于 r >= 2，单圈耗时指数增长，最多跑约 18 圈就一定不如换胎
        const int MAX_LAP = 18;
        vector<long long> best(MAX_LAP + 1, LLONG_MAX);
        
        for (auto& t : tires) {
            long long f = t[0], r = t[1];
            long long total = 0;     // 累计时间
            long long lapTime = f;   // 当前这一圈的耗时
            for (int x = 1; x <= MAX_LAP; x++) {
                total += lapTime;
                // 当单圈耗时已经超过 changeTime + f（换胎重跑更快），停止
                if (total < best[x]) {
                    best[x] = total;
                }
                // 提前退出：下一圈耗时会溢出或已经不值得继续
                if (lapTime > (long long)changeTime + f) break;
                lapTime *= r;
            }
        }
        
        // Step 2: DP
        // dp[i] = 跑完前 i 圈的最少时间（每段都含 changeTime，最后减一次）
        vector<long long> dp(numLaps + 1, LLONG_MAX);
        dp[0] = 0;
        
        for (int i = 1; i <= numLaps; i++) {
            for (int j = 1; j <= min(i, MAX_LAP); j++) {
                if (best[j] == LLONG_MAX || dp[i - j] == LLONG_MAX) continue;
                dp[i] = min(dp[i], dp[i - j] + changeTime + best[j]);
            }
        }
        
        // 第一段不需要换胎，多算了一次 changeTime
        return (int)(dp[numLaps] - changeTime);
    }
};
```

**关键点**：
- `MAX_LAP = 18` 的推导是本题的核心洞察
- 预处理时用 `long long` 防溢出，并设置提前退出条件
- DP 转移中统一加 `changeTime`，最后减一次，避免特判第一段

## 解法对比

这道题核心只有一种思路（预处理+DP），变体在于实现细节：

| | 统一加 changeTime 后减 | 特判第一段 |
|---|---|---|
| 代码简洁度 | ✓ 转移公式统一 | 需要 if 判断 |
| 容易出错 | 最后忘减 changeTime | 第一段转移写错 |

## 易错点

1. **溢出**：`f * r^(x-1)` 在 x 稍大时就溢出 `int`。
   - ✗ `int lapTime = f; lapTime *= r;` — 溢出后变负数，逻辑全错
   - ✓ `long long lapTime = f; lapTime *= r;` 并在超过阈值时 `break`

2. **MAX_LAP 取太小或太大**：
   - ✗ `MAX_LAP = 10` — 当 `r = 2, f = 1` 时，连跑 17 圈的总时间可能比多次换胎更优
   - ✓ `MAX_LAP = 18`（或保守取 19-20 也行）

3. **第一段换胎代价**：
   - ✗ 直接输出 `dp[numLaps]` — 多算了一次 changeTime
   - ✓ `dp[numLaps] - changeTime`

4. **best[j] 未更新就使用**：
   - ✗ 没判断 `best[j] == LLONG_MAX` 就做加法 — 溢出
   - ✓ 加 `if (best[j] == LLONG_MAX) continue`

5. **提前退出条件写错**：
   - ✗ `if (lapTime > changeTime) break;` — 应该和 `changeTime + f_min` 比，但更安全的做法是和 `changeTime + f`（当前轮胎的 f）比
   - ✓ 保守做法：只要 `lapTime > changeTime + f` 就退出，或者干脆不提前退出（反正最多 18 轮）

## 面试追问

**Q1: 为什么每段最多跑 18 圈？能严格证明吗？**

> 因为 `r >= 2`，第 x 圈耗时至少 `f * 2^(x-1)`。当 `f * 2^(x-1) > changeTime + f_min` 时，这一圈的时间已经超过"换胎+用最快轮胎跑1圈"的代价，所以继续跑一定不如换胎。`f_min >= 1`, `changeTime <= 10^5`，所以 `2^(x-1) > 10^5 + 10^5 = 2*10^5`，即 `x > 18`。

**Q2: 如果 `r` 可以等于 1 怎么办？**

> 如果 `r = 1`，每圈耗时恒为 `f`，永远不需要换胎。此时 `best[j] = f * j`，不会在 18 圈内截断。处理方式：对 `r = 1` 的轮胎单独处理，直接用 `f * numLaps` 作为候选答案。（本题约束 `r >= 2`，不会出现这种情况。）

**Q3: 预处理 `best` 数组时，能否进一步优化减少轮胎数量？**

> 可以。如果轮胎 A 的 `f_A <= f_B` 且 `r_A <= r_B`，那么 A 在每一圈上都比 B 快，B 永远不会被选中。可以先按 `(f, r)` 排序，用类似"天际线"的方法去除被支配的轮胎。不过由于 MAX_LAP 只有 18，即使不去重，预处理也是 O(n * 18)，实际影响不大。

**Q4: 这个 DP 能空间优化吗？**

> `dp[i]` 依赖 `dp[i-1], dp[i-2], ..., dp[i-18]`，需要保留最近 18 个状态。可以用长度 19 的滚动数组，但由于 `numLaps <= 1000`，O(numLaps) 空间已经很小，优化意义不大。

## 相关题型

- **1235. 规划兼职工作** — 同样是"选不重叠的区间使总价值最优"的 DP，区别是本题区间长度有上界 18
- **完全背包问题** — `best[j]` 相当于物品重量为 j、价值为 best[j]+changeTime 的物品，dp 求恰好装满容量 numLaps 的最小代价。本题本质是一个完全背包变体
- **322. 零钱兑换** — 结构几乎相同：dp[i] = min(dp[i-coin] + 1)，本题是 dp[i] = min(dp[i-j] + cost[j])，"硬币面值"就是 1~18