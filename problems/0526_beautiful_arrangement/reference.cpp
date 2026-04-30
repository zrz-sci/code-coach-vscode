/*
 * 【题目本质】
 * 计算 1~n 的排列中满足 perm[i] % i == 0 || i % perm[i] == 0 的排列数。
 * n <= 15 -> 状压 DP 或回溯。
 *
 * 【解法总览】
 * Solution1: 状压 DP    O(2^n * n) / O(2^n)  ⭐推荐
 * Solution2: 回溯剪枝   O(n!) worst / O(n)
 */

// ===================== Solution1: 状压 DP =====================
// 思路：dp[mask] = 用 mask 代表的数字集合填前 popcount(mask) 个位置的方案数
class Solution1 {
public:
    int countArrangement(int n) {
        vector<int> dp(1 << n, 0);
        dp[0] = 1; // 空集，一种方案（什么都不放）

        for (int mask = 1; mask < (1 << n); mask++) {
            int pos = __builtin_popcount(mask); // 当前要填的位置（1-indexed）
            for (int num = 1; num <= n; num++) {
                // num 必须在 mask 中（表示已被选中放在某个位置）
                if (!(mask & (1 << (num - 1)))) continue;
                // 检查整除条件
                if (num % pos == 0 || pos % num == 0) {
                    dp[mask] += dp[mask ^ (1 << (num - 1))]; // 去掉 num 后的子问题
                }
            }
        }
        return dp[(1 << n) - 1]; // 所有数字都用上
    }
};

// ===================== Solution2: 回溯 =====================
// 思路：逐个位置尝试放数字，不满足条件则剪枝
class Solution2 {
public:
    int count = 0;

    void backtrack(int pos, int n, vector<bool>& used) {
        if (pos > n) {
            count++;
            return;
        }
        for (int num = 1; num <= n; num++) {
            if (!used[num] && (num % pos == 0 || pos % num == 0)) {
                used[num] = true;
                backtrack(pos + 1, n, used);
                used[num] = false;
            }
        }
    }

    int countArrangement(int n) {
        count = 0;
        vector<bool> used(n + 1, false);
        backtrack(1, n, used);
        return count;
    }
};

/*
 * 【解法对比】
 * Solution1: 状压 DP，时间严格 O(2^n * n)，无重复计算，面试推荐
 * Solution2: 回溯，代码更直观，剪枝效果好时实际快于 DP
 *
 * 【易错点】
 * 1. 位运算偏移：数字 1~n 对应 bit 0~(n-1)，别写成 1 << num
 * 2. 整除条件是双向的：num%pos==0 OR pos%num==0
 * 3. 回溯中忘记恢复 used 状态
 * 4. 位置是 1-indexed，popcount(mask) 直接就是当前位置
 *
 * 【面试追问】
 * Q1: 状压 DP vs 回溯的优劣？
 * Q2: popcount 为什么等于当前位置？
 * Q3: n 更大时还能用状压吗？极限在哪？
 */
