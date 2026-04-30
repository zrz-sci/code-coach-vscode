/*
 * LeetCode 322: 零钱兑换 (Coin Change)
 *
 * 【题目本质】
 * 完全背包问题：每种硬币无限使用，凑满目标金额的最少硬币数。
 * 也可以看作图的最短路径：从 amount 到 0，每步减去一个硬币面值。
 *
 * 【解法总览】
 * 解法1: 暴力递归         — O(S^n) / O(S)     — 理解问题结构
 * 解法2: 记忆化搜索       — O(S×n) / O(S)     — 消除重叠子问题
 * 解法3: 动态规划(递推)   — O(S×n) / O(S)     — 面试首选 ⭐
 * 解法4: BFS             — O(S×n) / O(S)     — 最短路径视角
 *
 * 其中 S = amount, n = coins.length
 */

// ============================================================
// 解法1: 暴力递归 — 决策树穷举
// 时间: O(S^n)  空间: O(S) 递归栈深度
//
// 【思路】
// 要凑金额 amount，枚举选哪种硬币，选一枚后递归求解剩余金额。
// 这是最直觉的方式，展示了问题的递归结构。
//
// 决策树 (coins=[1,2,5], amount=11):
//
//                          11
//                       /   |    \
//                     10    9     6         ← 选1, 选2, 选5
//                    /|\   /|\   /|\
//                   9 8 5 8 7 4 5 4 1       ← 注意: 9出现两次(重叠子问题!)
//                  ...
//
// 瓶颈：指数级时间，大量重复计算。
// ============================================================
class Solution1 {
public:
    int coinChange(vector<int>& coins, int amount) {
        // base case
        if (amount == 0) return 0;
        if (amount < 0) return -1;

        int res = INT_MAX;
        for (int coin : coins) {
            int sub = coinChange(coins, amount - coin);
            if (sub == -1) continue;       // 这条路走不通
            res = min(res, sub + 1);       // +1 = 当前选的这枚硬币
        }
        return res == INT_MAX ? -1 : res;
    }
};

// ============================================================
// 解法2: 记忆化搜索 (自顶向下 DP)
// 时间: O(S × n)  空间: O(S)
//
// 【思路】
// 解法1的瓶颈是重复计算子问题。比如 coinChange(9) 从
// coinChange(11)-2 和 coinChange(10)-1 都会调用到。
// 加一个 memo 数组，每个金额只算一次。
//
// 关键：memo 初始值不能用 -1（和"无解"冲突），用 -2 表示"未计算"。
// ============================================================
class Solution2 {
public:
    int coinChange(vector<int>& coins, int amount) {
        // -2 = 未计算, -1 = 无解, >=0 = 最少硬币数
        vector<int> memo(amount + 1, -2);
        return dfs(coins, amount, memo);
    }

private:
    int dfs(vector<int>& coins, int amount, vector<int>& memo) {
        if (amount == 0) return 0;
        if (amount < 0) return -1;
        if (memo[amount] != -2) return memo[amount];  // 已算过，直接返回

        int res = INT_MAX;
        for (int coin : coins) {
            int sub = dfs(coins, amount - coin, memo);
            if (sub == -1) continue;
            res = min(res, sub + 1);
        }
        memo[amount] = (res == INT_MAX) ? -1 : res;
        return memo[amount];
    }
};

// ============================================================
// 解法3: 动态规划 (自底向上递推) ⭐ 面试首选
// 时间: O(S × n)  空间: O(S)
//
// 【思路】
// 把记忆化搜索翻译成循环：
//   dp[i] = 凑出金额 i 最少需要多少枚硬币
//   dp[0] = 0 (凑出0需要0枚)
//   dp[i] = min(dp[i - coin] + 1) 对所有 coin <= i
//
// 这就是完全背包：每种硬币可以选无限次。
// 外层正序遍历金额(从小到大)，保证 dp[i-coin] 包含了
// "当前这枚硬币已经被选过"的情况 → 允许重复选。
//
// dp 填表过程 (coins=[1,2,5], amount=11):
//
//   i:    0  1  2  3  4  5  6  7  8  9  10  11
//   dp:   0  1  1  2  2  1  2  2  3  3   2   3
//
//   dp[1]  = dp[0]+1 = 1                (选1枚1)
//   dp[2]  = min(dp[1]+1, dp[0]+1) = 1  (选1枚2)
//   dp[3]  = min(dp[2]+1, dp[1]+1) = 2  (选1+2 或 1+1+1)
//   dp[5]  = min(dp[4]+1, dp[3]+1, dp[0]+1) = 1  (选1枚5)
//   dp[11] = min(dp[10]+1, dp[9]+1, dp[6]+1) = 3 (选5+5+1)
// ============================================================
class Solution3 {
public:
    int coinChange(vector<int>& coins, int amount) {
        // 用 amount+1 作为"不可能"的初始值
        // 为什么不用 INT_MAX？因为 dp[i-coin]+1 会溢出！
        vector<int> dp(amount + 1, amount + 1);
        dp[0] = 0;  // base case: 凑出金额0需要0枚硬币

        for (int i = 1; i <= amount; i++) {
            for (int coin : coins) {
                if (coin <= i) {
                    // 选这枚硬币：需要 dp[i-coin] + 1 枚
                    // 不选：保持当前 dp[i]
                    // 两者取较小值
                    dp[i] = min(dp[i], dp[i - coin] + 1);
                }
            }
        }

        // dp[amount] > amount 说明凑不出来（最多用 amount 枚面值1的硬币）
        return dp[amount] > amount ? -1 : dp[amount];
    }
};

// ============================================================
// 解法4: BFS — 最短路径视角
// 时间: O(S × n)  空间: O(S)
//
// 【思路】
// 换个角度：把每个金额看作图中的一个节点。
// 从节点 v 到节点 v-coin 有一条边(对每种硬币)。
// 从 amount 出发 BFS，第一次到达 0 的层数 = 最少硬币数。
// BFS 天然保证"最先到达 = 最短路径"。
//
// BFS 过程 (coins=[1,2,5], amount=11):
//
//   Level 0: {11}
//   Level 1: {10, 9, 6}                ← 11-1, 11-2, 11-5
//   Level 2: {8, 7, 5, 4, 1}          ← 去重后
//   Level 3: {3, 2, 0}                ← 到达 0! 返回 3
//
// 关键：必须用 visited 数组去重，否则同一金额反复入队，
//       时间退化为指数级。
// ============================================================
class Solution4 {
public:
    int coinChange(vector<int>& coins, int amount) {
        if (amount == 0) return 0;

        queue<int> q;
        vector<bool> visited(amount + 1, false);
        q.push(amount);
        visited[amount] = true;
        int steps = 0;

        while (!q.empty()) {
            steps++;
            int size = q.size();
            for (int i = 0; i < size; i++) {
                int curr = q.front();
                q.pop();
                for (int coin : coins) {
                    int next = curr - coin;
                    if (next == 0) return steps;       // 到达目标!
                    if (next > 0 && !visited[next]) {
                        visited[next] = true;          // 入队前标记，防止重复
                        q.push(next);
                    }
                }
            }
        }

        return -1;  // BFS 遍历完仍未到达 0
    }
};

// ============================================================
// 【解法对比】
//
// | 解法       | 时间      | 空间 | 特点                          |
// |-----------|-----------|------|-------------------------------|
// | 暴力递归   | O(S^n)   | O(S) | 理解递归结构，但超时          |
// | 记忆化搜索 | O(S×n)   | O(S) | 自顶向下，子问题稀疏时有优势  |
// | DP 递推 ⭐ | O(S×n)   | O(S) | 面试标准答案，简洁无栈溢出    |
// | BFS       | O(S×n)   | O(S) | 最短路径视角，面试提及加分    |
//
// 面试推荐顺序：先说暴力递归思路 → 识别重叠子问题 → 给出 DP 递推
//
// 【为什么不需要空间优化】
// dp[i] 本身就是一维数组，已经是 O(S) 空间，无法进一步优化。
// (对比二维 DP 问题可以从 O(m×n) 降到 O(n)，本题天然一维)
//
// 【完全背包 vs 0/1 背包】
// 本题是完全背包（每种硬币可以用无限次）：
//   for (int i = 1; i <= amount; i++)       ← 金额从小到大
//       for (int coin : coins)
//           dp[i] = min(dp[i], dp[i-coin]+1)
//
// 如果改成 0/1 背包（每种硬币只能用一次）：
//   for (int coin : coins)                  ← 外层遍历硬币
//       for (int i = amount; i >= coin; i--) ← 金额从大到小!
//           dp[i] = min(dp[i], dp[i-coin]+1)
//
// 【易错点】
// 1. 初始化用 INT_MAX 导致 +1 溢出:
//    ✗ vector<int> dp(amount+1, INT_MAX);  → dp[i-coin]+1 溢出为负数
//    ✓ vector<int> dp(amount+1, amount+1); → 安全的不可能上界
//
// 2. 记忆化搜索 memo 标记值和返回值冲突:
//    ✗ memo 初始化为 -1，无解也返回 -1 → 无解的子问题每次都重新计算
//    ✓ memo 初始化为 -2（或用 optional），-1 表示无解
//
// 3. BFS 忘记 visited 去重:
//    ✗ 不加 visited → 金额 9 会从多条路径重复入队，内存爆炸
//    ✓ 入队前标记 visited[next] = true
//
// 4. coins[i] 可能很大(最大 2^31-1)，但 amount 最大 10^4:
//    ✗ 不加 coin <= i 判断 → i - coin 变成负数(但因为是 int 不会越界，
//       只是访问 dp[负数]，未定义行为!)
//    ✓ if (coin <= i) dp[i] = min(dp[i], dp[i-coin]+1);
//
// 【面试追问】
// Q1: "暴力递归为什么超时？画出递归树说明重叠子问题"
//     → 递归树中同一金额被从不同路径多次计算，比如 f(9) 从
//       f(11-2) 和 f(10-1) 各走一次。总节点数指数级。
//
// Q2: "贪心为什么不行？举个反例"
//     → coins=[1,3,4], amount=6。贪心选4+1+1=3枚，最优3+3=2枚。
//       大面额优先不能保证全局最优。
//
// Q3: "这道题和完全背包什么关系？如果每种硬币只能用一次怎么改？"
//     → 本题就是完全背包(物品=硬币, 容量=amount, 价值=1, 求最小)。
//       0/1背包：外层遍历硬币，内层金额从大到小（保证每种最多选一次）。
//
// Q4: "如果要输出具体选了哪些硬币（而不只是最少数量），怎么改？"
//     → 额外维护 parent[i] 记录 dp[i] 是从哪个 dp[i-coin] 转移来的，
//       最后从 parent[amount] 逆推回 0，每步的 coin = i - parent[i]。
//
// 【相关题型】
// 518. 零钱兑换 II — 同样的完全背包框架，改 min 为 sum:
//      dp[i] = min(dp[i], dp[i-coin]+1)  →  dp[i] += dp[i-coin]
//      注意518外层必须遍历硬币(避免重复组合)，本题两层顺序皆可。
//
// 279. 完全平方数 — 完全相同的结构:
//      把 coins 换成 [1,4,9,16,...], 代码一模一样。
//
// 377. 组合总和 Ⅳ — 完全背包求排列数(顺序有关):
//      外层遍历金额，内层遍历硬币（和本题一样），但求的是 sum 不是 min。
// ============================================================
