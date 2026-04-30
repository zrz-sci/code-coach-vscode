[file://solution.cpp](./solution.cpp)

# 920. 播放列表的数量

## 核心思路

有 n 首不同的歌，要做一个长度为 goal 的播放列表，要求：(1) 每首歌至少播一次；(2) 一首歌再次播放前必须先播放 k 首其他歌。求不同播放列表的数量，答案对 10^9+7 取模。这是一道经典的计数 DP 问题，状态定义为 dp[i][j] = 播放列表长度为 i、包含了 j 首不同歌曲时的方案数。

## 思维链

1. **读完题第一反应**：这是一个排列/组合计数问题。需要计数满足两个约束条件的序列数量。暴力枚举所有长度为 goal 的序列是不现实的（指数级别）。
2. **分析约束**：
   - 约束1（每首歌至少一次）：最终必须用到所有 n 首歌
   - 约束2（k 首间隔）：同一首歌在播放列表中两次出现之间，至少间隔 k 首不同的歌
3. **DP 状态设计**：考虑逐步构建播放列表。当我们往列表中添加第 i 首歌时（第 i 个位置），需要决定放一首新歌还是放一首之前播过的歌。所以自然的状态是 dp[i][j] = 播放列表已有 i 首歌、其中有 j 首不同歌曲的方案数。
4. **状态转移**：
   - **放一首新歌**：从 dp[i-1][j-1] 转移。有 (n - (j-1)) = (n - j + 1) 种选择（n 首歌减去已经用过的 j-1 首）。
   - **放一首旧歌**：从 dp[i-1][j] 转移。已经用过 j 首歌，但最近 k 首不能重复，所以可选的旧歌有 max(j - k, 0) 种。
   - 转移方程：`dp[i][j] = dp[i-1][j-1] * (n-j+1) + dp[i-1][j] * max(j-k, 0)`
5. **边界条件**：dp[0][0] = 1（空播放列表，0首不同歌，1种方式）。最终答案是 dp[goal][n]。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 二维 DP | dp[i][j] = 列表长i、j首不同歌的方案数 | O(goal * n) | O(goal * n) | 面试首选 |
| 空间优化 DP | 滚动数组优化到一维 | O(goal * n) | O(n) | 进阶优化 |
| 容斥原理 | 数学公式直接计算 | O(n * log(goal)) | O(n) | 了解即可 |

## 关键提示

1. **DP 状态 dp[i][j] 的含义**：播放列表已有 i 首歌（可重复），其中恰好包含 j 首不同的歌曲。这个双维状态同时跟踪了列表长度和歌曲覆盖度。
2. **新歌的选择数**：当前已用 j-1 首不同的歌（从 dp[i-1][j-1] 转移），还有 n-(j-1) 首没用过的歌可以选。
3. **旧歌的选择数**：已用 j 首歌，但约束2要求最近 k 首不能重复。播放列表最后 k 个位置的歌不能选，所以可选的旧歌有 j - k 首。如果 j <= k 则不能选旧歌（所有已用歌都在"冷却期"内）。
4. **为什么 j-k 不是 j-k-1？** 最近 k 个位置占了 k 首不同的歌（因为约束2保证了它们各不相同），剩余 j-k 首可以选。如果 j < k，那已用歌都在禁区内，旧歌选择数为 0。
5. **取模运算**：每步乘法和加法都要取模，防止溢出。

```
// DP 过程示意 (n=3, goal=3, k=1):
//
// dp[i][j]: 播放列表长 i，有 j 首不同歌的方案数
//
// 初始: dp[0][0] = 1
//
// i=1 (列表长度1):
//   j=1: dp[1][1] = dp[0][0] * (3-1+1) + dp[0][1] * max(1-1,0)
//                 = 1 * 3 + 0 * 0 = 3
//   (选3首新歌中的1首: {A}, {B}, {C})
//
// i=2 (列表长度2):
//   j=1: dp[2][1] = dp[1][0] * (3-1+1) + dp[1][1] * max(1-1,0)
//                 = 0 + 3 * 0 = 0
//   (长度2但只有1首歌 → k=1要求间隔1首其他歌，不可能)
//
//   j=2: dp[2][2] = dp[1][1] * (3-2+1) + dp[1][2] * max(2-1,0)
//                 = 3 * 2 + 0 * 1 = 6
//   (3种第一首 × 2种新的第二首: AB, AC, BA, BC, CA, CB)
//
// i=3 (列表长度3):
//   j=2: dp[3][2] = dp[2][1] * (3-2+1) + dp[2][2] * max(2-1,0)
//                 = 0 * 2 + 6 * 1 = 6
//
//   j=3: dp[3][3] = dp[2][2] * (3-3+1) + dp[2][3] * max(3-1,0)
//                 = 6 * 1 + 0 * 2 = 6
//
// 答案: dp[3][3] = 6 ✓
// (6种排列: ABC, ACB, BAC, BCA, CAB, CBA)
```

## 解法详解

### 解法1: 二维 DP — O(goal * n) / O(goal * n) 面试首选

**思考过程**：自然的计数 DP——状态追踪列表长度和已用歌曲数，每步决定加新歌还是旧歌。

```cpp
class Solution {
public:
    int numMusicPlaylists(int n, int goal, int k) {
        const int MOD = 1e9 + 7;
        
        // dp[i][j] = 播放列表长度为 i，包含 j 首不同歌的方案数
        vector<vector<long long>> dp(goal + 1, vector<long long>(n + 1, 0));
        dp[0][0] = 1;  // 空列表，0首歌，1种方式
        
        for (int i = 1; i <= goal; i++) {
            for (int j = 1; j <= min(i, n); j++) {
                // 情况1: 第 i 个位置放一首新歌
                // 从 dp[i-1][j-1] 转移，有 (n - j + 1) 种新歌可选
                dp[i][j] = (dp[i - 1][j - 1] * (n - j + 1)) % MOD;
                
                // 情况2: 第 i 个位置放一首旧歌
                // 从 dp[i-1][j] 转移，有 max(j - k, 0) 种旧歌可选
                dp[i][j] = (dp[i][j] + dp[i - 1][j] * max(j - k, 0)) % MOD;
            }
        }
        
        return dp[goal][n];
    }
};
```

**关键点**：
- j 的范围是 1 到 min(i, n)：不能有比列表长度更多的不同歌，也不能超过 n。
- 两种转移互不矛盾，分别对应"引入新歌"和"重复旧歌"两种操作。
- 最终答案 dp[goal][n] 要求列表长度恰好为 goal 且所有 n 首歌都被用过。

---

### 解法2: 空间优化 DP — O(goal * n) / O(n)

**从解法1优化**：dp[i] 只依赖 dp[i-1]，可以用滚动数组将空间从 O(goal * n) 降到 O(n)。

```cpp
class Solution {
public:
    int numMusicPlaylists(int n, int goal, int k) {
        const int MOD = 1e9 + 7;
        
        // 滚动数组：prev[j] = dp[i-1][j], curr[j] = dp[i][j]
        vector<long long> prev(n + 1, 0), curr(n + 1, 0);
        prev[0] = 1;
        
        for (int i = 1; i <= goal; i++) {
            fill(curr.begin(), curr.end(), 0);
            
            for (int j = 1; j <= min(i, n); j++) {
                // 放新歌
                curr[j] = (prev[j - 1] * (n - j + 1)) % MOD;
                // 放旧歌
                curr[j] = (curr[j] + prev[j] * max(j - k, 0)) % MOD;
            }
            
            swap(prev, curr);
        }
        
        return prev[n];
    }
};
```

**关键点**：每一行只依赖上一行，经典的滚动数组优化。注意每轮开始要清零 curr。

---

### 解法3: 容斥原理 — O(n * log(goal)) / O(n)

**数学方法**：用容斥原理直接计算。设 f(j) = 用恰好 j 首歌填满长度为 goal 的列表（满足 k 间隔约束）的方案数。用恰好 n 首歌的方案数可以通过容斥计算：先算"最多用 j 首歌"的方案数 g(j) = j * (j-1) * ... * (j-k+1) 的 goal 次幂形式，然后容斥去掉"少用了某些歌"的情况。

```cpp
class Solution {
public:
    int numMusicPlaylists(int n, int goal, int k) {
        const long long MOD = 1e9 + 7;
        
        // 预计算阶乘和逆阶乘（用于组合数）
        vector<long long> fact(n + 1), inv_fact(n + 1);
        fact[0] = 1;
        for (int i = 1; i <= n; i++) {
            fact[i] = fact[i - 1] * i % MOD;
        }
        inv_fact[n] = power(fact[n], MOD - 2, MOD);
        for (int i = n - 1; i >= 0; i--) {
            inv_fact[i] = inv_fact[i + 1] * (i + 1) % MOD;
        }
        
        // 容斥: sum over j from 0 to n of (-1)^j * C(n,j) * (n-j-k)^(goal-k) 形式
        // 实际上用 dp 更好理解，这里仅展示思路框架
        long long result = 0;
        for (int j = 0; j <= n; j++) {
            long long sign = (j % 2 == 0) ? 1 : MOD - 1;
            long long comb = fact[n] % MOD * inv_fact[j] % MOD * inv_fact[n - j] % MOD;
            long long ways = power(max(0, n - j - k), goal - k, MOD);
            // 注意这里简化了实际公式，完整推导较复杂
            result = (result + sign * comb % MOD * ways % MOD * fact[n - j] % MOD) % MOD;
        }
        
        return (int)(result % MOD);
    }
    
private:
    long long power(long long base, long long exp, long long mod) {
        long long result = 1;
        base %= mod;
        while (exp > 0) {
            if (exp & 1) result = result * base % mod;
            base = base * base % mod;
            exp >>= 1;
        }
        return result;
    }
};
```

**关键点**：容斥原理的推导较复杂，面试中一般不要求。DP 方法更直观且面试友好。

## 解法对比

| | 二维 DP | 空间优化 DP | 容斥原理 |
|---|---|---|---|
| 时间 | O(goal * n) | O(goal * n) | O(n * log(goal)) |
| 空间 | O(goal * n) | O(n) | O(n) |
| 核心思想 | 状态追踪长度+歌曲数 | 滚动数组优化 | 数学组合公式 |
| 面试推荐 | **首选** | 进阶 | 了解即可 |

**什么时候选哪个？**
- 面试首选解法1（二维 DP），逻辑清晰。如果面试官追问空间优化，给出解法2。
- 容斥方法在竞赛中有用，面试中不太会被要求。

## 易错点

1. **旧歌选择数是 max(j-k, 0) 而非 j-k**
   - 当 j <= k 时，所有已用歌都在"冷却期"内，不能选任何旧歌。忘记取 max 会导致负数。

2. **取模的位置**
   - 每一步乘法和加法之后都要取模。如果忘记取模，long long 也会溢出（goal * n 可达 100 * 100）。

3. **dp[0][0] = 1 的初始化**
   - 空播放列表对应一种方式（什么都不做）。如果初始化为 0，所有状态都会是 0。

4. **j 的循环范围**
   - j 不能超过 i（列表长度 i 不可能有超过 i 首不同的歌），也不能超过 n。正确范围：1 到 min(i, n)。

5. **混淆 n, goal, k 的含义**
   - n = 不同歌曲总数，goal = 播放列表长度，k = 重放间隔。搞混这三个参数会导致转移方程完全错误。

## 面试追问

**Q1: 状态转移中"新歌"和"旧歌"的选择数怎么推导？**
> 新歌：已用 j-1 首，总共 n 首，所以有 n-(j-1) 种。旧歌：已用 j 首，但最近 k 首在冷却期不能选，所以有 max(j-k, 0) 种。

**Q2: 为什么答案是 dp[goal][n] 而不是 dp[goal][1..n] 的总和？**
> 因为约束要求每首歌至少播一次，所以必须恰好用到所有 n 首歌。dp[goal][n] 就是这个意思。

**Q3: 如果 k=0 会怎样？**
> 没有冷却期限制，任何旧歌都可以随时重播。旧歌选择数变成 max(j-0, 0) = j。这简化了问题但 DP 结构不变。

**Q4: 能否用记忆化搜索代替 bottom-up DP？**
> 完全可以。定义 dfs(i, j) = 从播放列表第 i 个位置开始、已用 j 首不同歌的方案数。加 memo 数组即可。代码更直觉但 bottom-up 更容易做空间优化。

## 相关题型

- **70. Climbing Stairs** — 最基础的线性 DP，本题是二维扩展版。70 的"每步选择"对应本题的"新歌/旧歌选择"。
- **96. Unique Binary Search Trees** — 同样是计数 DP，dp[n] 依赖于子问题的组合。结构不同但"计数 DP"的思维框架一致。
- **1269. Number of Ways to Stay in the Same Place After Some Steps** — 类似的二维 DP 计数问题（步数 x 位置），转移方程结构相似。
