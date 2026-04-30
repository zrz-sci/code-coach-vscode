# 518. 零钱兑换 II

## 核心思路

本题本质是**完全背包问题求方案数**：每种硬币（物品）可以无限次使用，问恰好凑出 `amount`（背包容量）的**组合数**。关键难点在于区分"组合"和"排列"——`{1,2}` 和 `{2,1}` 算同一种组合。

## 思维链

1. **读完题第一反应**：这是一个"凑零钱"问题，每种硬币可以用无限次，要求所有不同组合的数量。最朴素的想法——枚举每种硬币用多少个，用递归/回溯穷举所有可能。

2. **暴力解的瓶颈**：递归树分支巨大，而且存在大量重叠子问题。比如 `coins=[1,2,5], amount=5`，"先用1后用2凑到3" 和 "先用2后用1凑到3" 会重复计算 "凑剩余3" 的子问题。

3. **如何消除重叠子问题？**→ 记忆化搜索 / 动态规划。但这里有一个陷阱：如果 DP 的外层遍历金额、内层遍历硬币，得到的是**排列数**（`{1,2}` 和 `{2,1}` 算两种）。要得到**组合数**，必须让硬币的选取有序——外层遍历硬币，内层遍历金额。

4. **为什么外层遍历硬币能去重？**→ 遍历到硬币 `coins[i]` 时，只会用 `coins[0..i]` 来组合，不会回头用更早的硬币。这保证了每种组合只按一种"字典序"被统计一次。

5. **空间优化**：二维 `dp[i][j]` 中，`dp[i][j]` 只依赖 `dp[i-1][j]` 和 `dp[i][j-coin]`（同一行更靠左的值），因此可以压缩为一维数组，**正序遍历**金额即可（因为完全背包允许重复选取）。

6. **与 Coin Change I（LeetCode 322）的对比**：322 求最少硬币数（最优值），本题求组合方案数（计数）。转移方程从 `min` 变成了 `sum`。

## 解法概览

| 解法 | 思路 | 时间 | 空间 | 面试 |
|------|------|------|------|------|
| 暴力回溯 | 枚举每种硬币用多少个 | 指数级 | O(amount) | 能说出即可 |
| 记忆化搜索 | 自顶向下 + memo | O(n × amount) | O(n × amount) | 能说出即可 |
| 二维 DP | 完全背包，外层硬币内层金额 | O(n × amount) | O(n × amount) | 加分项 |
| 一维 DP（空间优化） | 滚动数组压缩 | O(n × amount) | O(amount) | ⭐ 必须写出 |

> n = coins.length

## 关键提示

1. **组合 vs 排列的关键区分**：如果你写出的 DP 是先遍历金额再遍历硬币，跑一下 `amount=3, coins=[1,2]` 看看结果是不是 2（组合）还是 3（排列，多了 `{2,1}`）。

2. **初始化 `dp[0] = 1`**：凑出金额 0 有且仅有 1 种方式——什么都不选。这不是"没有方案"，而是"空集也是一种合法方案"。

3. **完全背包 vs 0/1 背包的遍历方向**：0/1 背包一维优化时**倒序**遍历金额（每个物品只能用一次），完全背包**正序**遍历（物品可以重复用）。

4. **DP 填表过程示意**（`coins=[1,2,5], amount=5`）：
```
        金额:  0  1  2  3  4  5
初始化:        1  0  0  0  0  0
+coin=1:       1  1  1  1  1  1   ← 只用面值1，每个金额只有1种方式
+coin=2:       1  1  2  2  3  3   ← 加入面值2
+coin=5:       1  1  2  2  3  4   ← 加入面值5，dp[5]+=dp[0]=1
```

5. **这是完全背包系列题**：如果你理解了本题，可以直接迁移到任何"无限物品求方案数"的问题。

## 解法详解

### 解法1: 暴力回溯 — 指数级 / O(amount)

**思考过程**：最直觉的做法——从第一种硬币开始，枚举它用 0 个、1 个、2 个……直到超过剩余金额，然后递归处理下一种硬币。为了避免重复组合，我们用 `start` 索引保证只往后选硬币。

```cpp
class Solution {
public:
    int change(int amount, vector<int>& coins) {
        return dfs(coins, 0, amount);
    }
    
    int dfs(vector<int>& coins, int start, int remaining) {
        // 恰好凑完，找到一种合法组合
        if (remaining == 0) return 1;
        // 所有硬币都考虑过了，无法凑出
        if (start == coins.size()) return 0;
        
        int count = 0;
        // 枚举当前硬币 coins[start] 用多少个
        for (int k = 0; k * coins[start] <= remaining; k++) {
            count += dfs(coins, start + 1, remaining - k * coins[start]);
        }
        return count;
    }
};
```

**关键点**：`start` 参数保证了组合的有序性，避免 `{1,2}` 和 `{2,1}` 被重复统计。

---

### 解法2: 记忆化搜索 — O(n × amount) / O(n × amount)

**从解法1优化**：解法1 的递归树中存在大量重叠子问题。例如 `dfs(1, 3)` 可能被多次调用（不同的 `coins[0]` 使用次数都可能剩余 3 交给后续硬币处理）。加一个二维 memo 数组缓存即可。

```cpp
class Solution {
public:
    int change(int amount, vector<int>& coins) {
        int n = coins.size();
        // memo[i][j] = 从 coins[i..n-1] 中凑出金额 j 的组合数, -1 表示未计算
        vector<vector<int>> memo(n, vector<int>(amount + 1, -1));
        return dfs(coins, 0, amount, memo);
    }
    
    int dfs(vector<int>& coins, int start, int remaining, 
            vector<vector<int>>& memo) {
        if (remaining == 0) return 1;
        if (start == coins.size()) return 0;
        if (memo[start][remaining] != -1) return memo[start][remaining];
        
        int count = 0;
        for (int k = 0; k * coins[start] <= remaining; k++) {
            count += dfs(coins, start + 1, remaining - k * coins[start], memo);
        }
        return memo[start][remaining] = count;
    }
};
```

**关键点**：状态空间是 `O(n × amount)`，每个状态只计算一次。

---

### 解法3: 二维 DP — O(n × amount) / O(n × amount)

**从记忆化搜索翻译**：将自顶向下的递归翻译成自底向上的递推。

- **状态定义**：`dp[i][j]` = 用前 `i` 种硬币（`coins[0..i-1]`）凑出金额 `j` 的组合数
- **转移方程**：`dp[i][j] = dp[i-1][j] + dp[i][j - coins[i-1]]`
  - `dp[i-1][j]`：不使用第 `i` 种硬币
  - `dp[i][j - coins[i-1]]`：至少再用一枚第 `i` 种硬币（注意是 `dp[i]` 不是 `dp[i-1]`，因为可以重复选）
- **初始化**：`dp[0][0] = 1`（0 种硬币凑出 0 元 = 1 种方式），`dp[0][j>0] = 0`

```
// dp 填表方向 (coins=[1,2,5], amount=5):
//
// dp[i][j]:  j= 0  1  2  3  4  5
// i=0 (无)      1  0  0  0  0  0   ← base case
// i=1 (coin=1)  1  1  1  1  1  1   ← dp[1][j] = dp[0][j] + dp[1][j-1]
// i=2 (coin=2)  1  1  2  2  3  3   ← dp[2][j] = dp[1][j] + dp[2][j-2]
// i=3 (coin=5)  1  1  2  2  3  4   ← dp[3][5] = dp[2][5] + dp[3][0] = 3+1
//
// 依赖关系: dp[i][j] 依赖 ↑上方 和 ←左方(同行)
```

```cpp
class Solution {
public:
    int change(int amount, vector<int>& coins) {
        int n = coins.size();
        // dp[i][j] = 用前i种硬币凑出金额j的组合数
        vector<vector<int>> dp(n + 1, vector<int>(amount + 1, 0));
        dp[0][0] = 1; // 空集凑出0元，算1种方案
        
        for (int i = 1; i <= n; i++) {
            int coin = coins[i - 1];
            for (int j = 0; j <= amount; j++) {
                // 不用第i种硬币
                dp[i][j] = dp[i - 1][j];
                // 至少再用一枚第i种硬币（前提：j >= coin）
                if (j >= coin) {
                    dp[i][j] += dp[i][j - coin];
                }
            }
        }
        return dp[n][amount];
    }
};
```

---

### 解法4: 一维 DP（空间优化）— O(n × amount) / O(amount) ⭐ 面试首选

**从解法3压缩**：观察到 `dp[i][j]` 只依赖 `dp[i-1][j]`（上一行同列）和 `dp[i][j-coin]`（当前行左边）。用一维数组 `dp[j]` 即可：
- `dp[i-1][j]` 就是更新前的 `dp[j]`（天然保留）
- `dp[i][j-coin]` 就是当前轮**已更新过的** `dp[j-coin]`（正序遍历时左边已更新）

所以**正序遍历**金额 `j`，每次 `dp[j] += dp[j - coin]` 就自动实现了完全背包的"可重复选取"。

```cpp
class Solution {
public:
    int change(int amount, vector<int>& coins) {
        vector<int> dp(amount + 1, 0);
        dp[0] = 1; // 凑出0元有1种方式：什么都不选
        
        // 外层遍历硬币 → 保证组合（不重复计数）
        for (int coin : coins) {
            // 内层正序遍历金额 → 完全背包（硬币可重复使用）
            for (int j = coin; j <= amount; j++) {
                dp[j] += dp[j - coin];
            }
        }
        return dp[amount];
    }
};
```

**关键点**：这段代码极其简洁，但每一个设计选择都有深意：
- 外层 coin，内层 j → 组合（不是排列）
- j 从 coin 开始正序 → 允许重复选同一硬币
- dp[0] = 1 → 空集是合法方案

---

## 解法对比

| | 暴力回溯 | 记忆化搜索 | 二维 DP | 一维 DP |
|---|---|---|---|---|
| 时间 | 指数级 | O(n·amount) | O(n·amount) | O(n·amount) |
| 空间 | O(amount) 栈深 | O(n·amount) | O(n·amount) | O(amount) |
| 思维方向 | 自顶向下 | 自顶向下+缓存 | 自底向上 | 自底向上+压缩 |
| 面试推荐 | 说思路 | 中等 | 理解用 | ⭐ 首选 |

**一维 DP 是面试中写出来的标准答案**，但面试官可能追问你怎么从二维推导到一维的，所以必须理解二维版本。

## 易错点

1. **遍历顺序搞反导致求成排列数**：
   - ✗ 错误：外层遍历金额 `j`，内层遍历 `coin` → 这是 LeetCode 377 组合总和 IV 的排列写法
   - ✓ 正确：外层遍历 `coin`，内层遍历金额 `j` → 组合数
   ```cpp
   // ✗ 排列（错误）             // ✓ 组合（正确）
   for (int j = 0; ...)          for (int coin : coins)
     for (int coin : coins)        for (int j = coin; ...)
       dp[j] += dp[j-coin];          dp[j] += dp[j-coin];
   ```

2. **dp[0] 初始化为 0 而不是 1**：
   - ✗ `dp[0] = 0` → 所有 dp 值都是 0，因为没有"种子"
   - ✓ `dp[0] = 1` → 凑出 0 元的方式是"不选任何硬币"，这是 1 种合法方案

3. **完全背包正序 vs 0/1 背包倒序搞混**：
   - 本题硬币可以无限使用 → **正序**遍历 `j`（允许 `dp[j-coin]` 是当前轮已更新的值）
   - 如果每种硬币只能用一次 → **倒序**遍历 `j`（确保每个硬币只被计入一次）

4. **二维转一维时 j 的起始位置**：
   - ✗ `for (int j = 0; j <= amount; j++)` 然后在里面判断 `j >= coin`
   - ✓ 直接 `for (int j = coin; j <= amount; j++)`，更简洁且避免无用计算

## 面试追问

**Q1（基础理解）**：为什么外层遍历硬币、内层遍历金额就能保证不重复？
> 因为处理到硬币 `coins[i]` 时，`dp[j]` 只包含了 `coins[0..i]` 的贡献。不会出现"先用 coin2 再用 coin1"这种逆序情况，因为 coin1 的循环在 coin2 之前就已经固定了。

**Q2（变体：排列数）**：如果 `{1,2}` 和 `{2,1}` 算两种，怎么改？
> 交换两层循环的顺序：外层遍历金额，内层遍历硬币。这就是 LeetCode 377（组合总和 IV）。

**Q3（变体：0/1 背包）**：如果每种硬币只能用一次，怎么改？
> 内层循环**倒序**遍历金额：`for (int j = amount; j >= coin; j--)`。倒序保证 `dp[j-coin]` 是"不包含当前硬币"的值。

**Q4（Follow-up：输出所有具体组合）**：不只要数量，要输出所有组合？
> 用回溯（解法1），在 `remaining == 0` 时记录路径。DP 只能计数，无法还原路径。

**Q5（系列对比）**：和 LeetCode 322 Coin Change I 的区别？
> 322 求最少硬币数，转移用 `min`；本题求方案数，转移用 `+`。322 不需要区分组合/排列（因为 `min` 不受顺序影响），本题必须注意遍历顺序。

## 相关题型

- **LeetCode 322 零钱兑换 I** — 同样是完全背包，但求最小值不是计数。转移方程从 `dp[j] += dp[j-coin]` 改为 `dp[j] = min(dp[j], dp[j-coin]+1)`。
- **LeetCode 377 组合总和 IV** — 和本题几乎一样的代码，唯一区别是**交换两层循环顺序**（外层金额、内层硬币），求的是排列数而非组合数。
- **LeetCode 416 分割等和子集** — 0/1 背包（每个元素只能用一次），内层循环改为**倒序**遍历。
- **LeetCode 494 目标和** — 0/1 背包求方案数，和本题类似但每个数只能用一次。
- **LeetCode 279 完全平方数** — 完全背包求最小值，硬币变成了完全平方数。