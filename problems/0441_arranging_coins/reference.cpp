/*
 * 【题目本质】
 * 找最大的 k 使得 k*(k+1)/2 <= n，即阶梯形排列硬币的最大完整行数。
 *
 * 【解法总览】
 * 解法1: 二分搜索 — O(logn) / O(1) ⭐ 面试首选
 * 解法2: 数学公式 — O(1) / O(1)
 */

// ===================== Solution1: 二分搜索 =====================
// 思路：在 [1, n] 上二分 k，检查 k*(k+1)/2 是否 <= n
// 时间 O(logn)  空间 O(1)
class Solution1 {
public:
    int arrangeCoins(int n) {
        long lo = 1, hi = n;
        while (lo <= hi) {
            long mid = lo + (hi - lo) / 2;
            long coins = mid * (mid + 1) / 2;
            if (coins == n) return (int)mid;
            else if (coins < n) lo = mid + 1;
            else hi = mid - 1;
        }
        return (int)hi; // hi 是最后一个满足 k*(k+1)/2 <= n 的值
    }
};

// ===================== Solution2: 数学公式 =====================
// 思路：解方程 k*(k+1)/2 = n → k = (-1 + sqrt(1+8n)) / 2，取整
// 时间 O(1)  空间 O(1)
class Solution2 {
public:
    int arrangeCoins(int n) {
        // 8.0*n 用 double 防止 8*n 溢出 int
        return (int)((-1.0 + sqrt(1.0 + 8.0 * n)) / 2.0);
    }
};

/*
 * 【解法对比】
 * | 解法 | 时间 | 空间 | 优势 | 劣势 |
 * |------|------|------|------|------|
 * | Solution1 二分搜索 | O(logn) | O(1) | 无精度问题，展示二分能力 | 代码略长 |
 * | Solution2 数学公式 | O(1) | O(1) | 极简一行 | 浮点精度风险 |
 *
 * 【易错点】
 * 1. mid*(mid+1)/2 溢出：n 最大 2^31-1，mid 可达 ~65535，mid*mid 不会溢出 long
 *    但如果用 int 计算就会溢出
 * 2. 数学公式中 8*n 溢出：必须用 8.0*n 或先转 long
 * 3. 返回值：二分结束后返回 hi 而非 lo
 *
 * 【面试追问】
 * Q1: 二分的左右边界如何确定？
 *     左边界 1，右边界 n（k <= n 一定成立）
 * Q2: 浮点精度会不会导致数学公式出错？
 *     极端情况下可能差 1，可以对结果做 +1/-1 验证
 * Q3: 能否用牛顿迭代法？
 *     可以，等价于求 f(k) = k*(k+1)/2 - n = 0 的正根
 */
