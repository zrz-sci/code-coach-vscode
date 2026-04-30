/*
 * 276. Paint Fence
 *
 * 【题目本质】
 * 线性 DP：n 个栅栏 k 种颜色，不能有连续 3 个同色。
 * 分解为「同色」和「异色」两个状态独立转移。
 * same[i] = diff[i-1]（只能从异色转来，避免三连同色）
 * diff[i] = (same[i-1] + diff[i-1]) * (k-1)
 *
 * 【解法总览】
 * Solution1: DP same/diff 分离 — O(n) 时间, O(1) 空间 [推荐]
 * Solution2: DP 合并递推公式 — O(n) 时间, O(1) 空间
 */

// ===================== Solution 1: DP same/diff 分离 =====================
// 思路: same = 与前一个同色方案数, diff = 与前一个异色方案数
// 转移: same = diff_prev, diff = (same_prev + diff_prev) * (k-1)
// 时间 O(n), 空间 O(1)
class Solution1 {
public:
    int numWays(int n, int k) {
        if (n == 0) return 0;
        if (n == 1) return k;

        // i = 2 的初始状态
        int same = k;           // 第1第2个同色: k 种
        int diff = k * (k - 1); // 第1第2个异色: k*(k-1) 种

        for (int i = 3; i <= n; i++) {
            int prevDiff = diff;
            // 异色: 前面(same+diff)总方案 * (k-1)种不同颜色
            diff = (same + diff) * (k - 1);
            // 同色: 只能从上一步异色转移(避免三连)
            same = prevDiff;
        }

        return same + diff;
    }
};

// ===================== Solution 2: DP 合并递推公式 =====================
// 思路: total[i] = same[i] + diff[i]
//       = diff[i-1] + (same[i-1] + diff[i-1]) * (k-1)
//       = total[i-1] * (k-1) + diff[i-1]
//   而 diff[i-1] = total[i-2] * (k-1)
//   所以 total[i] = (total[i-1] + total[i-2]) * (k-1)
// 时间 O(n), 空间 O(1)
class Solution2 {
public:
    int numWays(int n, int k) {
        if (n == 0) return 0;
        if (n == 1) return k;
        if (n == 2) return k * k;

        int prev2 = k;      // total[1] = k
        int prev1 = k * k;  // total[2] = k * k

        for (int i = 3; i <= n; i++) {
            int cur = (prev1 + prev2) * (k - 1);
            prev2 = prev1;
            prev1 = cur;
        }

        return prev1;
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 特点 |
 * |------|------|------|------|
 * | Solution1 same/diff | O(n) | O(1) | 状态含义清晰，易理解 |
 * | Solution2 合并公式 | O(n) | O(1) | 代码更简洁，推导更数学 |
 *
 * 【易错点】
 * 1. same 的转移来源搞错
 *    ✗ same = same_prev (三连同色!)
 *    ✓ same = diff_prev (只能从异色转来)
 *
 * 2. diff 的转移漏掉 same
 *    ✗ diff = diff_prev * (k-1)
 *    ✓ diff = (same_prev + diff_prev) * (k-1)
 *
 * 3. 初始值设置
 *    n=1: return k
 *    n=2: same=k, diff=k*(k-1) (总方案 k*k)
 *
 * 4. k=1 的特殊情况
 *    n=1: 1, n=2: 1, n>=3: 0
 *    公式自动处理: k-1=0 → diff=0, same无法增长
 *
 * 【面试追问】
 * Q1: 约束改为不能连续 m 个同色?
 *     --> 维护 same[1..m-1] 表示已连续同色的长度
 *     --> same[j] = same[j-1]_prev, diff = total_prev * (k-1)
 *
 * Q2: 能否 O(log n)?
 *     --> 矩阵快速幂，状态转移矩阵 2x2
 *     --> [same, diff]^T = M * [same_prev, diff_prev]^T
 *
 * Q3: 256. Paint House 与本题区别?
 *     --> 256: 相邻不能同色(连续2个都不行)，3种固定颜色+代价
 *     --> 276: 最多连续2个同色，k种颜色，求方案数
 */
