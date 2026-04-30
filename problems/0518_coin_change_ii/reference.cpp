/*
 * LeetCode 518: 零钱兑换 II (Coin Change II)
 *
 * 【题目本质】
 * 完全背包问题求组合方案数：每种硬币可以无限使用，求恰好凑出 amount 的组合数。
 * 关键：组合（{1,2} 和 {2,1} 算一种），不是排列。
 *
 * 【解法总览】
 * 解法1: 暴力回溯           — 指数级 / O(amount)       — 最直觉
 * 解法2: 记忆化搜索         — O(n·amount) / O(n·amount) — 消除重叠子问题
 * 解法3: 二维 DP (完全背包)  — O(n·amount) / O(n·amount) — 理解填表过程
 * 解法4: 一维 DP (空间优化)  — O(n·amount) / O(amount)   — ⭐ 面试首选
 *
 * 【与系列题的关系】
 * - 322 Coin Change I: 同样是完全背包，但转移用 min 求最少硬币数，本题用 + 求方案数
 * - 377 组合总和 IV:   和本题代码几乎相同，唯一区别是交换两层循环(外层金额内层硬币)→排列数
 * - 416 分割等和子集:  0/1 背包，内层循环倒序
 */

// ============================================================
// 解法1: 暴力回溯 — 枚举每种硬币使用次数
// 时间: 指数级（大量重复计算）  空间: O(amount) 递归栈深度
//
// 【思路】
// 最直觉的想法：对每种硬币，枚举用 0 个、1 个、2 个...
// 用 start 参数保证硬币按顺序选取，避免 {1,2} 和 {2,1} 重复。
//
// 决策树 (coins=[1,2,5], amount=5):
//                          dfs(start=0, rem=5)
//                     /          |           \
//              coin1用0个    coin1用1个   coin1用2个  ... coin1用5个
//            dfs(1, 5)     dfs(1, 4)    dfs(1, 3)     dfs(1, 0)→✓
//           /     \        /     \
//      c2用0个  c2用1个  c2用0个 c2用1个 ...
//     dfs(2,5) dfs(2,3) dfs(2,4) dfs(2,2)
//        |       |        |       |
//    c5用1个→✓  rem=3>0  c5不够   c2用1个→dfs(2,0)→✓
//              无解                 → {2,2,1} ✓
// ============================================================
class Solution1 {
public:
    int change(int amount, vector<int>& coins) {
        return dfs(coins, 0, amount);
    }

private:
    int dfs(vector<int>& coins, int start, int remaining) {
        // 恰好凑完 → 找到一种合法组合
        if (remaining == 0) return 1;
        // 所有硬币都考虑过了
        if (start == (int)coins.size()) return 0;

        int count = 0;
        // 枚举 coins[start] 使用 k 枚
        for (int k = 0; k * coins[start] <= remaining; k++) {
            // 递归处理后续硬币，remaining 减少 k * coins[start]
            count += dfs(coins, start + 1, remaining - k * coins[start]);
        }
        return count;
    }
};

// ============================================================
// 解法2: 记忆化搜索 — 自顶向下 DP
// 时间: O(n × amount)  空间: O(n × amount)
//
// 【思路】
// 解法1 的瓶颈：大量重叠子问题。例如 dfs(1, 3) 可能从多条路径到达。
// 加一个二维 memo 数组，memo[start][remaining] 缓存已算过的结果。
// 状态总数 = n × amount，每个状态内的循环累计也是 O(amount/coin)，
// 但总工作量不超过 O(n × amount)。
// ============================================================
class Solution2 {
public:
    int change(int amount, vector<int>& coins) {
        int n = coins.size();
        // memo[i][j] = 用 coins[i..n-1] 凑出金额 j 的方案数，-1 表示未计算
        vector<vector<int>> memo(n, vector<int>(amount + 1, -1));
        return dfs(coins, 0, amount, memo);
    }

private:
    int dfs(vector<int>& coins, int start, int remaining,
            vector<vector<int>>& memo) {
        if (remaining == 0) return 1;
        if (start == (int)coins.size()) return 0;
        if (memo[start][remaining] != -1) return memo[start][remaining];

        int count = 0;
        for (int k = 0; k * coins[start] <= remaining; k++) {
            count += dfs(coins, start + 1, remaining - k * coins[start], memo);
        }
        return memo[start][remaining] = count;
    }
};

// ============================================================
// 解法3: 二维 DP — 完全背包递推
// 时间: O(n × amount)  空间: O(n × amount)
//
// 【思路】
// 把记忆化搜索翻译为自底向上递推。
//
// 状态定义: dp[i][j] = 用前 i 种硬币凑出金额 j 的组合数
// 转移方程:
//   dp[i][j] = dp[i-1][j]             // 不用第 i 种硬币
//            + dp[i][j - coins[i-1]]   // 至少再用一枚第 i 种（注意是 dp[i] 不是 dp[i-1]）
//   其中 dp[i][j-coin] 体现了"可以重复选取"（完全背包特征）
//
// 初始化: dp[0][0] = 1 (0种硬币凑出0元=1种方案), dp[0][j>0] = 0
//
// dp 填表过程 (coins=[1,2,5], amount=5):
//
//              j=  0   1   2   3   4   5
// i=0 (无硬币)    1   0   0   0   0   0    ← base case: 只有空集凑出0
// i=1 (coin=1)    1   1   1   1   1   1    ← 只用面值1: 每个金额恰好1种
// i=2 (coin=2)    1   1   2   2   3   3    ← dp[2][2]=dp[1][2]+dp[2][0]=1+1=2
// i=3 (coin=5)    1   1   2   2   3   4    ← dp[3][5]=dp[2][5]+dp[3][0]=3+1=4
//
// 依赖方向:  dp[i][j] 依赖 ↑正上方(dp[i-1][j]) 和 ←同行左方(dp[i][j-coin])
//            所以行从上到下，列从左到右遍历
// ============================================================
class Solution3 {
public:
    int change(int amount, vector<int>& coins) {
        int n = coins.size();
        vector<vector<int>> dp(n + 1, vector<int>(amount + 1, 0));

        // base case: 0种硬币凑出0元 = 1种方案（空集）
        dp[0][0] = 1;

        for (int i = 1; i <= n; i++) {
            int coin = coins[i - 1];
            for (int j = 0; j <= amount; j++) {
                // 不用第 i 种硬币
                dp[i][j] = dp[i - 1][j];
                // 至少再用一枚第 i 种硬币
                if (j >= coin) {
                    // 注意是 dp[i] 而非 dp[i-1]，允许重复选取同一硬币
                    dp[i][j] += dp[i][j - coin];
                }
            }
        }
        return dp[n][amount];
    }
};

// ============================================================
// 解法4: 一维 DP (空间优化) — ⭐ 面试首选
// 时间: O(n × amount)  空间: O(amount)
//
// 【思路】
// 从解法3的二维 DP 压缩而来。
// 观察依赖关系:
//   dp[i][j] = dp[i-1][j] + dp[i][j-coin]
//   - dp[i-1][j]     = 更新前的 dp[j]（一维数组中天然保留）
//   - dp[i][j-coin]  = 当前轮已更新过的 dp[j-coin]（正序遍历时左边先更新）
//
// 所以用一维数组，正序遍历金额，dp[j] += dp[j-coin] 即可。
//
// 【关键：为什么外层遍历硬币 → 组合数？】
// 处理完 coin=1 后，dp 中只包含"仅用1"的方案。
// 处理 coin=2 时，dp[j] += dp[j-2]，加入的是"至少含一枚2"的新方案。
// 这保证了每种组合只被统计一次，不会出现 {1,2} 和 {2,1} 重复。
//
// 【如果外层遍历金额，内层遍历硬币呢？】
// 那就变成了排列数！因为对于金额 j，每种硬币都有机会作为"最后一枚"，
// {1,2} 和 {2,1} 就被当作不同方案了。→ 这就是 LeetCode 377
//
// 一维 dp 更新过程 (coins=[1,2,5], amount=5):
//
// 初始:     dp = [1, 0, 0, 0, 0, 0]
//
// coin=1:   j=1: dp[1]+=dp[0]=1  → [1, 1, 0, 0, 0, 0]
//           j=2: dp[2]+=dp[1]=1  → [1, 1, 1, 0, 0, 0]
//           j=3: dp[3]+=dp[2]=1  → [1, 1, 1, 1, 0, 0]
//           j=4: dp[4]+=dp[3]=1  → [1, 1, 1, 1, 1, 0]
//           j=5: dp[5]+=dp[4]=1  → [1, 1, 1, 1, 1, 1]
//
// coin=2:   j=2: dp[2]+=dp[0]=1  → [1, 1, 2, 1, 1, 1]
//           j=3: dp[3]+=dp[1]=1  → [1, 1, 2, 2, 1, 1]
//           j=4: dp[4]+=dp[2]=2  → [1, 1, 2, 2, 3, 1]
//           j=5: dp[5]+=dp[3]=2  → [1, 1, 2, 2, 3, 3]
//
// coin=5:   j=5: dp[5]+=dp[0]=1  → [1, 1, 2, 2, 3, 4]
//
// 答案: dp[5] = 4 ✓
// ============================================================
class Solution {
public:
    int change(int amount, vector<int>& coins) {
        vector<int> dp(amount + 1, 0);
        dp[0] = 1; // 凑出0元的方式：什么都不选，这是1种合法方案

        // 外层遍历硬币种类 → 保证统计的是组合数（不是排列数）
        for (int coin : coins) {
            // 内层正序遍历金额 → 完全背包（同一硬币可重复使用）
            // j 从 coin 开始，因为 j < coin 时 dp[j] 不会变
            for (int j = coin; j <= amount; j++) {
                // dp[j-coin] 是当前轮已更新的值，包含了"再用一枚当前硬币"的方案
                dp[j] += dp[j - coin];
            }
        }
        return dp[amount];
    }
};

// ============================================================
// 【解法对比】
//
// | 解法        | 时间           | 空间           | 特点                 |
// |-------------|----------------|----------------|----------------------|
// | 暴力回溯    | 指数级         | O(amount)      | 最直觉，用于理解     |
// | 记忆化搜索  | O(n·amount)    | O(n·amount)    | 自顶向下，好理解     |
// | 二维 DP     | O(n·amount)    | O(n·amount)    | 自底向上，可视化填表 |
// | 一维 DP     | O(n·amount)    | O(amount)      | ⭐ 最优，面试标准答案 |
//
// 解法3 → 解法4: 空间从 O(n·amount) 降到 O(amount)
// 因为每一行只依赖上一行和当前行左边，用一维滚动即可
//
// ============================================================
// 【易错点】
//
// 1. 遍历顺序搞反 → 变成排列数:
//    ✗ for(j) for(coin) dp[j]+=dp[j-coin]  → 排列数（LeetCode 377）
//    ✓ for(coin) for(j) dp[j]+=dp[j-coin]  → 组合数（本题）
//
// 2. dp[0] 初始化错误:
//    ✗ dp[0] = 0  → 所有值永远是0，因为没有"种子"
//    ✓ dp[0] = 1  → 空集凑出0元是1种合法方案
//
// 3. 完全背包 vs 0/1背包遍历方向:
//    ✗ 内层倒序 for(j=amount; j>=coin; j--) → 这是0/1背包，每种硬币只用一次
//    ✓ 内层正序 for(j=coin; j<=amount; j++) → 完全背包，允许重复选取
//
// 4. 二维DP转移时写成 dp[i-1][j-coin]:
//    ✗ dp[i][j] = dp[i-1][j] + dp[i-1][j-coin]  → 0/1背包（每种硬币最多用1次）
//    ✓ dp[i][j] = dp[i-1][j] + dp[i][j-coin]     → 完全背包（可重复选取）
//
// ============================================================
// 【面试追问 — 递进链】
//
// Q1（基础）: 为什么外层遍历硬币就能避免重复？
// → 处理 coins[i] 时，dp 只包含 coins[0..i] 的贡献，
//   不可能出现"先选大面值后选小面值"的逆序情况。
//
// Q2（变体）: 如果 {1,2} 和 {2,1} 算两种（排列），怎么改？
// → 交换两层循环：外层遍历金额，内层遍历硬币。就是 LeetCode 377。
//
// Q3（变体）: 如果每种硬币只能用一次，怎么改？
// → 内层循环改为倒序：for(j=amount; j>=coin; j--)
//   倒序保证 dp[j-coin] 是"还没选当前硬币"的旧值。就是 0/1 背包。
//
// Q4（追问）: 能不能输出所有具体组合？
// → DP 只能计数。要输出方案，用回溯（解法1），在 remaining==0 时记录路径。
//
// Q5（系列对比）: 和 LeetCode 322 的关系？
// → 322 求最少硬币数: dp[j] = min(dp[j], dp[j-coin]+1)
//   本题求方案数:     dp[j] = dp[j] + dp[j-coin]
//   322 不需要区分组合/排列（min 不受顺序影响），本题必须注意遍历顺序。
// ============================================================
