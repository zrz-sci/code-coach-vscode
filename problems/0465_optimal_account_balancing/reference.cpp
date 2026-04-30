/*
 * LeetCode 465: Optimal Account Balancing (最优账单平衡)
 *
 * 【题目本质】
 * 把所有交易汇总成每个人的净余额后，问：最少需要几笔转账才能让
 * 所有非零余额清零？等价于：把非零余额集合划分成尽可能多的
 * "和为0的子集"，总交易数 = n - 子集数。
 *
 * 【解法总览】
 * 解法1: 回溯 + 剪枝 — O(n!) / O(n) — 面试首选，思路直觉
 * 解法2: Bitmask DP — O(3^n) / O(2^n) — 最优解，确定性复杂度
 */

// ============================================================
// 解法1: 回溯 + 剪枝 — 逐个配对消除余额
// 时间: O(n!) 最坏，剪枝后远小于此
// 空间: O(n) 递归栈
//
// 【思路】
// 1. 先算每个人的净余额，过滤掉余额为0的
// 2. 对第一个非零余额，尝试和后面每个异号余额配对
//    配对 = 做一笔交易，start的余额转给i
// 3. 递归处理剩余，取最小交易数
// 4. 关键剪枝：如果某个配对恰好让两人都清零，直接选（贪心）
//
// 决策树 (debt = [-5, 3, 2]):
//              debt[0]=-5
//             /           \
//    配debt[1]=3         配debt[2]=2
//   debt=[-2, 0, 2]     debt=[-3, 3, 0]
//       |                     |
//   -2配2 → 全清零        -3配3 → 全清零
//   交易数=2              交易数=2
// ============================================================
class Solution1 {
public:
    int minTransfers(vector<vector<int>>& transactions) {
        unordered_map<int, int> balance;
        for (auto& t : transactions) {
            balance[t[0]] -= t[2];  // 付钱的人余额减少
            balance[t[1]] += t[2];  // 收钱的人余额增加
        }

        vector<int> debt;
        for (auto& [id, bal] : balance) {
            if (bal != 0) debt.push_back(bal);
        }

        return dfs(debt, 0);
    }

private:
    int dfs(vector<int>& debt, int start) {
        // 跳过已经在之前的交易中被清零的位置
        while (start < (int)debt.size() && debt[start] == 0) start++;

        if (start == (int)debt.size()) return 0;

        int res = INT_MAX;
        for (int i = start + 1; i < (int)debt.size(); i++) {
            // 只和符号相反的余额配对（正配负）才能互相抵消
            if ((long long)debt[start] * debt[i] < 0) {
                // 做一笔交易：把 debt[start] 的全部余额转给 i
                // 等价于 i 承担了 start 的债务/债权
                debt[i] += debt[start];
                res = min(res, 1 + dfs(debt, start + 1));
                debt[i] -= debt[start];  // 撤销选择

                // 剪枝：如果恰好完全抵消(debt[i]原值 == -debt[start])
                // 那么选这个配对一定最优（一笔交易消两人），不需要再试别的
                if (debt[i] + debt[start] == 0) break;
            }
        }
        return res;
    }
};


// ============================================================
// 解法2: Bitmask DP — 最大化和为0的子集数量
// 时间: O(3^n)  空间: O(2^n)
//
// 【思路】
// 核心观察：一组 k 个人的余额和为 0，他们内部需要恰好 k-1 笔交易。
// 所以如果能把 n 个非零余额划分为 g 个组（每组和为 0），
// 总交易数 = n - g。问题变成：最大化 g。
//
// dp[mask] = mask 这个子集最多能被分成多少个"和为0"的独立组。
//
// 转移：对于每个 mask，枚举它的子集 sub：
//   如果 sum[sub] == 0，则 dp[mask] = max(dp[mask], dp[mask^sub] + 1)
//
// 预处理每个 mask 的余额和 sum[mask]，用 lowbit 技巧 O(1) 推导。
//
// 子集枚举复杂度分析：
//   每个元素有三种归属：在sub / 在mask\sub / 不在mask
//   总操作 = Σ C(n,k)·2^k = 3^n
//
// 示例 debt = [-5, 3, 2]:
//   sum[0b111] = -5+3+2 = 0 → 整体是一个和为0的组
//   枚举子集：0b011(和=-2), 0b101(和=-3), 0b110(和=5)
//   没有更小的和为0子集 → dp[0b111] = 1
//   答案 = 3 - 1 = 2
// ============================================================
class Solution2 {
public:
    int minTransfers(vector<vector<int>>& transactions) {
        unordered_map<int, int> balance;
        for (auto& t : transactions) {
            balance[t[0]] -= t[2];
            balance[t[1]] += t[2];
        }

        vector<int> debt;
        for (auto& [id, bal] : balance) {
            if (bal != 0) debt.push_back(bal);
        }

        int n = debt.size();
        if (n == 0) return 0;

        int total = 1 << n;

        // 预处理：sum[mask] = mask 对应子集的余额之和
        // 利用 lowbit 递推：sum[mask] = sum[mask去掉最低位] + debt[最低位索引]
        vector<int> sum(total, 0);
        for (int mask = 1; mask < total; mask++) {
            int lowbit = mask & (-mask);
            int idx = __builtin_ctz(lowbit);
            sum[mask] = sum[mask ^ lowbit] + debt[idx];
        }

        // dp[mask] = mask 子集最多能分成多少个和为0的组
        vector<int> dp(total, 0);

        for (int mask = 1; mask < total; mask++) {
            // 只有 sum[mask]==0 时，mask 整体才可能至少算一个组
            // 否则 dp[mask] 只能从子集组合中得到

            // 枚举 mask 的所有非空真子集
            // 经典技巧：sub = (mask-1) & mask 开始，每次 sub = (sub-1) & mask
            for (int sub = (mask - 1) & mask; sub > 0; sub = (sub - 1) & mask) {
                if (sum[sub] == 0) {
                    // sub 是一个和为0的子集，可以独立清算
                    // 剩余 mask^sub 的最优划分 + 1
                    dp[mask] = max(dp[mask], dp[mask ^ sub] + 1);
                }
            }

            // 如果 mask 整体和为0，它本身也可以作为一个组
            // （可能在上面循环中已经被更好的划分覆盖了）
            if (sum[mask] == 0) {
                dp[mask] = max(dp[mask], 1);
            }
        }

        // 最少交易数 = 非零余额数 - 最多能分出的和为0子集数
        return n - dp[total - 1];
    }
};


// ============================================================
// 【解法对比】
//
// | 维度         | 回溯+剪枝        | Bitmask DP      |
// |-------------|------------------|-----------------|
// | 时间复杂度   | O(n!) 最坏       | O(3^n) 确定性    |
// | 空间复杂度   | O(n) 递归栈      | O(2^n) dp数组   |
// | 代码量       | 较少             | 中等            |
// | 面试推荐     | ⭐首选           | 加分项          |
// | 正确性证明   | 枚举所有可能     | 数学最优划分     |
//
// 面试建议：先写回溯，如果被追问复杂度，再给 Bitmask DP。
//
// ============================================================
// 【易错点】
//
// 1. 余额方向搞反：
//    ✗ balance[t[0]] += t[2]  ← from的人余额增加？不对
//    ✓ balance[t[0]] -= t[2]  ← from付出钱，余额减少
//
// 2. 回溯中不跳过已清零位置：
//    ✗ 直接 if (start == n) return 0;
//    ✓ 先 while(start < n && debt[start]==0) start++;
//    否则清零位置会被当作有效余额参与配对
//
// 3. 回溯中漏掉符号检查：
//    ✗ 不判断符号直接配对 → 同号配对只会让余额更大
//    ✓ if ((long long)debt[start] * debt[i] < 0) 只配异号
//
// 4. Bitmask DP 中 dp[0] 必须为 0：
//    空集划分成 0 个子集，这是递推的 base case
//
// 5. 子集枚举漏掉 mask 本身：
//    for (sub = (mask-1)&mask; ...) 不包含 sub==mask
//    需要额外处理 sum[mask]==0 时 dp[mask] = max(dp[mask], 1)
//
// ============================================================
// 【面试追问】
//
// Q1: 为什么不能贪心（每次选绝对值最大的正负配对）？
// A1: 反例 debt=[-5,-3,2,3,3]。贪心配对需4笔，
//     但分成{-5,2,3}和{-3,3}两组只需3笔。
//     贪心只看局部最大，无法找到全局最优划分。
//
// Q2: 回溯中的 break 剪枝为什么正确？
// A2: 当 debt[i] == -debt[start] 时，一笔交易消除两人。
//     如果不选这个而选部分抵消的，start消了但i还在，
//     后续还要额外交易处理i。所以完全抵消一定不亏。
//
// Q3: 如果人数很多（如n=20），还能用这些方法吗？
// A3: 3^20 ≈ 3.5 billion，太大。需要更高级的方法如
//     ILP(整数线性规划) 或近似算法。这道题本质是
//     NP-hard (minimum edge cover in complete bipartite graph)，
//     精确解在大规模下不可行。
//
// ============================================================
