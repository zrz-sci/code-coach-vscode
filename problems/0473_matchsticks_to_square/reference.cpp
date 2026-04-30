/*
 * LeetCode 473: Matchsticks to Square (火柴拼正方形)
 *
 * 将所有火柴分成 4 组，每组之和等于 sum/4。
 * 本质是 k=4 的等分子集问题。
 *
 * 难度：Medium
 * 标签：Bit Manipulation, Array, Dynamic Programming, Backtracking, Bitmask
 */

/* ============================================================
 * 解法1：回溯 + 剪枝 — O(4^n) 最坏，剪枝后远好于此 / O(n)
 *
 * 核心思想：
 *   对每根火柴做选择——放入 4 条边中的哪一条？
 *   DFS 搜索所有可能的分配方案。
 *
 * 四大剪枝：
 *   1. 降序排序：大的先放，更早触发失败（最关键！）
 *   2. 超长剪枝：sides[j] + ms[idx] > target → 跳过
 *   3. 相同边剪枝：sides[j] == sides[j-1] → 跳过（避免对称重复）
 *   4. 最大值预检：matchsticks[0] > target → 直接 false
 *
 * 决策树示意（matchsticks = [2,2,2,1,1], target = 2）:
 *
 *                          sides=[0,0,0,0]
 *                             放 ms[0]=2
 *              ┌─────────────────┤
 *           边0                 (边1/2/3同边0,跳过)
 *        [2,0,0,0]
 *           放 ms[1]=2
 *        ┌─────┤
 *       (边0满)  边1
 *             [2,2,0,0]
 *               放 ms[2]=2
 *            ┌─────┤
 *           (边0满,边1满) 边2
 *                   [2,2,2,0]
 *                     放 ms[3]=1
 *                   边3: [2,2,2,1]
 *                     放 ms[4]=1
 *                   边3: [2,2,2,2] ✓ 成功!
 *
 * ============================================================ */


class Solution {
public:
    bool makesquare(vector<int>& matchsticks) {
        int n = matchsticks.size();
        if (n < 4) return false;

        // 用 long long 避免潜在溢出（15 * 10^8 接近 INT_MAX）
        long long total = accumulate(matchsticks.begin(), matchsticks.end(), 0LL);
        if (total % 4 != 0) return false;

        int target = total / 4;

        // 【剪枝1】降序排序 — 大的火柴先处理，约束更强，搜索树更窄
        // 这是最关键的剪枝，没有它大概率 TLE
        sort(matchsticks.rbegin(), matchsticks.rend());

        // 【剪枝2】最大的火柴超过 target，不可能放入任何一条边
        if (matchsticks[0] > target) return false;

        // sides[j] 表示第 j 条边当前已累积的长度
        vector<int> sides(4, 0);
        return backtrack(matchsticks, sides, 0, target);
    }

private:
    bool backtrack(vector<int>& ms, vector<int>& sides, int idx, int target) {
        // 所有火柴都放完了 → 检查是否所有边都等于 target
        // 因为总和已经保证 % 4 == 0 且每条边 <= target，
        // 只需检查前 3 条边（第 4 条由总和保证）
        if (idx == (int)ms.size()) {
            return sides[0] == target && sides[1] == target
                && sides[2] == target;
        }

        // 尝试将当前火柴 ms[idx] 放入 4 条边之一
        for (int j = 0; j < 4; j++) {
            // 【剪枝3】放入后超过 target → 不可行，跳过
            if (sides[j] + ms[idx] > target) continue;

            // 【剪枝4】跳过相同长度的边，避免对称重复搜索
            // 例如 sides=[3,3,0,0]，放入 sides[0] 和 sides[1] 效果一样
            if (j > 0 && sides[j] == sides[j - 1]) continue;

            // 做选择
            sides[j] += ms[idx];

            // 递归处理下一根火柴
            if (backtrack(ms, sides, idx + 1, target)) return true;

            // 撤销选择（回溯）
            sides[j] -= ms[idx];
        }

        return false;  // 所有边都试过了，无解
    }
};


/* ============================================================
 * 解法2：状态压缩 DP — O(n * 2^n) / O(2^n)
 *
 * 核心思想：
 *   用 n 位 bitmask 表示哪些火柴已被使用。
 *   dp[mask] = 当前正在填充的那条边已累积的长度。
 *   当累积长度 == target 时，自动开始填下一条边（重置为 0）。
 *   最终 dp[全1] == 0 说明所有边都被恰好填满。
 *
 * 状态转移：
 *   对于每个 mask，尝试加入一根未使用的火柴 i：
 *   newMask = mask | (1 << i)
 *   newLen = dp[mask] + matchsticks[i]
 *   if (newLen > target) continue;          // 超长
 *   if (newLen == target) dp[newMask] = 0;  // 填满一条边，重置
 *   else dp[newMask] = newLen;              // 继续累积
 *
 * ============================================================ */

class Solution_DP {
public:
    bool makesquare(vector<int>& matchsticks) {
        int n = matchsticks.size();
        if (n < 4) return false;

        int total = accumulate(matchsticks.begin(), matchsticks.end(), 0);
        if (total % 4 != 0) return false;

        int target = total / 4;

        // 预检查：任何火柴超过 target 就不可能
        for (int m : matchsticks) {
            if (m > target) return false;
        }

        int fullMask = (1 << n) - 1;

        // dp[mask] = 使用 mask 中火柴后，当前边已累积的长度
        // -1 表示该状态不可达
        vector<int> dp(1 << n, -1);
        dp[0] = 0;  // 初始状态：没有火柴被使用，累积为 0

        for (int mask = 0; mask < (1 << n); mask++) {
            if (dp[mask] == -1) continue;  // 不可达状态，跳过

            for (int i = 0; i < n; i++) {
                // 火柴 i 已被使用，跳过
                if (mask & (1 << i)) continue;

                int newLen = dp[mask] + matchsticks[i];

                // 超过 target，非法
                if (newLen > target) continue;

                int newMask = mask | (1 << i);

                // 已经有更优的路径到达 newMask，跳过
                // （只要 dp[newMask] != -1 就说明已经有路径到达）
                if (dp[newMask] != -1) continue;

                // 恰好填满一条边 → 重置为 0，开始下一条边
                dp[newMask] = (newLen == target) ? 0 : newLen;
            }
        }

        // 所有火柴都被使用，且最后一条边也恰好填满（累积为 0）
        return dp[fullMask] == 0;
    }
};


/* ============================================================
 * 剪枝效果对比（matchsticks = [5,5,5,5,4,4,4,4,3,3,3,3]）
 *
 * 无剪枝回溯：搜索节点数约 4^12 ≈ 1600万
 * 降序排序：搜索节点数减少约 90%
 * 降序 + 跳过相同边：搜索节点数减少约 99%
 * 状态压缩DP：固定 12 * 4096 ≈ 5万次操作
 *
 * 结论：对于 n 较大的情况，状态压缩 DP 更稳定；
 *       对于大多数面试用例，回溯 + 剪枝已经足够。
 * ============================================================ */


// ============================================================
// 【解法对比】
// ============================================================
// 回溯+剪枝    O(4^n)最坏/O(n) — 剪枝后通常远快于理论上限，面试首选（好讲解）
// 状态压缩DP   O(n*2^n)/O(2^n) — 时间稳定可预测，n<=15 时无TLE风险
//
// 选用场景:
//   面试讲解/n<=20 → 回溯+剪枝，重点讲 4 大剪枝思路
//   追求稳定/竞赛 → 状态压缩 DP，不依赖剪枝效果
//   n>20 → 两种方法都超时，需要其他思路（但本题 n<=15）

// ============================================================
// 【易错点】
// ============================================================
// ✗ 忘记降序排序 → 小火柴先放，搜索树极宽，大概率 TLE
// ✓ sort(matchsticks.rbegin(), matchsticks.rend()); 必须降序
//
// ✗ 跳过相同边剪枝写错: if (j > 0 && sides[j] == sides[j-1]) continue;
//   在 sides[j] 被修改后才检查 → 剪枝失效
// ✓ 在 sides[j] += ms[idx] 之前检查（代码中位置正确）
//
// ✗ 状态压缩 DP 用 int total 累加，忘记取模重置
// ✓ dp[newMask] = (newLen == target) ? 0 : newLen;  填满一条边时重置为 0
//
// ✗ 回溯忘记撤销选择: 漏了 sides[j] -= ms[idx];
// ✓ 每次递归返回 false 后必须回溯 sides[j] -= ms[idx]

// ============================================================
// 【面试追问 Interview Follow-ups】
// ============================================================
// Q1: 如何判断能否分成 k 组等和子集？(泛化为 LeetCode 698)
// A1: 将 target 改为 sum/k，sides 数组长度改为 k，回溯逻辑完全一致。
//     剪枝策略不变：降序排序、超长跳过、相同桶跳过。
//
// Q2: 状态压缩 DP 中为什么 dp[newMask] 已被访问过就跳过？
// A2: 因为我们从 mask=0 到 fullMask 线性遍历，较小 mask 先处理。
//     如果 dp[newMask] != -1 说明已有路径到达且累积值更优（或等价），
//     无需重复设置。这是 BFS 式的"最先到达即最优"性质。
//
// Q3: 如果火柴可以折断（每根可以拆成两段），问题怎么变？
// A3: 问题变得简单很多——只要 sum % 4 == 0 且 max <= sum/4 就一定可以。
//     因为可以自由分配长度，贪心地分配即可，不需要搜索。
