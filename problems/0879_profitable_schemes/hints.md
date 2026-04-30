# 879. 盈利计划

## 核心思路

这是一道**三维背包 DP** 问题。有若干犯罪活动，每个活动需要一定人数且产生一定利润。在总人数 <= n 的约束下，求满足总利润 >= minProfit 的方案数。关键：这是 0/1 背包的变体，多了一个"利润"维度，且利润维度有"下界约束"（>= minProfit）而非通常的"上界约束"。

## 思维链

1. **读完题第一反应** → 每个犯罪活动选或不选（0/1 选择），有两个约束：人数 <= n，利润 >= minProfit。这是一个带约束的组合计数问题，用 DP。
2. **状态设计** → dp[i][j][k] = 考虑前 i 个活动，使用了 j 个人，获得了 k 利润的方案数。但 k 可以超过 minProfit，怎么处理？
3. **利润维度处理** → 利润超过 minProfit 的都算有效方案。技巧：将利润 clamp 到 minProfit，即 `min(k + profit[i], minProfit)`。这样所有 >= minProfit 的利润都映射到同一个状态，减少状态空间。
4. **空间优化** → 活动维度可以滚动掉（标准 0/1 背包技巧），变为 dp[j][k]。j 和 k 倒序遍历（因为每个活动只能选一次）。
5. **最终答案** → dp[0..n][minProfit] 的总和？不对——clamp 后 dp[j][minProfit] 就已经包含了所有利润 >= minProfit 的方案。所以答案是 sum(dp[j][minProfit]) for j in [0, n]。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 三维 DP | dp[i][j][k] 完整状态 | O(G*n*P) | O(G*n*P) | 理解用 |
| 二维 DP（滚动） | 滚掉活动维度 | O(G*n*P) | O(n*P) | 首选 |

其中 G = group.length, P = minProfit。

## 关键提示

1. **利润 clamp 技巧**：`newProfit = min(k + profit[i], minProfit)`。超过 minProfit 的利润不需要区分，统一映射到 minProfit。这把利润维度从 [0, sum(profit)] 压缩到 [0, minProfit]。
2. **初始状态**：dp[0][0] = 1（不选任何活动，0 人，0 利润，这算一种方案）。
3. **遍历顺序**：外层遍历活动（0 到 G-1），内层遍历人数 j 从 n 到 group[i]（倒序，0/1 背包），利润 k 从 minProfit 到 0（倒序）。
4. **取模**：答案可能很大，每步加法都要 mod 10^9+7。
5. **最终答案**：`sum(dp[j][minProfit]) for j = 0 to n`。因为 clamp 后，dp[j][minProfit] 包含了所有利润 >= minProfit 的方案。

## 解法详解

### 解法1: 三维 DP — O(G*n*P) / O(G*n*P)

**思考过程**: 标准三维 DP，清晰但空间大。

```
示例: n=5, minProfit=3, group=[2,2], profit=[2,3]

dp[i][j][k] = 考虑前 i 个活动, 用 j 人, 利润为 k 的方案数

初始: dp[0][0][0] = 1

活动0 (group=2, profit=2):
  dp[1][0][0] = dp[0][0][0] = 1  (不选)
  dp[1][2][2] = dp[0][0][0] = 1  (选: 用2人, 利润2)

活动1 (group=2, profit=3):
  dp[2][0][0] = dp[1][0][0] = 1  (都不选)
  dp[2][2][2] = dp[1][2][2] = 1  (只选活动0)
  dp[2][2][3] = dp[1][0][0] = 1  (只选活动1, 利润3 clamp到3)
  dp[2][4][3] = dp[1][2][2] = 1  (选活动0和1, 利润5 clamp到3)

答案 = dp[2][j][3] for j=0..5 = 0+0+1+0+1+0 = 2
```

```cpp
class Solution {
public:
    int profitableSchemes(int n, int minProfit, vector<int>& group, vector<int>& profit) {
        const int MOD = 1e9 + 7;
        int G = group.size();
        
        // dp[j][k]: 使用 j 人，获利 k 的方案数
        vector<vector<int>> dp(n + 1, vector<int>(minProfit + 1, 0));
        dp[0][0] = 1; // 初始: 0人0利润有1种方案（什么都不选）
        
        for (int i = 0; i < G; i++) {
            int g = group[i], p = profit[i];
            // 倒序遍历（0/1背包经典技巧）
            for (int j = n; j >= g; j--) {
                for (int k = minProfit; k >= 0; k--) {
                    // 选活动 i: 人数从 j-g 来，利润从 max(0, k-p) 来
                    int newK = min(k + p, minProfit); // clamp
                    // 但我们是从 k 角度看: 如果目标利润是 k，
                    // 则选了活动 i 后只需要之前达到 max(0, k-p) 利润
                    int prevK = max(0, k - p);
                    dp[j][k] = (dp[j][k] + dp[j - g][prevK]) % MOD;
                }
            }
        }
        
        // 答案: 所有人数 j (0..n) 下利润 >= minProfit 的方案数
        int ans = 0;
        for (int j = 0; j <= n; j++) {
            ans = (ans + dp[j][minProfit]) % MOD;
        }
        return ans;
    }
};
```

**关键点**:
- `max(0, k - p)` 处理了利润溢出（选了活动 i 后利润可能直接达到 minProfit）。
- 倒序遍历保证每个活动最多选一次（0/1 背包）。

---

### 解法2: 更清晰的二维 DP 写法

**正向思维**: dp[j][k] 表示恰好用 j 人、获利恰好为 k 的方案数（利润 clamp 到 minProfit）。

```cpp
class Solution {
public:
    int profitableSchemes(int n, int minProfit, vector<int>& group, vector<int>& profit) {
        const int MOD = 1e9 + 7;
        int G = group.size();
        
        // dp[j][k]: 用 j 人，利润为 k 的方案数
        // k 上限为 minProfit (利润超过 minProfit 的都 clamp 到 minProfit)
        vector<vector<int>> dp(n + 1, vector<int>(minProfit + 1, 0));
        dp[0][0] = 1;
        
        for (int i = 0; i < G; i++) {
            int g = group[i], p = profit[i];
            // 倒序遍历人数和利润
            for (int j = n; j >= g; j--) {
                for (int k = minProfit; k >= 0; k--) {
                    // 选活动 i: 新利润 = min(k + p, minProfit)
                    int newK = min(k + p, minProfit);
                    dp[j][newK] = (dp[j][newK] + dp[j - g][k]) % MOD;
                }
            }
        }
        
        // 答案: 利润 == minProfit 的所有人数方案之和
        int ans = 0;
        for (int j = 0; j <= n; j++) {
            ans = (ans + dp[j][minProfit]) % MOD;
        }
        return ans;
    }
};
```

**注意**: 这种写法中转移方向是"从旧状态推到新状态"，需要小心倒序遍历不会覆盖还需要用的值。当 newK > k 时（总是如此因为 p >= 0），倒序遍历 k 保证 dp[j-g][k] 还是旧值。

## 易错点

1. **利润维度不 clamp 导致数组越界或状态爆炸**：
   - 如果直接用 `k + profit[i]` 作为新利润索引，利润可能超过 minProfit，数组越界。
   - 即使用足够大的数组，状态空间从 minProfit 膨胀到 sum(profit)，太大。
   - 必须 clamp：`newK = min(k + p, minProfit)`。

2. **初始状态遗漏**：
   - 必须 dp[0][0] = 1。"不选任何活动" 也是一种方案（虽然利润为 0，可能不满足 minProfit）。

3. **遍历顺序搞反**：
   - 人数和利润都必须倒序遍历（0/1 背包）。正序遍历会导致同一个活动被选多次（变成完全背包）。

4. **取模遗漏**：
   - 方案数可能非常大。每次加法都必须 mod 10^9+7，不能只在最后取模。

5. **答案只取 dp[n][minProfit] 而非 sum**：
   - 人数不要求恰好用完，所以答案是 sum(dp[j][minProfit]) for j=0..n，不只是 dp[n][minProfit]。

## 面试追问

**Q1: 这个问题和普通 0/1 背包有什么区别？**
> 普通 0/1 背包有一个"重量上界"约束，求最大价值或方案数。本题有两个约束：人数上界(<=n) 和利润下界(>=minProfit)。利润是"至少达到"而非"至多"，所以需要 clamp 技巧把 >= minProfit 的所有状态合并。

**Q2: 为什么可以 clamp 利润到 minProfit？**
> 因为我们只关心利润"是否 >= minProfit"，不关心超出多少。利润 50 和利润 100 只要都 >= minProfit 就都算有效方案。把它们合并到同一个状态不影响计数。

**Q3: 时间复杂度 O(G*n*P) 能否优化？**
> 很难。三个维度 G(活动数)、n(人数上限)、P(最小利润) 各最大 100，总计 10^6，完全可接受。如果约束范围更大，可能需要数学方法或近似算法。

**Q4: 如果每个活动可以选多次（完全背包），怎么改？**
> 内层遍历改为正序：j 从 group[i] 到 n，k 从 0 到 minProfit。这是完全背包和 0/1 背包的经典区别。

## 相关题型

- **494. 目标和** — 0/1 背包计数问题，求达到目标和的方案数。同样是"选或不选"的 DP。
- **416. 分割等和子集** — 0/1 背包判定问题，判断能否选出子集和等于 total/2。
- **518. 零钱兑换 II** — 完全背包计数问题，每种硬币可用无限次。
- **474. 一和零** — 二维背包（0 的个数和 1 的个数两个约束），状态设计类似本题。
- **1049. 最后一块石头的重量 II** — 0/1 背包变体，求两组最小差。
